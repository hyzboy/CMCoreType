#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<hgl/color/Color.h>
#include<hgl/math/FloatPrecision.h>
namespace hgl
{
    //--------------------------------------------------------------------------------------------------
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
}
