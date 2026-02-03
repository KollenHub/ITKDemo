#include <iostream>
#include "itkConnectedThresholdImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"

using PixelType = float;
using WritePixelType = unsigned char;
using ImageType = itk::Image<PixelType, 2>;
using WriteImageType = itk::Image<WritePixelType, 2>;
using CurvatureFlowImageFilterType = itk::CurvatureFlowImageFilter<ImageType, ImageType>;
using ReadeType = itk::ImageFileReader<ImageType>;
using WriterType = itk::ImageFileWriter<WriteImageType>;
using ConnectedFilterType = itk::ConnectedThresholdImageFilter<ImageType, ImageType>;
using CastImageFilterType = itk::CastImageFilter<ImageType, WriteImageType>;

int main(int argc, char **argv)
{
    ReadeType::Pointer reader = ReadeType::New();
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
        ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
        connectedThreshold->SetInput(smoothing->GetOutput());
        connectedThreshold->SetLower(lower[i]);
        connectedThreshold->SetUpper(upper[i]);
        connectedThreshold->SetReplaceValue(255);
        connectedThreshold->SetSeed(itk::Index<2>{{xCoords[i], yCoords[i]}});

        CastImageFilterType::Pointer castImageFilter = CastImageFilterType::New();
        castImageFilter->SetInput(connectedThreshold->GetOutput());

        WriterType::Pointer writer = WriterType::New();
        writer->SetInput(castImageFilter->GetOutput());
        writer->SetFileName("../../output/9_1_1ConnectedThresholdImageFilter"+std::to_string(i+1)+".png");
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