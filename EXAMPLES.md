# 颜色插值使用示例 / Color Interpolation Usage Examples

本文档展示如何使用 CMCoreType 中的颜色插值功能。

This document demonstrates how to use the color interpolation features in CMCoreType.

## 基础示例 / Basic Examples

### 1. 线性插值 / Linear Interpolation

```cpp
#include<hgl/color/Color3f.h>
#include<hgl/color/ColorLerp.h>

Color3f red(1.0f, 0.0f, 0.0f);
Color3f blue(0.0f, 0.0f, 1.0f);

// 方法 1: 使用成员方法 / Method 1: Using member method
Color3f purple = red;
purple.lerp(blue, 0.5f);  // 50% 在红色和蓝色之间

// 方法 2: 使用全局函数 / Method 2: Using global function
Color3f purple2 = ColorLerp(red, blue, 0.5f);
```

### 2. 余弦插值 / Cosine Interpolation (Smooth)

余弦插值提供比线性插值更平滑的过渡，特别适合动画。

Cosine interpolation provides smoother transitions than linear, ideal for animations.

```cpp
Color3f start(1.0f, 0.0f, 0.0f);  // 红色 / Red
Color3f end(0.0f, 1.0f, 0.0f);    // 绿色 / Green

// 使用成员方法 / Using member method
start.lerpSmooth(end, 0.5f);

// 使用全局函数 / Using global function
Color3f smooth = ColorLerpSmooth(start, end, 0.5f);
```

### 3. 三次插值 / Cubic Interpolation

三次插值使用 Hermite 曲线，在起点和终点速度为零。

Cubic interpolation uses Hermite curves with zero velocity at endpoints.

```cpp
Color3f a(1.0f, 0.0f, 0.0f);      // 红色 / Red
Color3f b(0.0f, 0.0f, 1.0f);      // 蓝色 / Blue

// 使用成员方法 / Using member method
a.lerpCubic(b, 0.5f);

// 使用全局函数 / Using global function
Color3f cubic = ColorLerpCubic(a, b, 0.5f);
```

### 4. Bezier 曲线插值 / Bezier Curve Interpolation

Bezier 曲线插值允许通过控制点来控制曲线形状。

Bezier curve interpolation allows controlling the curve shape via control points.

```cpp
Color3f start(1.0f, 0.0f, 0.0f);     // 起点：红色 / Start: Red
Color3f control(0.0f, 1.0f, 0.0f);   // 控制点：绿色 / Control: Green
Color3f end(0.0f, 0.0f, 1.0f);       // 终点：蓝色 / End: Blue

// 使用成员方法 / Using member method
start.lerpBezier(control, end, 0.5f);

// 使用全局函数 / Using global function
Color3f bezier = ColorLerpBezier(start, control, end, 0.5f);
```

## 支持的颜色类型 / Supported Color Types

所有插值方法都支持以下颜色类型：

All interpolation methods support the following color types:

- `Color3f` - RGB 浮点颜色 / RGB float color (0.0 - 1.0)
- `Color4f` - RGBA 浮点颜色 / RGBA float color (0.0 - 1.0)
- `Color3ub` - RGB 字节颜色 / RGB byte color (0 - 255)
- `Color4ub` - RGBA 字节颜色 / RGBA byte color (0 - 255)

## Alpha 混合模式 / Alpha Blend Modes

```cpp
#include<hgl/color/AlphaBlend.h>
#include<hgl/color/BlendColor.h>

Color3f foreground(1.0f, 0.0f, 0.0f);  // 红色 / Red
Color3f background(0.0f, 0.0f, 1.0f);  // 蓝色 / Blue

// 使用不同的混合模式 / Using different blend modes
Color3f multiply = BlendColor(AlphaBlendMode::Multiply, foreground, background, 1.0f);
Color3f screen = BlendColor(AlphaBlendMode::Screen, foreground, background, 1.0f);
Color3f overlay = BlendColor(AlphaBlendMode::Overlay, foreground, background, 1.0f);

// 获取混合模式名称 / Get blend mode name
const char* name = GetAlphaBlendModeName(AlphaBlendMode::Multiply);
// name = "Multiply"
```

## 动画示例 / Animation Example

创建平滑的颜色过渡动画：

Creating smooth color transition animations:

```cpp
#include<hgl/color/Color3f.h>

Color3f current(1.0f, 0.0f, 0.0f);    // 当前颜色 / Current color
Color3f target(0.0f, 1.0f, 0.0f);     // 目标颜色 / Target color

// 在动画循环中 / In animation loop
for(float t = 0.0f; t <= 1.0f; t += 0.01f) {
    Color3f frame = current;
    
    // 选择插值类型 / Choose interpolation type:
    // frame.lerp(target, t);           // 线性 / Linear
    // frame.lerpSmooth(target, t);     // 平滑 / Smooth
    frame.lerpCubic(target, t);         // 三次 / Cubic
    
    // 使用 frame 渲染 / Use frame for rendering
}
```

## 性能提示 / Performance Tips

1. **内联函数 / Inline Functions**: ColorLerp.h 中的全局函数都是内联的，没有额外开销。
   Global functions in ColorLerp.h are inlined with no extra overhead.

2. **成员方法 / Member Methods**: 直接修改当前对象，避免拷贝。
   Member methods modify the current object directly, avoiding copies.

3. **选择合适的插值 / Choose Appropriate Interpolation**:
   - 线性 / Linear: 最快，适合简单过渡 / Fastest, for simple transitions
   - 余弦 / Cosine: 中等速度，平滑过渡 / Medium speed, smooth transitions
   - 三次 / Cubic: 中等速度，最平滑 / Medium speed, smoothest
   - Bezier: 较慢，需要控制点 / Slower, requires control points

## 更多信息 / More Information

详细的架构说明请参考 [README.md](README.md)。

For detailed architecture documentation, see [README.md](README.md).
