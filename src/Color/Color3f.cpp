#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color.h>
#include<hgl/math/FloatPrecision.h>
#include<cstdlib>
#include<cmath>
#include<numbers>
namespace hgl
{
    constexpr float HGL_PI = std::numbers::pi_v<float>;

    //--------------------------------------------------------------------------------------------------
    Color3f::Color3f(const Color3ub &v)
    {
        r = float(v.r) / 255.0f;
        g = float(v.g) / 255.0f;
        b = float(v.b) / 255.0f;
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    void Color3f::clamp()
    {
        if(r<0)r=0;if(r>1)r=1;
        if(g<0)g=0;if(g>1)g=1;
        if(b<0)b=0;if(b>1)b=1;
    }
    //--------------------------------------------------------------------------------------------------
    void Color3f::rnd()
    {
        r = float(rand()) / RAND_MAX;
        g = float(rand()) / RAND_MAX;
        b = float(rand()) / RAND_MAX;
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * 求当前颜色过渡到另一颜色时某一比例时的颜色
    * @param nr,ng,nb 新的颜色
    * @param t 过渡比例,0时为当前的颜色,1时为nr,ng,nb
    */
    void Color3f::lerp(float nr,float ng,float nb,float t)
    {
        if(t<=0)return;
        if(t>=1)
        {
            r=nr;
            g=ng;
            b=nb;
            return;
        }

        r+=(nr-r)*t;
        g+=(ng-g)*t;
        b+=(nb-b)*t;
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * 余弦插值到另一颜色
    * @param c 目标颜色
    * @param t 过渡比例,0时为当前的颜色,1时为目标颜色
    */
    void Color3f::lerpSmooth(const Color3f &c, float t)
    {
        if(t<=0)return;
        if(t>=1)
        {
            r=c.r;
            g=c.g;
            b=c.b;
            return;
        }

        // Cosine interpolation: (1 - cos(t * π)) / 2
        float smooth_t = (1.0f - cosf(t * HGL_PI)) * 0.5f;
        
        r+=(c.r-r)*smooth_t;
        g+=(c.g-g)*smooth_t;
        b+=(c.b-b)*smooth_t;
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * 三次插值到另一颜色
    * @param c 目标颜色
    * @param t 过渡比例,0时为当前的颜色,1时为目标颜色
    */
    void Color3f::lerpCubic(const Color3f &c, float t)
    {
        if(t<=0)return;
        if(t>=1)
        {
            r=c.r;
            g=c.g;
            b=c.b;
            return;
        }

        // Cubic Hermite: 3t² - 2t³
        float cubic_t = t * t * (3.0f - 2.0f * t);
        
        r+=(c.r-r)*cubic_t;
        g+=(c.g-g)*cubic_t;
        b+=(c.b-b)*cubic_t;
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * Bezier曲线插值到另一颜色
    * @param control 控制点颜色
    * @param end 目标颜色
    * @param t 过渡比例,0时为当前的颜色,1时为目标颜色
    */
    void Color3f::lerpBezier(const Color3f &control, const Color3f &end, float t)
    {
        if(t<=0)return;
        if(t>=1)
        {
            r=end.r;
            g=end.g;
            b=end.b;
            return;
        }

        // Quadratic Bezier: (1-t)²·P0 + 2(1-t)t·P1 + t²·P2
        float one_minus_t = 1.0f - t;
        float weight_start = one_minus_t * one_minus_t;
        float weight_control = 2.0f * one_minus_t * t;
        float weight_end = t * t;
        
        r = r * weight_start + control.r * weight_control + end.r * weight_end;
        g = g * weight_start + control.g * weight_control + end.g * weight_end;
        b = b * weight_start + control.b * weight_control + end.b * weight_end;
    }
    //--------------------------------------------------------------------------------------------------
    void Color3f::makeGrey()
    {
        float lum=RGB2Lum(r,g,b);

        r=lum;
        g=lum;
        b=lum;
    }
    //--------------------------------------------------------------------------------------------------
    Color3ub Color3f::toColor3ub() const
    {
        return Color3ub(uint8(r * 255.0f), uint8(g * 255.0f), uint8(b * 255.0f));
    }

    //--------------------------------------------------------------------------------------------------
    const Color3f &Color3f::operator = (const Color3ub &v)
    {
        r = float(v.r) / 255.0f;
        g = float(v.g) / 255.0f;
        b = float(v.b) / 255.0f;
        clamp();
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool Color3f::operator == (const Color3f &v) const
    {
        if(!math::IsNearlyEqual(r,v.r))return(false);
        if(!math::IsNearlyEqual(g,v.g))return(false);
        if(!math::IsNearlyEqual(b,v.b))return(false);

        return(true);
    }
    //--------------------------------------------------------------------------------------------------
    bool Color3f::operator != (const Color3f &v) const
    {
        if(!math::IsNearlyEqual(r,v.r))return(true);
        if(!math::IsNearlyEqual(g,v.g))return(true);
        if(!math::IsNearlyEqual(b,v.b))return(true);

        return(false);
    }
    //--------------------------------------------------------------------------------------------------
}

namespace hgl
{
    const Color3f BlackColor3f(0,0,0);
    const Color3f WhiteColor3f(1,1,1);
    const Color3f RedColor3f(1,0,0);
    const Color3f GreenColor3f(0,1,0);
    const Color3f BlueColor3f(0,0,1);
    const Color3f YellowColor3f(1,1,0);
}
