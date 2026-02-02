#pragma once

#include<hgl/platform/Platform.h>
#include<hgl/math/ClampUtil.h>
#include<glm/glm.hpp>

namespace hgl
{
    class Color3f;  // Forward declaration

    /**
    * r,g,b三原色类(无符号8位整型) - 继承自 glm::u8vec3
    *
    * 这个类通过继承GLM的u8vec3获得了trivially copyable特性，
    * 同时保留了对颜色操作的便利函数。
    *
    * @note 继承自glm::u8vec3后，自动具有trivially copyable性质，
    *       支持高效的内存复制和传输操作。
    */
    class Color3ub : public glm::u8vec3                                                             ///RGB三原色类(0-255范围)
    {
    private:
        void clamp();                                                                               ///<限制颜色值到[0,255]范围

    public:

        // 使用编译器生成的默认特殊成员函数（实现 trivially copyable）
        Color3ub():glm::u8vec3(0,0,0){};                                                            ///<默认构造：全部为0
        Color3ub(const Color3ub&) = default;                                                        ///<默认复制构造
        Color3ub& operator=(const Color3ub&) = default;                                             ///<默认复制赋值
        ~Color3ub() = default;                                                                      ///<默认析构

        // 显式构造函数 - 调用父类构造
        explicit Color3ub(uint8 l) : glm::u8vec3(l)                                                 ///<单参数构造（灰度）
        {
        }

        Color3ub(uint8 vr, uint8 vg, uint8 vb) : glm::u8vec3(vr, vg, vb)                          ///<三参数构造
        {
            clamp();
        }

        Color3ub(const Color3f &v);                                                                 ///<从Color3f构造

        // 设置函数
        void setZero(){x=0;y=0;z=0;}                                                               ///<全清为0
        void setOne() {x=255;y=255;z=255;}                                                          ///<全清为255

        void set(uint8 vr, uint8 vg, uint8 vb){x=vr;y=vg;z=vb;clamp();}                             ///<设置颜色
        void setf(float vr, float vg, float vb);                                                    ///<按0.0-1.0的范围值设置颜色

        void setLum(uint8 v){x=v;y=v;z=v;}                                                          ///<设置颜色

        // 插值函数
        void lerp(uint8 nr, uint8 ng, uint8 nb, float t);                                          ///<插值到另一颜色(t:0=当前,1=目标)
        void lerp(const uint8 *rgb, float t){lerp(*rgb,*(rgb+1),*(rgb+2),t);}                       ///<插值到另一颜色
        void lerp(const Color3ub &c, float t){lerp(c.r,c.g,c.b,t);}                                 ///<插值到另一颜色

        void lerpSmooth(const Color3ub &c, float t);                                                ///<余弦插值到另一颜色
        void lerpCubic(const Color3ub &c, float t);                                                 ///<三次插值到另一颜色
        void lerpBezier(const Color3ub &control, const Color3ub &end, float t);                     ///<Bezier曲线插值到另一颜色

        // 颜色操作
        void setBlack(){x=0;y=0;z=0;}                                                               ///<黑色
        void setWhite(){x=255;y=255;z=255;}                                                         ///<白色

        uint8 toGray() const;                                                                       ///<转换为灰度值
        void makeGray();                                                                             ///<将当前色彩变成灰色

        Color3f toColor3f() const;                                                                  ///<转换为Color3f

        // 操作符重载
        const Color3ub &operator = (const uint8 *v){x=*v++;y=*v++;z=*v;return *this;}
        const Color3ub &operator = (const Color3f &v);

        bool operator == (const Color3ub &)const;
        bool operator != (const Color3ub &)const;

        void operator += (const Color3ub &v){x+=v.x;y+=v.y;z+=v.z;clamp();}
        void operator -= (const Color3ub &v){x-=v.x;y-=v.y;z-=v.z;clamp();}
        void operator *= (const Color3ub &v){x*=v.x;y*=v.y;z*=v.z;clamp();}
        void operator /= (const Color3ub &v){x/=v.x;y/=v.y;z/=v.z;clamp();}

        void operator *= (float v){x=uint8(x*v);y=uint8(y*v);z=uint8(z*v);clamp();}
        void operator /= (float v){x=uint8(x/v);y=uint8(y/v);z=uint8(z/v);clamp();}

        Color3ub operator + (const Color3ub &v) const {return Color3ub(x+v.x,y+v.y,z+v.z);} 
        Color3ub operator - (const Color3ub &v) const {return Color3ub(x-v.x,y-v.y,z-v.z);} 
        Color3ub operator * (const Color3ub &v) const {return Color3ub(x*v.x,y*v.y,z*v.z);} 
        Color3ub operator / (const Color3ub &v) const {return Color3ub(x/v.x,y/v.y,z/v.z);} 

        Color3ub operator * (float v) const {return Color3ub(uint8(x*v),uint8(y*v),uint8(z*v));}
        Color3ub operator / (float v) const {return Color3ub(uint8(x/v),uint8(y/v),uint8(z/v));}

        operator uint8 *() const {return(const_cast<uint8*>(&x));}                                 //使得本类可以直接当做uint8 *使用
        operator const uint8 *() const {return(&x);}                                                //使得本类可以直接当做const uint8 *使用
    };//class Color3ub

    extern const Color3ub BlackColor3ub;
    extern const Color3ub WhiteColor3ub;
    extern const Color3ub RedColor3ub;
    extern const Color3ub GreenColor3ub;
    extern const Color3ub BlueColor3ub;
    extern const Color3ub YellowColor3ub;

    #define DEF_RGB_U8_TO_COLOR3UB(r,g,b)        Color3ub(r,g,b)

    // Non-member scalar-color operations to support expressions like "float + Color3ub" and return Color3ub
    inline hgl::Color3ub operator + (float lhs, const hgl::Color3ub &rhs)
    {
        int l = int(lhs);
        return hgl::Color3ub(hgl::ClampU8(l + int(rhs.x)), hgl::ClampU8(l + int(rhs.y)), hgl::ClampU8(l + int(rhs.z)));
    }

    inline hgl::Color3ub operator - (float lhs, const hgl::Color3ub &rhs)
    {
        int l = int(lhs);
        return hgl::Color3ub(hgl::ClampU8(l - int(rhs.x)), hgl::ClampU8(l - int(rhs.y)), hgl::ClampU8(l - int(rhs.z)));
    }

    inline hgl::Color3ub operator * (float lhs, const hgl::Color3ub &rhs)
    {
        return hgl::Color3ub(hgl::ClampU8(int(rhs.x * lhs)), hgl::ClampU8(int(rhs.y * lhs)), hgl::ClampU8(int(rhs.z * lhs)));
    }

    inline hgl::Color3ub operator / (float lhs, const hgl::Color3ub &rhs)
    {
        int l = int(lhs);
        int rx = (rhs.x != 0) ? (l / int(rhs.x)) : 255;
        int ry = (rhs.y != 0) ? (l / int(rhs.y)) : 255;
        int rz = (rhs.z != 0) ? (l / int(rhs.z)) : 255;
        return hgl::Color3ub(hgl::ClampU8(rx), hgl::ClampU8(ry), hgl::ClampU8(rz));
    }

}//namespace hgl
