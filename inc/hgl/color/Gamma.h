#pragma once

#include<hgl/color/sRGBConvert.h>
#include<concepts>

namespace hgl
{
    // ===== XYZ 转换矩阵常量 (D65 标准) =====
    // 这些常量用于色彩空间的更高级转换，与 sRGB↔Linear 分离

    // sRGB to XYZ (D65) conversion matrix coefficients
    constexpr const double SRGB2XYZ_X_R = 0.4124;
    constexpr const double SRGB2XYZ_X_G = 0.3576;
    constexpr const double SRGB2XYZ_X_B = 0.1805;

    constexpr const double SRGB2XYZ_Y_R = 0.2126;
    constexpr const double SRGB2XYZ_Y_G = 0.7152;
    constexpr const double SRGB2XYZ_Y_B = 0.0722;

    constexpr const double SRGB2XYZ_Z_R = 0.0193;
    constexpr const double SRGB2XYZ_Z_G = 0.1192;
    constexpr const double SRGB2XYZ_Z_B = 0.9505;

    // XYZ to sRGB (D65) conversion matrix coefficients
    constexpr const double XYZ2SRGB_R_X =  3.2406;
    constexpr const double XYZ2SRGB_R_Y = -1.5373;
    constexpr const double XYZ2SRGB_R_Z = -0.4986;

    constexpr const double XYZ2SRGB_G_X = -0.9689;
    constexpr const double XYZ2SRGB_G_Y =  1.8758;
    constexpr const double XYZ2SRGB_G_Z =  0.0416;

    constexpr const double XYZ2SRGB_B_X =  0.0557;
    constexpr const double XYZ2SRGB_B_Y = -0.2040;
    constexpr const double XYZ2SRGB_B_Z =  1.0570;

    // ===== C++20 概念 =====
    
    /**
    * 矩阵操作数概念 - 支持浮点数和整数类型
    */
    template<typename T>
    concept MatrixOperand = std::floating_point<T> || std::integral<T>;

    // ===== XYZ 颜色空间转换函数 =====

    /**
    * 将 sRGB RGB 转换为 XYZ 颜色空间 (D65 标准)
    * 使用 C++20 模板和概念确保类型安全
    * @param r, g, b sRGB 颜色分量
    * @param x, y, z 输出 XYZ 颜色分量
    */
    template<MatrixOperand T>
    constexpr inline void sRGB2LinearFast(T &x, T &y, T &z, const T &r, const T &g, const T &b) noexcept
    {
        x = static_cast<T>(SRGB2XYZ_X_R*r + SRGB2XYZ_X_G*g + SRGB2XYZ_X_B*b);
        y = static_cast<T>(SRGB2XYZ_Y_R*r + SRGB2XYZ_Y_G*g + SRGB2XYZ_Y_B*b);
        z = static_cast<T>(SRGB2XYZ_Z_R*r + SRGB2XYZ_Z_G*g + SRGB2XYZ_Z_B*b);
    }

    /**
    * 将 XYZ 颜色空间转换为 sRGB RGB (D65 标准)
    * 使用 C++20 模板和概念确保类型安全
    * @param x, y, z XYZ 颜色分量
    * @param r, g, b 输出 sRGB 颜色分量
    */
    template<MatrixOperand T>
    constexpr inline void Linear2sRGBFast(T &r, T &g, T &b, const T &x, const T &y, const T &z) noexcept
    {
        r = static_cast<T>(XYZ2SRGB_R_X*x + XYZ2SRGB_R_Y*y + XYZ2SRGB_R_Z*z);
        g = static_cast<T>(XYZ2SRGB_G_X*x + XYZ2SRGB_G_Y*y + XYZ2SRGB_G_Z*z);
        b = static_cast<T>(XYZ2SRGB_B_X*x + XYZ2SRGB_B_Y*y + XYZ2SRGB_B_Z*z);
    }

}//namespace hgl
