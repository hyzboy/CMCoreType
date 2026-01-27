#pragma once

#include <hgl/math/ClampUtil.h>
#include <cmath>

namespace hgl
{
    // ================= OKLab =================
    // Reference: Björn Ottosson — https://bottosson.github.io/posts/oklab/

    // Linear sRGB -> OKLab via LMS intermediate
    template<typename T>
    inline void RGB2OKLab(T &l, T &a, T &lab_b, const T &r, const T &g, const T &b)
    {
        // Linear RGB to LMS
        T lr = r;
        T lg = g;
        T lb = b;

        T l_val = static_cast<T>(0.4122214708) * lr + static_cast<T>(0.5363325363) * lg + static_cast<T>(0.0514459929) * lb;
        T m_val = static_cast<T>(0.2119034982) * lr + static_cast<T>(0.6806995451) * lg + static_cast<T>(0.1073969566) * lb;
        T s_val = static_cast<T>(0.0883024619) * lr + static_cast<T>(0.2817188376) * lg + static_cast<T>(0.6299787005) * lb;

        // Non-linearity (cube root)
        T l_ = std::cbrt(l_val);
        T m_ = std::cbrt(m_val);
        T s_ = std::cbrt(s_val);

        // LMS -> OKLab
        l     = static_cast<T>(0.2104542553) * l_ + static_cast<T>(0.7936177850) * m_ - static_cast<T>(0.0040720468) * s_;
        a     = static_cast<T>(1.9779984951) * l_ - static_cast<T>(2.4285922050) * m_ + static_cast<T>(0.4505937099) * s_;
        lab_b = static_cast<T>(0.0259040371) * l_ + static_cast<T>(0.7827717662) * m_ - static_cast<T>(0.8086757660) * s_;
    }

    // OKLab -> Linear sRGB via LMS intermediate
    template<typename T>
    inline void OKLab2RGB(T &r, T &g, T &b, const T &l, const T &a, const T &lab_b)
    {
        // OKLab to LMS'
        T l_ = l + static_cast<T>(0.3963377774) * a + static_cast<T>(0.2158037573) * lab_b;
        T m_ = l - static_cast<T>(0.1055613458) * a - static_cast<T>(0.0638541728) * lab_b;
        T s_ = l - static_cast<T>(0.0894841775) * a - static_cast<T>(1.2914855480) * lab_b;

        // Remove non-linearity
        T l3 = l_ * l_ * l_;
        T m3 = m_ * m_ * m_;
        T s3 = s_ * s_ * s_;

        // LMS -> Linear RGB
        r = static_cast<T>( 4.0767416621) * l3 + static_cast<T>(-3.3077115913) * m3 + static_cast<T>( 0.2309699284) * s3;
        g = static_cast<T>(-1.2684380046) * l3 + static_cast<T>( 2.6097574011) * m3 + static_cast<T>(-0.3413193965) * s3;
        b = static_cast<T>( 0.0044216692) * l3 + static_cast<T>(-0.7039404653) * m3 + static_cast<T>( 1.7047851197) * s3;
    }

    // ===== uint8 convenience overloads =====
    // Note: For 8-bit usage, we map L to [0,1]→[0,255], and a/b to [-0.5,0.5]→[0,255] with 0.5 offset.
    // This follows the style in YCoCg.h for signed channels.

    inline void RGB2OKLab(uint8 &l8, uint8 &a8, uint8 &b8, const uint8 &r8, const uint8 &g8, const uint8 &b_in8)
    {
        float l, a, b;
        RGB2OKLab(l, a, b, r8 / 255.0f, g8 / 255.0f, b_in8 / 255.0f);
        l8 = uint8(Clamp(l * 255));
        a8 = uint8(Clamp((a + 0.5f) * 255));
        b8 = uint8(Clamp((b + 0.5f) * 255));
    }

    inline void OKLab2RGB(uint8 &r8, uint8 &g8, uint8 &b8, const uint8 &l8, const uint8 &a8, const uint8 &b_in8)
    {
        float r, g, b;
        float l = l8 / 255.0f;
        float a = a8 / 255.0f - 0.5f;
        float lab_b = b_in8 / 255.0f - 0.5f;
        OKLab2RGB(r, g, b, l, a, lab_b);
        r8 = uint8(Clamp(r * 255));
        g8 = uint8(Clamp(g * 255));
        b8 = uint8(Clamp(b * 255));
    }
}// namespace hgl
