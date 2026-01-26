#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkImageFileWriter.h"

using PixelType = unsigned char;
using ImageType = itk::Image<PixelType, 3>;
using ReaderType = itk::ImageSeriesReader<ImageType>;
using ImageIOType = itk::GDCMImageIO;
using NamesGeneratorType = itk::GDCMSeriesFileNames;
using WriterType = itk::ImageFileWriter<ImageType>;

using namespace std;
int main(int argc, char **argv)
{
    ReaderType::Pointer reader = ReaderType::New();

    ImageIOType::Pointer dicomIO = ImageIOType::New();

    reader->SetImageIO(dicomIO);

    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

    nameGenerator->SetUseSeriesDetails(true);

     nameGenerator->SetDirectory("D:/DICOM/Subject");

    vector<string> seriesUID = nameGenerator->GetSeriesUIDs();

    for (auto &uid : seriesUID)
    {
        cout << uid << endl;
    }

    string seriesIdentifier;

    seriesIdentifier = seriesUID[0];

    std::vector<std::string> filenames = nameGenerator->GetFileNames(seriesIdentifier);

    reader->SetFileNames(filenames);

    try
    {
        reader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }

    WriterType::Pointer writer = WriterType::New();

    writer->SetInput(reader->GetOutput());

    writer->SetFileName("D:/DICOM/data1/output.dcm");

    try
    {
        writer->Update();
    }
    catch (itk::ExceptionObject &e)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }
}