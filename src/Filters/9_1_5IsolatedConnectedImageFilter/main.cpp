#include "itkIsolatedConnectedImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

using PixelType = float;
using InternalImageType = itk::Image<PixelType, 2>;
using CurvatureFlowImageFilterType = itk::CurvatureFlowImageFilter<InternalImageType, InternalImageType>;
using ConnectedFilterType = itk::IsolatedConnectedImageFilter<InternalImageType, InternalImageType>;
using WritePixelType = unsigned char;
using OutputImageType = itk::Image<WritePixelType>;
using CastFilter = itk::CastImageFilter<InternalImageType, OutputImageType>;
using ReaderType = itk::ImageFileReader<InternalImageType>;
using WriterType = itk::ImageFileWriter<OutputImageType>;

int main(int argc, char **argv)
{

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();
    smoothing->SetInput(reader->GetOutput());
    smoothing->SetNumberOfIterations(5);
    smoothing->SetTimeStep(0.125);

    ConnectedFilterType::Pointer isolatedConnected = ConnectedFilterType::New();
    isolatedConnected->SetInput(smoothing->GetOutput());

    // isolatedConnected->SetLower()
    isolatedConnected->SetReplaceValue(255);

    isolatedConnected->SetLower(150);
    isolatedConnected->SetSeed1(itk::MakeIndex(61, 140));
    isolatedConnected->SetSeed2(itk::MakeIndex(63, 43));

    CastFilter::Pointer caster = CastFilter::New();
    caster->SetInput(isolatedConnected->GetOutput());

    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(caster->GetOutput());

    writer->SetFileName("../../output/9_1_5IsolatedConnectedImageFilter.png");
    try
    {
        writer->Update();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}