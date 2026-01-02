#pragma once

#include<hgl/color/Lum.h>
namespace hgl
{
    class Color3ub;  // Forward declaration

    /**
    * r,g,b三原色类
    */
    class Color3f                                                                                   ///RGB三原色类
    {
        void clamp();

    public:

        union
        {
            struct
            {
                float r,g,b;                                                                                ///<rgb 三原色
            };

            float rgb[3];
        };

    public:

        Color3f(){r=0,g=0,b=0;}                                                                     ///<本类构造函数
        Color3f(float l){r=l,g=l,b=l;clamp();}                                                      ///<本类构造函数
        Color3f(float vr,float vg,float vb){r=vr,g=vg,b=vb;clamp();}                                ///<本类构造函数
        Color3f(const Color3f &v){r=v.r;g=v.g;b=v.b;}                                           ///<本类构造函数
        Color3f(const Color3ub &v);                                                                 ///<从Color3ub构造

        void setZero(){r=0,g=0,b=0;}                                                               ///<全清为0
        void setOne() {r=1,g=1,b=1;}                                                               ///<全清为1

        void set(float vr,float vg,float vb){r=vr,g=vg,b=vb;clamp();}                               ///<设置颜色
        void set255(int vr,int vg,int vb)                                                            ///<按0-255的范围值设置颜色
        {r=float(vr)/255.0f,g=float(vg)/255.0f,b=float(vb)/255.0f;clamp();}

        void setLum(float v){if(v<0)setZero();else if(v>1)setOne();else{r=g=b=v;}}                  ///<设置颜色

        void lerp(float nr, float ng, float nb, float t);                                         ///<插值到另一颜色(t:0=当前,1=目标)
        void lerp(const float *rgb, float t){lerp(*rgb,*(rgb+1),*(rgb+2),t);}                        ///<插值到另一颜色
        void lerp(const Color3f &c, float t){lerp(c.r,c.g,c.b,t);}                                  ///<插值到另一颜色

        void lerpSmooth(const Color3f &c, float t);                                                 ///<余弦插值到另一颜色
        void lerpCubic(const Color3f &c, float t);                                                  ///<三次插值到另一颜色
        void lerpBezier(const Color3f &control, const Color3f &end, float t);                       ///<Bezier曲线插值到另一颜色

        void setBlack(){r=0,g=0,b=0;}                                                               ///<黑色
        void setWhite(){r=1,g=1,b=1;}                                                               ///<白色

        float lum(){return RGB2Lum(r,g,b);}                                                        ///<灰度值
        void makeGrey();                                                                             ///<将当前色彩变成灰色

        Color3ub toColor3ub() const;                                                                ///<转换为Color3ub

        //操作符重载
        const Color3f &operator = (const float *v){r=*v++;g=*v++;b=*v;return *this;}
        const Color3f &operator = (const Color3f &v){r=v.r;g=v.g;b=v.b;return *this;}
        const Color3f &operator = (const Color3ub &v);

        bool operator == (const Color3f &)const;
        bool operator != (const Color3f &)const;

        void operator += (const Color3f &v){r+=v.r;g+=v.g;b+=v.b;clamp();}
        void operator -= (const Color3f &v){r-=v.r;g-=v.g;b-=v.b;clamp();}
        void operator *= (const Color3f &v){r*=v.r;g*=v.g;b*=v.b;clamp();}
        void operator /= (const Color3f &v){r/=v.r;g/=v.g;b/=v.b;clamp();}

        void operator *= (float v){r*=v;g*=v;b*=v;clamp();}
        void operator /= (float v){r/=v;g/=v;b/=v;clamp();}

        Color3f operator + (const Color3f &v){return(Color3f(r+v.r,g+v.g,b+v.b));}
        Color3f operator - (const Color3f &v){return(Color3f(r-v.r,g-v.g,b-v.b));}
        Color3f operator * (const Color3f &v){return(Color3f(r*v.r,g*v.g,b*v.b));}
        Color3f operator / (const Color3f &v){return(Color3f(r/v.r,g/v.g,b/v.b));}

        Color3f operator * (float v){return(Color3f(r*v,g*v,b*v));}
        Color3f operator / (float v){return(Color3f(r/v,g/v,b/v));}

        operator float *() const {return((float *)this);}                                           //使得本类可以直接当做float *使用
        operator const float *() const {return((const float *)this);}                               //使得本类可以直接当做const float *使用
    };//class Color3f

    extern const Color3f BlackColor3f;
    extern const Color3f WhiteColor3f;
    extern const Color3f RedColor3f;
    extern const Color3f GreenColor3f;
    extern const Color3f BlueColor3f;
    extern const Color3f YellowColor3f;

    #define DEF_RGB_U8_TO_COLOR3F(r,g,b)        Color3f(float(r)/255.0f,float(g)/255.0f,float(b)/255.0f)
}//namespace hgl
