#include "itkConfidenceConnectedImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

using PixelType = float;
using InternalImageType = itk::Image<PixelType, 2>;
using CurvatureFlowImageFilterType = itk::CurvatureFlowImageFilter<InternalImageType, InternalImageType>;
using ConnectedFilterType = itk::ConfidenceConnectedImageFilter<InternalImageType, InternalImageType>;
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

    ConnectedFilterType::Pointer confidenceConnected = ConnectedFilterType::New();
    confidenceConnected->SetInput(smoothing->GetOutput());
    confidenceConnected->SetMultiplier(2.5);
    confidenceConnected->SetNumberOfIterations(5);
    confidenceConnected->SetReplaceValue(255);

    confidenceConnected->SetInitialNeighborhoodRadius(2);

    CastFilter::Pointer caster = CastFilter::New();
    caster->SetInput(confidenceConnected->GetOutput());

    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(caster->GetOutput());

    int xCoords[3] = {61, 81, 107};
    int yCoords[3] = {116, 112, 69};

    for (size_t i = 0; i < 3; i++)
    {
        confidenceConnected->SetSeed(itk::MakeIndex(xCoords[i], yCoords[i]));
        writer->SetFileName("../../output/9_1_4ConfidenceConnectedImageFilter"+std::to_string(i+1)+".png");
        try
        {
            writer->Update();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    return 0;
}