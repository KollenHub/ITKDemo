
#include "itkDCMTKImageIO.h"
#include "itkImageSeriesReader.h"
#include "itkImage.h"
#include "itkDCMTKSeriesFileNames.h"
#include "itkExtractImageFilter.h"
#include "ImageViewer.h"
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <atomic>

using PixelType = unsigned short;
using ImageType = itk::Image<PixelType, 3>;
using Image2DType = itk::Image<PixelType, 2>;
using NamespaceGenertor = itk::DCMTKSeriesFileNames;
using ReaderType = itk::ImageSeriesReader<ImageType>;
using DCMTKIO = itk::DCMTKImageIO;

using ExtractImageFilter = itk::ExtractImageFilter<ImageType, Image2DType>;

int main(int argc, char **argv)
{
    NamespaceGenertor::Pointer namespacegenertor = NamespaceGenertor::New();
    namespacegenertor->SetDirectory("D:/DICOM/data1");

    auto uids = namespacegenertor->GetSeriesUIDs();

    const auto fileNames = namespacegenertor->GetFileNames(uids[0]);

    std::cout << "file count: " << fileNames.size() << std::endl;

    ReaderType::Pointer reader = ReaderType::New();

    reader->SetFileNames(fileNames);
    reader->SetImageIO(DCMTKIO::New());

    try
    {
        reader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
        std::cout << e << std::endl;
    }

    ImageType::Pointer image = reader->GetOutput();

    std::cout << "Image size: " << image->GetLargestPossibleRegion().GetSize() << std::endl;

    ImageViewer<PixelType> viewer("DICOM Viewer");

    // 线程安全队列：生产者-消费者模型
    std::mutex queueMutex;
    std::deque<cv::Mat> frameQueue;
    std::condition_variable queueCondVar;
    std::atomic<bool> producerDone{false};

    // 后台线程：解码切片并放入队列
    std::thread thread([&]()
    {
        ImageType::RegionType inputRegion = image->GetLargestPossibleRegion();
        ImageType::SizeType size = inputRegion.GetSize();
        ImageType::IndexType start = inputRegion.GetIndex();

        ExtractImageFilter::Pointer extract = ExtractImageFilter::New();

        // 提取 2D 切片：沿 Z 方向
        size[2] = 0;
        extract->SetDirectionCollapseToSubmatrix();
        extract->SetInput(image);

        int startIndex = 1;
        const int maxSlice = inputRegion.GetSize()[2];

        while (startIndex < maxSlice)
        {
            start[2] = startIndex;
            ImageType::RegionType desiredRegion;
            desiredRegion.SetSize(size);
            desiredRegion.SetIndex(start);

            extract->SetExtractionRegion(desiredRegion);

            extract->Update();

            Image2DType::Pointer slice = extract->GetOutput();

            // ITK -> cv::Mat，并深拷贝（确保数据独立于切片生命周期）
            ImageParse<PixelType> parser(slice);
            cv::Mat frame = parser.GetMat(); // GetMat 按值返回，已深拷贝

            // 放入队列
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                frameQueue.push_back(std::move(frame));

                // 限制队列最大长度，防止内存无限增长
               	constexpr size_t maxQueueSize = 30;
                while (frameQueue.size() > maxQueueSize)
                {
                    frameQueue.pop_front();
                }
            }

            queueCondVar.notify_one();

            // 预读/解码频率控制（如果解码很快，可适当 sleep 避免队列填满）
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            startIndex++;
        }

        // 生产结束标记
        producerDone.store(true);
        queueCondVar.notify_one();
    });

    // 主线程：消费队列并显示
    bool windowCreated = false;
    const std::string windowName = viewer.GetWindowName();

    while (true)
    {
        cv::Mat frame;

        // 从队列取帧
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondVar.wait(lock, [&]{ return !frameQueue.empty() || producerDone.load(); });

            // 如果队列为空且生产已结束，退出循环
            if (frameQueue.empty() && producerDone.load())
                break;

            if (!frameQueue.empty())
            {
                frame = std::move(frameQueue.front());
                frameQueue.pop_front();
            }
        }

        if (!frame.empty())
        {
            // 懒创建窗口（只在第一次显示时创建）
            if (!windowCreated)
            {
                cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
                windowCreated = true;
            }

            cv::imshow(windowName, frame);
        }

        // 处理窗口消息，并控制播放速度
        // 30ms ≈ 33 FPS，可按需调整
        if (cv::waitKey(30) == 27) // ESC 键退出
            break;
    }

    // 等待后台线程结束
    if (thread.joinable())
        thread.join();

    return EXIT_SUCCESS;
}
