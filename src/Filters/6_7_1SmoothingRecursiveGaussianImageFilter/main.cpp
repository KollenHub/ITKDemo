
#include "itkRecursiveGaussianImageFilter.h"
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
using FilterType = itk::RecursiveGaussianImageFilter<InputImageType, OutputImageType>;
using ThresholdFilterType = itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    // IIR高斯滤波器
    FilterType::Pointer filterX = FilterType::New();
    FilterType::Pointer filterY = FilterType::New();

    filterX->SetDirection(0); // 0表示X方向
    filterY->SetDirection(1); // 1表示Y方向

    filterX->SetOrder(FilterType::ZeroOrder);
    filterY->SetOrder(FilterType::ZeroOrder);

    // 允许用户在两个标准化之间进行选择
    filterX->SetNormalizeAcrossScale(false);
    filterY->SetNormalizeAcrossScale(false);

    filterX->SetInput(reader->GetOutput());
    filterY->SetInput(filterX->GetOutput());

    filterX->SetSigma(5);
    filterY->SetSigma(5);

    filterY->Update();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("output/SmoothingRecursiveGaussianImageFilter5x5.png");
    writer->SetInput(filterY->GetOutput());
    writer->Update();

    ImageViewer<WriterPixelType> viewer;
    viewer.SetITKImage(filterY->GetOutput());
    viewer.Show();
    cv::waitKey(0);

    return EXIT_SUCCESS;
}