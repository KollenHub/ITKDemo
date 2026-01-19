
#include <itkDiscreteGaussianImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <ImageViewer.h>
using InputPixelType = unsigned char;
using OutPixelType = unsigned char;
using WriterPixelType = unsigned char;
using InputImageType = itk::Image<InputPixelType, 2>;
using OutputImageType = itk::Image<OutPixelType, 2>;
using WriterImageType = itk::Image<WriterPixelType, 2>;
using ReaderType = itk::ImageFileReader<InputImageType>;
using WriterType = itk::ImageFileWriter<WriterImageType>;
using FilterType = itk::DiscreteGaussianImageFilter<InputImageType, OutputImageType>;
using ThresholdFilterType = itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    //高斯滤波器
    FilterType::Pointer gaussianFilter = FilterType::New();
    gaussianFilter->SetInput(reader->GetOutput());
    //模糊强度 ($\sigma^2$) 散光的严重程度 决定图像变得多“肉”、多模糊。
    gaussianFilter->SetVariance(1.73);
    //窗口大小	镜片的物理尺寸	决定计算量的大小以及模糊是否完整。
    gaussianFilter->SetMaximumKernelWidth(10);
    gaussianFilter->Update();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("../../output/DiscreteGaussianImageFilter.png");
    writer->SetInput(gaussianFilter->GetOutput());
    writer->Update();

    ImageViewer<WriterPixelType> viewer;
    viewer.SetITKImage(gaussianFilter->GetOutput());
    viewer.Show();
    cv::waitKey(0);

    return EXIT_SUCCESS;
}