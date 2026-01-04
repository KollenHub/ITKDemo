#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkThresholdImageFilter.h>
#include <itkImageRegionIterator.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include "ImageViewer.h"


using PixelType = unsigned char;
constexpr unsigned int Dimension = 2;
using ImageType = itk::Image<PixelType, Dimension>;
using ReaderType = itk::ImageFileReader<ImageType>;
using WriterType = itk::ImageFileWriter<ImageType>;
using ThresholdFilterType = itk::ThresholdImageFilter<ImageType>;

// 打印图像统计信息
void PrintImageStatistics(const ImageType* image, const std::string& name)
{
    itk::ImageRegionConstIterator<ImageType> it(image, image->GetLargestPossibleRegion());

    PixelType minVal = 255;
    PixelType maxVal = 0;
    double sum = 0;
    unsigned int count = 0;
    unsigned int nonZero = 0;

    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
        PixelType val = it.Get();
        if (val < minVal) minVal = val;
        if (val > maxVal) maxVal = val;
        if (val > 0) nonZero++;
        sum += val;
        ++count;
    }

    std::cout << "  " << name << ":\n";
    std::cout << "    尺寸: " << image->GetLargestPossibleRegion().GetSize()[0]
              << " x " << image->GetLargestPossibleRegion().GetSize()[1] << "\n";
    std::cout << "    Min: " << static_cast<int>(minVal) << "\n";
    std::cout << "    Max: " << static_cast<int>(maxVal) << "\n";
    std::cout << "    Mean: " << std::fixed << std::setprecision(2) << (sum / count) << "\n";
    std::cout << "    非零像素: " << nonZero << " / " << count << " ("
              << std::fixed << std::setprecision(1) << (100.0 * nonZero / count) << "%)\n";
}

// 方式1：ThresholdAbove - 保留高于阈值的像素
void TestThresholdAbove(ImageType::Pointer input, PixelType threshold, const std::string& outputFile)
{
    std::cout << "\n=== 方式1: ThresholdAbove (保留高于阈值的像素) ===\n";
    std::cout << "阈值: " << static_cast<int>(threshold) << "\n";
    std::cout << "说明: 像素值 > " << static_cast<int>(threshold) << " 保持不变，其他设为 " << static_cast<int>(0) << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    auto filter = ThresholdFilterType::New();
    filter->SetInput(input);
    filter->ThresholdAbove(threshold);
    filter->SetOutsideValue(0);

    try
    {
        filter->Update();
    }
    catch (const itk::ExceptionObject& ex)
    {
        std::cerr << "Error: " << ex << "\n";
        return;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    PrintImageStatistics(filter->GetOutput(), "输出图像");
    std::cout << "耗时: " << duration.count() << " μs\n";

    // 保存结果
    auto writer = WriterType::New();
    writer->SetFileName(outputFile);
    writer->SetInput(filter->GetOutput());
    try
    {
        writer->Update();
        std::cout << "已保存: " << outputFile << "\n";
    }
    catch (const itk::ExceptionObject& ex)
    {
        std::cerr << "保存失败: " << ex << "\n";
    }

    // 使用 ImageViewer 显示结果
    std::cout << "正在显示图像...\n";
    ImageViewer<PixelType> viewer("ThresholdAbove (> 180)");
    viewer.SetITKImage(filter->GetOutput());
    viewer.Show(0);  // 等待按键
}

// 方式2：ThresholdBelow - 保留低于阈值的像素
void TestThresholdBelow(ImageType::Pointer input, PixelType threshold, const std::string& outputFile)
{
    std::cout << "\n=== 方式2: ThresholdBelow (保留低于阈值的像素) ===\n";
    std::cout << "阈值: " << static_cast<int>(threshold) << "\n";
    std::cout << "说明: 像素值 < " << static_cast<int>(threshold) << " 保持不变，其他设为 " << static_cast<int>(255) << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    auto filter = ThresholdFilterType::New();
    filter->SetInput(input);
    filter->ThresholdBelow(threshold);
    filter->SetOutsideValue(255);

    try
    {
        filter->Update();
    }
    catch (const itk::ExceptionObject& ex)
    {
        std::cerr << "Error: " << ex << "\n";
        return;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    PrintImageStatistics(filter->GetOutput(), "输出图像");
    std::cout << "耗时: " << duration.count() << " μs\n";

    // 保存结果
    auto writer = WriterType::New();
    writer->SetFileName(outputFile);
    writer->SetInput(filter->GetOutput());
    try
    {
        writer->Update();
        std::cout << "已保存: " << outputFile << "\n";
    }
    catch (const itk::ExceptionObject& ex)
    {
        std::cerr << "保存失败: " << ex << "\n";
    }

    // 使用 ImageViewer 显示结果
    std::cout << "正在显示图像...\n";
    ImageViewer<PixelType> viewer("ThresholdBelow (< 180)");
    viewer.SetITKImage(filter->GetOutput());
    viewer.Show(0);  // 等待按键
}

// 方式3：ThresholdOutside - 保留在范围之外的像素（双阈值）
void TestThresholdOutside(ImageType::Pointer input, PixelType lower, PixelType upper, const std::string& outputFile)
{
    std::cout << "\n=== 方式3: ThresholdOutside (保留范围之外的像素) ===\n";
    std::cout << "下限: " << static_cast<int>(lower) << ", 上限: " << static_cast<int>(upper) << "\n";
    std::cout << "说明: 像素值在 [" << static_cast<int>(lower) << ", " << static_cast<int>(upper)
              << "] 范围内的设为 " << static_cast<int>(128) << "，范围外的保持不变\n";

    auto start = std::chrono::high_resolution_clock::now();

    auto filter = ThresholdFilterType::New();
    filter->SetInput(input);
    filter->ThresholdOutside(lower, upper);
    filter->SetOutsideValue(128);

    try
    {
        filter->Update();
    }
    catch (const itk::ExceptionObject& ex)
    {
        std::cerr << "Error: " << ex << "\n";
        return;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    PrintImageStatistics(filter->GetOutput(), "输出图像");
    std::cout << "耗时: " << duration.count() << " μs\n";

    // 保存结果
    auto writer = WriterType::New();
    writer->SetFileName(outputFile);
    writer->SetInput(filter->GetOutput());
    try
    {
        writer->Update();
        std::cout << "已保存: " << outputFile << "\n";
    }
    catch (const itk::ExceptionObject& ex)
    {
        std::cerr << "保存失败: " << ex << "\n";
    }

    // 使用 ImageViewer 显示结果
    std::cout << "正在显示图像...\n";
    ImageViewer<PixelType> viewer("ThresholdOutside (<170 或 >190)");
    viewer.SetITKImage(filter->GetOutput());
    viewer.Show(0);  // 等待按键
}

int main()
{
    std::cout << "========================================\n";
    std::cout << "ITK ThresholdImageFilter 三种方式对比测试\n";
    std::cout << "========================================\n";

    // 读取输入图像
    std::string inputFile = "../../data/BrainT1Slice.png";

    std::cout << "\n读取图像: " << inputFile << "\n";

    auto reader = ReaderType::New();
    reader->SetFileName(inputFile);

    try
    {
        reader->Update();
    }
    catch (const itk::ExceptionObject& ex)
    {
        std::cerr << "读取图像失败: " << ex << "\n";
        return EXIT_FAILURE;
    }

    auto inputImage = reader->GetOutput();

    std::cout << "\n输入图像信息:\n";
    PrintImageStatistics(inputImage, "原始图像");

    // 显示原始图像
    std::cout << "\n正在显示原始图像...\n";
    ImageViewer<PixelType> originalViewer("Original Image");
    originalViewer.SetITKImage(inputImage);
    originalViewer.Show(0);  // 等待按键关闭原始图像窗口

    // 测试三种方式
    // 根据脑部 MRI 图像的特点选择合适的阈值
    TestThresholdBelow(inputImage, 180, "threshold_below.png");
    TestThresholdAbove(inputImage, 180, "threshold_above.png");
    TestThresholdOutside(inputImage, 170, 190, "threshold_outside.png");

    std::cout << "\n========================================\n";
    std::cout << "测试完成！生成的图像文件:\n";
    std::cout << "  - threshold_below.png    (暗部区域: < 180)\n";
    std::cout << "  - threshold_above.png    (高亮区域: > 180)\n";
    std::cout << "  - threshold_outside.png  (两端区域: < 170 或 > 190)\n";
    std::cout << "\n三种方式对比:\n";
    std::cout << "  1. ThresholdBelow:  保留暗部区域 (< 180)，其他设为 255\n";
    std::cout << "  2. ThresholdAbove:  保留明亮区域 (> 180)，其他设为 0\n";
    std::cout << "  3. ThresholdOutside: 保留两端 (< 170 或 > 190)，中间 [170, 190] 设为 128\n";
    std::cout << "========================================\n";

    return 0;
}
