#include "itkResampleImageFilter.h"
#include "itkAffineTransform.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

using InputPixelType = unsigned char;
using OutputPixelType = unsigned char;

using InputImageType = itk::Image<InputPixelType, 2>;
using OutputImageType = itk::Image<OutputPixelType, 2>;

using ReaderType = itk::ImageFileReader<InputImageType>;
using WriterType = itk::ImageFileWriter<OutputImageType>;

using TransformType = itk::AffineTransform<double, 2>;
using FilterType = itk::ResampleImageFilter<InputImageType, OutputImageType>;
using InterpolatorType = itk::NearestNeighborInterpolateImageFunction<InputImageType, double>;

int main(int argc, char **argv)
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("../../data/BrainProtonDensitySlice.png");
    reader->Update();

    FilterType::Pointer filter = FilterType::New();

    TransformType::Pointer transform = TransformType::New();
    TransformType::TranslationType translation;
    // 移动{30,30}
    translation[0] = -30;
    translation[1] = -30;
    transform->Translate(translation);

    InterpolatorType::Pointer interpolator = InterpolatorType::New();

    filter->SetTransform(transform);
    filter->SetInterpolator(interpolator);

    filter->SetDefaultPixelValue(0);
    double spacing[2];
    spacing[0] = 1.0;
    spacing[1] = 1.0;

    filter->SetOutputSpacing(spacing);
    double origin[2];
    origin[0] = 0.0;
    origin[1] = 0.0;
    filter->SetOutputOrigin(origin);

    InputImageType::SizeType size;
    size[0] = 300;
    size[1] = 300;
    filter->SetSize(size);

    filter->SetInput(reader->GetOutput());

    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(filter->GetOutput());
    writer->SetFileName("../../output/ResampleImageFilter.png");
    writer->Update();

    return 0;
}