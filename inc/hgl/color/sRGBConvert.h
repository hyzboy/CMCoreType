#pragma once

#include<cmath>
#include<concepts>

namespace hgl
{
    /**
    * sRGB ↔ Linear 颜色空间转换函数 (IEC 61966-2-1 标准)
    * C++20 版本
    * 
    * sRGB 是一种伽马修正的颜色空间，用于显示设备
    * Linear 是线性颜色空间，用于颜色计算和 HDR 处理
    * 
    * 标准转换公式:
    * sRGB → Linear: if(c ≤ 0.04045) c/12.92 else ((c+0.055)/1.055)^2.4
    * Linear → sRGB: if(c ≤ 0.0031308) c*12.92 else 1.055*c^(1/2.4)-0.055
    */

    // ===== 常量定义 =====
    constexpr const double GAMMA              = 2.4;
    constexpr const double INV_GAMMA          = 1.0 / GAMMA;
    constexpr const double SRGB_ALPHA         = 0.055;
    constexpr const double SRGB_LINEAR_THRESHOLD    = 0.04045;
    constexpr const double SRGB_LINEAR_DIVISOR      = 12.92;
    constexpr const double LINEAR_SRGB_THRESHOLD    = 0.0031308;

    // ===== C++20 概念定义 =====
    
    /**
    * 浮点数概念 - 用于类型安全的模板函数
    */
    template<typename T>
    concept FloatingPoint = std::floating_point<T>;

    /**
    * 颜色分量概念 - 支持浮点数或整数作为颜色分量
    */
    template<typename T>
    concept ColorComponent = std::floating_point<T> || std::integral<T>;

    // ===== 标准转换函数 (IEC 61966-2-1) =====

    /**
    * 将单个 sRGB 颜色分量转换为线性颜色分量
    * @param c sRGB 颜色分量 (0.0 - 1.0)
    * @return 线性颜色分量 (0.0 - 1.0)
    */
    constexpr inline float sRGBToLinear(float c) noexcept
    {
        if(c <= 0.04045f)
            return c / 12.92f;
        else
            return powf((c + 0.055f) / 1.055f, 2.4f);
    }

    /**
    * 将单个线性颜色分量转换为 sRGB 颜色分量
    * @param c 线性颜色分量 (0.0 - 1.0)
    * @return sRGB 颜色分量 (0.0 - 1.0)
    */
    constexpr inline float linearToSRGB(float c) noexcept
    {
        if(c <= 0.0031308f)
            return c * 12.92f;
        else
            return 1.055f * powf(c, 1.0f / 2.4f) - 0.055f;
    }

    /**
    * 泛型 sRGB 转线性转换 (支持 double 等其他浮点类型)
    * @param c sRGB 颜色分量
    * @return 线性颜色分量
    */
    template<FloatingPoint T>
    constexpr inline T sRGBToLinearGeneric(T c) noexcept
    {
        if(c <= static_cast<T>(SRGB_LINEAR_THRESHOLD))
            return c / static_cast<T>(SRGB_LINEAR_DIVISOR);
        else
            return std::pow((c + static_cast<T>(SRGB_ALPHA)) / (static_cast<T>(1.0) + static_cast<T>(SRGB_ALPHA)), static_cast<T>(GAMMA));
    }

    /**
    * 泛型线性转 sRGB 转换 (支持 double 等其他浮点类型)
    * @param c 线性颜色分量
    * @return sRGB 颜色分量
    */
    template<FloatingPoint T>
    constexpr inline T linearToSRGBGeneric(T c) noexcept
    {
        if(c <= static_cast<T>(LINEAR_SRGB_THRESHOLD))
            return c * static_cast<T>(SRGB_LINEAR_DIVISOR);
        else
            return static_cast<T>(1.055) * std::pow(c, static_cast<T>(INV_GAMMA)) - static_cast<T>(SRGB_ALPHA);
    }

    /**
    * 将 sRGB RGB 三原色转换为线性 RGB
    * @param sr, sg, sb sRGB 颜色分量 (0.0 - 1.0)
    * @param lr, lg, lb 输出线性颜色分量
    */
    template<ColorComponent T>
    constexpr inline void sRGBToLinear(T sr, T sg, T sb, T &lr, T &lg, T &lb) noexcept
    {
        if constexpr (std::floating_point<T>) {
            lr = sRGBToLinearGeneric(sr);
            lg = sRGBToLinearGeneric(sg);
            lb = sRGBToLinearGeneric(sb);
        } else {
            // 整数类型转换为浮点数处理
            float fsr = static_cast<float>(sr) / 255.0f;
            float fsg = static_cast<float>(sg) / 255.0f;
            float fsb = static_cast<float>(sb) / 255.0f;
            lr = static_cast<T>(sRGBToLinear(fsr) * 255.0f);
            lg = static_cast<T>(sRGBToLinear(fsg) * 255.0f);
            lb = static_cast<T>(sRGBToLinear(fsb) * 255.0f);
        }
    }

    /**
    * 将线性 RGB 三原色转换为 sRGB
    * @param lr, lg, lb 线性颜色分量
    * @param sr, sg, sb 输出 sRGB 颜色分量 (0.0 - 1.0)
    */
    template<ColorComponent T>
    constexpr inline void linearToSRGB(T lr, T lg, T lb, T &sr, T &sg, T &sb) noexcept
    {
        if constexpr (std::floating_point<T>) {
            sr = linearToSRGBGeneric(lr);
            sg = linearToSRGBGeneric(lg);
            sb = linearToSRGBGeneric(lb);
        } else {
            // 整数类型转换为浮点数处理
            float flr = static_cast<float>(lr) / 255.0f;
            float flg = static_cast<float>(lg) / 255.0f;
            float flb = static_cast<float>(lb) / 255.0f;
            sr = static_cast<T>(linearToSRGB(flr) * 255.0f);
            sg = static_cast<T>(linearToSRGB(flg) * 255.0f);
            sb = static_cast<T>(linearToSRGB(flb) * 255.0f);
        }
    }

    // ===== 性能优化版本 =====

    /**
    * 快速 sRGB 转线性 (仅使用伽马幂运算，不考虑分段)
    * 速度更快但精度较低
    */
    template<FloatingPoint T>
    constexpr inline T sRGBToLinearFast(T c) noexcept
    {
        return std::pow(c, static_cast<T>(GAMMA));
    }

    /**
    * 快速线性转 sRGB (仅使用伽马幂运算，不考虑分段)
    * 速度更快但精度较低
    */
    template<FloatingPoint T>
    constexpr inline T linearToSRGBFast(T c) noexcept
    {
        return std::pow(c, static_cast<T>(INV_GAMMA));
    }

    /**
    * 最快速版本 (平方/开方近似)
    * 仅用于实时性要求极高的场景
    */
    template<FloatingPoint T>
    constexpr inline T sRGBToLinearFastest(T c) noexcept
    {
        return c * c;
    }

    template<FloatingPoint T>
    constexpr inline T linearToSRGBFastest(T c) noexcept
    {
        return std::sqrt(c);
    }

}//namespace hgl
