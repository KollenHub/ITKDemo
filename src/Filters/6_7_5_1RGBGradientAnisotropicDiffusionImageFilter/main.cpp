
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include <itkGradientRecursiveGaussianImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <ImageViewer.h>
#include <itkVectorIndexSelectionCastImageFilter.h>
#include "ImageViewer.h"

using InputPixelType = float;
using VectorPixelType = itk::CovariantVector<InputPixelType, 2>;
using InputImageType = itk::Image<InputPixelType, 2>;
using VectorImageType = itk::Image<VectorPixelType, 2>;
using FilterType = itk::VectorGradientAnisotropicDiffusionImageFilter<VectorImageType, VectorImageType>;
using ReaderType = itk::ImageFileReader<InputImageType>;
using WriterType = itk::ImageFileWriter<InputImageType>;

using GradientFilterType =
    itk::GradientRecursiveGaussianImageFilter<InputImageType,
                                              VectorImageType>;

using OutputPixelType = float;
using OutputImageType = itk::Image<OutputPixelType, 2>;
using ComponentFilterType =
    itk::VectorIndexSelectionCastImageFilter<VectorImageType,
                                             OutputImageType>;

using WritePixelType = unsigned char;
using WriteImageType = itk::Image<WritePixelType, 2>;
using RescaleFilterType =
    itk::RescaleIntensityImageFilter<OutputImageType, WriteImageType>;

// 向量曲先各向异性扩散
int main()
{

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    GradientFilterType::Pointer gradientFilter = GradientFilterType::New();

    gradientFilter->SetInput(reader->GetOutput());

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(gradientFilter->GetOutput());

    const unsigned int numberOfIterations = 5;
    const double timeStep = 0.25;

    filter->SetNumberOfIterations(numberOfIterations);
    filter->SetTimeStep(timeStep);
    filter->SetConductanceParameter(1.0);
    filter->Update();

    auto component = ComponentFilterType::New();

    // Select the component to extract.
    component->SetIndex(0);

    auto rescaler = RescaleFilterType::New();
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);

    using WriterType = itk::ImageFileWriter<WriteImageType>;
    auto writer = WriterType::New();
    rescaler->SetInput(component->GetOutput());
    writer->SetInput(rescaler->GetOutput());

    // Save the component of the original gradient
    component->SetInput(gradientFilter->GetOutput());
    writer->SetFileName("output/OriginalGradientComponent.png");
    writer->Update();

    // Save the component of the smoothed gradient
    component->SetInput(filter->GetOutput());
    writer->SetFileName("output/VectorCurvatureAnisotropicDiffusionImageFilter.png");
    writer->Update();

    return EXIT_SUCCESS;
}