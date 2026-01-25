#pragma once

#include<cmath>
#include<concepts>
#include<algorithm>
#include<cstdint>

namespace hgl
{
    /**
    * sRGB ↔ Linear 颜色空间转换函数 (IEC 61966-2-1 标准)
    * 扩展支持所有 Vulkan 颜色空间 (VK_COLOR_SPACE_*)
    * C++20 版本
    *
    * sRGB 是一种伽马修正的颜色空间，用于显示设备
    * Linear 是线性颜色空间，用于颜色计算和 HDR 处理
    *
    * 支持的颜色空间:
    * - sRGB (标准 Gamma 2.4)
    * - Display P3 (Gamma 2.4, 更宽的色域)
    * - BT.709 (标准电视色域)
    * - BT.2020 (UHD色域)
    * - HDR10 (Perceptual Quantizer PQ)
    * - HLG (Hybrid Log-Gamma)
    * - AdobeRGB (Gamma 2.2)
    * - DCI-P3 (Display P3 DCinema variant)
    *
    * 标准sRGB转换公式:
    * sRGB → Linear: if(c ≤ 0.04045) c/12.92 else ((c+0.055)/1.055)^2.4
    * Linear → sRGB: if(c ≤ 0.0031308) c*12.92 else 1.055*c^(1/2.4)-0.055
    */

    // ===== 转移函数类型 (Transfer Functions / EOTF) =====
    enum class TransferFunction
    {
        Linear,         ///< 线性 (无转移函数)
        SRGB,           ///< sRGB Gamma 2.4 (IEC 61966-2-1)
        LinearSRGB,     ///< 扩展线性 sRGB
        DisplayP3,      ///< Display P3 Gamma 2.4
        BT709,          ///< BT.709 Gamma 2.4
        BT2020,         ///< BT.2020 Gamma 2.4
        AdobeRGB,       ///< Adobe RGB Gamma 2.2
        DCI_P3,         ///< DCI-P3 (DCinema)
        PQ,             ///< Perceptual Quantizer (HDR10)
        HLG,            ///< Hybrid Log-Gamma (HDR)
        DCIP3XYZ,       ///< DCI-P3 XYZ
        DolbyVision,    ///< Dolby Vision
    };

    // ===== 颜色空间类型 =====
    enum class ColorSpace
    {
        sRGB_NONLINEAR,                     ///< VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        DISPLAY_P3_NONLINEAR,               ///< VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT
        DCI_P3_NONLINEAR,                   ///< VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT
        BT709_NONLINEAR,                    ///< VK_COLOR_SPACE_BT709_NONLINEAR_EXT
        BT709_LINEAR,                       ///< VK_COLOR_SPACE_BT709_LINEAR_EXT
        BT2020_LINEAR,                      ///< VK_COLOR_SPACE_BT2020_LINEAR_EXT
        EXTENDED_SRGB_NONLINEAR,            ///< VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT
        EXTENDED_SRGB_LINEAR,               ///< VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT
        DISPLAY_P3_LINEAR,                  ///< VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT
        ADOBERGB_NONLINEAR,                 ///< VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT
        ADOBERGB_LINEAR,                    ///< VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT
        HDR10_ST2084,                       ///< VK_COLOR_SPACE_HDR10_ST2084_EXT
        HDR10_HLG,                          ///< VK_COLOR_SPACE_HDR10_HLG_EXT
        DOLBYVISION,                        ///< VK_COLOR_SPACE_DOLBYVISION_EXT
        DISPLAY_NATIVE_AMD,                 ///< VK_COLOR_SPACE_DISPLAY_NATIVE_AMD
    };

    // ===== 常量定义 =====
    constexpr const double GAMMA              = 2.4;
    constexpr const double INV_GAMMA          = 1.0 / GAMMA;
    constexpr const double SRGB_ALPHA         = 0.055;
    constexpr const double SRGB_LINEAR_THRESHOLD    = 0.04045;
    constexpr const double SRGB_LINEAR_DIVISOR      = 12.92;
    constexpr const double LINEAR_SRGB_THRESHOLD    = 0.0031308;

    // Adobe RGB Gamma 2.2
    constexpr const double ADOBERGB_GAMMA            = 2.2;
    constexpr const double ADOBERGB_INV_GAMMA        = 1.0 / ADOBERGB_GAMMA;

    // PQ (Perceptual Quantizer) Constants (SMPTE ST 2084)
    constexpr const double PQ_M1                = 0.1593017578125;      // 2610 / (4 * 4096)
    constexpr const double PQ_M2                = 78.84375;             // (2523 / 4096) * 128
    constexpr const double PQ_C1                = 0.8359375;            // 3424 / 4096
    constexpr const double PQ_C2                = 18.8515625;           // (2413 / 4096) * 32
    constexpr const double PQ_C3                = 18.6875;              // (2392 / 4096) * 32

    // HLG (Hybrid Log-Gamma) Constants (ITU-R BT.2100)
    constexpr const double HLG_A                = 0.17883277;
    constexpr const double HLG_B                = 0.28466892;
    constexpr const double HLG_C                = 0.55991073;
    constexpr const double HLG_INV_A            = 1.0 / HLG_A;

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
    inline float sRGB2Linear(float c) noexcept
    {
        if(c <= 0.04045f)
            return c / 12.92f;
        else
            return std::pow((c + 0.055f) / 1.055f, 2.4f);
    }

    // uint8 重载：输入输出均为 0-255 范围
    inline uint8_t sRGB2Linear(uint8_t c) noexcept
    {
        float cf = static_cast<float>(c) * (1.0f / 255.0f);
        float lf = sRGB2Linear(cf);
        return static_cast<uint8_t>(std::round(lf * 255.0f));
    }

    /**
    * 将单个线性颜色分量转换为 sRGB 颜色分量
    * @param c 线性颜色分量 (0.0 - 1.0)
    * @return sRGB 颜色分量 (0.0 - 1.0)
    */
    inline float Linear2sRGB(float c) noexcept
    {
        if(c <= 0.0031308f)
            return c * 12.92f;
        else
            return 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
    }

    // uint8 重载：输入输出均为 0-255 范围
    inline uint8_t Linear2sRGB(uint8_t c) noexcept
    {
        float lf = static_cast<float>(c) * (1.0f / 255.0f);
        float sf = Linear2sRGB(lf);
        return static_cast<uint8_t>(std::round(sf * 255.0f));
    }

    /**
    * 泛型 sRGB 转线性转换 (支持 double 等其他浮点类型)
    * @param c sRGB 颜色分量
    * @return 线性颜色分量
    */
    template<FloatingPoint T>
    inline T sRGBToLinearGeneric(T c) noexcept
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
    inline T linearToSRGBGeneric(T c) noexcept
    {
        if(c <= static_cast<T>(LINEAR_SRGB_THRESHOLD))
            return c * static_cast<T>(SRGB_LINEAR_DIVISOR);
        else
            return static_cast<T>(1.055) * std::pow(c, static_cast<T>(INV_GAMMA)) - static_cast<T>(SRGB_ALPHA);
    }

    // ===== Adobe RGB 转换函数 (Gamma 2.2) =====

    /**
    * Adobe RGB 转线性
    * @param c Adobe RGB 颜色分量
    * @return 线性颜色分量
    */
    template<FloatingPoint T>
    inline T adobeRGBToLinear(T c) noexcept
    {
        return std::pow(c, static_cast<T>(ADOBERGB_GAMMA));
    }

    /**
    * 线性转 Adobe RGB
    * @param c 线性颜色分量
    * @return Adobe RGB 颜色分量
    */
    template<FloatingPoint T>
    inline T linearToAdobeRGB(T c) noexcept
    {
        return std::pow(c, static_cast<T>(ADOBERGB_INV_GAMMA));
    }

    // ===== BT.709 转换函数 (Gamma 2.4) =====
    // BT.709 与 sRGB 使用相同的转移函数

    template<FloatingPoint T>
    inline T bt709ToLinear(T c) noexcept
    {
        return sRGBToLinearGeneric(c);
    }

    template<FloatingPoint T>
    inline T linearToBT709(T c) noexcept
    {
        return linearToSRGBGeneric(c);
    }

    // ===== BT.2020 转换函数 (Gamma 2.4) =====
    // BT.2020 在线性空间中与 BT.709 相同，主要区别在于色域

    template<FloatingPoint T>
    inline T bt2020ToLinear(T c) noexcept
    {
        return sRGBToLinearGeneric(c);
    }

    template<FloatingPoint T>
    inline T linearToBT2020(T c) noexcept
    {
        return linearToSRGBGeneric(c);
    }

    // ===== PQ (Perceptual Quantizer) 转换函数 (SMPTE ST 2084 / HDR10) =====

    /**
    * PQ 逆转移函数 (OETF) - 将 PQ 编码的值转为线性
    * 用于 HDR10 显示
    * @param x PQ 编码值 (0.0 - 1.0)
    * @return 线性亮度值 (0.0 - 1.0 或更高用于 HDR)
    */
    template<FloatingPoint T>
    inline T pqToLinear(T x) noexcept
    {
        // Inverse PQ function
        T xpow = std::pow(x, 1.0 / static_cast<T>(PQ_M2));
        T numerator = std::max(xpow - static_cast<T>(PQ_C1), static_cast<T>(0.0));
        T denominator = static_cast<T>(PQ_C2) - static_cast<T>(PQ_C3) * xpow;

        if (denominator <= static_cast<T>(0.0))
            return static_cast<T>(0.0);

        T lms = std::pow(numerator / denominator, 1.0 / static_cast<T>(PQ_M1));

        // LMS to linear RGB (approximate, assuming D65 white point)
        return lms;
    }

    /**
    * 线性转 PQ 编码 (EOTF) - 将线性值编码为 PQ
    * 用于 HDR10 显示
    * @param l 线性亮度值 (0.0 - 1.0 或更高用于 HDR)
    * @return PQ 编码值 (0.0 - 1.0)
    */
    template<FloatingPoint T>
    inline T linearToPQ(T l) noexcept
    {
        // Linear to LMS (approximate)
        T lms = l;

        T lmspow = std::pow(lms, static_cast<T>(PQ_M1));
        T numerator = static_cast<T>(PQ_C1) + static_cast<T>(PQ_C2) * lmspow;
        T denominator = static_cast<T>(1.0) + static_cast<T>(PQ_C3) * lmspow;
        T ratio = numerator / denominator;

        return std::pow(ratio, static_cast<T>(PQ_M2));
    }

    // ===== HLG (Hybrid Log-Gamma) 转换函数 (ITU-R BT.2100) =====

    /**
    * HLG 逆转移函数 - 将 HLG 编码的值转为线性
    * @param x HLG 编码值 (0.0 - 1.0)
    * @return 线性值 (0.0 - 1.0)
    */
    template<FloatingPoint T>
    inline T hlgToLinear(T x) noexcept
    {
        if (x == static_cast<T>(0.0))
            return static_cast<T>(0.0);

        if (x < static_cast<T>(0.5))
        {
            return (x / static_cast<T>(HLG_B)) * (x / static_cast<T>(HLG_B));
        }
        else
        {
            return (std::exp((x - static_cast<T>(HLG_C)) / static_cast<T>(HLG_A)) + static_cast<T>(HLG_B)) / 12.0;
        }
    }

    /**
    * 线性转 HLG 编码
    * @param l 线性值 (0.0 - 1.0)
    * @return HLG 编码值 (0.0 - 1.0)
    */
    template<FloatingPoint T>
    inline T linearToHLG(T l) noexcept
    {
        if (l == static_cast<T>(0.0))
            return static_cast<T>(0.0);

        if (l < static_cast<T>(1.0 / 12.0))
        {
            return std::sqrt(3.0 * l);
        }
        else
        {
            return static_cast<T>(HLG_A) * std::log(12.0 * l - static_cast<T>(HLG_B)) + static_cast<T>(HLG_C);
        }
    }

    // ===== 通用转移函数转换 =====

    /**
    * 根据转移函数类型将颜色转换到线性空间
    * @param c 颜色分量
    * @param tf 转移函数类型
    * @return 线性颜色分量
    */
    template<FloatingPoint T>
    inline T toLinear(T c, TransferFunction tf) noexcept
    {
        switch (tf)
        {
            case TransferFunction::Linear:
            case TransferFunction::EXTENDED_SRGB_LINEAR:
                return c;

            case TransferFunction::SRGB:
            case TransferFunction::LinearSRGB:
            case TransferFunction::DisplayP3:
            case TransferFunction::BT709:
            case TransferFunction::BT2020:
            case TransferFunction::DCI_P3:
                return sRGBToLinearGeneric(c);

            case TransferFunction::AdobeRGB:
                return adobeRGBToLinear(c);

            case TransferFunction::PQ:
                return pqToLinear(c);

            case TransferFunction::HLG:
                return hlgToLinear(c);

            case TransferFunction::DCIP3XYZ:
            case TransferFunction::DolbyVision:
            default:
                return c;
        }
    }

    /**
    * 根据转移函数类型将线性颜色转换到指定的转移函数编码
    * @param c 线性颜色分量
    * @param tf 转移函数类型
    * @return 转移函数编码的颜色分量
    */
    template<FloatingPoint T>
    inline T fromLinear(T c, TransferFunction tf) noexcept
    {
        switch (tf)
        {
            case TransferFunction::Linear:
            case TransferFunction::EXTENDED_SRGB_LINEAR:
                return c;

            case TransferFunction::SRGB:
            case TransferFunction::LinearSRGB:
            case TransferFunction::DisplayP3:
            case TransferFunction::BT709:
            case TransferFunction::BT2020:
            case TransferFunction::DCI_P3:
                return linearToSRGBGeneric(c);

            case TransferFunction::AdobeRGB:
                return linearToAdobeRGB(c);

            case TransferFunction::PQ:
                return linearToPQ(c);

            case TransferFunction::HLG:
                return linearToHLG(c);

            case TransferFunction::DCIP3XYZ:
            case TransferFunction::DolbyVision:
            default:
                return c;
        }
    }

    // ===== 从 Vulkan 颜色空间获取转移函数 =====

    /**
    * 从 Vulkan 颜色空间常数获取对应的转移函数类型
    * 注意: VkColorSpaceKHR 是一个整数类型的句柄，这里使用 int 进行参数化
    * @param colorspace Vulkan 颜色空间 (VkColorSpaceKHR 值)
    * @return 转移函数类型
    *
    * Vulkan 颜色空间值 (from vulkan_core.h):
    * VK_COLOR_SPACE_SRGB_NONLINEAR_KHR = 0
    * VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT = 1000104001
    * VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT = 1000104002
    * VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT = 1000104003
    * VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT = 1000104004
    * VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT = 1000104005
    * VK_COLOR_SPACE_BT709_LINEAR_EXT = 1000456000
    * VK_COLOR_SPACE_BT709_NONLINEAR_EXT = 1000456001
    * VK_COLOR_SPACE_BT2020_LINEAR_EXT = 1000456002
    * VK_COLOR_SPACE_HDR10_ST2084_EXT = 1000462000
    * VK_COLOR_SPACE_HDR10_HLG_EXT = 1000462001
    * VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT = 1000462002
    * VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT = 1000462003
    * VK_COLOR_SPACE_DOLBYVISION_EXT = 1000379000
    * VK_COLOR_SPACE_DISPLAY_NATIVE_AMD = 1000213000
    */
    constexpr TransferFunction getTransferFunctionFromVulkanColorSpace(int colorspace) noexcept
    {
        // Nonlinear color spaces (with OETF/gamma)
        if (colorspace == 0)  // VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
            return TransferFunction::SRGB;
        if (colorspace == 1000104001)  // VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT
            return TransferFunction::DisplayP3;
        if (colorspace == 1000104005)  // VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT
            return TransferFunction::DCI_P3;
        if (colorspace == 1000456001)  // VK_COLOR_SPACE_BT709_NONLINEAR_EXT
            return TransferFunction::BT709;
        if (colorspace == 1000462003)  // VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT
            return TransferFunction::AdobeRGB;
        if (colorspace == 1000104003)  // VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT
            return TransferFunction::SRGB;
        if (colorspace == 1000213000)  // VK_COLOR_SPACE_DISPLAY_NATIVE_AMD
            return TransferFunction::SRGB;

        // Linear color spaces (no OETF/gamma)
        if (colorspace == 1000104002)  // VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT
            return TransferFunction::LinearSRGB;
        if (colorspace == 1000104004)  // VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT
            return TransferFunction::LinearSRGB;
        if (colorspace == 1000456000)  // VK_COLOR_SPACE_BT709_LINEAR_EXT
            return TransferFunction::Linear;
        if (colorspace == 1000456002)  // VK_COLOR_SPACE_BT2020_LINEAR_EXT
            return TransferFunction::Linear;
        if (colorspace == 1000462002)  // VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT
            return TransferFunction::Linear;

        // HDR color spaces
        if (colorspace == 1000462000)  // VK_COLOR_SPACE_HDR10_ST2084_EXT
            return TransferFunction::PQ;
        if (colorspace == 1000462001)  // VK_COLOR_SPACE_HDR10_HLG_EXT
            return TransferFunction::HLG;
        if (colorspace == 1000379000)  // VK_COLOR_SPACE_DOLBYVISION_EXT
            return TransferFunction::DolbyVision;

        return TransferFunction::Linear;
    }

    /**
    * 将 sRGB RGB 三原色转换为线性 RGB
    * @param sr, sg, sb sRGB 颜色分量 (0.0 - 1.0)
    * @param lr, lg, lb 输出线性颜色分量
    */
    template<ColorComponent T>
    inline void sRGB2Linear(T sr, T sg, T sb, T &lr, T &lg, T &lb) noexcept
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
            lr = static_cast<T>(sRGB2Linear(fsr) * 255.0f);
            lg = static_cast<T>(sRGB2Linear(fsg) * 255.0f);
            lb = static_cast<T>(sRGB2Linear(fsb) * 255.0f);
        }
    }

    /**
    * 将线性 RGB 三原色转换为 sRGB
    * @param lr, lg, lb 线性颜色分量
    * @param sr, sg, sb 输出 sRGB 颜色分量 (0.0 - 1.0)
    */
    template<ColorComponent T>
    inline void Linear2sRGB(T lr, T lg, T lb, T &sr, T &sg, T &sb) noexcept
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
            sr = static_cast<T>(Linear2sRGB(flr) * 255.0f);
            sg = static_cast<T>(Linear2sRGB(flg) * 255.0f);
            sb = static_cast<T>(Linear2sRGB(flb) * 255.0f);
        }
    }

    // ===== Vulkan 颜色空间通用转换 =====

    /**
    * 使用指定的 Vulkan 颜色空间将 RGB 三原色转换为线性 RGB
    * @param r, g, b 在指定颜色空间中的颜色分量 (0.0 - 1.0)
    * @param colorspace Vulkan 颜色空间 (VkColorSpaceKHR 值作为 int)
    * @param lr, lg, lb 输出线性颜色分量
    */
    template<FloatingPoint T>
    inline void colorspaceToLinear(T r, T g, T b, int colorspace, T &lr, T &lg, T &lb) noexcept
    {
        TransferFunction tf = getTransferFunctionFromVulkanColorSpace(colorspace);
        lr = toLinear(r, tf);
        lg = toLinear(g, tf);
        lb = toLinear(b, tf);
    }

    /**
    * 使用指定的 Vulkan 颜色空间将线性 RGB 三原色转换到目标颜色空间
    * @param lr, lg, lb 线性颜色分量
    * @param colorspace 目标 Vulkan 颜色空间 (VkColorSpaceKHR 值作为 int)
    * @param r, g, b 输出在指定颜色空间中的颜色分量
    */
    template<FloatingPoint T>
    inline void linearToColorspace(T lr, T lg, T lb, int colorspace, T &r, T &g, T &b) noexcept
    {
        TransferFunction tf = getTransferFunctionFromVulkanColorSpace(colorspace);
        r = fromLinear(lr, tf);
        g = fromLinear(lg, tf);
        b = fromLinear(lb, tf);
    }

    // ===== 性能优化版本 =====

    /**
    * 快速 sRGB 转线性 (仅使用伽马幂运算，不考虑分段)
    * 速度更快但精度较低
    */
    template<FloatingPoint T>
    inline T sRGBToLinearFast(T c) noexcept
    {
        return std::pow(c, static_cast<T>(GAMMA));
    }

    /**
    * 快速线性转 sRGB (仅使用伽马幂运算，不考虑分段)
    * 速度更快但精度较低
    */
    template<FloatingPoint T>
    inline T linearToSRGBFast(T c) noexcept
    {
        return std::pow(c, static_cast<T>(INV_GAMMA));
    }

    /**
    * 最快速版本 (平方/开方近似)
    * 仅用于实时性要求极高的场景
    */
    template<FloatingPoint T>
    inline T sRGBToLinearFastest(T c) noexcept
    {
        return c * c;
    }

    template<FloatingPoint T>
    inline T linearToSRGBFastest(T c) noexcept
    {
        return std::sqrt(c);
    }

}//namespace hgl
