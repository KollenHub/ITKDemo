#include "itkNeighborhoodConnectedImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"

using InternalPixelType = float;
using InternalImageType = itk::Image<InternalPixelType, 2>;
using CurvatureFlowImageFilterType = itk::CurvatureFlowImageFilter<InternalImageType, InternalImageType>;
using ConnectedFilterType = itk::NeighborhoodConnectedImageFilter<InternalImageType, InternalImageType>;

using ReaderType = itk::ImageFileReader<InternalImageType>;
using WritePixelType = unsigned char;
using WriteImageType = itk::Image<WritePixelType, 2>;
using WriteType = itk::ImageFileWriter<WriteImageType>;

using CastFilter = itk::CastImageFilter<InternalImageType, WriteImageType>;

int main(int argc, char **argv)
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();
    smoothing->SetInput(reader->GetOutput());
    smoothing->SetNumberOfIterations(5);
    smoothing->SetTimeStep(0.125);

    int lower[3] = {150, 210, 180};
    int upper[3] = {180, 250, 210};
    int xCoords[3] = {60, 81, 107};
    int yCoords[3] = {116, 112, 69};

    for (size_t i = 0; i < 3; i++)
    {
        ConnectedFilterType::Pointer neighborhoodConnected = ConnectedFilterType::New();
        neighborhoodConnected->SetInput(smoothing->GetOutput());
        neighborhoodConnected->SetLower(lower[i]);
        neighborhoodConnected->SetUpper(upper[i]);

        InternalImageType::SizeType radius;
        radius[0] = 2;
        radius[1] = 2;
        neighborhoodConnected->SetRadius(radius);
        neighborhoodConnected->SetSeed(itk::Index<2>{xCoords[i],yCoords[i]});
        neighborhoodConnected->SetReplaceValue(255);

        CastFilter::Pointer castImageFilter = CastFilter::New();
        castImageFilter->SetInput(neighborhoodConnected->GetOutput());

        WriteType::Pointer writer = WriteType::New();
        writer->SetInput(castImageFilter->GetOutput());
        writer->SetFileName("../../output/9_1_3NeighborhoodConnectedImageFilter" + std::to_string(i + 1) + ".png");
        try
        {
            writer->Update();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    return 0;
}