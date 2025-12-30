#pragma once

#include<hgl/platform/Platform.h>
namespace hgl
{
    class Color3f;  // Forward declaration

    /**
    * r,g,b三原色类(无符号8位整型)
    */
    class Color3ub                                                                                  ///RGB三原色类(0-255范围)
    {
        void clamp();

    public:

        union
        {
            struct
            {
                uint8 r,g,b;                                                                        ///<rgb 三原色 (0-255)
            };

            uint8 rgb[3];
        };

    public:

        Color3ub(){r=0,g=0,b=0;}                                                                    ///<本类构造函数
        Color3ub(uint8 l){r=l,g=l,b=l;}                                                           ///<本类构造函数
        Color3ub(uint8 vr,uint8 vg,uint8 vb){r=vr,g=vg,b=vb;clamp();}                             ///<本类构造函数
        Color3ub(const Color3ub &v){r=v.r;g=v.g;b=v.b;}                                         ///<本类构造函数
        Color3ub(const Color3f &v);                                                                ///<从Color3f构造

        void setZero(){r=0,g=0,b=0;}                                                               ///<全清为0
        void setOne() {r=255,g=255,b=255;}                                                         ///<全清为255
        void rnd();                                                                                ///<全随机

        void set(uint8 vr,uint8 vg,uint8 vb){r=vr,g=vg,b=vb;clamp();}                              ///<设置颜色
        void setf(float vr,float vg,float vb);                                                     ///<按0.0-1.0的范围值设置颜色

        void setLum(uint8 v){r=g=b=v;}                                                        ///<设置颜色

        void lerp(uint8 nr, uint8 ng, uint8 nb, float t);                                         ///<插值到另一颜色(t:0=当前,1=目标)
        void lerp(const uint8 *rgb, float t){lerp(*rgb,*(rgb+1),*(rgb+2),t);}                        ///<插值到另一颜色
        void lerp(const Color3ub &c, float t){lerp(c.r,c.g,c.b,t);}                                  ///<插值到另一颜色

        void lerpSmooth(const Color3ub &c, float t);                                                ///<余弦插值到另一颜色
        void lerpCubic(const Color3ub &c, float t);                                                 ///<三次插值到另一颜色
        void lerpBezier(const Color3ub &control, const Color3ub &end, float t);                     ///<Bezier曲线插值到另一颜色

        void setBlack(){r=0,g=0,b=0;}                                                               ///<黑色
        void setWhite(){r=255,g=255,b=255;}                                                         ///<白色

        uint8 toGray() const;                                                                       ///<转换为灰度值
        void makeGray();                                                                             ///<将当前色彩变成灰色

        Color3f toColor3f() const;                                                                  ///<转换为Color3f

        //操作符重载
        const Color3ub &operator = (const uint8 *v){r=*v++;g=*v++;b=*v;return *this;}
        const Color3ub &operator = (const Color3ub &v){r=v.r;g=v.g;b=v.b;return *this;}
        const Color3ub &operator = (const Color3f &v);

        bool operator == (const Color3ub &)const;
        bool operator != (const Color3ub &)const;

        void operator += (const Color3ub &v){r+=v.r;g+=v.g;b+=v.b;clamp();}
        void operator -= (const Color3ub &v){r-=v.r;g-=v.g;b-=v.b;clamp();}
        void operator *= (const Color3ub &v){r*=v.r;g*=v.g;b*=v.b;clamp();}
        void operator /= (const Color3ub &v){r/=v.r;g/=v.g;b/=v.b;clamp();}

        void operator *= (float v){r=uint8(r*v);g=uint8(g*v);b=uint8(b*v);clamp();}
        void operator /= (float v){r=uint8(r/v);g=uint8(g/v);b=uint8(b/v);clamp();}

        Color3ub operator + (const Color3ub &v){return(Color3ub(r+v.r,g+v.g,b+v.b));}
        Color3ub operator - (const Color3ub &v){return(Color3ub(r-v.r,g-v.g,b-v.b));}
        Color3ub operator * (const Color3ub &v){return(Color3ub(r*v.r,g*v.g,b*v.b));}
        Color3ub operator / (const Color3ub &v){return(Color3ub(r/v.r,g/v.g,b/v.b));}

        Color3ub operator * (float v){return(Color3ub(uint8(r*v),uint8(g*v),uint8(b*v)));}
        Color3ub operator / (float v){return(Color3ub(uint8(r/v),uint8(g/v),uint8(b/v)));}

        operator uint8 *() const {return((uint8 *)this);}                                          //使得本类可以直接当做uint8 *使用
        operator const uint8 *() const {return((const uint8 *)this);}                              //使得本类可以直接当做const uint8 *使用
    };//class Color3ub

    extern const Color3ub BlackColor3ub;
    extern const Color3ub WhiteColor3ub;
    extern const Color3ub RedColor3ub;
    extern const Color3ub GreenColor3ub;
    extern const Color3ub BlueColor3ub;
    extern const Color3ub YellowColor3ub;

    #define DEF_RGB_U8_TO_COLOR3UB(r,g,b)        Color3ub(r,g,b)
}//namespace hgl
