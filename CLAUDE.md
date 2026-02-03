# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 构建和运行

```bash
# 配置项目（首次或更改 CMakeLists.txt 后）
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# 构建所有目标
cmake --build build --config Release

# 构建特定目标（单个示例）
cmake --build build --config Release --target 6_9_3ResampleImageFilter

# 或者直接在 build 目录下使用 MSBuild
cd build
msbuild ITK_Demo.sln /p:Configuration=Release

# 运行可执行文件
.\bin\Release\6_9_3ResampleImageFilter.exe
```

输出目录：`bin/Debug/` 或 `bin/Release/` 包含所有可执行文件和自动复制的 DLL。

**注意**: 外部依赖路径需修改 `PackagePath.config`（ITK_DIR、OpenCV_DIR、GDCM_DIR）。

## 高层架构

### 核心模块

- `src/ImageViewer/` - **ImageCore** 静态库
  - `ImageParse<PixelType>`: ITK Image ↔ OpenCV Mat 转换模板
  - `ImageViewer`: OpenCV 显示封装（`imshow` 包装）

- `src/Filters/` - 独立可执行文件，演示各类 ITK 滤波器：
  - 平滑：均值/中值/双边滤波、各向异性扩散
  - 梯度/导数：梯度幅值、拉普拉斯、递归高斯
  - 形态学：二值/灰度形态学、孔洞填充
  - 重采样：`ResampleImageFilter` + `AffineTransform`

- `src/Example/` - 基础读写示例：
  - ITK 图像类型定义与手动类型使用
  - GDCM 单文件读取（`GDCMImageIO` + `ImageFileReader`）
  - DICOM 序列读取（`GDCMSeriesFileNames` + `ImageSeriesReader`）
  - DICOM 标签读取

- `cmake/` - 自定义 CMake 辅助函数：
  - `directory.cmake`: 目录遍历（`get_direct_subdirectories`）
  - `binary_finder.cmake`: 自动收集并复制 DLL 到输出目录
  - `package.cmake`: 包路径解析

### DICOM 读取数据流

```
DICOM 目录
  ↓
GDCMSeriesFileNames::SetInputDirectory()  // 扫描获取文件列表
  ↓
GDCMImageIO  // 指定 DICOM 解码器
  ↓
ImageSeriesReader::SetImageIO() + SetFileNames()  // ITK 读取 3D Volume
  ↓
ITK Image
  ↓
Filter（如 ResampleImageFilter）
  ↓
ImageParse::ITKToMat()  // ITK → OpenCV Mat
  ↓
ImageViewer::Show()  // 显示
```

### 关键设计

- **ImageCore** 作为共享库被所有 Filter 示例链接（`target_link_libraries(... PRIVATE ImageCore)`）
- CMake 自动遍历 `src/` 下所有子目录的 `.cpp` 并生成可执行文件
- 硬编码路径：输入在 `data/`，输出在 `output/`，部分示例使用 D 盘 DICOM 路径（需修改）

## 依赖说明

- ITK 6.0+（ITKSmoothing、ITKIOGDCM、ITKIODCMTK 等模块）
- OpenCV 4.10
- GDCM 3.22

无测试框架、lint 或格式化工具配置，需要手动运行可执行文件验证功能。
