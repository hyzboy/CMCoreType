#pragma once

#include<hgl/color/LinearColor3f.h>
namespace hgl
{
    class LinearColor4f;
    class Color4f;

    /**
    * 线性颜色空间RGBA四原色(浮点,无限制范围用于HDR)
    * 用于线性颜色空间计算,支持超过1.0的HDR值
    * Alpha通道不参与线性化处理,保持0-1范围
    */
    class LinearColor4f
    {
        void clamp();  //用于防止极端值,但不强制限制在0-1范围

    public:

        union
        {
            struct
            {
                float r,g,b,a;                                                                      ///<linear rgba (r,g,b无限制, a为0-1)
            };

            float rgba[4];
        };

    public:

        LinearColor4f(){r=0,g=0,b=0,a=1;}                                                           ///<本类构造函数
        LinearColor4f(float v){r=v,g=v,b=v,a=1;}                                                    ///<本类构造函数(灰度)
        LinearColor4f(float vr,float vg,float vb,float va){r=vr,g=vg,b=vb;a=va;}                    ///<本类构造函数
        LinearColor4f(const LinearColor3f &v,float va=1){r=v.r;g=v.g;b=v.b;a=va;}                   ///<从LinearColor3f构造
        LinearColor4f(const LinearColor4f &v){r=v.r;g=v.g;b=v.b;a=v.a;}                             ///<本类构造函数
        LinearColor4f(const Color4f &v);                                                            ///<从sRGB Color4f构造

        void setZero(){r=0,g=0,b=0,a=0;}                                                            ///<全清为0
        void setOne(){r=1,g=1,b=1,a=1;}                                                             ///<全清为1

        void blendA(const LinearColor4f &v,float va){r=v.r;g=v.g;b=v.b;a=v.a*va;}                   ///<混合Alpha

        void set(float vr,float vg,float vb){r=vr;g=vg;b=vb;}                                       ///<设置RGB
        void set(float vr,float vg,float vb,float va){r=vr;g=vg;b=vb;a=va;}                         ///<设置RGBA
        void setFrom3f(const LinearColor3f &v,float va=1){r=v.r;g=v.g;b=v.b;a=va;}                  ///<从LinearColor3f设置

        void setLum(float v){r=g=b=v;}                                                              ///<设置亮度

        void lerp(float nr, float ng, float nb, float t);                                           ///<插值RGB到另一颜色(t:0=当前,1=目标)
        void lerp(const LinearColor4f &c, float t){lerp(c.r,c.g,c.b,t);}                            ///<插值RGB到另一颜色
        void lerp(const LinearColor3f &c, float t){lerp(c.r,c.g,c.b,t);}                            ///<插值RGB到另一颜色

        float lum() const {return RGB2Lum(r,g,b);}                                                  ///<线性亮度值

        Color4f toSRGB() const;                                                                     ///<转换为sRGB Color4f

        //操作符重载
        const LinearColor4f &operator = (const LinearColor4f &v){r=v.r;g=v.g;b=v.b;a=v.a;return *this;}
        const LinearColor4f &operator = (const Color4f &v);

        bool operator == (const LinearColor4f &)const;
        bool operator != (const LinearColor4f &)const;

        void operator += (const LinearColor4f &v){r+=v.r;g+=v.g;b+=v.b;a+=v.a;}
        void operator -= (const LinearColor4f &v){r-=v.r;g-=v.g;b-=v.b;a-=v.a;}
        void operator *= (const LinearColor4f &v){r*=v.r;g*=v.g;b*=v.b;a*=v.a;}
        void operator /= (const LinearColor4f &v){r/=v.r;g/=v.g;b/=v.b;a/=v.a;}

        void operator += (const LinearColor3f &v){r+=v.r;g+=v.g;b+=v.b;}
        void operator -= (const LinearColor3f &v){r-=v.r;g-=v.g;b-=v.b;}
        void operator *= (const LinearColor3f &v){r*=v.r;g*=v.g;b*=v.b;}
        void operator /= (const LinearColor3f &v){r/=v.r;g/=v.g;b/=v.b;}

        void operator *= (float v){r*=v;g*=v;b*=v;a*=v;}
        void operator /= (float v){r/=v;g/=v;b/=v;a/=v;}

        LinearColor4f operator + (const LinearColor4f &v) const {return(LinearColor4f(r+v.r,g+v.g,b+v.b,a+v.a));}
        LinearColor4f operator - (const LinearColor4f &v) const {return(LinearColor4f(r-v.r,g-v.g,b-v.b,a-v.a));}
        LinearColor4f operator * (const LinearColor4f &v) const {return(LinearColor4f(r*v.r,g*v.g,b*v.b,a*v.a));}
        LinearColor4f operator / (const LinearColor4f &v) const {return(LinearColor4f(r/v.r,g/v.g,b/v.b,a/v.a));}

        LinearColor4f operator + (const LinearColor3f &v) const {return(LinearColor4f(r+v.r,g+v.g,b+v.b,a));}
        LinearColor4f operator - (const LinearColor3f &v) const {return(LinearColor4f(r-v.r,g-v.g,b-v.b,a));}
        LinearColor4f operator * (const LinearColor3f &v) const {return(LinearColor4f(r*v.r,g*v.g,b*v.b,a));}
        LinearColor4f operator / (const LinearColor3f &v) const {return(LinearColor4f(r/v.r,g/v.g,b/v.b,a));}

        LinearColor4f operator * (float v) const {return(LinearColor4f(r*v,g*v,b*v,a*v));}
        LinearColor4f operator / (float v) const {return(LinearColor4f(r/v,g/v,b/v,a/v));}

        operator float *() {return((float *)this);}
        operator const float *() const {return((const float *)this);}
    };//class LinearColor4f

    extern const LinearColor4f LinearBlackColor4f;
    extern const LinearColor4f LinearWhiteColor4f;
    extern const LinearColor4f LinearRedColor4f;
    extern const LinearColor4f LinearGreenColor4f;
    extern const LinearColor4f LinearBlueColor4f;
    extern const LinearColor4f LinearYellowColor4f;

}//namespace hgl
