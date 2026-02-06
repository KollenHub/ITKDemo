#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"

using RGBPixelType = itk::RGBPixel<unsigned char>;
using RGBImageType = itk::Image<RGBPixelType, 2>;
using VectorPixelType = itk::Vector<float, 3>;
using VectorImageType = itk::Image<VectorPixelType, 2>;
using LabeledImageType = itk::Image<itk::IdentifierType, 2>;
using ScalarImageType = itk::Image<float, 2>;
using FileReaderType = itk::ImageFileReader<RGBImageType>;
using CastFilterType = itk::CastImageFilter<RGBImageType, VectorImageType>;
using DiffusionFilterType = itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType>;
using GradientMagnitudeFilterType = itk::VectorGradientMagnitudeImageFilter<VectorImageType>;
using WatershedFilterType = itk::WatershedImageFilter<ScalarImageType>;

using ColorMapFunctorType = itk::Functor::ScalarToRGBPixelFunctor<unsigned long>;
using ColorMapFilterType = itk::UnaryFunctorImageFilter<LabeledImageType, RGBImageType, ColorMapFunctorType>;

using WriterType = itk::ImageFileWriter<RGBImageType>;

struct Parameter
{
    int NumberOfIterations;
    float ConductanceParameter;
    bool UsePrincipleComponents;
    float Level;
    float Threshold;
};

int main(int argc, char **argv)
{
    FileReaderType::Pointer reader = FileReaderType::New();
    reader->SetFileName("../../data/VisibleWomanEyeSlice.png");
    reader->Update();

    CastFilterType::Pointer caster = CastFilterType::New();
    caster->SetInput(reader->GetOutput());

    std::vector<Parameter> parameters = {
        {10,
         2.0,
         true,
         0.05,
         0.0},
        {10,
         2.0,
         false,
         0.15,
         0.001}};

    for (auto &par : parameters)
    {
        DiffusionFilterType::Pointer diffusionFilter = DiffusionFilterType::New();
        diffusionFilter->SetNumberOfIterations(par.NumberOfIterations);
        diffusionFilter->SetConductanceParameter(par.ConductanceParameter);
        diffusionFilter->SetTimeStep(0.125);
        diffusionFilter->SetInput(caster->GetOutput());

        GradientMagnitudeFilterType::Pointer gradient = GradientMagnitudeFilterType::New();
        gradient->SetUsePrincipleComponents(par.UsePrincipleComponents);
        gradient->SetInput(diffusionFilter->GetOutput());

        WatershedFilterType::Pointer watershed = WatershedFilterType::New();
        watershed->SetLevel(par.Level);
        watershed->SetThreshold(par.Threshold);
        watershed->SetInput(gradient->GetOutput());

        ColorMapFilterType::Pointer colorMapper = ColorMapFilterType::New();
        colorMapper->SetInput(watershed->GetOutput());

        WriterType::Pointer writer = WriterType::New();
        writer->SetFileName("../../output/9_2_2WatershedSegmentation" + std::to_string(&par - &parameters[0]) + ".png");
        writer->SetInput(colorMapper->GetOutput());

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