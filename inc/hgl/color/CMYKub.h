#pragma once

#include<hgl/platform/Platform.h>
#include<glm/glm.hpp>

namespace hgl
{
    class CMYKf;  // Forward declaration

    /**
    * CMYK 四色模式(无符号8位整型, 0-255范围) - 继承自 glm::u8vec4
    * 用于印刷、油墨混合等工业应用
    * C=Cyan(青), M=Magenta(洋红), Y=Yellow(黄), K=Black(黑)
    * 范围: 0 - 255 (0%到100%)
    *
    * @note 继承自glm::u8vec4后，自动具有trivially copyable性质
    */
    class CMYKub : public glm::u8vec4
    {
    private:
        void clamp();                                                                               ///<限制颜色值到[0,255]范围

    public:

        // 使用编译器生成的默认特殊成员函数（实现 trivially copyable）
        CMYKub() = default;                                                                         ///<默认构造：全部为0
        CMYKub(const CMYKub&) = default;                                                            ///<默认复制构造
        CMYKub& operator=(const CMYKub&) = default;                                                 ///<默认复制赋值
        ~CMYKub() = default;                                                                        ///<默认析构

        // 显式构造函数 - 调用父类构造
        CMYKub(uint8 vc, uint8 vm, uint8 vy, uint8 vk) : glm::u8vec4(vc, vm, vy, vk)               ///<四参数构造
        {
            clamp();
        }

        CMYKub(const CMYKf &v);                                                                     ///<从CMYKf构造

        // 设置函数
        void setZero() {x=0;y=0;z=0;w=0;}                                                           ///<全清为0
        void setOne() {x=255;y=255;z=255;w=255;}                                                    ///<全清为255 (完全黑色)

        void set(uint8 vc, uint8 vm, uint8 vy, uint8 vk) {x=vc;y=vm;z=vy;w=vk;clamp();}             ///<设置颜色
        void setf(float fc, float fm, float fy, float fk);                                          ///<按0.0-1.0的范围值设置颜色

        /**
        * 设置为白色(无油墨)
        * CMYK白色 = (0,0,0,0)
        */
        void setWhite() {x=0;y=0;z=0;w=0;}

        /**
        * 设置为黑色(纯黑油墨)
        * CMYK黑色 = (0,0,0,255)
        */
        void setBlack() {x=0;y=0;z=0;w=255;}

        /**
        * 转换为CMYKf
        * @return CMYKf颜色
        */
        CMYKf toCMYKf() const;

        // 操作符重载
        const CMYKub &operator=(const uint8 *v) {x=*v++;y=*v++;z=*v++;w=*v;return *this;}
        const CMYKub &operator=(const CMYKf &v);

        bool operator==(const CMYKub &) const;
        bool operator!=(const CMYKub &) const;

        void operator+=(const CMYKub &v) {x+=v.x;y+=v.y;z+=v.z;w+=v.w;clamp();}
        void operator-=(const CMYKub &v) {x-=v.x;y-=v.y;z-=v.z;w-=v.w;clamp();}
        void operator*=(const CMYKub &v) {x*=v.x;y*=v.y;z*=v.z;w*=v.w;clamp();}
        void operator/=(const CMYKub &v) {x/=v.x;y/=v.y;z/=v.z;w/=v.w;clamp();}

        void operator*=(float v) {x=uint8(x*v);y=uint8(y*v);z=uint8(z*v);w=uint8(w*v);clamp();}
        void operator/=(float v) {x=uint8(x/v);y=uint8(y/v);z=uint8(z/v);w=uint8(w/v);clamp();}

        CMYKub operator+(const CMYKub &v) {return CMYKub(x+v.x,y+v.y,z+v.z,w+v.w);}
        CMYKub operator-(const CMYKub &v) {return CMYKub(x-v.x,y-v.y,z-v.z,w-v.w);}
        CMYKub operator*(const CMYKub &v) {return CMYKub(x*v.x,y*v.y,z*v.z,w*v.w);}
        CMYKub operator/(const CMYKub &v) {return CMYKub(x/v.x,y/v.y,z/v.z,w/v.w);}

        CMYKub operator*(float v) {return CMYKub(uint8(x*v),uint8(y*v),uint8(z*v),uint8(w*v));}
        CMYKub operator/(float v) {return CMYKub(uint8(x/v),uint8(y/v),uint8(z/v),uint8(w/v));}

        operator uint8 *() {return(&x);}
        operator const uint8 *() const {return(&x);}

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
