
#include <itkVotingBinaryHoleFillingImageFilter.h>
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
using FilterType = itk::VotingBinaryHoleFillingImageFilter<InputImageType, OutputImageType>;
using ThresholdFilterType = itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    
    ThresholdFilterType::Pointer thresholdFilter = ThresholdFilterType::New();
    // 阈值
    thresholdFilter->SetInput(reader->GetOutput());
    thresholdFilter->SetLowerThreshold(170);
    thresholdFilter->SetUpperThreshold(190);
    thresholdFilter->SetOutsideValue(0);
    thresholdFilter->SetInsideValue(255);

    // 临域
    InputImageType::SizeType indexRadius;
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
    writer->SetFileName("output/VotingBinaryHoleFillingImageFilter3x3.png");
    writer->SetInput(filter->GetOutput());
    writer->Update();

    ImageViewer<WriterPixelType> viewer;
    viewer.SetITKImage(filter->GetOutput());
    viewer.Show();
    cv::waitKey(0);

    return EXIT_SUCCESS;
}