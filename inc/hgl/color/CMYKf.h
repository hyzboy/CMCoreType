#pragma once

#include<hgl/color/Color3f.h>
namespace hgl
{
    class CMYKub;  // Forward declaration

    /**
    * CMYK 四色模式(浮点,0-1范围)
    * 用于印刷、油墨混合等工业应用
    * C=Cyan(青), M=Magenta(洋红), Y=Yellow(黄), K=Black(黑)
    * 范围: 0.0 - 1.0 (0%到100%)
    */
    class CMYKf
    {
        void clamp();

    public:

        union
        {
            struct
            {
                float c, m, y, k;  ///<CMYK 四色分量 (0.0 - 1.0)
            };

            float cmyk[4];
        };

    public:

        CMYKf() { c = 0, m = 0, y = 0, k = 0; }                                                     ///<本类构造函数
        CMYKf(float vc, float vm, float vy, float vk) { c = vc, m = vm, y = vy, k = vk; clamp(); } ///<本类构造函数
        CMYKf(const CMYKf &v) { c = v.c, m = v.m, y = v.y, k = v.k; }                              ///<本类构造函数
        CMYKf(const CMYKub &v);                                                                     ///<从CMYKub构造
        CMYKf(const Color3f &rgb);                                                                  ///<从RGB颜色构造

        void setZero() { c = 0, m = 0, y = 0, k = 0; }                                              ///<全清为0
        void setOne() { c = 1, m = 1, y = 1, k = 1; }                                               ///<全清为1 (完全黑色)

        void set(float vc, float vm, float vy, float vk) { c = vc, m = vm, y = vy, k = vk; clamp(); } ///<设置颜色

        /**
        * 设置为白色(无油墨)
        * CMYK白色 = (0,0,0,0)
        */
        void setWhite() { c = 0, m = 0, y = 0, k = 0; }

        /**
        * 设置为黑色(纯黑油墨)
        * CMYK黑色 = (0,0,0,1)
        */
        void setBlack() { c = 0, m = 0, y = 0, k = 1; }

        /**
        * 从RGB转换到CMYK
        * @param rgb RGB颜色
        */
        void fromRGB(const Color3f &rgb);

        /**
        * 转换为RGB颜色
        * @return RGB颜色
        */
        Color3f toRGB() const;

        /**
        * 转换为CMYKub
        * @return CMYKub颜色
        */
        CMYKub toCMYKub() const;

        //操作符重载
        const CMYKf &operator=(const float *v) { c = *v++, m = *v++, y = *v++, k = *v; return *this; }
        const CMYKf &operator=(const CMYKf &v) { c = v.c, m = v.m, y = v.y, k = v.k; return *this; }
        const CMYKf &operator=(const CMYKub &v);

        bool operator==(const CMYKf &) const;
        bool operator!=(const CMYKf &) const;

        void operator+=(const CMYKf &v) { c += v.c, m += v.m, y += v.y, k += v.k; clamp(); }
        void operator-=(const CMYKf &v) { c -= v.c, m -= v.m, y -= v.y, k -= v.k; clamp(); }
        void operator*=(const CMYKf &v) { c *= v.c, m *= v.m, y *= v.y, k *= v.k; clamp(); }
        void operator/=(const CMYKf &v) { c /= v.c, m /= v.m, y /= v.y, k /= v.k; clamp(); }

        void operator*=(float v) { c *= v, m *= v, y *= v, k *= v; clamp(); }
        void operator/=(float v) { c /= v, m /= v, y /= v, k /= v; clamp(); }

        CMYKf operator+(const CMYKf &v) { return (CMYKf(c + v.c, m + v.m, y + v.y, k + v.k)); }
        CMYKf operator-(const CMYKf &v) { return (CMYKf(c - v.c, m - v.m, y - v.y, k - v.k)); }
        CMYKf operator*(const CMYKf &v) { return (CMYKf(c * v.c, m * v.m, y * v.y, k * v.k)); }
        CMYKf operator/(const CMYKf &v) { return (CMYKf(c / v.c, m / v.m, y / v.y, k / v.k)); }

        CMYKf operator*(float v) { return (CMYKf(c * v, m * v, y * v, k * v)); }
        CMYKf operator/(float v) { return (CMYKf(c / v, m / v, y / v, k / v)); }

        operator float *() { return ((float *)this); }
        operator const float *() const { return ((const float *)this); }

    }; //class CMYKf

    extern const CMYKf WhiteCMYKf;  // (0,0,0,0) 无油墨
    extern const CMYKf BlackCMYKf;  // (0,0,0,1) 纯黑
    extern const CMYKf CyanCMYKf;   // (1,0,0,0)
    extern const CMYKf MagentaCMYKf; // (0,1,0,0)
    extern const CMYKf YellowCMYKf;  // (0,0,1,0)
    extern const CMYKf RedCMYKf;     // (0,1,1,0) = Magenta + Yellow
    extern const CMYKf GreenCMYKf;   // (1,0,1,0) = Cyan + Yellow
    extern const CMYKf BlueCMYKf;    // (1,1,0,0) = Cyan + Magenta

} // namespace hgl
