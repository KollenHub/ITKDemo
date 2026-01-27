
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

using PixelType = unsigned short;
using ImageType = itk::Image<PixelType, 3>;
using ReaderType = itk::ImageSeriesReader<ImageType>;
using ImageIOType = itk::GDCMImageIO;
using NamesGeneratorType = itk::GDCMSeriesFileNames;
using DictionaryType = itk::MetaDataDictionary;
using MetaDataStringType = itk::MetaDataObject<std::string>;
int main(int argc, char **argv)
{
    ReaderType::Pointer reader = ReaderType::New();

    ImageIOType::Pointer imageIO = ImageIOType::New();

    reader->SetImageIO(imageIO);

    NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
    namesGenerator->SetUseSeriesDetails(true);
    // namesGenerator->SetInputDirectory("D:/DICOM/data1");
    namesGenerator->SetDirectory("D:/DICOM/data2");
    
    std::vector<std::string> fileNames = namesGenerator->GetInputFileNames();
    reader->SetFileNames(fileNames);

    reader->Update();

    const DictionaryType &dictionary = imageIO->GetMetaDataDictionary();

    for (auto it = dictionary.Begin(); it != dictionary.End(); ++it)
    {
        itk::MetaDataObjectBase::Pointer entry = it->second;
        MetaDataStringType::Pointer entryvalue = dynamic_cast<MetaDataStringType *>(entry.GetPointer());

        if (entryvalue)
        {
            std::string tagkey = it->first;

            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            std::cout << tagkey << "==" << tagvalue << std::endl;
        }
    }

    std::string entryId = "0010|0010";

    auto findIt = dictionary.Find(entryId);
    if (findIt != dictionary.End())
    {
        itk::MetaDataObjectBase::Pointer entry = findIt->second;

        MetaDataStringType::Pointer entryvalue = dynamic_cast<MetaDataStringType *>(entry.GetPointer());

        if (entryvalue)
        {
            std::cout << "PatientName: " << entryvalue->GetMetaDataObjectValue() << std::endl;
        }
    }

    return 0;
}