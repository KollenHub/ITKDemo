#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <optional>
#include "ImageParse.h"

template <typename PixelType>
class ImageViewer
{
private:
    std::string m_windowName;
    std::optional<ImageParse<PixelType>> m_imageParser;
    bool m_windowCreated;

public:
    ImageViewer(const std::string& windowName = "Image Viewer")
        : m_windowName(windowName), m_windowCreated(false)
    {
    }

    ~ImageViewer()
    {
        // 只销毁自己创建的窗口
        if (m_windowCreated)
        {
            cv::destroyWindow(m_windowName);
        }
    }

    // 禁止拷贝构造和拷贝赋值（避免窗口重复销毁）
    ImageViewer(const ImageViewer&) = delete;
    ImageViewer& operator=(const ImageViewer&) = delete;

    // 支持移动
    ImageViewer(ImageViewer&& other) noexcept
        : m_windowName(std::move(other.m_windowName))
        , m_imageParser(std::move(other.m_imageParser))
        , m_windowCreated(other.m_windowCreated)
    {
        other.m_windowCreated = false;
    }

    ImageViewer& operator=(ImageViewer&& other) noexcept
    {
        if (this != &other)
        {
            if (m_windowCreated)
            {
                cv::destroyWindow(m_windowName);
            }
            m_windowName = std::move(other.m_windowName);
            m_imageParser = std::move(other.m_imageParser);
            m_windowCreated = other.m_windowCreated;
            other.m_windowCreated = false;
        }
        return *this;
    }

    // 设置 ImageParse 对象（存储副本）
    void SetImage(const ImageParse<PixelType>& parser)
    {
        m_imageParser = parser;
    }

    // 设置 ImageParse 对象（移动语义）
    void SetImage(ImageParse<PixelType>&& parser)
    {
        m_imageParser = std::move(parser);
    }

    // 从 ITK 图像直接设置
    void SetITKImage(typename itk::Image<PixelType, 2>::Pointer itkImg)
    {
        m_imageParser = ImageParse<PixelType>(itkImg);
    }

    // 显示图像
    void Show(int delay = 0)
    {
        if (!m_imageParser)
        {
            throw std::runtime_error("No image to display");
        }

        // 懒创建窗口
        if (!m_windowCreated)
        {
            cv::namedWindow(m_windowName, cv::WINDOW_AUTOSIZE);
            m_windowCreated = true;
        }

        cv::Mat mat = m_imageParser->GetMat();
        cv::imshow(m_windowName, mat);
        cv::waitKey(delay);
    }

    void Update()
    {
        if (!m_imageParser)
        {
            throw std::runtime_error("No image to display");
        }

        // 懒创建窗口
        if (!m_windowCreated)
        {
            cv::namedWindow(m_windowName, cv::WINDOW_AUTOSIZE);
            m_windowCreated = true;
        }

        cv::Mat mat = m_imageParser->GetMat();
        cv::imshow(m_windowName, mat);
    }

    // 获取窗口名称
    std::string GetWindowName() const { return m_windowName; }

    // 设置窗口位置
    void MoveWindow(int x, int y) const
    {
        cv::moveWindow(m_windowName, x, y);
    }

    // 调整窗口大小
    void ResizeWindow(int width, int height) const
    {
        cv::resizeWindow(m_windowName, width, height);
    }
};
