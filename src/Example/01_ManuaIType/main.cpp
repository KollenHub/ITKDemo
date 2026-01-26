#include <iostream>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkPNGImageIO.h"
#include "itkJPEGImageIO.h"

using InputPixelType = unsigned char;
using OutputPixelType = unsigned char;
using InputImageType = itk::Image<InputPixelType, 2>;
using OutputImageType = itk::Image<OutputPixelType, 2>;

using ReaderType = itk::ImageFileReader<InputImageType>;
using WriterType = itk::ImageFileWriter<OutputImageType>;

using PNGIOType = itk::PNGImageIO;
using JPEGIOType = itk::JPEGImageIO;

int main(int argc, char **argv)
{
    const std::string inputPath = "../../data/BrainProtonDensitySlice.png";
    const std::string outputPath = "../../output/jpgOutput.jpg";

    try
    {
        // 创建读取器和PNG IO
        ReaderType::Pointer reader = ReaderType::New();
        PNGIOType::Pointer pngIO = PNGIOType::New();

        // 读取PNG图像
        reader->SetFileName(inputPath);
        reader->SetImageIO(pngIO);
        reader->Update();

        // 创建写入器和JPEG IO
        WriterType::Pointer writer = WriterType::New();
        JPEGIOType::Pointer jpegIO = JPEGIOType::New();

        // 写入JPEG图像
        writer->SetInput(reader->GetOutput());
        writer->SetFileName(outputPath);
        writer->SetImageIO(jpegIO);
        writer->Update();

        std::cout << "图像转换成功: " << inputPath << " -> " << outputPath << std::endl;
        return 0;
    }
    catch (const itk::ExceptionObject &error)
    {
        std::cerr << "ITK错误: " << error.GetDescription() << std::endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "标准库错误: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "未知错误发生" << std::endl;
        return 1;
    }
}