#pragma once

#include<hgl/color/Color3f.h>
namespace hgl
{
    class Color4ub;  // Forward declaration

    #define HGL_FLOAT_TO_U32(c1,c2,c3,c4)   uint32( \
                                                    (uint8(c1 * 255) << 24  )   |   \
                                                    (uint8(c2 * 255) << 16  )   |   \
                                                    (uint8(c3 * 255) << 8   )   |   \
                                                    (uint8(c4 * 255)        )   )

    #define HGL_FLOAT_TO_RGBA8(r,g,b,a) HGL_FLOAT_TO_U32(r,g,b,a)
    #define HGL_FLOAT_TO_BGRA8(r,g,b,a) HGL_FLOAT_TO_U32(b,g,r,a)
    #define HGL_FLOAT_TO_ARGB8(r,g,b,a) HGL_FLOAT_TO_U32(a,r,g,b)
    #define HGL_FLOAT_TO_ABGR8(r,g,b,a) HGL_FLOAT_TO_U32(a,b,g,r)

    /**
    * r,g,b,a四原色类
    */
    class Color4f                                                                                   ///RGBA四原色类
    {
        void clamp();

    public:

        union
        {
            struct
            {
                float r,g,b,a;                                                                              ///<rgba 4原色
            };

            float rgba[4];
        };

    public:

        Color4f(){r=0,g=0,b=0,a=1;}                                                                 ///<本类构造函数
        Color4f(float v){r=v,g=v,b=v,a=1;clamp();}                                                  ///<本类构造函数
        Color4f(float vr,float vg,float vb,float va){r=vr,g=vg,b=vb;a=va;clamp();}                  ///<本类构造函数
        Color4f(const Color3f &v,float va=1){setFrom3f(v,va);}                                       ///<本类构造函数
        Color4f(const Color4ub &v);                                                                 ///<从Color4ub构造

        void setZero(){r=0,g=0,b=0,a=0;}                                                            ///<全清为0
        void setOne() {r=1,g=1,b=1,a=1;}                                                            ///<全清为1

        void blendA(const Color4f &v,float va){r=v.r;g=v.g;b=v.b;a=v.a*va;clamp();}                 ///<混合Alpha

        void set(const float *rgba){r=rgba[0];g=rgba[1];b=rgba[2];a=rgba[3];clamp();}
        void setFrom3f(const Color3f &v,float va=1){r=v.r;g=v.g;b=v.b;a=va;clamp();}                ///<设置颜色
        void set(float vr,float vg,float vb){r=vr,g=vg,b=vb;clamp();}                                ///<设置颜色
        void set(float vr,float vg,float vb,float va){r=vr,g=vg,b=vb;a=va;clamp();}                  ///<设置颜色
        void set255(int vr,int vg,int vb,int va)                                                    ///<按0-255的范围值设置颜色
            {r=float(vr)/255.0f,g=float(vg)/255.0f,b=float(vb)/255.0f;a=float(va)/255.0f;clamp();}

        void setLum(float v){if(v<0)setZero();else if(v>1)setOne();else{r=v;g=v;b=v;}}               ///<设置颜色

        void lerp(float,float,float,float);                                                         ///<插值到另一颜色
        void lerp(const Color4f &c,float v){lerp(c.r,c.g,c.b,v);}                                   ///<插值到另一颜色
        void lerp(const Color3f &c,float v){lerp(c.r,c.g,c.b,v);}                                   ///<插值到另一颜色

        uint32 toRGBA8()const{ return HGL_FLOAT_TO_RGBA8(r, g, b, a); }                             ///<输出一个rgba8格式的颜色数据
        uint32 toBGRA8()const{ return HGL_FLOAT_TO_BGRA8(r, g, b, a); }                             ///<输出一个bgra8格式的颜色数据
        uint32 toARGB8()const{ return HGL_FLOAT_TO_ARGB8(r, g, b, a); }                             ///<输出一个argb8格式的颜色数据
        uint32 toABGR8()const{ return HGL_FLOAT_TO_ABGR8(r, g, b, a); }                             ///<输出一个abgr8格式的颜色数据

        uint32 toShaderRGBA8()const{return toRGBA8();}                                              ///<输出一个shader rgba8格式的颜色数据

        float lum(){return RGB2Lum(r,g,b);}                                                         ///<灰度值
        void makeGrey();                                                                             ///<将当前色彩变成灰色

        Color4ub toColor4ub() const;                                                                ///<转换为Color4ub

        //操作符重载
        const Color4f &operator = (const float *v){r=*v++;g=*v++;b=*v++;a=*v;return *this;}
        const Color4f &operator = (const Color3f &v){r=v.r;g=v.g;b=v.b;a=1;;return *this;}
        const Color4f &operator = (const Color4f &v){r=v.r;g=v.g;b=v.b;a=v.a;;return *this;}
        const Color4f &operator = (const Color4ub &v);

        bool operator == (const Color4f &)const;
        bool operator != (const Color4f &)const;

        void operator += (const Color4f &v){r+=v.r;g+=v.g;b+=v.b;a+=v.a;clamp();}
        void operator -= (const Color4f &v){r-=v.r;g-=v.g;b-=v.b;a-=v.a;clamp();}
        void operator *= (const Color4f &v){r*=v.r;g*=v.g;b*=v.b;a*=v.a;clamp();}
        void operator /= (const Color4f &v){r/=v.r;g/=v.g;b/=v.b;a/=v.a;clamp();}

        void operator += (const Color3f &v){r+=v.r;g+=v.g;b+=v.b;clamp();}
        void operator -= (const Color3f &v){r-=v.r;g-=v.g;b-=v.b;clamp();}
        void operator *= (const Color3f &v){r*=v.r;g*=v.g;b*=v.b;clamp();}
        void operator /= (const Color3f &v){r/=v.r;g/=v.g;b/=v.b;clamp();}

        void operator *= (float v){r*=v;g*=v;b*=v;a*=v;clamp();}
        void operator /= (float v){r/=v;g/=v;b/=v;a/=v;clamp();}

        Color4f operator + (const Color4f &v){return(Color4f(r+v.r,g+v.g,b+v.b,a+v.a));}
        Color4f operator - (const Color4f &v){return(Color4f(r-v.r,g-v.g,b-v.b,a-v.a));}
        Color4f operator * (const Color4f &v){return(Color4f(r*v.r,g*v.g,b*v.b,a*v.a));}
        Color4f operator / (const Color4f &v){return(Color4f(r/v.r,g/v.g,b/v.b,a/v.a));}

        Color4f operator * (float v){return(Color4f(r*v,g*v,b*v,a*v));}
        Color4f operator / (float v){return(Color4f(r/v,g/v,b/v,a/v));}

        operator float *() const {return((float *)this);}                                           //使得本类可以直接当做float *使用
        operator const float *() const {return((const float *)this);}                               //使得本类可以直接当做const float *使用
    };//class Color4f

    #define DEF_RGBA_U8_TO_COLOR4F(r,g,b,a)     Color4f(float(r)/255.0f,float(g)/255.0f,float(b)/255.0f,float(a)/255.0f)
}//namespace hgl
