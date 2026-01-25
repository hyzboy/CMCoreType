#pragma once

#include<hgl/color/Color3ub.h>
#include<hgl/math/ClampUtil.h>
#include<glm/glm.hpp>

namespace hgl
{
    class Color4f;  // Forward declaration

    #define HGL_U8_TO_U32(c1,c2,c3,c4)   uint32( \
                                                (uint8(c1) << 24  )   |   \
                                                (uint8(c2) << 16  )   |   \
                                                (uint8(c3) << 8   )   |   \
                                                (uint8(c4)        )   )

    #define HGL_U8_TO_RGBA8(r,g,b,a) HGL_U8_TO_U32(r,g,b,a)
    #define HGL_U8_TO_BGRA8(r,g,b,a) HGL_U8_TO_U32(b,g,r,a)
    #define HGL_U8_TO_ARGB8(r,g,b,a) HGL_U8_TO_U32(a,r,g,b)
    #define HGL_U8_TO_ABGR8(r,g,b,a) HGL_U8_TO_U32(a,b,g,r)

    /**
    * r,g,b,a四原色类(无符号8位整型) - 继承自 glm::u8vec4
    *
    * 这个类通过继承GLM的u8vec4获得了trivially copyable特性，
    * 同时保留了对颜色操作的便利函数。
    *
    * @note 继承自glm::u8vec4后，自动具有trivially copyable性质，
    *       支持高效的内存复制和传输操作。
    */
    class Color4ub : public glm::u8vec4                                                             ///RGBA四原色类(0-255范围)
    {
    private:
        void clamp();                                                                               ///<限制颜色值到[0,255]范围

    public:

        // 使用编译器生成的默认特殊成员函数（实现 trivially copyable）
        Color4ub() = default;                                                                       ///<默认构造：RGB为0，A为255
        Color4ub(const Color4ub&) = default;                                                        ///<默认复制构造
        Color4ub& operator=(const Color4ub&) = default;                                             ///<默认复制赋值
        ~Color4ub() = default;                                                                      ///<默认析构

        // 显式构造函数 - 调用父类构造
        explicit Color4ub(uint8 v) : glm::u8vec4(v, v, v, 255)                                     ///<单参数构造（灰度）
        {
            clamp();
        }

        Color4ub(uint8 vr, uint8 vg, uint8 vb, uint8 va) : glm::u8vec4(vr, vg, vb, va)            ///<四参数构造
        {
            clamp();
        }

        Color4ub(const Color3ub &v, uint8 va = 255) : glm::u8vec4(v.x, v.y, v.z, va)              ///<从Color3ub构造
        {
            clamp();
        }

        Color4ub(const Color4f &v);                                                                 ///<从Color4f构造

        // 设置函数
        void setZero(){x=0;y=0;z=0;w=0;}                                                            ///<全清为0
        void setOne() {x=255;y=255;z=255;w=255;}                                                    ///<全清为255

        void blendA(const Color4ub &v, uint8 va){x=v.x;y=v.y;z=v.z;w=uint8((v.w*va)/255);clamp();}  ///<混合Alpha

        void set(const uint8 *rgba){x=rgba[0];y=rgba[1];z=rgba[2];w=rgba[3];clamp();}
        void setFrom3ub(const Color3ub &v, uint8 va = 255){x=v.x;y=v.y;z=v.z;w=va;clamp();}         ///<设置颜色
        void set(uint8 vr, uint8 vg, uint8 vb){x=vr;y=vg;z=vb;clamp();}                             ///<设置颜色（RGB）
        void set(uint8 vr, uint8 vg, uint8 vb, uint8 va){x=vr;y=vg;z=vb;w=va;clamp();}              ///<设置颜色（RGBA）
        void setf(float vr, float vg, float vb, float va);                                          ///<按0.0-1.0的范围值设置颜色

        void setLum(uint8 v){if(v<0)setZero();else if(v>255)setOne();else{x=v;y=v;z=v;}}           ///<设置颜色

        // 插值函数
        void lerp(uint8 nr, uint8 ng, uint8 nb, float t);                                          ///<插值到另一颜色(t:0=当前,1=目标)
        void lerp(const Color4ub &c, float t){lerp(c.r,c.g,c.b,t);}                                 ///<插值到另一颜色
        void lerp(const Color3ub &c, float t){lerp(c.r,c.g,c.b,t);}                                 ///<插值到另一颜色

        void lerpSmooth(const Color4ub &c, float t);                                                ///<余弦插值到另一颜色
        void lerpCubic(const Color4ub &c, float t);                                                 ///<三次插值到另一颜色
        void lerpBezier(const Color4ub &control, const Color4ub &end, float t);                     ///<Bezier曲线插值到另一颜色

        // 格式转换
        uint32 toRGBA8()const{ return HGL_U8_TO_RGBA8(x, y, z, w); }                               ///<输出一个rgba8格式的颜色数据
        uint32 toBGRA8()const{ return HGL_U8_TO_BGRA8(x, y, z, w); }                               ///<输出一个bgra8格式的颜色数据
        uint32 toARGB8()const{ return HGL_U8_TO_ARGB8(x, y, z, w); }                               ///<输出一个argb8格式的颜色数据
        uint32 toABGR8()const{ return HGL_U8_TO_ABGR8(x, y, z, w); }                               ///<输出一个abgr8格式的颜色数据

        uint8 toGray() const;                                                                       ///<转换为灰度值
        void makeGray();                                                                             ///<将当前色彩变成灰色

        Color4f toColor4f() const;                                                                  ///<转换为Color4f

        // 操作符重载
        const Color4ub &operator = (const uint8 *v){x=*v++;y=*v++;z=*v++;w=*v;return *this;}
        const Color4ub &operator = (const Color3ub &v){x=v.x;y=v.y;z=v.z;w=255;return *this;}
        const Color4ub &operator = (const Color4f &v);

        bool operator == (const Color4ub &)const;
        bool operator != (const Color4ub &)const;

        void operator += (const Color4ub &v){x+=v.x;y+=v.y;z+=v.z;w+=v.w;clamp();}
        void operator -= (const Color4ub &v){x-=v.x;y-=v.y;z-=v.z;w-=v.w;clamp();}
        void operator *= (const Color4ub &v){x*=v.x;y*=v.y;z*=v.z;w*=v.w;clamp();}
        void operator /= (const Color4ub &v){x/=v.x;y/=v.y;z/=v.z;w/=v.w;clamp();}

        void operator += (const Color3ub &v){x+=v.x;y+=v.y;z+=v.z;clamp();}
        void operator -= (const Color3ub &v){x-=v.x;y-=v.y;z-=v.z;clamp();}
        void operator *= (const Color3ub &v){x*=v.x;y*=v.y;z*=v.z;clamp();}
        void operator /= (const Color3ub &v){x/=v.x;y/=v.y;z/=v.z;clamp();}

        void operator *= (float v){x=uint8(x*v);y=uint8(y*v);z=uint8(z*v);w=uint8(w*v);clamp();}
        void operator /= (float v){x=uint8(x/v);y=uint8(y/v);z=uint8(z/v);w=uint8(w/v);clamp();}

        Color4ub operator + (const Color4ub &v){return Color4ub(x+v.x,y+v.y,z+v.z,w+v.w);}
        Color4ub operator - (const Color4ub &v){return Color4ub(x-v.x,y-v.y,z-v.z,w-v.w);}
        Color4ub operator * (const Color4ub &v){return Color4ub(x*v.x,y*v.y,z*v.z,w*v.w);}
        Color4ub operator / (const Color4ub &v){return Color4ub(x/v.x,y/v.y,z/v.z,w/v.w);}

        Color4ub operator * (float v){return Color4ub(uint8(x*v),uint8(y*v),uint8(z*v),uint8(w*v));}
        Color4ub operator / (float v){return Color4ub(uint8(x/v),uint8(y/v),uint8(z/v),uint8(w/v));}

        operator uint8 *() const {return(const_cast<uint8*>(&x));}                                 //使得本类可以直接当做uint8 *使用
        operator const uint8 *() const {return(&x);}                                                //使得本类可以直接当做const uint8 *使用
    };//class Color4ub

    extern const Color4ub BlackColor4ub;
    extern const Color4ub WhiteColor4ub;
    extern const Color4ub RedColor4ub;
    extern const Color4ub GreenColor4ub;
    extern const Color4ub BlueColor4ub;
    extern const Color4ub YellowColor4ub;

    #define DEF_RGBA_U8_TO_COLOR4UB(r,g,b,a)     Color4ub(r,g,b,a)
}//namespace hgl

inline hgl::Color4ub operator + (float lhs, const hgl::Color4ub &rhs)
{
    int l = int(lhs);
    return hgl::Color4ub(hgl::ClampU8(l + int(rhs.x)), hgl::ClampU8(l + int(rhs.y)), hgl::ClampU8(l + int(rhs.z)), hgl::ClampU8(l + int(rhs.w)));
}

inline hgl::Color4ub operator - (float lhs, const hgl::Color4ub &rhs)
{
    int l = int(lhs);
    return hgl::Color4ub(hgl::ClampU8(l - int(rhs.x)), hgl::ClampU8(l - int(rhs.y)), hgl::ClampU8(l - int(rhs.z)), hgl::ClampU8(l - int(rhs.w)));
}

inline hgl::Color4ub operator * (float lhs, const hgl::Color4ub &rhs)
{
    return hgl::Color4ub(hgl::ClampU8(int(rhs.x * lhs)), hgl::ClampU8(int(rhs.y * lhs)), hgl::ClampU8(int(rhs.z * lhs)), hgl::ClampU8(int(rhs.w * lhs)));
}

inline hgl::Color4ub operator / (float lhs, const hgl::Color4ub &rhs)
{
    int l = int(lhs);
    int rx = (rhs.x != 0) ? (l / int(rhs.x)) : 255;
    int ry = (rhs.y != 0) ? (l / int(rhs.y)) : 255;
    int rz = (rhs.z != 0) ? (l / int(rhs.z)) : 255;
    int rw = (rhs.w != 0) ? (l / int(rhs.w)) : 255;
    return hgl::Color4ub(hgl::ClampU8(rx), hgl::ClampU8(ry), hgl::ClampU8(rz), hgl::ClampU8(rw));
}
