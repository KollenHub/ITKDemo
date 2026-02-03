#include <iostream>
#include <string>
#include "itkOtsuThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

using InputPixelType = unsigned char;
using OutputPixelType = unsigned char;
using InputImageType = itk::Image<InputPixelType, 2>;
using OutputImageType = itk::Image<OutputPixelType, 2>;
using FilterType = itk::OtsuThresholdImageFilter<InputImageType, OutputImageType>;
using ReaderType = itk::ImageFileReader<InputImageType>;
using WriterType = itk::ImageFileWriter<OutputImageType>;
int main(int argc, char **argv)
{

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());

    int outSideValue[1] = {255};
    int insideValue[1] = {0};

    filter->SetOutsideValue(outSideValue[0]);
    filter->SetInsideValue(insideValue[0]);

    // 计算直方图bin的数目，直方图在内部计算来得到otsu阈值
    filter->SetNumberOfHistogramBins(128);

    filter->Update();

    int threshold = filter->GetThreshold();
    std::cout << "threshold: " << threshold << std::endl;

    WriterType::Pointer writer = WriterType::New();

    writer->SetFileName("../../output/9_1_2OtsuThresholdImageFilter.png");
    writer->SetInput(filter->GetOutput());

    try
    {
        writer->Update();
    }
    catch (itk::ExceptionObject &e)
    {
        std::cerr << "Error: " << e << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}