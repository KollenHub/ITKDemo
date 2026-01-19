
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <ImageViewer.h>
using InputPixelType = float;
using OutPixelType = float;
using WriterPixelType = unsigned char;
using InputImageType = itk::Image<InputPixelType, 2>;
using OutputImageType = itk::Image<OutPixelType, 2>;
using WriterImageType = itk::Image<WriterPixelType, 2>;
using ReaderType = itk::ImageFileReader<InputImageType>;
using WriterType = itk::ImageFileWriter<WriterImageType>;
using RescaleFilterType =
    itk::RescaleIntensityImageFilter<OutputImageType, WriterImageType>;
using FilterType =itk::GradientMagnitudeRecursiveGaussianImageFilter<InputImageType, OutputImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    // 梯度强度
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());
    filter->SetSigma(5.0);
    filter->Update();

    // 线性映射
    auto rescaler = RescaleFilterType::New();
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);
    rescaler->SetInput(filter->GetOutput());
    rescaler->Update();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("../../output/GradientMangnitudeRecursiveGaussianImageFilter5.0.png");
    writer->SetInput(rescaler->GetOutput());
    writer->Update();

    ImageViewer<WriterPixelType> viewer;
    viewer.SetITKImage(rescaler->GetOutput());
    viewer.Show();
    cv::waitKey(0);

    return EXIT_SUCCESS;
}