#include "itkSigmoidImageFilter.h"
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <ImageViewer.h>

using InputPixelType = unsigned char;
using OutputPixelType = unsigned char;
using InputImageType = itk::Image<InputPixelType, 2>;
using OutputImageType = itk::Image<OutputPixelType, 2>;
using SigmoidFilterType = itk::SigmoidImageFilter<InputImageType, OutputImageType>;
using WriterType = itk::ImageFileWriter<OutputImageType>;
using ReaderType = itk::ImageFileReader<InputImageType>;

int main()
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    // 定义非线性映射
    SigmoidFilterType::Pointer sigmoidFilter = SigmoidFilterType::New();

    sigmoidFilter->SetInput(reader->GetOutput());
    sigmoidFilter->SetOutputMinimum(10);
    sigmoidFilter->SetOutputMaximum(240);

    sigmoidFilter->SetAlpha(10);
    sigmoidFilter->SetBeta(170);

    sigmoidFilter->Update();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("output/sigmoid.png");
    writer->SetInput(sigmoidFilter->GetOutput());
    writer->Update();
    return EXIT_SUCCESS;
}