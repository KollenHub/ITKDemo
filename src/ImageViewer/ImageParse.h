#pragma once
#include <opencv2/opencv.hpp>
#include <itkImage.h>
#include <itkImageRegionConstIterator.h>
#include <stdexcept>

// 确定对应的 OpenCV 类型
template <typename PixelType>
int GetOpenCVType();

template <>
inline int GetOpenCVType<unsigned char>() { return CV_8U; }

template <>
inline int GetOpenCVType<char>() { return CV_8S; }

template <>
inline int GetOpenCVType<unsigned short>() { return CV_16U; }

template <>
inline int GetOpenCVType<short>() { return CV_16S; }

template <>
inline int GetOpenCVType<int>() { return CV_32S; }

template <>
inline int GetOpenCVType<float>() { return CV_32F; }

template <>
inline int GetOpenCVType<double>() { return CV_64F; }

template <typename PixelType>
class ImageParse
{
private:
    cv::Mat m_image;

public:
    ImageParse(typename itk::Image<PixelType, 2>::Pointer itkImg)
    {
        if (!itkImg)
        {
            throw std::invalid_argument("ITK image is null");
        }

        // 获取图像尺寸
        const auto size = itkImg->GetLargestPossibleRegion().GetSize();
        const int width = static_cast<int>(size[0]);
        const int height = static_cast<int>(size[1]);

        // 确定 OpenCV 类型
        const int cvType = GetOpenCVType<PixelType>();

        // 创建 OpenCV Mat
        m_image.create(height, width, cvType);

        // 使用迭代器复制数据
        using ImageType = itk::Image<PixelType, 2>;
        using ConstIteratorType = itk::ImageRegionConstIterator<ImageType>;

        typename ImageType::RegionType region = itkImg->GetLargestPossibleRegion();
        ConstIteratorType it(itkImg, region);

        it.GoToBegin();

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x, ++it)
            {
                m_image.at<PixelType>(y, x) = it.Get();
            }
        }
    }

    ~ImageParse() = default;

    cv::Mat GetMat() const { return m_image; }
};
