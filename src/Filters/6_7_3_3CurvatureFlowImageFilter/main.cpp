
#include "itkCurvatureFlowImageFilter.h"
#include <itkRescaleIntensityImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <ImageViewer.h>
using InputPixelType = float;
using OutPixelType = float;
using WriterPixelType = unsigned char;
using InputImageType = itk::Image<InputPixelType, 2>;
using OutputImageType = itk::Image<OutPixelType, 2>;
using WriterImageType = itk::Image<WriterPixelType, 2>;
using ReaderType = itk::ImageFileReader<InputImageType>;
using WriterType = itk::ImageFileWriter<WriterImageType>;
using FilterType = itk::CurvatureFlowImageFilter<InputImageType, OutputImageType>;
using ThresholdFilterType = itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>;
using RescaleFilterType = itk::RescaleIntensityImageFilter<OutputImageType, WriterImageType>;

// 曲线流
int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());

    // 参与迭代的次数
    filter->SetNumberOfIterations(5);
    // time step	每次迭代的步长	决定每次迭代的变化幅度，步长过大会导致不稳定。
    filter->SetTimeStep(0.125);
    // 水平集活动计算中的电导系数
    filter->Update();

    auto rescaler = RescaleFilterType::New();
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);
    rescaler->SetInput(filter->GetOutput());
    rescaler->Update();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("../../output/GradientAnisotropicDiffusionImageFilter.png");
    writer->SetInput(rescaler->GetOutput());
    writer->Update();

    ImageViewer<WriterPixelType> viewer;
    viewer.SetITKImage(rescaler->GetOutput());
    viewer.Show();
    cv::waitKey(0);

    // 下面的类提供了相似的功能
    //  itk::BilateralImageFilter
    //  itk::CurvatureAnisotropicDiffusionImageFilter
    //  itk::CurvatureFlowImageFilter

    return EXIT_SUCCESS;
}