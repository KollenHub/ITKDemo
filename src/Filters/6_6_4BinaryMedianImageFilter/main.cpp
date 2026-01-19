
#include <itkBinaryMedianImageFilter.h>
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
using FilterType = itk::BinaryMedianImageFilter<InputImageType, OutputImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../output/BinaryThresholdImageFilter.png");
    reader->Update();

    // 临域
    InputImageType::SizeType indexRadius;
    indexRadius[0] = 3;
    indexRadius[1] = 3;

    // 二值中值滤波
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());
    filter->SetRadius(indexRadius);
    filter->Update();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("../../output/BinaryMedianImage3x3.png");
    writer->SetInput(filter->GetOutput());
    writer->Update();

    ImageViewer<WriterPixelType> viewer;
    viewer.SetITKImage(filter->GetOutput());
    viewer.Show();
    cv::waitKey(0);

    return EXIT_SUCCESS;
}