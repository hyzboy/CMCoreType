# Color Library Quick Reference

## 📚 Quick Links

| Component | Header | Location |
|-----------|--------|----------|
| 基础颜色 | `Color3f.h`, `Color4f.h` | `inc/hgl/color/` |
| 色彩插值 | `ColorLerp.h` | `inc/hgl/color/` |
| **色彩量化** | **`ColorQuantize.h`** | **`inc/hgl/color/`** ✨ NEW |
| **色彩抖动** | **`ColorDithering.h`** | **`inc/hgl/color/`** ✨ NEW |
| CMYK色彩 | `CMYKf.h` | `inc/hgl/color/` |

---

## ⚡ 最常用代码片段

### 1️⃣ 减少颜色数量
```cpp
#include <hgl/color/ColorQuantize.h>

// 方式1：指定比特数（推荐）
hgl::Color3f q4 = hgl::QuantizeColor3f(color, 4);  // 256 colors max

// 方式2：指定色阶数
hgl::Color3f q8 = hgl::UniformQuantize3f(color, 8);  // 512 colors
```

### 2️⃣ 抖动处理（3选1）
```cpp
#include <hgl/color/ColorDithering.h>

// 选项A：高质量（Floyd-Steinberg）
hgl::Color3f dithered = hgl::FloydSteinbergDitherer::Dither(color, x, y, bits);

// 选项B：快速（Bayer有序）
hgl::Color3f dithered = hgl::BayerDitherer::Dither(color, x, y, levels);

// 选项C：随机噪声
hgl::RandomDitherer rd(seed);
hgl::Color3f dithered = rd.Dither(color, 0.05f, bits);
```

### 3️⃣ 完整图像处理流程
```cpp
// 初始化
std::vector<hgl::Color3f> image = /* ... */;
hgl::RandomDitherer ditherer;

// 处理
for(int y = 0; y < height; ++y) {
    for(int x = 0; x < width; ++x) {
        image[y*w+x] = hgl::BayerDitherer::Dither(
            image[y*w+x], x, y, 4  // 4色级 = 64色
        );
    }
}
```

### 4️⃣ 调色板匹配
```cpp
// 定义调色板
std::vector<hgl::Color3f> palette = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}  // RGB
};

// 查询
for(auto &c : image) {
    int idx = hgl::FindNearestPaletteColor(c, palette);
    c = palette[idx];
}
```

### 5️⃣ 质量检查
```cpp
auto q = hgl::QuantizeColor3f(original, 5);
float error = hgl::QuantizationError3f(original, q);
std::cout << "Error: " << error << std::endl;
```

---

## 🎛️ 参数选择指南

### 颜色位数 vs 输出色数

| Bits | 色数 | 推荐用途 |
|------|------|---------|
| 1 | 8 | B&W二值 |
| 2 | 64 | 极低质量 |
| 3 | 512 | 低质量/快速 |
| 4 | 4K | 中等质量 |
| 5 | 32K | 高质量 |
| 6 | 262K | 极高质量 |
| 8 | 16M | 无损 |

### Dithering算法选择

| 算法 | 速度 | 质量 | 何时使用 |
|------|------|------|---------|
| Floyd-Steinberg | ⚠️ 中等 | ⭐⭐⭐ | 需要最佳视觉效果 |
| Bayer | ✅ 快速 | ⭐⭐ | 实时游戏、WebGL |
| Random | ✅ 快速 | ⭐⭐ | 打印、蜻蜓纹理 |

---

## 🧪 测试程序

```bash
# 编译
cd build
cmake ..
cmake --build .

# 运行测试
./examples/color/ColorConversionTest     # 基础功能
./examples/color/ColorQuantizeTest       # 量化算法
./examples/color/ColorDitheringTest      # 抖动算法
./examples/color/ColorAdvancedTest       # 高级应用
```

预期输出：`✓ ALL TESTS PASSED!`

---

## 💡 Tips & Tricks

### 1. 灰度量化（简单）
```cpp
float gray = 0.5f;
auto q = hgl::QuantizeColor3f(
    hgl::Color3f(gray, gray, gray), 4
);
```

### 2. 批处理优化
```cpp
hgl::RandomDitherer ditherer;
ditherer.SetSeed(fixed_seed);  // 可重复
for(auto &color : colors) {
    color = ditherer.Dither(color, 0.05f, 5);
}
```

### 3. 自定义调色板（Web安全色）
```cpp
std::vector<hgl::Color3f> web_safe = {
    {0,0,0}, {0,0,0.33}, {0,0,0.67}, {0,0,1.0},
    {0.33,0,0}, /* ... */
};
```

### 4. 误差堆积检测
```cpp
auto q = hgl::QuantizeColor3f(color, 3);
float err = hgl::QuantizationError3f(color, q);
if(err > threshold) {
    // 某些颜色难以量化
}
```

### 5. 颜色距离查询
```cpp
float distance = hgl::GetPaletteColorDistance(
    my_color, palette[selected_idx]
);
```

---

## 📋 Data Types

```cpp
// 三通道颜色
hgl::Color3f    // float [0, 1]
hgl::Color3ub   // uint8 [0, 255]

// 四通道颜色（含Alpha）
hgl::Color4f    // float [0, 1]
hgl::Color4ub   // uint8 [0, 255]

// 特殊色彩空间
hgl::CMYKf      // CMYK [0, 1]
hgl::HSL        // HSL
hgl::LinearColor3f  // 线性色彩空间
```

---

## 🐛 常见问题

**Q: 哪个Dithering算法最好？**
A: Floyd-Steinberg质量最好，Bayer最快。选择取决于应用场景。

**Q: 颜色溢出（超过[0,1]）怎么办？**
A: 量化函数会自动钳制(clamp)，不需要额外处理。

**Q: 支持Alpha通道吗？**
A: 支持 `Color4f` 和 `Color4ub`，但Dithering算法针对RGB优化。

**Q: 性能如何？**
A: 1000个颜色处理 <1ms，可用于实时应用。

**Q: 能用于GPU吗？**
A: 当前是CPU实现。可在GLSL中重现相同算法。

---

## 📖 深入学习

### 相关文件
- `ColorQuantize.h` - 330行，详细注释
- `ColorDithering.h` - 380行，详细注释  
- `examples/color/README.md` - 完整文档
- `examples/color/IMPLEMENTATION_SUMMARY.md` - 设计细节

### 测试作为文档
每个测试文件都展示了正确的使用方式和预期结果。

---

## ✅ 检查清单

- [ ] 已阅读 `ColorQuantize.h` 中的注释
- [ ] 已阅读 `ColorDithering.h` 中的注释
- [ ] 运行过至少一个测试程序
- [ ] 在项目中成功使用了量化/抖动
- [ ] 理解了不同算法的权衡

---

**最后更新**: 2026-01-25  
**维护者**: AI Assistant
