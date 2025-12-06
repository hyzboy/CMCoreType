#pragma once

#include<hgl/color/Lum.h>
namespace hgl
{
    class LinearColor3f;
    class Color3f;

    /**
    * 线性颜色空间RGB三原色(浮点,无限制范围用于HDR)
    * 用于线性颜色空间计算,支持超过1.0的HDR值
    */
    class LinearColor3f
    {
        void clamp();  //用于防止极端值,但不强制限制在0-1范围

    public:

        union
        {
            struct
            {
                float r,g,b;                                                                        ///<linear rgb 三原色 (无范围限制)
            };

            float rgb[3];
        };

    public:

        LinearColor3f(){r=0,g=0,b=0;}                                                               ///<本类构造函数
        LinearColor3f(float l){r=l,g=l,b=l;}                                                        ///<本类构造函数(灰度)
        LinearColor3f(float vr,float vg,float vb){r=vr,g=vg,b=vb;}                                  ///<本类构造函数
        LinearColor3f(const LinearColor3f &v){r=v.r;g=v.g;b=v.b;}                                   ///<本类构造函数
        LinearColor3f(const Color3f &v);                                                            ///<从sRGB Color3f构造

        void setZero(){r=0,g=0,b=0;}                                                                ///<全清为0
        void setOne(){r=1,g=1,b=1;}                                                                 ///<全清为1

        void set(float vr,float vg,float vb){r=vr;g=vg;b=vb;}                                       ///<设置颜色
        void setLum(float v){r=g=b=v;}                                                              ///<设置亮度

        void lerp(float nr, float ng, float nb, float t);                                           ///<插值到另一颜色(t:0=当前,1=目标)
        void lerp(const LinearColor3f &c, float t){lerp(c.r,c.g,c.b,t);}                            ///<插值到另一颜色

        void setBlack(){r=0,g=0,b=0;}                                                               ///<黑色
        void setWhite(){r=1,g=1,b=1;}                                                               ///<白色

        float lum() const {return RGB2Lum(r,g,b);}                                                  ///<线性亮度值

        Color3f toSRGB() const;                                                                     ///<转换为sRGB Color3f

        //操作符重载
        const LinearColor3f &operator = (const LinearColor3f &v){r=v.r;g=v.g;b=v.b;return *this;}
        const LinearColor3f &operator = (const Color3f &v);

        bool operator == (const LinearColor3f &)const;
        bool operator != (const LinearColor3f &)const;

        void operator += (const LinearColor3f &v){r+=v.r;g+=v.g;b+=v.b;}
        void operator -= (const LinearColor3f &v){r-=v.r;g-=v.g;b-=v.b;}
        void operator *= (const LinearColor3f &v){r*=v.r;g*=v.g;b*=v.b;}
        void operator /= (const LinearColor3f &v){r/=v.r;g/=v.g;b/=v.b;}

        void operator *= (float v){r*=v;g*=v;b*=v;}
        void operator /= (float v){r/=v;g/=v;b/=v;}

        LinearColor3f operator + (const LinearColor3f &v) const {return(LinearColor3f(r+v.r,g+v.g,b+v.b));}
        LinearColor3f operator - (const LinearColor3f &v) const {return(LinearColor3f(r-v.r,g-v.g,b-v.b));}
        LinearColor3f operator * (const LinearColor3f &v) const {return(LinearColor3f(r*v.r,g*v.g,b*v.b));}
        LinearColor3f operator / (const LinearColor3f &v) const {return(LinearColor3f(r/v.r,g/v.g,b/v.b));}

        LinearColor3f operator * (float v) const {return(LinearColor3f(r*v,g*v,b*v));}
        LinearColor3f operator / (float v) const {return(LinearColor3f(r/v,g/v,b/v));}

        operator float *() {return((float *)this);}
        operator const float *() const {return((const float *)this);}
    };//class LinearColor3f

    extern const LinearColor3f LinearBlackColor3f;
    extern const LinearColor3f LinearWhiteColor3f;
    extern const LinearColor3f LinearRedColor3f;
    extern const LinearColor3f LinearGreenColor3f;
    extern const LinearColor3f LinearBlueColor3f;
    extern const LinearColor3f LinearYellowColor3f;

}//namespace hgl
