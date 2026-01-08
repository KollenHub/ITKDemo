# ITK 测试项目

## 项目结构

```
src/
├── Filters/              # ITK 滤波器示例
│   ├── 6_1_1ThresholdImageFilter/
│   ├── 6_3_2SigmoidImageFilter/
│   ├── 6_4_1GradientMagnitudeImageFilter/
│   ├── 6_4_1GradientMangnitudeRecursiveGaussianImageFilter/
│   ├── 6_4_2DerivativeImageFilter/
│   ├── 6_5_2LaplacianRecursiveGaussianImageFilter/
│   ├── 6_6_1MeanImageFilter/
│   ├── 6_6_2MedianImageFilter/
│   ├── 6_6_3MathematicalMorphologyBinaryFilter/
│   ├── 6_6_3MathematicalMorpologyGrayscaleFilter/
│   ├── 6_6_4BinaryMedianImageFilter/
│   └── 6_6_4VotingBinaryHoleFillingImageFilter/
└── ImageViewer/          # 图像查看器
    ├── ImageViewer.h
    ├── ImageParse.h
    └── ImageParse.cpp
```

## Filters

### 6_1_1 ThresholdImageFilter 阈值滤波
- **路径**: `src/Filters/6_1_1ThresholdImageFilter/`
- **说明**: 演示 ITK 阈值滤波器的使用

### 6_3_2 SigmoidImageFilter 非线性滤波
- **路径**: `src/Filters/6_3_2SigmoidImageFilter/`
- **说明**: 演示 ITK Sigmoid 非线性滤波器的使用

### 6_4_1 GradientMagnitudeImageFilter 梯度滤波
- **路径**: `src/Filters/6_4_1GradientMagnitudeImageFilter/`
- **说明**: 演示 ITK 梯度幅值滤波器的使用

### 6_4_1 GradientMangnitudeRecursiveGaussianImageFilter 递归高斯梯度滤波
- **路径**: `src/Filters/6_4_1GradientMangnitudeRecursiveGaussianImageFilter/`
- **说明**: 演示 ITK 递归高斯梯度幅值滤波器的使用

### 6_4_2 DerivativeImageFilter 导数滤波
- **路径**: `src/Filters/6_4_2DerivativeImageFilter/`
- **说明**: 演示 ITK 导数滤波器的使用

### 6_5_2 LaplacianRecursiveGaussianImageFilter 拉普拉斯滤波
- **路径**: `src/Filters/6_5_2LaplacianRecursiveGaussianImageFilter/`
- **说明**: 演示 ITK 递归高斯拉普拉斯滤波器的使用

### 6_6_1 MeanImageFilter 均值滤波
- **路径**: `src/Filters/6_6_1MeanImageFilter/`
- **说明**: 演示 ITK 均值滤波器的使用

### 6_6_2 MedianImageFilter 中值滤波
- **路径**: `src/Filters/6_6_2MedianImageFilter/`
- **说明**: 演示 ITK 中值滤波器的使用

### 6_6_3 MathematicalMorphologyBinaryFilter 二值数学形态学
- **路径**: `src/Filters/6_6_3MathematicalMorphologyBinaryFilter/`
- **说明**: 演示 ITK 二值数学形态学滤波器的使用

### 6_6_3 MathematicalMorpologyGrayscaleFilter 灰度数学形态学
- **路径**: `src/Filters/6_6_3MathematicalMorpologyGrayscaleFilter/`
- **说明**: 演示 ITK 灰度数学形态学滤波器的使用

### 6_6_4 BinaryMedianImageFilter 二值中值滤波
- **路径**: `src/Filters/6_6_4BinaryMedianImageFilter/`
- **说明**: 演示 ITK 二值中值滤波器的使用

### 6_6_4 VotingBinaryHoleFillingImageFilter 投票二值孔洞填充
- **路径**: `src/Filters/6_6_4VotingBinaryHoleFillingImageFilter/`
- **说明**: 演示 ITK 二值孔洞填充滤波器的使用

## ImageViewer

- **路径**: `src/ImageViewer/`
- **文件**:
  - `ImageViewer.h` - 图像查看器头文件
  - `ImageParse.h` - 图像解析头文件
  - `ImageParse.cpp` - 图像解析实现
- **说明**: 图像显示和解析模块