#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageRegionIterator.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include "ImageViewer.h"
#include <filesystem>

using PixelType = unsigned char;
constexpr unsigned int Dimension = 2;
using ImageType = itk::Image<PixelType, Dimension>;
using ReaderType = itk::ImageFileReader<ImageType>;
using WriterType = itk::ImageFileWriter<ImageType>;
using FilterType = itk::BinaryThresholdImageFilter<ImageType, ImageType>;

int main(int argc, char *argv[])
{
    ReaderType::Pointer reader = ReaderType::New();

    FilterType::Pointer filter = FilterType::New();

    reader->SetFileName("../../data/BrainProtonDensitySlice.png");

    reader->Update();

    filter->SetInput(reader->GetOutput());

    filter->SetOutsideValue(0);
    filter->SetInsideValue(255);

    filter->SetLowerThreshold(150);
    filter->SetUpperThreshold(180);

    filter->Update();

    WriterType::Pointer writer = WriterType::New();

    writer->SetFileName("../../output/BinaryThresholdImageFilter.png");

    writer->SetInput(filter->GetOutput());

    writer->Update();
  
    return EXIT_SUCCESS;
}