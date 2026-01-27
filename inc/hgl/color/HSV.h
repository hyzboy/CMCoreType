#pragma once

#include <hgl/math/ClampUtil.h>
#include <algorithm>
#include <cmath>

namespace hgl
{
    // ================= HSV =================
    template<typename T>
    inline void RGB2HSV(T &h, T &s, T &v, const T &r, const T &g, const T &b)
    {
        T maxc = std::max({r, g, b});
        T minc = std::min({r, g, b});
        v = maxc;
        T delta = maxc - minc;
        if (maxc == 0) {
            s = 0;
            h = 0;
            return;
        }
        s = delta / maxc;
        if (delta == 0) {
            h = 0;
        } else if (maxc == r) {
            h = (g - b) / delta;
        } else if (maxc == g) {
            h = 2 + (b - r) / delta;
        } else {
            h = 4 + (r - g) / delta;
        }
        h *= 60;
        if (h < 0) h += 360;
    }

    template<typename T>
    inline void HSV2RGB(T &r, T &g, T &b, const T &h, const T &s, const T &v)
    {
        if (s == 0) {
            r = g = b = v;
            return;
        }
        T hh = h;
        if (hh >= 360) hh = 0;
        hh /= 60;
        int i = int(hh);
        T ff = hh - i;
        T p = v * (1 - s);
        T q = v * (1 - s * ff);
        T t = v * (1 - s * (1 - ff));
        switch (i) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            default: r = v; g = p; b = q; break;
        }
    }

    // uint8 特化
    inline void RGB2HSV(uint8 &h, uint8 &s, uint8 &v, const uint8 &r, const uint8 &g, const uint8 &b)
    {
        float fh, fs, fv;
        RGB2HSV(fh, fs, fv, r / 255.0f, g / 255.0f, b / 255.0f);
        h = uint8(Clamp(fh / 360.0f * 255));
        s = uint8(Clamp(fs * 255));
        v = uint8(Clamp(fv * 255));
    }
    inline void HSV2RGB(uint8 &r, uint8 &g, uint8 &b, const uint8 &h, const uint8 &s, const uint8 &v)
    {
        float fr, fg, fb;
        HSV2RGB(fr, fg, fb, h / 255.0f * 360.0f, s / 255.0f, v / 255.0f);
        r = uint8(Clamp(fr * 255));
        g = uint8(Clamp(fg * 255));
        b = uint8(Clamp(fb * 255));
    }
}
