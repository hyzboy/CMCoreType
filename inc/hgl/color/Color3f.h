#pragma once

#include<hgl/color/Lum.h>
#include<glm/glm.hpp>

namespace hgl
{
    class Color3ub;  // Forward declaration

    /**
    * r,g,b三原色类 - 继承自 glm::vec3
    *
    * 这个类通过继承GLM的vec3获得了trivially copyable特性，
    * 同时保留了对颜色操作的便利函数。
    *
    * @note 继承自glm::vec3后，自动具有trivially copyable性质，
    *       支持高效的内存复制和传输操作。
    */
    class Color3f : public glm::vec3                                                                ///RGB三原色类
    {
    private:
        void clamp();                                                                               ///<限制颜色值到[0,1]范围

    public:

        // 使用编译器生成的默认特殊成员函数（实现 trivially copyable）
        Color3f() : glm::vec3(0, 0, 0) {}                                                          ///<默认构造：全部为0
        Color3f(const Color3f&) = default;                                                          ///<默认复制构造
        Color3f& operator=(const Color3f&) = default;                                               ///<默认复制赋值
        ~Color3f() = default;                                                                       ///<默认析构

        // 显式构造函数 - 调用父类构造
        explicit Color3f(float l) : glm::vec3(l)                                                    ///<单参数构造（灰度）
        {
            clamp();
        }

        Color3f(float vr, float vg, float vb) : glm::vec3(vr, vg, vb)                             ///<三参数构造
        {
            clamp();
        }

        // 从 glm::vec3 构造（用于与 GLM 运算结果互操作，如 glm::clamp/lerp 等）
        explicit Color3f(const glm::vec3 &v) : glm::vec3(v)                                        ///<从glm::vec3构造
        {
            clamp();
        }

        Color3f(const Color3ub &v);                                                                 ///<从Color3ub构造

        // 设置函数
        void setZero(){x=0;y=0;z=0;}                                                               ///<全清为0
        void setOne() {x=1;y=1;z=1;}                                                               ///<全清为1

        void set(float vr,float vg,float vb){x=vr;y=vg;z=vb;clamp();}                               ///<设置颜色
        void set255(int vr,int vg,int vb)                                                            ///<按0-255的范围值设置颜色
        {x=float(vr)/255.0f;y=float(vg)/255.0f;z=float(vb)/255.0f;clamp();}

        void setLum(float v){if(v<0)setZero();else if(v>1)setOne();else{x=y=z=v;}}                  ///<设置颜色

        // 插值函数
        void lerp(float nr, float ng, float nb, float t);                                         ///<插值到另一颜色(t:0=当前,1=目标)
        void lerp(const float *rgb, float t){lerp(*rgb,*(rgb+1),*(rgb+2),t);}                        ///<插值到另一颜色
        void lerp(const Color3f &c, float t){lerp(c.r,c.g,c.b,t);}                                  ///<插值到另一颜色

        void lerpSmooth(const Color3f &c, float t);                                                 ///<余弦插值到另一颜色
        void lerpCubic(const Color3f &c, float t);                                                  ///<三次插值到另一颜色
        void lerpBezier(const Color3f &control, const Color3f &end, float t);                       ///<Bezier曲线插值到另一颜色

        // 颜色操作
        void setBlack(){x=0;y=0;z=0;}                                                               ///<黑色
        void setWhite(){x=1;y=1;z=1;}                                                               ///<白色

        float lum(){return RGB2Lum(x,y,z);}                                                        ///<灰度值
        void makeGrey();                                                                             ///<将当前色彩变成灰色

        Color3ub toColor3ub() const;                                                                ///<转换为Color3ub

        // 操作符重载
        const Color3f &operator = (const float *v){x=*v++;y=*v++;z=*v;return *this;}
        const Color3f &operator = (const Color3ub &v);

        bool operator == (const Color3f &)const;
        bool operator != (const Color3f &)const;

        void operator += (const Color3f &v){x+=v.x;y+=v.y;z+=v.z;clamp();}
        void operator -= (const Color3f &v){x-=v.x;y-=v.y;z-=v.z;clamp();}
        void operator *= (const Color3f &v){x*=v.x;y*=v.y;z*=v.z;clamp();}
        void operator /= (const Color3f &v){x/=v.x;y/=v.y;z/=v.z;clamp();}

        void operator *= (float v){x*=v;y*=v;z*=v;clamp();}
        void operator /= (float v){x/=v;y/=v;z/=v;clamp();}

        Color3f operator + (const Color3f &v) const {return Color3f(x+v.x,y+v.y,z+v.z);}
        Color3f operator - (const Color3f &v) const {return Color3f(x-v.x,y-v.y,z-v.z);}
        Color3f operator * (const Color3f &v) const {return Color3f(x*v.x,y*v.y,z*v.z);}
        Color3f operator / (const Color3f &v) const {return Color3f(x/v.x,y/v.y,z/v.z);}

        Color3f operator * (float v) const {return Color3f(x*v,y*v,z*v);}
        Color3f operator / (float v) const {return Color3f(x/v,y/v,z/v);}

        operator float *() const {return(const_cast<float*>(&x));}                                 //使得本类可以直接当做float *使用
        operator const float *() const {return(&x);}                                                //使得本类可以直接当做const float *使用
    };//class Color3f

    extern const Color3f BlackColor3f;
    extern const Color3f WhiteColor3f;
    extern const Color3f RedColor3f;
    extern const Color3f GreenColor3f;
    extern const Color3f BlueColor3f;
    extern const Color3f YellowColor3f;

    #define DEF_RGB_U8_TO_COLOR3F(r,g,b)        Color3f(float(r)/255.0f,float(g)/255.0f,float(b)/255.0f)

    // Non-member scalar-color operations to support expressions like "float + Color3f" and return Color3f
    inline Color3f operator + (float lhs, const Color3f &rhs)
    {
        return Color3f(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
    }

    inline Color3f operator - (float lhs, const Color3f &rhs)
    {
        return Color3f(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
    }

    inline Color3f operator * (float lhs, const Color3f &rhs)
    {
        return Color3f(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
    }

    inline Color3f operator / (float lhs, const Color3f &rhs)
    {
        float rx = (rhs.x != 0.0f) ? (lhs / rhs.x) : 1.0f;
        float ry = (rhs.y != 0.0f) ? (lhs / rhs.y) : 1.0f;
        float rz = (rhs.z != 0.0f) ? (lhs / rhs.z) : 1.0f;
        return Color3f(rx, ry, rz);
    }

}//namespace hgl
