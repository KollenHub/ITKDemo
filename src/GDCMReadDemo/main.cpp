#include <gdcmImageReader.h>
#include <gdcmImage.h>
#include <gdcmPixelFormat.h>
#include <gdcmFile.h>
#include <gdcmDataSet.h>
#include <gdcmTag.h>
#include <gdcmAttribute.h>
#include <gdcmReader.h>
#include <gdcmTransferSyntax.h>
#include <gdcmTrace.h>

#include "itkDCMTKImageIO.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"

#include <iostream>
#include <array>
#include <vector>
#include "gdcmSwapper.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

namespace
{
bool TryParseDouble(const std::string &text, double &value)
{
    try
    {
        value = std::stod(text);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool TryParseInt(const std::string &text, int &value)
{
    try
    {
        value = std::stoi(text);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

template <unsigned int Dimension>
bool TryReadWithDCMTKImpl(const char *filename)
{
    using PixelType = unsigned short;
    using ImageType = itk::Image<PixelType, Dimension>;
    using ReaderType = itk::ImageFileReader<ImageType>;

    ReaderType::Pointer reader = ReaderType::New();
    itk::DCMTKImageIO::Pointer dicomIO = itk::DCMTKImageIO::New();
    reader->SetImageIO(dicomIO);
    reader->SetFileName(filename);

    try
    {
        reader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
        std::cerr << "DCMTK fallback failed: " << e << std::endl;
        return false;
    }

    ImageType::Pointer image = reader->GetOutput();
    const auto size = image->GetLargestPossibleRegion().GetSize();
    std::cout << "Image Size: " << size[0] << " x " << size[1];
    if (Dimension > 2)
    {
        std::cout << " x " << size[2];
    }
    else
    {
        std::cout << " x 1";
    }
    std::cout << std::endl;

    const itk::MetaDataDictionary &dict = dicomIO->GetMetaDataDictionary();
    std::string text;
    int intValue = 0;

    if (itk::ExposeMetaData<std::string>(dict, "0028|0100", text) && TryParseInt(text, intValue))
    {
        std::cout << "Bits Allocated : " << intValue << std::endl;
    }
    else
    {
        std::cout << "Bits Allocated : " << (sizeof(PixelType) * 8) << std::endl;
    }

    if (itk::ExposeMetaData<std::string>(dict, "0028|0101", text) && TryParseInt(text, intValue))
    {
        std::cout << "Bits Stored    : " << intValue << std::endl;
    }
    else
    {
        std::cout << "Bits Stored    : " << (sizeof(PixelType) * 8) << std::endl;
    }

    if (itk::ExposeMetaData<std::string>(dict, "0028|0102", text) && TryParseInt(text, intValue))
    {
        std::cout << "High Bit       : " << intValue << std::endl;
    }
    else
    {
        std::cout << "High Bit       : " << (sizeof(PixelType) * 8 - 1) << std::endl;
    }

    if (itk::ExposeMetaData<std::string>(dict, "0028|0103", text) && TryParseInt(text, intValue))
    {
        std::cout << "Signed         : " << (intValue ? "Yes" : "No") << std::endl;
    }
    else
    {
        std::cout << "Signed         : " << "No" << std::endl;
    }

    double wc = 0.0, ww = 0.0;
    if (itk::ExposeMetaData<std::string>(dict, "0028|1050", text) && TryParseDouble(text, wc) &&
        itk::ExposeMetaData<std::string>(dict, "0028|1051", text) && TryParseDouble(text, ww))
    {
        std::cout << "Window Center : " << wc << std::endl;
        std::cout << "Window Width  : " << ww << std::endl;
    }
    else
    {
        std::cout << "No Window info found\n";
    }

    double slope = 1.0;
    double intercept = 0.0;

    if (itk::ExposeMetaData<std::string>(dict, "0028|1053", text))
    {
        TryParseDouble(text, slope);
    }

    if (itk::ExposeMetaData<std::string>(dict, "0028|1052", text))
    {
        TryParseDouble(text, intercept);
    }

    std::cout << "Rescale Slope     : " << slope << std::endl;
    std::cout << "Rescale Intercept : " << intercept << std::endl;

    const auto pixelCount = image->GetPixelContainer()->Size();
    const size_t bufferLength = pixelCount * sizeof(PixelType);
    std::cout << "Pixel buffer size: " << bufferLength << " bytes" << std::endl;
    return true;
}

bool TryReadWithDCMTK(const char *filename)
{
    if (TryReadWithDCMTKImpl<2>(filename))
    {
        return true;
    }
    return TryReadWithDCMTKImpl<3>(filename);
}
} // namespace

int main(int argc, char *argv[])
{
    const char *filename = "D:/DICOM/data1/output_slice_1.dcm";

    // std::ifstream is(filename, std::ios::binary);
    // if (!is)
    // {
    //     std::cerr << "open failed\n";
    //     return 1;
    // }

    // // 1. 检查 DICM
    // char dicm[4] = {};
    // is.seekg(128, std::ios::beg);
    // is.read(dicm, 4);

    // std::cout << "DICM = '"
    //           << dicm[0] << dicm[1] << dicm[2] << dicm[3]
    //           << "'\n";

    // // 2. 读第一个 Tag
    // gdcm::Tag tag;
    // is.read(reinterpret_cast<char *>(&tag), 4);

    // std::cout << std::hex << std::setfill('0')
    //           << "First Tag = ("
    //           << std::setw(4) << tag.GetGroup() << ","
    //           << std::setw(4) << tag.GetElement() << ")\n";

    // return 0;

    // 1️⃣ 读取 DICOM
    gdcm::Trace::WarningOn();
    gdcm::Trace::ErrorOn();

    gdcm::ImageReader reader;
    reader.SetFileName(filename);

    gdcm::ImageReader fallbackReader;
    gdcm::Reader dsReader;
    bool useFallbackReader = false;

    bool gdcmReadOk = false;
    try
    {
        gdcmReadOk = reader.Read();
    }
    catch (const gdcm::Exception &e)
    {
        std::cerr << "GDCM ImageReader exception: " << e.what() << std::endl;
        gdcmReadOk = false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "GDCM ImageReader std::exception: " << e.what() << std::endl;
        gdcmReadOk = false;
    }

    if (!gdcmReadOk)
    {
        std::cerr << "GDCM ImageReader failed, try dataset fallback: " << filename << std::endl;

        dsReader.SetFileName(filename);
        bool dsReadOk = false;
        try
        {
            dsReadOk = dsReader.Read();
        }
        catch (const gdcm::Exception &e)
        {
            std::cerr << "GDCM Reader exception: " << e.what() << std::endl;
            dsReadOk = false;
        }
        catch (const std::exception &e)
        {
            std::cerr << "GDCM Reader std::exception: " << e.what() << std::endl;
            dsReadOk = false;
        }

        if (!dsReadOk)
        {
            std::cerr << "Failed to read dataset: " << filename << std::endl;
            if (TryReadWithDCMTK(filename))
            {
                return 0;
            }
            return -1;
        }

        gdcm::File &file = dsReader.GetFile();
        gdcm::FileMetaInformation &hdr = file.GetHeader();

        bool readOk = false;
        const bool hasHeaderTS = hdr.GetDataSetTransferSyntax().IsValid();
        gdcm::TransferSyntax headerTS;

        if (hasHeaderTS)
        {
            headerTS = hdr.GetDataSetTransferSyntax();
            fallbackReader.SetFile(file);
            try
            {
                readOk = fallbackReader.Read();
            }
            catch (const gdcm::Exception &e)
            {
                std::cerr << "GDCM fallback exception: " << e.what() << std::endl;
                readOk = false;
            }
            catch (const std::exception &e)
            {
                std::cerr << "GDCM fallback std::exception: " << e.what() << std::endl;
                readOk = false;
            }
        }

        if (!readOk)
        {
            // Part-10 header missing/invalid or header TS failed; try common transfer syntaxes.
            const std::array<gdcm::TransferSyntax::TSType, 3> guesses = {
                gdcm::TransferSyntax::ImplicitVRLittleEndian,
                gdcm::TransferSyntax::ExplicitVRLittleEndian,
                gdcm::TransferSyntax::ExplicitVRBigEndian,
            };

            for (const auto ts : guesses)
            {
                if (hasHeaderTS && headerTS == ts)
                {
                    continue;
                }

                hdr.SetDataSetTransferSyntax(gdcm::TransferSyntax(ts));
                fallbackReader.SetFile(file);
                bool fallbackOk = false;
                try
                {
                    fallbackOk = fallbackReader.Read();
                }
                catch (const gdcm::Exception &e)
                {
                    std::cerr << "GDCM fallback exception: " << e.what() << std::endl;
                    fallbackOk = false;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "GDCM fallback std::exception: " << e.what() << std::endl;
                    fallbackOk = false;
                }

                if (fallbackOk)
                {
                    readOk = true;
                    break;
                }
            }
        }

        if (!readOk)
        {
            std::cerr << "Fallback read failed after trying transfer syntaxes.\n";
            if (TryReadWithDCMTK(filename))
            {
                return 0;
            }
            return -1;
        }

        useFallbackReader = true;
    }

    const gdcm::Image &image = useFallbackReader ? fallbackReader.GetImage() : reader.GetImage();
    const gdcm::File &file = useFallbackReader ? fallbackReader.GetFile() : reader.GetFile();
    const gdcm::DataSet &ds = file.GetDataSet();

    // 2️⃣ 基础尺寸信息
    const unsigned int *dims = image.GetDimensions();

    std::cout << "Image Size: "
              << dims[0] << " x "
              << dims[1] << " x "
              << dims[2] << std::endl;

    // 3️⃣ 像素格式
    const gdcm::PixelFormat &pf = image.GetPixelFormat();

    std::cout << "Bits Allocated : " << pf.GetBitsAllocated() << std::endl;
    std::cout << "Bits Stored    : " << pf.GetBitsStored() << std::endl;
    std::cout << "High Bit       : " << pf.GetHighBit() << std::endl;
    std::cout << "Signed         : "
              << (pf.GetPixelRepresentation() ? "Yes" : "No") << std::endl;

    // 4️⃣ 窗宽窗位
    double wc = 0.0, ww = 0.0;
    if (ds.FindDataElement(gdcm::Tag(0x0028, 0x1050)) &&
        ds.FindDataElement(gdcm::Tag(0x0028, 0x1051)))
    {
        gdcm::Attribute<0x0028, 0x1050> atWC;
        gdcm::Attribute<0x0028, 0x1051> atWW;

        atWC.SetFromDataSet(ds);
        atWW.SetFromDataSet(ds);

        wc = atWC.GetValue();
        ww = atWW.GetValue();

        std::cout << "Window Center : " << wc << std::endl;
        std::cout << "Window Width  : " << ww << std::endl;
    }
    else
    {
        std::cout << "No Window info found\n";
    }

    // 5️⃣ Rescale (CT HU)
    double slope = 1.0;
    double intercept = 0.0;

    if (ds.FindDataElement(gdcm::Tag(0x0028, 0x1053)))
    {
        gdcm::Attribute<0x0028, 0x1053> at;
        at.SetFromDataSet(ds);
        slope = at.GetValue();
    }

    if (ds.FindDataElement(gdcm::Tag(0x0028, 0x1052)))
    {
        gdcm::Attribute<0x0028, 0x1052> at;
        at.SetFromDataSet(ds);
        intercept = at.GetValue();
    }

    std::cout << "Rescale Slope     : " << slope << std::endl;
    std::cout << "Rescale Intercept : " << intercept << std::endl;

    // 6️⃣ 读取 Pixel Data
    size_t bufferLength = image.GetBufferLength();
    std::vector<char> pixelBuffer(bufferLength);

    if (!image.GetBuffer(pixelBuffer.data()))
    {
        std::cerr << "Failed to read pixel data\n";
        return -1;
    }

    std::cout << "Pixel buffer size: " << bufferLength << " bytes" << std::endl;

    return 0;
}
