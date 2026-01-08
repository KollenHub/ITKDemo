
#include <itkBinaryErodeImageFilter.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <ImageViewer.h>
using InputPixelType = unsigned char;
using OutPixelType = unsigned char;
using WriterPixelType = unsigned char;
using InputImageType = itk::Image<InputPixelType, 2>;
using OutputImageType = itk::Image<OutPixelType, 2>;
using WriterImageType = itk::Image<WriterPixelType, 2>;
using ReaderType = itk::ImageFileReader<InputImageType>;
using WriterType = itk::ImageFileWriter<WriterImageType>;

using StructuringElementType = itk::BinaryBallStructuringElement<InputPixelType, 2>;
using ErodeFilterType = itk::BinaryErodeImageFilter<InputImageType, OutputImageType, StructuringElementType>;

using DialateFilterType = itk::BinaryDilateImageFilter<InputImageType, OutputImageType, StructuringElementType>;

using BinaryThresholdImageFilterType = itk::BinaryThresholdImageFilter<InputImageType, InputImageType>;

using RescaleFilterType =
    itk::RescaleIntensityImageFilter<OutputImageType, WriterImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    // 二值阈值
    BinaryThresholdImageFilterType::Pointer thresholder = BinaryThresholdImageFilterType::New();
    thresholder->SetInput(reader->GetOutput());

    InputPixelType background = 0;
    InputPixelType foreground = 255;
    thresholder->SetOutsideValue(background);
    thresholder->SetInsideValue(foreground);
    thresholder->SetLowerThreshold(170);
    thresholder->SetUpperThreshold(190);

    //形态学扩张
    DialateFilterType::Pointer binaryDialate = DialateFilterType::New();
    //形态学腐蚀
    ErodeFilterType::Pointer binaryErode = ErodeFilterType::New();

    StructuringElementType structuringElemnt;
    structuringElemnt.SetRadius(1);
    structuringElemnt.CreateStructuringElement();

    binaryDialate->SetKernel(structuringElemnt);
    binaryErode->SetKernel(structuringElemnt);
    binaryErode->SetErodeValue(foreground);
    binaryDialate->SetDilateValue(foreground);

    binaryDialate->SetInput(thresholder->GetOutput());
    binaryErode->SetInput(thresholder->GetOutput());

    WriterType::Pointer writeBinaryThreolder = WriterType::New();
    writeBinaryThreolder->SetFileName("output/BinaryThreoldBaseImage.png");
    writeBinaryThreolder->SetInput(thresholder->GetOutput());
    writeBinaryThreolder->Update();
    

    WriterType::Pointer writeDilation = WriterType::New();
    writeDilation->SetFileName("output/BinaryDilate.png");
    writeDilation->SetInput(binaryDialate->GetOutput());
    writeDilation->Update();

    WriterType::Pointer writeErode = WriterType::New();
    writeErode->SetFileName("output/BinaryErode.png");
    writeErode->SetInput(binaryErode->GetOutput());
    writeErode->Update();

    // ImageViewer<WriterPixelType> viewer;
    // viewer.SetITKImage(rescaler->GetOutput());
    // viewer.Show();
    cv::waitKey(0);

    return EXIT_SUCCESS;
}