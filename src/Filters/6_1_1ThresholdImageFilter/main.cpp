#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkThresholdImageFilter.h>
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
using FilterType = itk::ThresholdImageFilter<ImageType>;

int main(int argc, char *argv[])
{
    ReaderType::Pointer reader = ReaderType::New();

    FilterType::Pointer filter = FilterType::New();

    reader->SetFileName("../../data/BrainProtonDensitySlice.png");

    reader->Update();

    filter->SetInput(reader->GetOutput());

    filter->SetOutsideValue(0);

    filter->ThresholdBelow(180);

    filter->Update();

    WriterType::Pointer writer = WriterType::New();

    writer->SetFileName("../../output/threshold_below_180.png");

    writer->SetInput(filter->GetOutput());

    writer->Update();

    filter->ThresholdOutside(170, 190);
    writer->SetFileName("../../output/threshold_outside_170_190.png");
    filter->Update();
    writer->SetInput(filter->GetOutput());
    writer->Update();

    filter->ThresholdAbove(180);
    writer->SetFileName("../../output/threshold_above_180.png");
    filter->Update();
    writer->SetInput(filter->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;
}