#include "itkImageFileReader.h"
#include "itkGDCMImageIO.h"
#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"

using PixelType = unsigned short;
using ImageType = itk::Image<PixelType, 2>;
using ReaderType = itk::ImageFileReader<ImageType>;
using ImageIOType = itk::GDCMImageIO;
using DictionaryType = itk::MetaDataDictionary;
using MetaDataStringType = itk::MetaDataObject<std::string>;

using namespace std;
int main(int argc, char **argv)
{
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("D:/DICOM/DCM/011958333339.dcm");
    ImageIOType::Pointer dicomIO = ImageIOType::New();
    reader->SetImageIO(dicomIO);
    reader->Update();

    const DictionaryType &dictionary = dicomIO->GetMetaDataDictionary();
    DictionaryType::ConstIterator itr = dictionary.Begin();
    DictionaryType::ConstIterator end = dictionary.End();

    while (itr != end)
    {
        itk::MetaDataObjectBase::Pointer entry = itr->second;
        MetaDataStringType::Pointer entryValue = dynamic_cast<MetaDataStringType *>(entry.GetPointer());

        if (entryValue)
        {
            std::string tagkey = itr->first;
            std::string labelId;
            bool found = itk::GDCMImageIO::GetLabelFromTag(tagkey, labelId);
            std::string tagValue = entryValue->GetMetaDataObjectValue();
            if (found)
            {
                std::cout << "(" << tagkey << ") " << labelId << " == " << tagValue << std::endl;
            }
            ++itr;
        }
    }

    std::string entryId = "0010|0010";

    DictionaryType::ConstIterator entryItr = dictionary.Find(entryId);

    if (entryItr != dictionary.End())
    {
        MetaDataStringType::Pointer entryValue = dynamic_cast<MetaDataStringType *>(entryItr->second.GetPointer());

        if (entryValue)
        {
            std::string tagValue = entryValue->GetMetaDataObjectValue();
            std::cout << "Patient's Name (" << entryId << ") is" << tagValue << std::endl;
        }
    }

    return EXIT_SUCCESS;
}