#include<hgl/color/Color4ub.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Lum.h>
#include<cmath>
#include<numbers>
namespace hgl
{
    constexpr float HGL_PI = std::numbers::pi_v<float>;

    void Color4ub::clamp()
    {
        // uint8 values are naturally clamped to 0-255, but we check for arithmetic overflow
        // This is mostly for consistency with the float version
    }

    //--------------------------------------------------------------------------------------------------
    Color4ub::Color4ub(const Color4f &v)
    {
        r = uint8(v.r * 255.0f);
        g = uint8(v.g * 255.0f);
        b = uint8(v.b * 255.0f);
        a = uint8(v.a * 255.0f);
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    void Color4ub::setf(float vr, float vg, float vb, float va)
    {
        r = uint8(vr * 255.0f);
        g = uint8(vg * 255.0f);
        b = uint8(vb * 255.0f);
        a = uint8(va * 255.0f);
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    /**
    * 求当前颜色过渡到另一颜色时某一比例时的颜色
    * @param nr,ng,nb 新的颜色
    * @param t 过渡比例,0时为当前的颜色,1时为nr,ng,nb
    */
    void Color4ub::lerp(uint8 nr, uint8 ng, uint8 nb, float t)
    {
        if(t<=0)return;
        if(t>=1)
        {
            r=nr;
            g=ng;
            b=nb;
            return;
        }

        r = uint8(r + (nr-r)*t);
        g = uint8(g + (ng-g)*t);
        b = uint8(b + (nb-b)*t);
    }

    //--------------------------------------------------------------------------------------------------
    /**
    * 余弦插值到另一颜色
    * @param c 目标颜色
    * @param t 过渡比例,0时为当前的颜色,1时为目标颜色
    */
    void Color4ub::lerpSmooth(const Color4ub &c, float t)
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
        float smooth_t = (1.0f - cosf(t * HGL_PI)) * 0.5f;
        
        r = uint8(r + (c.r-r)*smooth_t);
        g = uint8(g + (c.g-g)*smooth_t);
        b = uint8(b + (c.b-b)*smooth_t);
        a = uint8(a + (c.a-a)*smooth_t);
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * 三次插值到另一颜色
    * @param c 目标颜色
    * @param t 过渡比例,0时为当前的颜色,1时为目标颜色
    */
    void Color4ub::lerpCubic(const Color4ub &c, float t)
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
        
        r = uint8(r + (c.r-r)*cubic_t);
        g = uint8(g + (c.g-g)*cubic_t);
        b = uint8(b + (c.b-b)*cubic_t);
        a = uint8(a + (c.a-a)*cubic_t);
    }
    //--------------------------------------------------------------------------------------------------
    /**
    * Bezier曲线插值到另一颜色
    * @param control 控制点颜色
    * @param end 目标颜色
    * @param t 过渡比例,0时为当前的颜色,1时为目标颜色
    */
    void Color4ub::lerpBezier(const Color4ub &control, const Color4ub &end, float t)
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
        
        r = uint8(r * weight_start + control.r * weight_control + end.r * weight_end);
        g = uint8(g * weight_start + control.g * weight_control + end.g * weight_end);
        b = uint8(b * weight_start + control.b * weight_control + end.b * weight_end);
        a = uint8(a * weight_start + control.a * weight_control + end.a * weight_end);
    }

    //--------------------------------------------------------------------------------------------------
    uint8 Color4ub::toGray() const
    {
        // Convert to float, apply RGB2Lum, then back to uint8
        float fr = float(r) / 255.0f;
        float fg = float(g) / 255.0f;
        float fb = float(b) / 255.0f;
        return uint8(RGB2Lum(fr, fg, fb) * 255.0f);
    }

    //--------------------------------------------------------------------------------------------------
    void Color4ub::makeGray()
    {
        uint8 lum = toGray();
        r = lum;
        g = lum;
        b = lum;
    }

    //--------------------------------------------------------------------------------------------------
    Color4f Color4ub::toColor4f() const
    {
        return Color4f(float(r)/255.0f, float(g)/255.0f, float(b)/255.0f, float(a)/255.0f);
    }

    //--------------------------------------------------------------------------------------------------
    const Color4ub &Color4ub::operator = (const Color4f &v)
    {
        r = uint8(v.r * 255.0f);
        g = uint8(v.g * 255.0f);
        b = uint8(v.b * 255.0f);
        a = uint8(v.a * 255.0f);
        clamp();
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool Color4ub::operator == (const Color4ub &v) const
    {
        if(r!=v.r)return(false);
        if(g!=v.g)return(false);
        if(b!=v.b)return(false);
        if(a!=v.a)return(false);

        return(true);
    }

    //--------------------------------------------------------------------------------------------------
    bool Color4ub::operator != (const Color4ub &v) const
    {
        if(r!=v.r)return(true);
        if(g!=v.g)return(true);
        if(b!=v.b)return(true);
        if(a!=v.a)return(true);

        return(false);
    }
    //--------------------------------------------------------------------------------------------------
}

namespace hgl
{
    const Color4ub BlackColor4ub(0,0,0,255);
    const Color4ub WhiteColor4ub(255,255,255,255);
    const Color4ub RedColor4ub(255,0,0,255);
    const Color4ub GreenColor4ub(0,255,0,255);
    const Color4ub BlueColor4ub(0,0,255,255);
    const Color4ub YellowColor4ub(255,255,0,255);
}