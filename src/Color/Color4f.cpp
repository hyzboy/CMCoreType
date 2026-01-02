#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<hgl/color/Color.h>
#include<hgl/math/FloatPrecision.h>
#include<hgl/math/MathConstants.h>
#include<cmath>

namespace hgl
{
    Color4f::Color4f(const Color4ub &v)
    {
        r = float(v.r) / 255.0f;
        g = float(v.g) / 255.0f;
        b = float(v.b) / 255.0f;
        a = float(v.a) / 255.0f;
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    void Color4f::clamp()
    {
        if(r<0)r=0;if(r>1)r=1;
        if(g<0)g=0;if(g>1)g=1;
        if(b<0)b=0;if(b>1)b=1;
        if(a<0)a=0;if(a>1)a=1;
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * 求当前颜色过渡到另一颜色时某一比例时的颜色
    * @param nr,ng,nb 新的颜色
    * @param t 过渡比例,0时为当前的颜色,1时为nr,ng,nb
    */
    void Color4f::lerp(float nr,float ng,float nb,float t)
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
    void Color4f::lerpSmooth(const Color4f &c, float t)
    {
        if(t<=0)return;
        if(t>=1)
        {
            r=c.r;
            g=c.g;
            b=c.b;
            a=c.a;
            return;
        }

        // Cosine interpolation: (1 - cos(t * π)) / 2
        float smooth_t = (1.0f - cosf(t * float(hgl::math::pi))) * 0.5f;
        
        r+=(c.r-r)*smooth_t;
        g+=(c.g-g)*smooth_t;
        b+=(c.b-b)*smooth_t;
        a+=(c.a-a)*smooth_t;
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * 三次插值到另一颜色
    * @param c 目标颜色
    * @param t 过渡比例,0时为当前的颜色,1时为目标颜色
    */
    void Color4f::lerpCubic(const Color4f &c, float t)
    {
        if(t<=0)return;
        if(t>=1)
        {
            r=c.r;
            g=c.g;
            b=c.b;
            a=c.a;
            return;
        }

        // Cubic Hermite: 3t² - 2t³
        float cubic_t = t * t * (3.0f - 2.0f * t);
        
        r+=(c.r-r)*cubic_t;
        g+=(c.g-g)*cubic_t;
        b+=(c.b-b)*cubic_t;
        a+=(c.a-a)*cubic_t;
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * Bezier曲线插值到另一颜色
    * @param control 控制点颜色
    * @param end 目标颜色
    * @param t 过渡比例,0时为当前的颜色,1时为目标颜色
    */
    void Color4f::lerpBezier(const Color4f &control, const Color4f &end, float t)
    {
        if(t<=0)return;
        if(t>=1)
        {
            r=end.r;
            g=end.g;
            b=end.b;
            a=end.a;
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
        a = a * weight_start + control.a * weight_control + end.a * weight_end;
    }
    //--------------------------------------------------------------------------------------------------
    void Color4f::makeGrey()
    {
        float lum=RGB2Lum(r,g,b);

        r=lum;
        g=lum;
        b=lum;
    }
    //--------------------------------------------------------------------------------------------------
    Color4ub Color4f::toColor4ub() const
    {
        return Color4ub(uint8(r * 255.0f), uint8(g * 255.0f), uint8(b * 255.0f), uint8(a * 255.0f));
    }

    //--------------------------------------------------------------------------------------------------
    const Color4f &Color4f::operator = (const Color4ub &v)
    {
        r = float(v.r) / 255.0f;
        g = float(v.g) / 255.0f;
        b = float(v.b) / 255.0f;
        a = float(v.a) / 255.0f;
        clamp();
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool Color4f::operator == (const Color4f &v) const
    {
        if(!math::IsNearlyEqual(r,v.r))return(false);
        if(!math::IsNearlyEqual(g,v.g))return(false);
        if(!math::IsNearlyEqual(b,v.b))return(false);
        if(!math::IsNearlyEqual(a,v.a))return(false);

        return(true);
    }
    //--------------------------------------------------------------------------------------------------
    bool Color4f::operator != (const Color4f &v) const
    {
        if(!math::IsNearlyEqual(r,v.r))return(true);
        if(!math::IsNearlyEqual(g,v.g))return(true);
        if(!math::IsNearlyEqual(b,v.b))return(true);
        if(!math::IsNearlyEqual(a,v.a))return(true);

        return(false);
    }
    //--------------------------------------------------------------------------------------------------
}//namespace hgl
