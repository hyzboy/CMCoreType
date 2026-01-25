# Color Library - Quantization & Dithering Enhancement

## Overview

The Color library has been enhanced with comprehensive **color quantization** and **dithering algorithms** to support advanced image processing tasks.

## New Headers

### 1. `ColorQuantize.h` - Color Quantization
Provides various color quantization techniques to reduce color palette size while maintaining visual quality.

#### Key Functions:

**Bit-based Quantization:**
```cpp
Color3f QuantizeColor3f(const Color3f &color, int bits);
Color3ub QuantizeColor3ub(const Color3ub &color, int bits);
```
- Quantize colors to a specified number of bits per channel
- Supports 1-8 bits per channel
- Maintains color values in [0, 1] or [0, 255] ranges

**Uniform Quantization:**
```cpp
Color3f UniformQuantize3f(const Color3f &color, int levels);
```
- Reduces to uniform levels per channel
- Better control over output color count

**Error Analysis:**
```cpp
float QuantizationError3f(const Color3f &original, const Color3f &quantized);
float QuantizationError3ub(const Color3ub &original, const Color3ub &quantized);
```
- Calculate quantization error (squared Euclidean distance)
- Useful for quality metrics

**Palette-based Quantization:**
```cpp
int FindNearestPaletteColor(const Color3f &color, const std::vector<Color3f> &palette);
float GetPaletteColorDistance(const Color3f &color, const Color3f &palette_color);
```
- Find nearest color in a predefined palette
- Calculate distance to palette colors

---

### 2. `ColorDithering.h` - Dithering Algorithms
Implements multiple dithering techniques to reduce banding artifacts in quantized images.

#### Floyd-Steinberg Dithering
Classic error diffusion algorithm:
```cpp
class FloydSteinbergDitherer
{
public:
    static Color3f Dither(const Color3f &original, int error_x, int error_y, int bits);
    static Color3ub Dither(const Color3ub &original, int error_x, int error_y, int bits);
};
```

Error distribution pattern:
```
      [current]  7/16
  3/16  5/16  1/16
```

#### Bayer (Ordered) Dithering
Fast ordered dithering with 4x4 Bayer matrix:
```cpp
class BayerDitherer
{
public:
    static Color3f Dither(const Color3f &original, int x, int y, int levels);
    static Color3ub Dither(const Color3ub &original, int x, int y, int levels);
    static float GetThreshold(int x, int y);
};
```

Bayer 4×4 matrix:
```
 0   8   2  10
12   4  14   6
 3  11   1   9
15   7  13   5
```

#### Random Dithering
Adds noise for dithering effect:
```cpp
class RandomDitherer
{
public:
    RandomDitherer(uint32_t seed = std::random_device{}());
    Color3f Dither(const Color3f &original, float noise_amount = 0.05f, int bits = 5);
    Color3ub Dither(const Color3ub &original, float noise_amount = 0.05f, int bits = 5);
    void SetSeed(uint32_t seed);
};
```

#### Palette-based Dithering
```cpp
int PaletteDither(const Color3f &original, const std::vector<Color3f> &palette, 
                 float dither_pattern = 0.0f);
```

---

## Usage Examples

### Basic Quantization
```cpp
#include <hgl/color/ColorQuantize.h>

hgl::Color3f original(0.567f, 0.234f, 0.789f);

// Reduce to 256 colors (8 bits per channel)
hgl::Color3f q8 = hgl::QuantizeColor3f(original, 8);

// Reduce to 64 colors (5 bits per channel)
hgl::Color3f q5 = hgl::QuantizeColor3f(original, 5);

// Reduce to 8 colors (2 bits per channel)
hgl::Color3f q2 = hgl::QuantizeColor3f(original, 2);
```

### Quantization with Floyd-Steinberg Dithering
```cpp
#include <hgl/color/ColorDithering.h>

hgl::Color3f color(0.6f, 0.3f, 0.75f);

// Apply dithering at pixel position (x, y) with 3-bit quantization
hgl::Color3f dithered = hgl::FloydSteinbergDitherer::Dither(color, x, y, 3);
```

### Bayer Dithering (Fast Ordered Pattern)
```cpp
// Process entire image with Bayer dithering
for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
        hgl::Color3f dithered = hgl::BayerDitherer::Dither(
            original_image[y*width + x], 
            x, y, 
            4  // 4 levels per channel = 64 colors
        );
    }
}
```

### Random Dithering with Seed
```cpp
hgl::RandomDitherer ditherer(12345);  // Fixed seed for reproducibility

for (const auto &color : image) {
    hgl::Color3f dithered = ditherer.Dither(
        color,
        0.05f,   // 5% noise amount
        5        // 5-bit quantization
    );
}
```

### Custom Palette Quantization
```cpp
std::vector<hgl::Color3f> palette = {
    hgl::Color3f(1, 0, 0),  // Red
    hgl::Color3f(0, 1, 0),  // Green
    hgl::Color3f(0, 0, 1),  // Blue
    hgl::Color3f(1, 1, 0),  // Yellow
    // ... more colors
};

for (const auto &color : image) {
    int palette_idx = hgl::FindNearestPaletteColor(color, palette);
    result[i] = palette[palette_idx];
}
```

---

## Test Suite

Comprehensive test suite in `examples/color/`:

### ColorConversionTest.cpp
- Basic color construction and conversion
- Color interpolation
- CMYK color space
- Color constants
- Arithmetic operations
- Boundary cases

### ColorQuantizeTest.cpp
- Bit-based quantization (1-8 bits)
- Uniform quantization with variable levels
- Uint8 quantization
- Quantization error analysis
- Palette-based color matching
- Edge cases (0, 1, extreme values)

### ColorDitheringTest.cpp
- Floyd-Steinberg dithering
- Bayer matrix dithering
- Random dithering with various noise levels
- Palette-based dithering
- Quality comparison between methods
- Edge cases (black, white, tiny values)

### ColorAdvancedTest.cpp
- Complete color processing pipeline
- Color space transitions
- Batch color processing
- Performance characteristics (1000+ colors)
- Advanced color filtering
- Dominant channel detection

---

## Algorithm Characteristics

| Algorithm | Speed | Quality | Artifacts | Use Case |
|-----------|-------|---------|-----------|----------|
| **Direct Quantization** | Very Fast | Poor | Severe banding | Quick preview |
| **Bayer Dithering** | Fast | Good | Visible pattern | Real-time, ordered pattern |
| **Floyd-Steinberg** | Medium | Excellent | Minimal | Quality-focused processing |
| **Random Dithering** | Fast | Good | Fine noise | Stochastic appearance |

---

## Integration Notes

1. **Header Dependencies:**
   - All quantization/dithering headers depend on `Color3f`, `Color3ub`, etc.
   - No external dependencies beyond standard library

2. **Performance:**
   - All inline functions for maximum optimization
   - Single-pass algorithms for streaming data
   - No dynamic memory allocation

3. **Precision:**
   - Float-based for [0, 1] range
   - Uint8-based for [0, 255] range
   - Automatic conversion between formats

4. **API Stability:**
   - Stable interface following existing Color library conventions
   - Naming consistent with CMYKf, Color3f patterns
   - Extensible for future algorithms

---

## Future Enhancements

Potential additions:
- [ ] K-means color clustering
- [ ] Octree-based color reduction
- [ ] Perceptual color distance (Delta E)
- [ ] Error accumulation over scanlines
- [ ] SIMD optimizations
- [ ] GPU shader implementations

---

## Testing

Compile and run all tests:
```bash
cmake .
cmake --build .
./ColorConversionTest
./ColorQuantizeTest
./ColorDitheringTest
./ColorAdvancedTest
```

All tests include assertions and print detailed diagnostics for debugging.

---

**Last Updated:** 2026-01-25
