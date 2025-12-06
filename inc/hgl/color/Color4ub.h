#pragma once

#include<hgl/color/Color3ub.h>
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
    * r,g,b,a四原色类(无符号8位整型)
    */
    class Color4ub                                                                                  ///RGBA四原色类(0-255范围)
    {
        void clamp();

    public:

        union
        {
            struct
            {
                uint8 r,g,b,a;                                                                      ///<rgba 4原色 (0-255)
            };

            uint8 rgba[4];
        };

    public:

        Color4ub(){r=0,g=0,b=0,a=255;}                                                              ///<本类构造函数
        Color4ub(uint8 v){r=v,g=v,b=v,a=255;clamp();}                                             ///<本类构造函数
        Color4ub(uint8 vr,uint8 vg,uint8 vb,uint8 va){r=vr,g=vg,b=vb;a=va;clamp();}              ///<本类构造函数
        Color4ub(const Color3ub &v,uint8 va=255){setFrom3ub(v,va);}                                 ///<本类构造函数
        Color4ub(const Color4f &v);                                                                ///<从Color4f构造

        void setZero(){r=0,g=0,b=0,a=0;}                                                            ///<全清为0
        void setOne() {r=255,g=255,b=255,a=255;}                                                    ///<全清为255

        void blendA(const Color4ub &v,uint8 va){r=v.r;g=v.g;b=v.b;a=uint8((v.a*va)/255);clamp();}   ///<混合Alpha

        void set(const uint8 *rgba){r=rgba[0];g=rgba[1];b=rgba[2];a=rgba[3];clamp();}
        void setFrom3ub(const Color3ub &v,uint8 va=255){r=v.r;g=v.g;b=v.b;a=va;clamp();}            ///<设置颜色
        void set(uint8 vr,uint8 vg,uint8 vb){r=vr,g=vg,b=vb;clamp();}                               ///<设置颜色
        void set(uint8 vr,uint8 vg,uint8 vb,uint8 va){r=vr,g=vg,b=vb;a=va;clamp();}                  ///<设置颜色
        void setf(float vr,float vg,float vb,float va);                                           ///<按0.0-1.0的范围值设置颜色

        void setLum(uint8 v){if(v<0)setZero();else if(v>255)setOne();else{r=v;g=v;b=v;}}           ///<设置颜色

        void lerp(uint8,uint8,uint8,float);                                                        ///<插值到另一颜色
        void lerp(const Color4ub &c,float v){lerp(c.r,c.g,c.b,v);}                                  ///<插值到另一颜色
        void lerp(const Color3ub &c,float v){lerp(c.r,c.g,c.b,v);}                                  ///<插值到另一颜色

        uint32 toRGBA8()const{ return HGL_U8_TO_RGBA8(r, g, b, a); }                               ///<输出一个rgba8格式的颜色数据
        uint32 toBGRA8()const{ return HGL_U8_TO_BGRA8(r, g, b, a); }                               ///<输出一个bgra8格式的颜色数据
        uint32 toARGB8()const{ return HGL_U8_TO_ARGB8(r, g, b, a); }                               ///<输出一个argb8格式的颜色数据
        uint32 toABGR8()const{ return HGL_U8_TO_ABGR8(r, g, b, a); }                               ///<输出一个abgr8格式的颜色数据

        uint8 toGray() const;                                                                       ///<转换为灰度值
        void makeGray();                                                                             ///<将当前色彩变成灰色

        Color4f toColor4f() const;                                                                  ///<转换为Color4f

        //操作符重载
        const Color4ub &operator = (const uint8 *v){r=*v++;g=*v++;b=*v++;a=*v;return *this;}
        const Color4ub &operator = (const Color3ub &v){r=v.r;g=v.g;b=v.b;a=255;return *this;}
        const Color4ub &operator = (const Color4ub &v){r=v.r;g=v.g;b=v.b;a=v.a;return *this;}
        const Color4ub &operator = (const Color4f &v);

        bool operator == (const Color4ub &)const;
        bool operator != (const Color4ub &)const;

        void operator += (const Color4ub &v){r+=v.r;g+=v.g;b+=v.b;a+=v.a;clamp();}
        void operator -= (const Color4ub &v){r-=v.r;g-=v.g;b-=v.b;a-=v.a;clamp();}
        void operator *= (const Color4ub &v){r*=v.r;g*=v.g;b*=v.b;a*=v.a;clamp();}
        void operator /= (const Color4ub &v){r/=v.r;g/=v.g;b/=v.b;a/=v.a;clamp();}

        void operator += (const Color3ub &v){r+=v.r;g+=v.g;b+=v.b;clamp();}
        void operator -= (const Color3ub &v){r-=v.r;g-=v.g;b-=v.b;clamp();}
        void operator *= (const Color3ub &v){r*=v.r;g*=v.g;b*=v.b;clamp();}
        void operator /= (const Color3ub &v){r/=v.r;g/=v.g;b/=v.b;clamp();}

        void operator *= (float v){r=uint8(r*v);g=uint8(g*v);b=uint8(b*v);a=uint8(a*v);clamp();}
        void operator /= (float v){r=uint8(r/v);g=uint8(g/v);b=uint8(b/v);a=uint8(a/v);clamp();}

        Color4ub operator + (const Color4ub &v){return(Color4ub(r+v.r,g+v.g,b+v.b,a+v.a));}
        Color4ub operator - (const Color4ub &v){return(Color4ub(r-v.r,g-v.g,b-v.b,a-v.a));}
        Color4ub operator * (const Color4ub &v){return(Color4ub(r*v.r,g*v.g,b*v.b,a*v.a));}
        Color4ub operator / (const Color4ub &v){return(Color4ub(r/v.r,g/v.g,b/v.b,a/v.a));}

        Color4ub operator * (float v){return(Color4ub(uint8(r*v),uint8(g*v),uint8(b*v),uint8(a*v)));}
        Color4ub operator / (float v){return(Color4ub(uint8(r/v),uint8(g/v),uint8(b/v),uint8(a/v)));}

        operator uint8 *() const {return((uint8 *)this);}                                          //使得本类可以直接当做uint8 *使用
        operator const uint8 *() const {return((const uint8 *)this);}                              //使得本类可以直接当做const uint8 *使用
    };//class Color4ub

    #define DEF_RGBA_U8_TO_COLOR4UB(r,g,b,a)     Color4ub(r,g,b,a)
}//namespace hgl
