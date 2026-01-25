#include<hgl/color/LinearColor4f.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/sRGBConvert.h>
#include<hgl/math/FloatPrecision.h>
namespace hgl
{

    //--------------------------------------------------------------------------------------------------
    LinearColor4f::LinearColor4f(const Color4f &v)
    {
        r = sRGB2Linear(v.r);
        g = sRGB2Linear(v.g);
        b = sRGB2Linear(v.b);
        a = v.a;  // Alpha不参与色彩空间转换
    }

    //--------------------------------------------------------------------------------------------------
    void LinearColor4f::clamp()
    {
        // 线性颜色允许无限制的值,所以不做clamping
        // Alpha保留在0-1范围
        if(a<0)a=0;
        if(a>1)a=1;
    }

    //--------------------------------------------------------------------------------------------------
    void LinearColor4f::lerp(float nr, float ng, float nb, float t)
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
    Color4f LinearColor4f::toSRGB() const
    {
        Color4f result;
        result.r = Linear2sRGB(r);
        result.g = Linear2sRGB(g);
        result.b = Linear2sRGB(b);
        result.a = a;  // Alpha保持不变
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    const LinearColor4f &LinearColor4f::operator = (const Color4f &v)
    {
        r = sRGB2Linear(v.r);
        g = sRGB2Linear(v.g);
        b = sRGB2Linear(v.b);
        a = v.a;
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool LinearColor4f::operator == (const LinearColor4f &v) const
    {
        if(!math::IsNearlyEqual(r,v.r))return(false);
        if(!math::IsNearlyEqual(g,v.g))return(false);
        if(!math::IsNearlyEqual(b,v.b))return(false);
        if(!math::IsNearlyEqual(a,v.a))return(false);
        return(true);
    }

    //--------------------------------------------------------------------------------------------------
    bool LinearColor4f::operator != (const LinearColor4f &v) const
    {
        if(!math::IsNearlyEqual(r,v.r))return(true);
        if(!math::IsNearlyEqual(g,v.g))return(true);
        if(!math::IsNearlyEqual(b,v.b))return(true);
        if(!math::IsNearlyEqual(a,v.a))return(true);
        return(false);
    }

    //--------------------------------------------------------------------------------------------------
}

namespace hgl
{
    const LinearColor4f LinearBlackColor4f(0,0,0,1);
    const LinearColor4f LinearWhiteColor4f(1,1,1,1);
    const LinearColor4f LinearRedColor4f(1,0,0,1);
    const LinearColor4f LinearGreenColor4f(0,1,0,1);
    const LinearColor4f LinearBlueColor4f(0,0,1,1);
    const LinearColor4f LinearYellowColor4f(1,1,0,1);
}
