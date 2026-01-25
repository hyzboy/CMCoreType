#pragma once

#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<vector>

namespace hgl
{
    /**
     * @brief Color quantization algorithms
     *
     * This module provides various color quantization methods to reduce the number of colors
     * in an image while maintaining visual quality.
     *
     * Implementation: See src/Color/ColorQuantize.cpp
     */

    //==================================================================================================
    // Bit-based Quantization
    //==================================================================================================

    /**
     * @brief Quantize a color to a specified number of bits per channel
     * @param color Input color [0, 1]
     * @param bits Number of bits per channel (1-8)
     * @return Quantized color
     */
    Color3f QuantizeColor3f(const Color3f &color, int bits);

    /**
     * @brief Quantize a color to a specified number of bits per channel
     * @param color Input color [0, 255]
     * @param bits Number of bits per channel (1-8)
     * @return Quantized color
     */
    Color3ub QuantizeColor3ub(const Color3ub &color, int bits);

    /**
     * @brief Quantize a color to a specified number of bits per channel
     * @param color Input color [0, 1]
     * @param bits Number of bits per channel (1-8)
     * @return Quantized color
     */
    Color4f QuantizeColor4f(const Color4f &color, int bits);

    /**
     * @brief Quantize a color to a specified number of bits per channel
     * @param color Input color [0, 255]
     * @param bits Number of bits per channel (1-8)
     * @return Quantized color
     */
    Color4ub QuantizeColor4ub(const Color4ub &color, int bits);

    //==================================================================================================
    // Uniform Quantization
    //==================================================================================================

    /**
     * @brief Uniform color quantization - reduces palette to uniform grid
     * @param color Input color [0, 1]
     * @param levels Number of levels per channel (2-256)
     * @return Quantized color
     */
    Color3f UniformQuantize3f(const Color3f &color, int levels);

    //==================================================================================================
    // Error Calculation
    //==================================================================================================

    /**
     * @brief Calculate quantization error (squared euclidean distance)
     * @param original Original color
     * @param quantized Quantized color
     * @return Quantization error value
     */
    float QuantizationError3f(const Color3f &original, const Color3f &quantized);

    /**
     * @brief Calculate quantization error for uint8 colors
     * @param original Original color
     * @param quantized Quantized color
     * @return Quantization error value
     */
    float QuantizationError3ub(const Color3ub &original, const Color3ub &quantized);

    //==================================================================================================
    // Palette-based Quantization
    //==================================================================================================

    /**
     * @brief Find nearest color in palette
     * @param color Target color
     * @param palette Vector of palette colors
     * @return Index of nearest color in palette
     */
    int FindNearestPaletteColor(const Color3f &color, const std::vector<Color3f> &palette);

    /**
     * @brief Find nearest color in palette (uint8)
     * @param color Target color
     * @param palette Vector of palette colors
     * @return Index of nearest color in palette
     */
    int FindNearestPaletteColor(const Color3ub &color, const std::vector<Color3ub> &palette);

    /**
     * @brief Get color distance in palette (useful for debugging)
     * @param color Target color
     * @param palette_color Palette color
     * @return Distance value (euclidean in [0,1] range)
     */
    float GetPaletteColorDistance(const Color3f &color, const Color3f &palette_color);

} // namespace hgl
