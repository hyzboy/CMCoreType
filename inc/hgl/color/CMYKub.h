#pragma once

#include<hgl/platform/Platform.h>
namespace hgl
{
    class CMYKf;  // Forward declaration

    /**
    * CMYK 四色模式(无符号8位整型, 0-255范围)
    * 用于印刷、油墨混合等工业应用
    * C=Cyan(青), M=Magenta(洋红), Y=Yellow(黄), K=Black(黑)
    * 范围: 0 - 255 (0%到100%)
    */
    class CMYKub
    {
        void clamp();

    public:

        union
        {
            struct
            {
                uint8 c, m, y, k;  ///<CMYK 四色分量 (0-255)
            };

            uint8 cmyk[4];
        };

    public:

        CMYKub() { c = 0, m = 0, y = 0, k = 0; }                                                     ///<本类构造函数
        CMYKub(uint8 vc, uint8 vm, uint8 vy, uint8 vk) { c = vc, m = vm, y = vy, k = vk; clamp(); } ///<本类构造函数
        CMYKub(const CMYKub &v) { c = v.c, m = v.m, y = v.y, k = v.k; }                             ///<本类构造函数
        CMYKub(const CMYKf &v);                                                                     ///<从CMYKf构造

        void setZero() { c = 0, m = 0, y = 0, k = 0; }                                              ///<全清为0
        void setOne() { c = 255, m = 255, y = 255, k = 255; }                                       ///<全清为255 (完全黑色)

        void set(uint8 vc, uint8 vm, uint8 vy, uint8 vk) { c = vc, m = vm, y = vy, k = vk; clamp(); } ///<设置颜色
        void setf(float fc, float fm, float fy, float fk);                                           ///<按0.0-1.0的范围值设置颜色

        /**
        * 设置为白色(无油墨)
        * CMYK白色 = (0,0,0,0)
        */
        void setWhite() { c = 0, m = 0, y = 0, k = 0; }

        /**
        * 设置为黑色(纯黑油墨)
        * CMYK黑色 = (0,0,0,255)
        */
        void setBlack() { c = 0, m = 0, y = 0, k = 255; }

        /**
        * 转换为CMYKf
        * @return CMYKf颜色
        */
        CMYKf toCMYKf() const;

        //操作符重载
        const CMYKub &operator=(const uint8 *v) { c = *v++, m = *v++, y = *v++, k = *v; return *this; }
        const CMYKub &operator=(const CMYKub &v) { c = v.c, m = v.m, y = v.y, k = v.k; return *this; }
        const CMYKub &operator=(const CMYKf &v);

        bool operator==(const CMYKub &) const;
        bool operator!=(const CMYKub &) const;

        void operator+=(const CMYKub &v) { c += v.c, m += v.m, y += v.y, k += v.k; clamp(); }
        void operator-=(const CMYKub &v) { c -= v.c, m -= v.m, y -= v.y, k -= v.k; clamp(); }
        void operator*=(const CMYKub &v) { c *= v.c, m *= v.m, y *= v.y, k *= v.k; clamp(); }
        void operator/=(const CMYKub &v) { c /= v.c, m /= v.m, y /= v.y, k /= v.k; clamp(); }

        void operator*=(float v) { c = uint8(c * v), m = uint8(m * v), y = uint8(y * v), k = uint8(k * v); clamp(); }
        void operator/=(float v) { c = uint8(c / v), m = uint8(m / v), y = uint8(y / v), k = uint8(k / v); clamp(); }

        CMYKub operator+(const CMYKub &v) { return (CMYKub(c + v.c, m + v.m, y + v.y, k + v.k)); }
        CMYKub operator-(const CMYKub &v) { return (CMYKub(c - v.c, m - v.m, y - v.y, k - v.k)); }
        CMYKub operator*(const CMYKub &v) { return (CMYKub(c * v.c, m * v.m, y * v.y, k * v.k)); }
        CMYKub operator/(const CMYKub &v) { return (CMYKub(c / v.c, m / v.m, y / v.y, k / v.k)); }

        CMYKub operator*(float v) { return (CMYKub(uint8(c * v), uint8(m * v), uint8(y * v), uint8(k * v))); }
        CMYKub operator/(float v) { return (CMYKub(uint8(c / v), uint8(m / v), uint8(y / v), uint8(k / v))); }

        operator uint8 *() { return ((uint8 *)this); }
        operator const uint8 *() const { return ((const uint8 *)this); }

    }; //class CMYKub

    extern const CMYKub WhiteCMYKub;  // (0,0,0,0) 无油墨
    extern const CMYKub BlackCMYKub;  // (0,0,0,255) 纯黑
    extern const CMYKub CyanCMYKub;   // (255,0,0,0)
    extern const CMYKub MagentaCMYKub; // (0,255,0,0)
    extern const CMYKub YellowCMYKub;  // (0,0,255,0)
    extern const CMYKub RedCMYKub;     // (0,255,255,0) = Magenta + Yellow
    extern const CMYKub GreenCMYKub;   // (255,0,255,0) = Cyan + Yellow
    extern const CMYKub BlueCMYKub;    // (255,255,0,0) = Cyan + Magenta

} // namespace hgl
