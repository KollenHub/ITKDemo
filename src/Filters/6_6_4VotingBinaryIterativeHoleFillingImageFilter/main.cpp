
#include <itkVotingBinaryIterativeHoleFillingImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <ImageViewer.h>

using PixelType = unsigned char;
using ImageType=itk::Image<PixelType, 2>;
using FilterType=itk::VotingBinaryIterativeHoleFillingImageFilter<ImageType>;
using ThresholdFilterType=itk::BinaryThresholdImageFilter<ImageType, ImageType>;
using ReaderType=itk::ImageFileReader<ImageType>;
using WriterType=itk::ImageFileWriter<ImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    
    ThresholdFilterType::Pointer thresholdFilter = ThresholdFilterType::New();
    // 阈值
    thresholdFilter->SetInput(reader->GetOutput());
    thresholdFilter->SetLowerThreshold(150);
    thresholdFilter->SetUpperThreshold(180);
    thresholdFilter->SetOutsideValue(0);
    thresholdFilter->SetInsideValue(255);

    // 临域
    ImageType::SizeType indexRadius;
    indexRadius[0] = 3;
    indexRadius[1] = 3;

    // 洞穴填充滤波器
    FilterType::Pointer filter = FilterType::New();
    filter->SetForegroundValue(255);
    filter->SetBackgroundValue(0);
    filter->SetMajorityThreshold(2);
    filter->SetInput(thresholdFilter->GetOutput());
    filter->SetRadius(indexRadius);
    filter->Update();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("output/VotingBinaryIterativeHoleFillingImageFilter2x2.png");
    writer->SetInput(filter->GetOutput());
    writer->Update();

    ImageViewer<PixelType> viewer;
    viewer.SetITKImage(filter->GetOutput());
    viewer.Show();
    cv::waitKey(0);

    return EXIT_SUCCESS;
}