#pragma once

#include<hgl/color/Lum.h>
#include<glm/glm.hpp>

namespace hgl
{
    class LinearColor3f;
    class Color3f;

    /**
    * 线性颜色空间RGB三原色(浮点,无限制范围用于HDR) - 继承自 glm::vec3
    * 用于线性颜色空间计算,支持超过1.0的HDR值
    * 
    * @note 继承自glm::vec3后，自动具有trivially copyable性质
    */
    class LinearColor3f : public glm::vec3
    {
    private:
        void clamp();                                                                               ///<用于防止极端值

    public:

        // 使用编译器生成的默认特殊成员函数（实现 trivially copyable）
        LinearColor3f() = default;                                                                  ///<默认构造：全部为0
        LinearColor3f(const LinearColor3f&) = default;                                              ///<默认复制构造
        LinearColor3f& operator=(const LinearColor3f&) = default;                                   ///<默认复制赋值
        ~LinearColor3f() = default;                                                                 ///<默认析构

        // 显式构造函数 - 调用父类构造
        explicit LinearColor3f(float l) : glm::vec3(l)                                              ///<单参数构造（灰度）
        {
        }

        LinearColor3f(float vr, float vg, float vb) : glm::vec3(vr, vg, vb)                       ///<三参数构造
        {
        }

        LinearColor3f(const Color3f &v);                                                            ///<从sRGB Color3f构造

        // 设置函数
        void setZero(){x=0;y=0;z=0;}                                                               ///<全清为0
        void setOne(){x=1;y=1;z=1;}                                                                ///<全清为1

        void set(float vr, float vg, float vb){x=vr;y=vg;z=vb;}                                     ///<设置颜色
        void setLum(float v){x=v;y=v;z=v;}                                                          ///<设置亮度

        // 插值函数
        void lerp(float nr, float ng, float nb, float t);                                          ///<插值到另一颜色(t:0=当前,1=目标)
        void lerp(const LinearColor3f &c, float t){lerp(c.x,c.y,c.z,t);}                            ///<插值到另一颜色

        // 颜色操作
        void setBlack(){x=0;y=0;z=0;}                                                               ///<黑色
        void setWhite(){x=1;y=1;z=1;}                                                               ///<白色

        float lum() const {return RGB2Lum(x,y,z);}                                                  ///<线性亮度值

        Color3f toSRGB() const;                                                                     ///<转换为sRGB Color3f

        // 操作符重载
        const LinearColor3f &operator = (const Color3f &v);

        bool operator == (const LinearColor3f &)const;
        bool operator != (const LinearColor3f &)const;

        void operator += (const LinearColor3f &v){x+=v.x;y+=v.y;z+=v.z;}
        void operator -= (const LinearColor3f &v){x-=v.x;y-=v.y;z-=v.z;}
        void operator *= (const LinearColor3f &v){x*=v.x;y*=v.y;z*=v.z;}
        void operator /= (const LinearColor3f &v){x/=v.x;y/=v.y;z/=v.z;}

        void operator *= (float v){x*=v;y*=v;z*=v;}
        void operator /= (float v){x/=v;y/=v;z/=v;}

        LinearColor3f operator + (const LinearColor3f &v) const {return LinearColor3f(x+v.x,y+v.y,z+v.z);}
        LinearColor3f operator - (const LinearColor3f &v) const {return LinearColor3f(x-v.x,y-v.y,z-v.z);}
        LinearColor3f operator * (const LinearColor3f &v) const {return LinearColor3f(x*v.x,y*v.y,z*v.z);}
        LinearColor3f operator / (const LinearColor3f &v) const {return LinearColor3f(x/v.x,y/v.y,z/v.z);}

        LinearColor3f operator * (float v) const {return LinearColor3f(x*v,y*v,z*v);}
        LinearColor3f operator / (float v) const {return LinearColor3f(x/v,y/v,z/v);}

        operator float *() {return(&x);}
        operator const float *() const {return(&x);}
    };//class LinearColor3f

    extern const LinearColor3f LinearBlackColor3f;
    extern const LinearColor3f LinearWhiteColor3f;
    extern const LinearColor3f LinearRedColor3f;
    extern const LinearColor3f LinearGreenColor3f;
    extern const LinearColor3f LinearBlueColor3f;
    extern const LinearColor3f LinearYellowColor3f;

    // Non-member scalar-color operations
    inline LinearColor3f operator + (float lhs, const LinearColor3f &rhs)
    {
        return LinearColor3f(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
    }

    inline LinearColor3f operator - (float lhs, const LinearColor3f &rhs)
    {
        return LinearColor3f(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
    }

    inline LinearColor3f operator * (float lhs, const LinearColor3f &rhs)
    {
        return LinearColor3f(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
    }

    inline LinearColor3f operator / (float lhs, const LinearColor3f &rhs)
    {
        float rx = (rhs.x != 0.0f) ? (lhs / rhs.x) : 1.0f;
        float ry = (rhs.y != 0.0f) ? (lhs / rhs.y) : 1.0f;
        float rz = (rhs.z != 0.0f) ? (lhs / rhs.z) : 1.0f;
        return LinearColor3f(rx, ry, rz);
    }
}//namespace hgl
