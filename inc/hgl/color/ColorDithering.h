#pragma once

#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<hgl/color/ColorQuantize.h>
#include<vector>
#include<cstdint>

namespace hgl
{
    /**
     * @brief Color dithering algorithms
     *
     * This module provides various dithering techniques to improve visual quality
     * of quantized colors by distributing quantization errors across neighboring pixels.
     *
     * Implementation: See src/Color/ColorDithering.cpp
     */

    //==================================================================================================
    // Error Diffusion Dithering
    //==================================================================================================

    /**
     * @brief Floyd-Steinberg dithering error coefficients
     * 
     * Classic error diffusion dithering algorithm:
     *
     *      [current]  7/16
     *  3/16  5/16  1/16
     */
    class FloydSteinbergDitherer
    {
    public:
        /**
         * @brief Apply Floyd-Steinberg dithering to a color based on position
         * @param original Original color [0, 1]
         * @param error_x X coordinate error (pixel position)
         * @param error_y Y coordinate error (pixel position)
         * @param bits Quantization bits per channel
         * @return Dithered color
         */
        static Color3f Dither(const Color3f &original, int error_x, int error_y, int bits);

        /**
         * @brief Apply Floyd-Steinberg dithering (uint8 version)
         */
        static Color3ub Dither(const Color3ub &original, int error_x, int error_y, int bits);
    };

    //==================================================================================================
    // Ordered Dithering (Bayer Matrix)
    //==================================================================================================

    /**
     * @brief Bayer matrix dithering - uses pre-computed threshold matrix
     */
    class BayerDitherer
    {
    public:
        /**
         * @brief Apply Bayer dithering to a color
         * @param original Original color [0, 1]
         * @param x Pixel X coordinate
         * @param y Pixel Y coordinate
         * @param levels Number of output levels per channel
         * @return Dithered color
         */
        static Color3f Dither(const Color3f &original, int x, int y, int levels);

        /**
         * @brief Apply Bayer dithering (uint8 version)
         */
        static Color3ub Dither(const Color3ub &original, int x, int y, int levels);

        /**
         * @brief Get Bayer matrix threshold value
         */
        static float GetThreshold(int x, int y);
    };

    //==================================================================================================
    // Random Dithering
    //==================================================================================================

    /**
     * @brief Random dithering - adds random noise before quantization
     */
    class RandomDitherer
    {
    private:
        class RandomImpl;
        RandomImpl* pImpl;

    public:
        /**
         * @brief Constructor with optional seed
         */
        RandomDitherer(uint32_t seed = 0);

        /**
         * @brief Destructor
         */
        ~RandomDitherer();

        /**
         * @brief Apply random dithering
         * @param original Original color [0, 1]
         * @param noise_amount Amount of noise [0, 1]
         * @param bits Quantization bits per channel
         * @return Dithered color
         */
        Color3f Dither(const Color3f &original, float noise_amount = 0.05f, int bits = 5);

        /**
         * @brief Apply random dithering (uint8 version)
         */
        Color3ub Dither(const Color3ub &original, float noise_amount = 0.05f, int bits = 5);

        /**
         * @brief Reseed the generator
         */
        void SetSeed(uint32_t seed);
    };

    //==================================================================================================
    // Palette-based Dithering
    //==================================================================================================

    /**
     * @brief Dither and quantize to a specific color palette
     * @param original Original color
     * @param palette Color palette
     * @param dither_pattern Optional dither value [-0.5, 0.5]
     * @return Index of selected palette color
     */
    int PaletteDither(const Color3f &original, const std::vector<Color3f> &palette, 
                     float dither_pattern = 0.0f);

} // namespace hgl
