
#include "itkBilateralImageFilter.h"
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
using FilterType = itk::BilateralImageFilter<InputImageType, OutputImageType>;
using ThresholdFilterType = itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>;
using RescaleFilterType = itk::RescaleIntensityImageFilter<OutputImageType, WriterImageType>;

// 最大最小曲线流
int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());

    const unsigned int dimension = InputImageType::ImageDimension;

    double domainSigma[dimension];
    for (size_t i = 0; i < dimension; i++)
    {
        domainSigma[i] = 6.0;
    }
    const double rangeSigma=5.0;
    filter->SetDomainSigma(domainSigma);
    filter->SetRangeSigma(rangeSigma);
    filter->Update();

    auto rescaler = RescaleFilterType::New();
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);
    rescaler->SetInput(filter->GetOutput());
    rescaler->Update();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("../../output/BilateralImageFilter.png");
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