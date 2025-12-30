# CMCoreType 颜色架构说明

## 概述

CMCoreType 是 HGL 引擎的核心类型库，负责提供基础的颜色类型和颜色操作功能。本文档说明了颜色功能在 CMCoreType 和 CMMath 库之间的职责分工。

## 架构原则

### 职责划分

1. **CMCoreType（本库）**：
   - 颜色类型定义（Color3f, Color4f, Color3ub, Color4ub）
   - 颜色基础操作（转换、混合、插值）
   - 颜色混合模式枚举定义
   - 颜色空间转换（RGB, HSL, CMYK, YCbCr）

2. **CMMath**：
   - 通用数学运算（向量、矩阵、四元数）
   - 通用插值算法（可用于非颜色数据）
   - 依赖 CMCoreType 的 AlphaBlendMode 枚举

### 设计理念

✅ **单一职责**：颜色相关功能集中在 CMCoreType  
✅ **避免重复**：删除冗余的颜色相关代码  
✅ **向后兼容**：通过 deprecated 标记实现平滑过渡  
✅ **语义一致**：与 CMMath 的通用算法保持语义一致

## 核心模块

### 1. AlphaBlend.h - Alpha 混合模式

定义了统一的 Alpha 混合模式枚举，作为颜色混合的权威位置。

**位置**：`inc/hgl/color/AlphaBlend.h`

**主要内容**：
```cpp
enum class AlphaBlendMode
{
    None,           // 无混合
    Normal,         // 标准 Alpha 混合
    Add,            // 加法混合
    Multiply,       // 乘法混合
    Screen,         // 屏幕混合
    Overlay,        // 叠加混合
    // ... 更多混合模式
};

// 获取混合模式名称
const char *GetAlphaBlendModeName(AlphaBlendMode mode);
```

**混合模式分类**：
- **基础模式**：None, Normal, Add, Subtract, Multiply, Screen
- **对比度模式**：Overlay, HardLight, SoftLight
- **比较模式**：Darken, Lighten, DarkerColor, LighterColor
- **颜色模式**：ColorDodge, ColorBurn, LinearBurn, LinearDodge
- **HSL 模式**：Hue, Saturation, Color, Luminosity
- **特殊模式**：Difference, Exclusion, Divide

### 2. ColorLerp.h - 颜色插值

提供多种颜色插值方法，支持所有颜色类型。

**位置**：`inc/hgl/color/ColorLerp.h`

**插值类型**：

1. **线性插值（Linear）**：
   ```cpp
   Color3f ColorLerp(const Color3f &a, const Color3f &b, float t);
   Color4f ColorLerp(const Color4f &a, const Color4f &b, float t);
   Color3ub ColorLerp(const Color3ub &a, const Color3ub &b, float t);
   Color4ub ColorLerp(const Color4ub &a, const Color4ub &b, float t);
   ```

2. **余弦插值（Smooth/Cosine）**：
   ```cpp
   Color3f ColorLerpSmooth(const Color3f &a, const Color3f &b, float t);
   // 使用余弦函数实现更平滑的过渡
   ```

3. **三次插值（Cubic Hermite）**：
   ```cpp
   Color3f ColorLerpCubic(const Color3f &a, const Color3f &b, float t);
   // 使用三次 Hermite 插值（smoothstep）
   ```

4. **Bezier 曲线插值**：
   ```cpp
   Color3f ColorLerpBezier(const Color3f &a, const Color3f &control, 
                           const Color3f &b, float t);
   // 二次 Bezier 曲线插值，支持控制点
   ```

### 3. 颜色类扩展

所有颜色类（Color3f, Color4f, Color3ub, Color4ub）现在都支持新的插值方法：

**成员方法**：
```cpp
class Color3f {
    void lerp(const Color3f &c, float t);           // 线性插值（已存在）
    void lerpSmooth(const Color3f &c, float t);     // 余弦插值（新增）
    void lerpCubic(const Color3f &c, float t);      // 三次插值（新增）
    void lerpBezier(const Color3f &control,         // Bezier 插值（新增）
                    const Color3f &end, float t);
};
```

## 使用示例

### 基础线性插值

```cpp
#include<hgl/color/Color3f.h>
#include<hgl/color/ColorLerp.h>

Color3f red(1.0f, 0.0f, 0.0f);
Color3f blue(0.0f, 0.0f, 1.0f);

// 方法 1：使用成员方法
Color3f purple = red;
purple.lerp(blue, 0.5f);

// 方法 2：使用全局函数
Color3f purple2 = ColorLerp(red, blue, 0.5f);
```

### 平滑插值

```cpp
Color3f start(1.0f, 0.0f, 0.0f);
Color3f end(0.0f, 1.0f, 0.0f);

// 余弦插值 - 比线性插值更平滑
Color3f smooth = ColorLerpSmooth(start, end, 0.5f);

// 或使用成员方法
start.lerpSmooth(end, 0.5f);
```

### 三次插值

```cpp
Color3f a(1.0f, 0.0f, 0.0f);
Color3f b(0.0f, 0.0f, 1.0f);

// 三次 Hermite 插值 - 首尾速度为零，中间加速
Color3f cubic = ColorLerpCubic(a, b, 0.5f);
```

### Bezier 曲线插值

```cpp
Color3f start(1.0f, 0.0f, 0.0f);      // 红色
Color3f control(0.0f, 1.0f, 0.0f);    // 绿色（控制点）
Color3f end(0.0f, 0.0f, 1.0f);        // 蓝色

// Bezier 曲线通过控制点
Color3f bezier = ColorLerpBezier(start, control, end, 0.5f);

// 或使用成员方法
start.lerpBezier(control, end, 0.5f);
```

### Alpha 混合模式

```cpp
#include<hgl/color/AlphaBlend.h>
#include<hgl/color/BlendColor.h>

Color3f foreground(1.0f, 0.0f, 0.0f);
Color3f background(0.0f, 0.0f, 1.0f);

// 使用不同的混合模式
Color3f result1 = BlendColor(AlphaBlendMode::Multiply, foreground, background, 1.0f);
Color3f result2 = BlendColor(AlphaBlendMode::Screen, foreground, background, 1.0f);
Color3f result3 = BlendColor(AlphaBlendMode::Overlay, foreground, background, 1.0f);

// 获取混合模式名称
const char* name = GetAlphaBlendModeName(AlphaBlendMode::Multiply);
```

## 与 CMMath 的集成

### 迁移指南

如果您之前在 CMMath 中使用颜色相关功能：

1. **AlphaBlendMode 枚举**：
   - ❌ 旧代码：`#include<hgl/math/Lerp3D.h>` 中的 AlphaBlendMode
   - ✅ 新代码：`#include<hgl/color/AlphaBlend.h>`

2. **颜色插值函数**：
   - ❌ 旧代码：CMMath 中的颜色插值函数
   - ✅ 新代码：`#include<hgl/color/ColorLerp.h>`

3. **颜色类型**：
   - ✅ 始终使用：`#include<hgl/color/Color3f.h>` 等

### 平滑过渡

为了实现平滑过渡，建议：

1. **在 CMMath 中**：
   - 添加 deprecated 标记到旧的颜色相关函数
   - 引用 CMCoreType 的 AlphaBlendMode 枚举
   - 在文档中说明新的位置

2. **在应用代码中**：
   - 逐步更新 include 路径
   - 更新函数调用到新的 API
   - 测试确保行为一致

## 编译要求

- **C++ 标准**：C++20
- **依赖**：
  - 标准数学库（`<cmath>`）
  - HGL 平台层（`hgl/platform/Platform.h`）

## 测试建议

建议为以下场景添加测试：

1. **插值精度测试**：
   - 边界值测试（t=0, t=1）
   - 中间值测试（t=0.5）
   - 越界值测试（t<0, t>1）

2. **插值类型比较**：
   - 线性 vs 余弦 vs 三次
   - 确保曲线特性符合预期

3. **Bezier 曲线测试**：
   - 控制点影响测试
   - 曲线通过控制点的验证

4. **类型一致性测试**：
   - Color3f 和 Color3ub 结果一致性
   - Color4f 和 Color4ub 结果一致性

## 性能考虑

1. **内联函数**：ColorLerp.h 中的函数都是内联的，无额外函数调用开销
2. **成员方法**：直接修改当前对象，避免拷贝
3. **全局函数**：返回新对象，适合函数式编程风格

## 未来计划

- [ ] 添加更多混合模式（如果需要）
- [ ] 添加三次 Bezier 曲线插值（4 个控制点）
- [ ] 添加 SIMD 优化版本
- [ ] 添加颜色空间感知插值（如 Lab 空间）

## 相关文档

- [BlendColor.h](inc/hgl/color/BlendColor.h) - 颜色混合实现
- [Color3f.h](inc/hgl/color/Color3f.h) - RGB 浮点颜色类
- [Color4f.h](inc/hgl/color/Color4f.h) - RGBA 浮点颜色类
- [HSL.h](inc/hgl/color/HSL.h) - HSL 颜色空间

## 贡献指南

如需添加新功能或修复问题：

1. 确保遵循现有代码风格
2. 为新功能添加文档注释
3. 考虑向后兼容性
4. 添加适当的测试用例

## 版权信息

Copyright (C) HYZ Game Engine
