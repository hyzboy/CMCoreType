#pragma once

#include<hgl/color/LinearColor3f.h>
#include<glm/glm.hpp>

namespace hgl
{
    class LinearColor4f;
    class Color4f;

    /**
    * 线性颜色空间RGBA四原色(浮点,无限制范围用于HDR) - 继承自 glm::vec4
    * 用于线性颜色空间计算,支持超过1.0的HDR值
    * Alpha通道不参与线性化处理,保持0-1范围
    * 
    * @note 继承自glm::vec4后，自动具有trivially copyable性质
    */
    class LinearColor4f : public glm::vec4
    {
    private:
        void clamp();                                                                               ///<用于防止极端值

    public:

        // 使用编译器生成的默认特殊成员函数（实现 trivially copyable）
        LinearColor4f() = default;                                                                  ///<默认构造：RGB为0，A为1
        LinearColor4f(const LinearColor4f&) = default;                                              ///<默认复制构造
        LinearColor4f& operator=(const LinearColor4f&) = default;                                   ///<默认复制赋值
        ~LinearColor4f() = default;                                                                 ///<默认析构

        // 显式构造函数 - 调用父类构造
        explicit LinearColor4f(float v) : glm::vec4(v, v, v, 1.0f)                                 ///<单参数构造（灰度）
        {
        }

        LinearColor4f(float vr, float vg, float vb, float va) : glm::vec4(vr, vg, vb, va)         ///<四参数构造
        {
        }

        LinearColor4f(const LinearColor3f &v, float va = 1.0f) : glm::vec4(v.x, v.y, v.z, va)      ///<从LinearColor3f构造
        {
        }

        LinearColor4f(const Color4f &v);                                                            ///<从sRGB Color4f构造

        // 设置函数
        void setZero(){x=0;y=0;z=0;w=0;}                                                            ///<全清为0
        void setOne(){x=1;y=1;z=1;w=1;}                                                             ///<全清为1

        void blendA(const LinearColor4f &v, float va){x=v.x;y=v.y;z=v.z;w=v.w*va;}                  ///<混合Alpha

        void set(float vr, float vg, float vb){x=vr;y=vg;z=vb;}                                     ///<设置RGB
        void set(float vr, float vg, float vb, float va){x=vr;y=vg;z=vb;w=va;}                      ///<设置RGBA
        void setFrom3f(const LinearColor3f &v, float va = 1.0f){x=v.x;y=v.y;z=v.z;w=va;}            ///<从LinearColor3f设置

        void setLum(float v){x=v;y=v;z=v;}                                                          ///<设置亮度

        // 插值函数
        void lerp(float nr, float ng, float nb, float t);                                          ///<插值RGB到另一颜色(t:0=当前,1=目标)
        void lerp(const LinearColor4f &c, float t){lerp(c.x,c.y,c.z,t);}                            ///<插值RGB到另一颜色
        void lerp(const LinearColor3f &c, float t){lerp(c.x,c.y,c.z,t);}                            ///<插值RGB到另一颜色

        float lum() const {return RGB2Lum(x,y,z);}                                                  ///<线性亮度值

        Color4f toSRGB() const;                                                                     ///<转换为sRGB Color4f

        // 操作符重载
        const LinearColor4f &operator = (const Color4f &v);

        bool operator == (const LinearColor4f &)const;
        bool operator != (const LinearColor4f &)const;

        void operator += (const LinearColor4f &v){x+=v.x;y+=v.y;z+=v.z;w+=v.w;}
        void operator -= (const LinearColor4f &v){x-=v.x;y-=v.y;z-=v.z;w-=v.w;}
        void operator *= (const LinearColor4f &v){x*=v.x;y*=v.y;z*=v.z;w*=v.w;}
        void operator /= (const LinearColor4f &v){x/=v.x;y/=v.y;z/=v.z;w/=v.w;}

        void operator += (const LinearColor3f &v){x+=v.x;y+=v.y;z+=v.z;}
        void operator -= (const LinearColor3f &v){x-=v.x;y-=v.y;z-=v.z;}
        void operator *= (const LinearColor3f &v){x*=v.x;y*=v.y;z*=v.z;}
        void operator /= (const LinearColor3f &v){x/=v.x;y/=v.y;z/=v.z;}

        void operator *= (float v){x*=v;y*=v;z*=v;w*=v;}
        void operator /= (float v){x/=v;y/=v;z/=v;w/=v;}

        LinearColor4f operator + (const LinearColor4f &v) const {return LinearColor4f(x+v.x,y+v.y,z+v.z,w+v.w);}
        LinearColor4f operator - (const LinearColor4f &v) const {return LinearColor4f(x-v.x,y-v.y,z-v.z,w-v.w);}
        LinearColor4f operator * (const LinearColor4f &v) const {return LinearColor4f(x*v.x,y*v.y,z*v.z,w*v.w);}
        LinearColor4f operator / (const LinearColor4f &v) const {return LinearColor4f(x/v.x,y/v.y,z/v.z,w/v.w);}

        LinearColor4f operator + (const LinearColor3f &v) const {return LinearColor4f(x+v.x,y+v.y,z+v.z,w);}
        LinearColor4f operator - (const LinearColor3f &v) const {return LinearColor4f(x-v.x,y-v.y,z-v.z,w);}
        LinearColor4f operator * (const LinearColor3f &v) const {return LinearColor4f(x*v.x,y*v.y,z*v.z,w);}
        LinearColor4f operator / (const LinearColor3f &v) const {return LinearColor4f(x/v.x,y/v.y,z/v.z,w);}

        LinearColor4f operator * (float v) const {return LinearColor4f(x*v,y*v,z*v,w*v);}
        LinearColor4f operator / (float v) const {return LinearColor4f(x/v,y/v,z/v,w/v);}

        operator float *() {return(&x);}
        operator const float *() const {return(&x);}
    };//class LinearColor4f

    extern const LinearColor4f LinearBlackColor4f;
    extern const LinearColor4f LinearWhiteColor4f;
    extern const LinearColor4f LinearRedColor4f;
    extern const LinearColor4f LinearGreenColor4f;
    extern const LinearColor4f LinearBlueColor4f;
    extern const LinearColor4f LinearYellowColor4f;

    // Non-member scalar-color operations
    inline LinearColor4f operator + (float lhs, const LinearColor4f &rhs)
    {
        return LinearColor4f(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
    }

    inline LinearColor4f operator - (float lhs, const LinearColor4f &rhs)
    {
        return LinearColor4f(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
    }

    inline LinearColor4f operator * (float lhs, const LinearColor4f &rhs)
    {
        return LinearColor4f(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
    }

    inline LinearColor4f operator / (float lhs, const LinearColor4f &rhs)
    {
        float rx = (rhs.x != 0.0f) ? (lhs / rhs.x) : 1.0f;
        float ry = (rhs.y != 0.0f) ? (lhs / rhs.y) : 1.0f;
        float rz = (rhs.z != 0.0f) ? (lhs / rhs.z) : 1.0f;
        float rw = (rhs.w != 0.0f) ? (lhs / rhs.w) : 1.0f;
        return LinearColor4f(rx, ry, rz, rw);
    }
}//namespace hgl
