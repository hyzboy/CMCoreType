#pragma once

#include<hgl/color/Color3f.h>
#include<glm/glm.hpp>

namespace hgl
{
    class CMYKub;  // Forward declaration

    /**
    * CMYK 四色模式(浮点,0-1范围) - 继承自 glm::vec4
    * 用于印刷、油墨混合等工业应用
    * C=Cyan(青), M=Magenta(洋红), Y=Yellow(黄), K=Black(黑)
    * 范围: 0.0 - 1.0 (0%到100%)
    *
    * @note 继承自glm::vec4后，自动具有trivially copyable性质
    */
    class CMYKf : public glm::vec4
    {
    private:
        void clamp();                                                                               ///<限制颜色值到[0,1]范围

    public:

        // 使用编译器生成的默认特殊成员函数（实现 trivially copyable）
        CMYKf() = default;                                                                          ///<默认构造：全部为0
        CMYKf(const CMYKf&) = default;                                                              ///<默认复制构造
        CMYKf& operator=(const CMYKf&) = default;                                                   ///<默认复制赋值
        ~CMYKf() = default;                                                                         ///<默认析构

        // 显式构造函数 - 调用父类构造
        CMYKf(float vc, float vm, float vy, float vk) : glm::vec4(vc, vm, vy, vk)                  ///<四参数构造
        {
            clamp();
        }

        CMYKf(const CMYKub &v);                                                                     ///<从CMYKub构造
        CMYKf(const Color3f &rgb);                                                                  ///<从RGB颜色构造

        // 设置函数
        void setZero() {x=0;y=0;z=0;w=0;}                                                           ///<全清为0
        void setOne() {x=1;y=1;z=1;w=1;}                                                            ///<全清为1 (完全黑色)

        void set(float vc, float vm, float vy, float vk) {x=vc;y=vm;z=vy;w=vk;clamp();}             ///<设置颜色

        /**
        * 设置为白色(无油墨)
        * CMYK白色 = (0,0,0,0)
        */
        void setWhite() {x=0;y=0;z=0;w=0;}

        /**
        * 设置为黑色(纯黑油墨)
        * CMYK黑色 = (0,0,0,1)
        */
        void setBlack() {x=0;y=0;z=0;w=1;}

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

        // 操作符重载
        const CMYKf &operator=(const float *v) {x=*v++;y=*v++;z=*v++;w=*v;return *this;}
        const CMYKf &operator=(const CMYKub &v);

        bool operator==(const CMYKf &) const;
        bool operator!=(const CMYKf &) const;

        void operator+=(const CMYKf &v) {x+=v.x;y+=v.y;z+=v.z;w+=v.w;clamp();}
        void operator-=(const CMYKf &v) {x-=v.x;y-=v.y;z-=v.z;w-=v.w;clamp();}
        void operator*=(const CMYKf &v) {x*=v.x;y*=v.y;z*=v.z;w*=v.w;clamp();}
        void operator/=(const CMYKf &v) {x/=v.x;y/=v.y;z/=v.z;w/=v.w;clamp();}

        void operator*=(float v) {x*=v;y*=v;z*=v;w*=v;clamp();}
        void operator/=(float v) {x/=v;y/=v;z/=v;w/=v;clamp();}

        CMYKf operator+(const CMYKf &v) {return CMYKf(x+v.x,y+v.y,z+v.z,w+v.w);}
        CMYKf operator-(const CMYKf &v) {return CMYKf(x-v.x,y-v.y,z-v.z,w-v.w);}
        CMYKf operator*(const CMYKf &v) {return CMYKf(x*v.x,y*v.y,z*v.z,w*v.w);}
        CMYKf operator/(const CMYKf &v) {return CMYKf(x/v.x,y/v.y,z/v.z,w/v.w);}

        CMYKf operator*(float v) {return CMYKf(x*v,y*v,z*v,w*v);}
        CMYKf operator/(float v) {return CMYKf(x/v,y/v,z/v,w/v);}

        operator float *() {return(&x);}
        operator const float *() const {return(&x);}

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
