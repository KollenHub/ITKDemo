#include <gdcmImageReader.h>
#include <gdcmImage.h>
#include <gdcmDataSet.h>
#include <gdcmFile.h>
#include <gdcmTag.h>
#include <iostream>
#include <vector>

int main()
{
    // Create GDCM ImageReader
    gdcm::ImageReader reader;
    const char* filename = "D:/DICOM/data1/output_slice_1.dcm";  // Change to your DICOM file path
    reader.SetFileName(filename);

    // Read file
    if (!reader.Read())
    {
        std::cerr << "Failed to read DICOM file: " << filename << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "DICOM file read successfully!" << std::endl;

    // Get image object
    const gdcm::Image& image = reader.GetImage();

    // Print image information
    std::cout << "\n========== Image Info ==========" << std::endl;

    const unsigned int* dimensions = image.GetDimensions();
    std::cout << "Dimensions: " << dimensions[0] << " x " << dimensions[1];
    if (image.GetNumberOfDimensions() > 2)
        std::cout << " x " << dimensions[2];
    std::cout << std::endl;

    const gdcm::PixelFormat& pixelFormat = image.GetPixelFormat();
    std::cout << "Pixel type: " << pixelFormat << std::endl;
    std::cout << "Pixel size: " << pixelFormat.GetPixelSize() << " bytes" << std::endl;

    const double* spacing = image.GetSpacing();
    std::cout << "Spacing: (" << spacing[0] << ", " << spacing[1];
    if (image.GetNumberOfDimensions() > 2)
        std::cout << ", " << spacing[2];
    std::cout << ")" << std::endl;

    const double* origin = image.GetOrigin();
    std::cout << "Origin: (" << origin[0] << ", " << origin[1];
    if (image.GetNumberOfDimensions() > 2)
        std::cout << ", " << origin[2];
    std::cout << ")" << std::endl;

    // Get file and dataset (read DICOM tags)
    const gdcm::File& dicomFile = reader.GetFile();
    const gdcm::DataSet& dataset = dicomFile.GetDataSet();

    std::cout << "\n========== DICOM Tags ==========" << std::endl;

    // Common tags to read
    struct DicomTag {
        const char* tag;
        const char* name;
    };

    DicomTag tags[] = {
        {"0010|0010", "Patient Name"},
        {"0010|0020", "Patient ID"},
        {"0008|0060", "Modality"},
        {"0008|0020", "Study Date"},
        {"0008|0030", "Study Time"},
        {"0008|0050", "Accession Number"},
        {"0008|0080", "Institution Name"},
        {"0020|000D", "Study Instance UID"},
        {"0020|000E", "Series Instance UID"},
        {"0028|0010", "Rows"},
        {"0028|0011", "Columns"},
        {"0028|0030", "Pixel Spacing"},
        {"0028|1050", "Window Center"},
        {"0028|1051", "Window Width"},
    };

    for (const auto& tagInfo : tags)
    {
        gdcm::Tag t;
        t.ReadFromCommaSeparatedString(tagInfo.tag);

        if (dataset.FindDataElement(t))
        {
            const gdcm::DataElement& de = dataset.GetDataElement(t);
            if (!de.IsEmpty())
            {
                const gdcm::ByteValue* bv = de.GetByteValue();
                if (bv && bv->GetPointer())
                {
                    std::string value(bv->GetPointer(), bv->GetLength());
                    // Remove trailing spaces and null characters
                    while (!value.empty() && (value.back() == ' ' || value.back() == '\0'))
                        value.pop_back();
                    std::cout << tagInfo.name << " (" << tagInfo.tag << "): " << value << std::endl;
                }
            }
        }
    }

    // Read pixel data
    std::cout << "\n========== Pixel Data ==========" << std::endl;

    // Calculate total number of pixels
    size_t pixelCount = 1;
    for (unsigned int i = 0; i < image.GetNumberOfDimensions(); ++i)
        pixelCount *= dimensions[i];

    std::cout << "Total pixels: " << pixelCount << std::endl;

    // Get buffer size (bytes)
    size_t bufferLen = image.GetBufferLength();
    std::cout << "Buffer size: " << bufferLen << " bytes" << std::endl;

    // Allocate memory and read pixel data
    std::vector<char> buffer(bufferLen);
    if (image.GetBuffer(buffer.data()))
    {
        std::cout << "Pixel data read successfully!" << std::endl;

        // Access first pixel as short (assuming 16-bit medical image)
        if (bufferLen >= 2)
        {
            short firstPixel = *reinterpret_cast<short*>(buffer.data());
            std::cout << "First pixel value: " << firstPixel << std::endl;
        }
    }
    else
    {
        std::cerr << "Pixel data read failed!" << std::endl;
    }

    return EXIT_SUCCESS;
}
