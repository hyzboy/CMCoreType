#pragma once

#include <hgl/math/ClampUtil.h>
#include <algorithm>
#include <cmath>

namespace hgl
{
    // ================= HSL =================
    template<typename T>
    inline void RGB2HSL(T &h, T &s, T &l, const T &r, const T &g, const T &b)
    {
        T maxc = std::max({r, g, b});
        T minc = std::min({r, g, b});
        l = (maxc + minc) / 2;
        if (maxc == minc) {
            s = 0;
            h = 0;
            return;
        }
        T delta = maxc - minc;
        s = (l > 0.5) ? delta / (2 - maxc - minc) : delta / (maxc + minc);
        if (maxc == r)
            h = (g - b) / delta + (g < b ? 6 : 0);
        else if (maxc == g)
            h = (b - r) / delta + 2;
        else
            h = (r - g) / delta + 4;
        h *= 60;
    }

    template<typename T>
    inline void HSL2RGB(T &r, T &g, T &b, const T &h, const T &s, const T &l)
    {
        if (s == 0) {
            r = g = b = l;
            return;
        }
        auto hue2rgb = [](T p, T q, T t) {
            if (t < static_cast<T>(0)) t += static_cast<T>(1);
            if (t > static_cast<T>(1)) t -= static_cast<T>(1);

            const T one_sixth = static_cast<T>(1.0 / 6.0);
            const T one_half    = static_cast<T>(1.0 / 2.0);
            const T two_thirds  = static_cast<T>(2.0 / 3.0);

            if (t < one_sixth)
                return p + (q - p) * static_cast<T>(6) * t;
            if (t < one_half)
                return q;
            if (t < two_thirds)
                return p + (q - p) * (two_thirds - t) * static_cast<T>(6);

            return p;
        };
        T q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        T p = 2 * l - q;
        T hk = h / 360;
        r = hue2rgb(p, q, hk + 1.0/3);
        g = hue2rgb(p, q, hk);
        b = hue2rgb(p, q, hk - 1.0/3);
    }

    // uint8 特化
    inline void RGB2HSL(uint8 &h, uint8 &s, uint8 &l, const uint8 &r, const uint8 &g, const uint8 &b)
    {
        float fh, fs, fl;
        RGB2HSL(fh, fs, fl, r / 255.0f, g / 255.0f, b / 255.0f);
        h = uint8(Clamp(fh / 360.0f * 255));
        s = uint8(Clamp(fs * 255));
        l = uint8(Clamp(fl * 255));
    }
    inline void HSL2RGB(uint8 &r, uint8 &g, uint8 &b, const uint8 &h, const uint8 &s, const uint8 &l)
    {
        float fr, fg, fb;
        HSL2RGB(fr, fg, fb, h / 255.0f * 360.0f, s / 255.0f, l / 255.0f);
        r = uint8(Clamp(fr * 255));
        g = uint8(Clamp(fg * 255));
        b = uint8(Clamp(fb * 255));
    }
}
