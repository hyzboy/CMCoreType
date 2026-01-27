#pragma once

#include <hgl/math/ClampUtil.h>
#include <cmath>

namespace hgl
{
    // ================= XYZ =================
    constexpr double XYZ_X_R = 0.4124564;
    constexpr double XYZ_X_G = 0.3575761;
    constexpr double XYZ_X_B = 0.1804375;
    constexpr double XYZ_Y_R = 0.2126729;
    constexpr double XYZ_Y_G = 0.7151522;
    constexpr double XYZ_Y_B = 0.0721750;
    constexpr double XYZ_Z_R = 0.0193339;
    constexpr double XYZ_Z_G = 0.1191920;
    constexpr double XYZ_Z_B = 0.9503041;

    template<typename T>
    inline void RGB2XYZ(T &x, T &y, T &z, const T &r, const T &g, const T &b)
    {
        x = XYZ_X_R * r + XYZ_X_G * g + XYZ_X_B * b;
        y = XYZ_Y_R * r + XYZ_Y_G * g + XYZ_Y_B * b;
        z = XYZ_Z_R * r + XYZ_Z_G * g + XYZ_Z_B * b;
    }

    template<typename T>
    inline void XYZ2RGB(T &r, T &g, T &b, const T &x, const T &y, const T &z)
    {
        r =  3.2404542 * x - 1.5371385 * y - 0.4985314 * z;
        g = -0.9692660 * x + 1.8760108 * y + 0.0415560 * z;
        b =  0.0556434 * x - 0.2040259 * y + 1.0572252 * z;
    }

    // uint8 特化
    inline void RGB2XYZ(uint8 &x, uint8 &y, uint8 &z, const uint8 &r, const uint8 &g, const uint8 &b)
    {
        float fx, fy, fz;
        RGB2XYZ(fx, fy, fz, r / 255.0f, g / 255.0f, b / 255.0f);
        x = uint8(Clamp(fx * 255));
        y = uint8(Clamp(fy * 255));
        z = uint8(Clamp(fz * 255));
    }
    inline void XYZ2RGB(uint8 &r, uint8 &g, uint8 &b, const uint8 &x, const uint8 &y, const uint8 &z)
    {
        float fr, fg, fb;
        XYZ2RGB(fr, fg, fb, x / 255.0f, y / 255.0f, z / 255.0f);
        r = uint8(Clamp(fr * 255));
        g = uint8(Clamp(fg * 255));
        b = uint8(Clamp(fb * 255));
    }
}
