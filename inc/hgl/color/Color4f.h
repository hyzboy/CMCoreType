#pragma once

#include<hgl/color/Color3f.h>
#include<glm/glm.hpp>

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
    * r,g,b,a四原色类 - 继承自 glm::vec4
    *
    * 这个类通过继承GLM的vec4获得了trivially copyable特性，
    * 同时保留了对颜色操作的便利函数。
    *
    * @note 继承自glm::vec4后，自动具有trivially copyable性质，
    *       支持高效的内存复制和传输操作。
    */
    class Color4f : public glm::vec4                                                                ///RGBA四原色类
    {
    private:
        void clamp();                                                                               ///<限制颜色值到[0,1]范围

    public:

        // 使用编译器生成的默认特殊成员函数（实现 trivially copyable）
        Color4f() = default;                                                                        ///<默认构造：RGB为0，A为1
        Color4f(const Color4f&) = default;                                                          ///<默认复制构造
        Color4f& operator=(const Color4f&) = default;                                               ///<默认复制赋值
        ~Color4f() = default;                                                                       ///<默认析构

        // 显式构造函数 - 调用父类构造
        explicit Color4f(float v) : glm::vec4(v, v, v, 1.0f)                                       ///<单参数构造（灰度）
        {
            clamp();
        }

        Color4f(float vr, float vg, float vb, float va) : glm::vec4(vr, vg, vb, va)               ///<四参数构造
        {
            clamp();
        }

        Color4f(const Color3f &v, float va = 1.0f) : glm::vec4(v.x, v.y, v.z, va)                  ///<从Color3f构造
        {
            clamp();
        }

        // 从 glm::vec4 构造（用于与 GLM 运算结果互操作，如 glm::clamp/lerp 等）
        Color4f(const glm::vec4 &v) : glm::vec4(v)                                                  ///<从glm::vec4构造
        {
            clamp();
        }

        Color4f(const Color4ub &v);                                                                 ///<从Color4ub构造

        // 设置函数
        void setZero(){x=0;y=0;z=0;w=0;}                                                            ///<全清为0
        void setOne() {x=1;y=1;z=1;w=1;}                                                            ///<全清为1

        void blendA(const Color4f &v, float va){x=v.x;y=v.y;z=v.z;w=v.w*va;clamp();}                 ///<混合Alpha

        void set(const float *rgba){x=rgba[0];y=rgba[1];z=rgba[2];w=rgba[3];clamp();}
        void setFrom3f(const Color3f &v, float va = 1.0f){x=v.x;y=v.y;z=v.z;w=va;clamp();}          ///<设置颜色
        void set(float vr, float vg, float vb){x=vr;y=vg;z=vb;clamp();}                             ///<设置颜色（RGB）
        void set(float vr, float vg, float vb, float va){x=vr;y=vg;z=vb;w=va;clamp();}              ///<设置颜色（RGBA）
        void set255(int vr, int vg, int vb, int va)                                                 ///<按0-255的范围值设置颜色
            {x=float(vr)/255.0f;y=float(vg)/255.0f;z=float(vb)/255.0f;w=float(va)/255.0f;clamp();}

        void setLum(float v){if(v<0)setZero();else if(v>1)setOne();else{x=v;y=v;z=v;}}               ///<设置颜色

        // 插值函数
        void lerp(float nr, float ng, float nb, float t);                                         ///<插值到另一颜色(t:0=当前,1=目标)
        void lerp(const Color4f &c, float t){lerp(c.x,c.y,c.z,t);}                                  ///<插值到另一颜色
        void lerp(const Color3f &c, float t){lerp(c.x,c.y,c.z,t);}                                  ///<插值到另一颜色

        void lerpSmooth(const Color4f &c, float t);                                                 ///<余弦插值到另一颜色
        void lerpCubic(const Color4f &c, float t);                                                  ///<三次插值到另一颜色
        void lerpBezier(const Color4f &control, const Color4f &end, float t);                       ///<Bezier曲线插值到另一颜色

        // 格式转换
        uint32 toRGBA8()const{ return HGL_FLOAT_TO_RGBA8(x, y, z, w); }                             ///<输出一个rgba8格式的颜色数据
        uint32 toBGRA8()const{ return HGL_FLOAT_TO_BGRA8(x, y, z, w); }                             ///<输出一个bgra8格式的颜色数据
        uint32 toARGB8()const{ return HGL_FLOAT_TO_ARGB8(x, y, z, w); }                             ///<输出一个argb8格式的颜色数据
        uint32 toABGR8()const{ return HGL_FLOAT_TO_ABGR8(x, y, z, w); }                             ///<输出一个abgr8格式的颜色数据

        uint32 toShaderRGBA8()const{return toRGBA8();}                                              ///<输出一个shader rgba8格式的颜色数据

        float lum(){return RGB2Lum(x,y,z);}                                                         ///<灰度值
        void makeGrey();                                                                             ///<将当前色彩变成灰色

        Color4ub toColor4ub() const;                                                                ///<转换为Color4ub

        // 操作符重载
        const Color4f &operator = (const float *v){x=*v++;y=*v++;z=*v++;w=*v;return *this;}
        const Color4f &operator = (const Color3f &v){x=v.x;y=v.y;z=v.z;w=1.0f;return *this;}
        const Color4f &operator = (const Color4ub &v);

        bool operator == (const Color4f &)const;
        bool operator != (const Color4f &)const;

        void operator += (const Color4f &v){x+=v.x;y+=v.y;z+=v.z;w+=v.w;clamp();}
        void operator -= (const Color4f &v){x-=v.x;y-=v.y;z-=v.z;w-=v.w;clamp();}
        void operator *= (const Color4f &v){x*=v.x;y*=v.y;z*=v.z;w*=v.w;clamp();}
        void operator /= (const Color4f &v){x/=v.x;y/=v.y;z/=v.z;w/=v.w;clamp();}

        void operator += (const Color3f &v){x+=v.x;y+=v.y;z+=v.z;clamp();}
        void operator -= (const Color3f &v){x-=v.x;y-=v.y;z-=v.z;clamp();}
        void operator *= (const Color3f &v){x*=v.x;y*=v.y;z*=v.z;clamp();}
        void operator /= (const Color3f &v){x/=v.x;y/=v.y;z/=v.z;clamp();}

        void operator *= (float v){x*=v;y*=v;z*=v;w*=v;clamp();}
        void operator /= (float v){x/=v;y/=v;z/=v;w/=v;clamp();}

        Color4f operator + (const Color4f &v) const {return Color4f(x+v.x,y+v.y,z+v.z,w+v.w);}
        Color4f operator - (const Color4f &v) const {return Color4f(x-v.x,y-v.y,z-v.z,w-v.w);}
        Color4f operator * (const Color4f &v) const {return Color4f(x*v.x,y*v.y,z*v.z,w*v.w);}
        Color4f operator / (const Color4f &v) const {return Color4f(x/v.x,y/v.y,z/v.z,w/v.w);}

        Color4f operator * (float v) const {return Color4f(x*v,y*v,z*v,w*v);}
        Color4f operator / (float v) const {return Color4f(x/v,y/v,z/v,w/v);}

        operator float *() const {return(const_cast<float*>(&x));}                                 //使得本类可以直接当做float *使用
        operator const float *() const {return(&x);}                                                //使得本类可以直接当做const float *使用
    };//class Color4f

    // Non-member scalar-color operations to support expressions like "float + Color4f" and return Color4f
    inline hgl::Color4f operator + (float lhs, const hgl::Color4f &rhs)
    {
        return hgl::Color4f(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
    }

    inline hgl::Color4f operator - (float lhs, const hgl::Color4f &rhs)
    {
        return hgl::Color4f(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
    }

    inline hgl::Color4f operator * (float lhs, const hgl::Color4f &rhs)
    {
        return hgl::Color4f(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
    }

    inline hgl::Color4f operator / (float lhs, const hgl::Color4f &rhs)
    {
        float rx = (rhs.x != 0.0f) ? (lhs / rhs.x) : 1.0f;
        float ry = (rhs.y != 0.0f) ? (lhs / rhs.y) : 1.0f;
        float rz = (rhs.z != 0.0f) ? (lhs / rhs.z) : 1.0f;
        float rw = (rhs.w != 0.0f) ? (lhs / rhs.w) : 1.0f;
        return hgl::Color4f(rx, ry, rz, rw);
    }

    #define DEF_RGBA_U8_TO_COLOR4F(r,g,b,a)     Color4f(float(r)/255.0f,float(g)/255.0f,float(b)/255.0f,float(a)/255.0f)
}//namespace hgl
