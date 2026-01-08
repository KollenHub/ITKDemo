
#include <itkGrayscaleErodeImageFilter.h>
#include <itkGrayscaleDilateImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
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
using ErodeFilterType = itk::GrayscaleErodeImageFilter<InputImageType, OutputImageType, StructuringElementType>;

using DialateFilterType = itk::GrayscaleDilateImageFilter<InputImageType, OutputImageType, StructuringElementType>;


using RescaleFilterType =
    itk::RescaleIntensityImageFilter<OutputImageType, WriterImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    //灰度形态学扩张
    DialateFilterType::Pointer grayscaleDialate = DialateFilterType::New();
    //灰度形态学腐蚀
    ErodeFilterType::Pointer grayscaleErode = ErodeFilterType::New();

    StructuringElementType structuringElemnt;
    structuringElemnt.SetRadius(1);
    structuringElemnt.CreateStructuringElement();

    grayscaleDialate->SetKernel(structuringElemnt);
    grayscaleErode->SetKernel(structuringElemnt);

    grayscaleDialate->SetInput(reader->GetOutput());
    grayscaleErode->SetInput(reader->GetOutput());

    WriterType::Pointer writegrayscaleThreolder = WriterType::New();
    writegrayscaleThreolder->SetFileName("output/GrayscaleBaseImage.png");
    writegrayscaleThreolder->SetInput(reader->GetOutput());
    writegrayscaleThreolder->Update();
    

    WriterType::Pointer writeDilation = WriterType::New();
    writeDilation->SetFileName("output/GrayscaleDilate.png");
    writeDilation->SetInput(grayscaleDialate->GetOutput());
    writeDilation->Update();

    WriterType::Pointer writeErode = WriterType::New();
    writeErode->SetFileName("output/GrayscaleErode.png");
    writeErode->SetInput(grayscaleErode->GetOutput());
    writeErode->Update();

    // ImageViewer<WriterPixelType> viewer;
    // viewer.SetITKImage(rescaler->GetOutput());
    // viewer.Show();
    cv::waitKey(0);

    return EXIT_SUCCESS;
}