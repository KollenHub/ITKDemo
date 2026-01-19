
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkRGBPixel.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
// #include "itkVectorCastImageFilter.h" 由下面的代替
#include "itkCastImageFilter.h"
using InputPixelType = itk::RGBPixel<float>;
using InputImageType = itk::Image<InputPixelType, 2>;
using FilterType = itk::VectorGradientAnisotropicDiffusionImageFilter<InputImageType, InputImageType>;
using ReaderType = itk::ImageFileReader<InputImageType>;
using WritePixelType = itk::RGBPixel<unsigned char>;
using WriteImageType = itk::Image<WritePixelType, 2>;
using CasterType = itk::CastImageFilter<InputImageType, WriteImageType>;

int main(int argc, char *argv[])
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());
    filter->SetNumberOfIterations(5);
    filter->SetTimeStep(0.125);
    filter->SetConductanceParameter(1.0);
    filter->Update();

    CasterType::Pointer caster = CasterType::New();
    caster->SetInput(filter->GetOutput());
    caster->Update();

    itk::ImageFileWriter<WriteImageType>::Pointer writer = itk::ImageFileWriter<WriteImageType>::New();
    writer->SetFileName("../../output/RGBGradientAnisotropicDiffusion.png");
    writer->SetInput(caster->GetOutput());
    writer->Update();
}
