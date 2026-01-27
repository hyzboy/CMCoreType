#pragma once

#include<hgl/math/ClampUtil.h>

namespace hgl
{
    // YCoCg 颜色空间转换系数
    constexpr const double YCOCG_OFFSET = 0.5;

    // RGB -> YCoCg
    // Y  = 0.25*R + 0.5*G + 0.25*B
    // Co = 0.5*R - 0.5*B
    // Cg = -0.25*R + 0.5*G - 0.25*B

    template<typename T>
    inline void RGB2YCoCg(T &y, T &co, T &cg, const T &r, const T &g, const T &b)
    {
        y  = 0.25 * r + 0.5 * g + 0.25 * b;
        co = 0.5 * r - 0.5 * b;
        cg = -0.25 * r + 0.5 * g - 0.25 * b;
    }

    template<typename T>
    inline void YCoCg2RGB(T &r, T &g, T &b, const T &y, const T &co, const T &cg)
    {
        r = y + co - cg;
        g = y + cg;
        b = y - co - cg;
    }

    inline void RGB2YCoCg(uint8 &y, uint8 &co, uint8 &cg, const uint8 &r, const uint8 &g, const uint8 &b)
    {
        y  = uint8(Clamp(0.25 * r + 0.5 * g + 0.25 * b));
        co = uint8(Clamp(0.5 * r - 0.5 * b + YCOCG_OFFSET * 255));
        cg = uint8(Clamp(-0.25 * r + 0.5 * g - 0.25 * b + YCOCG_OFFSET * 255));
    }

    inline void YCoCg2RGB(uint8 &r, uint8 &g, uint8 &b, const uint8 &y, const uint8 &co, const uint8 &cg)
    {
        double co_f = co - YCOCG_OFFSET * 255;
        double cg_f = cg - YCOCG_OFFSET * 255;
        r = uint8(Clamp(y + co_f - cg_f));
        g = uint8(Clamp(y + cg_f));
        b = uint8(Clamp(y - co_f - cg_f));
    }

    template<typename T>
    constexpr T RGB2Co(const T &r, const T &b)
    {
        return 0.5 * r - 0.5 * b;
    }

    template<>
    constexpr uint8 RGB2Co(const uint8 &r, const uint8 &b)
    {
        return uint8(Clamp(0.5 * r - 0.5 * b + YCOCG_OFFSET * 255));
    }

    template<typename T>
    constexpr T RGB2Cg(const T &r, const T &g, const T &b)
    {
        return -0.25 * r + 0.5 * g - 0.25 * b;
    }

    template<>
    constexpr uint8 RGB2Cg(const uint8 &r, const uint8 &g, const uint8 &b)
    {
        return uint8(Clamp(-0.25 * r + 0.5 * g - 0.25 * b + YCOCG_OFFSET * 255));
    }
}//namespace hgl
