# ITK 测试项目

## 项目结构

```
src/
├── Filters/              # ITK 滤波器示例
│   ├── 6_1_1ThresholdImageFilter/
│   ├── 6_3_2SigmoidImageFilter/
│   └── 6_4_1GradientMagnitudeImageFilter/
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

## ImageViewer

- **路径**: `src/ImageViewer/`
- **文件**:
  - `ImageViewer.h` - 图像查看器头文件
  - `ImageParse.h` - 图像解析头文件
  - `ImageParse.cpp` - 图像解析实现
- **说明**: 图像显示和解析模块