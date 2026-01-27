#include<hgl/color/LinearColor3f.h>
#include<hgl/color/Color3f.h>
#include<hgl/color/sRGBConvert.h>
#include<hgl/math/FloatPrecision.h>
namespace hgl
{

    //--------------------------------------------------------------------------------------------------
    LinearColor3f::LinearColor3f(const Color3f &v)
    {
        r = sRGB2Linear(v.r);
        g = sRGB2Linear(v.g);
        b = sRGB2Linear(v.b);
    }

    //--------------------------------------------------------------------------------------------------
    void LinearColor3f::clamp()
    {
        // 线性颜色允许无限制的值,所以不做clamping
        // 仅在极端情况下防止溢出
    }

    //--------------------------------------------------------------------------------------------------
    void LinearColor3f::lerp(float nr, float ng, float nb, float t)
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
    Color3f LinearColor3f::toSRGB() const
    {
        Color3f result;
        result.r = Linear2sRGB(r);
        result.g = Linear2sRGB(g);
        result.b = Linear2sRGB(b);
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    const LinearColor3f &LinearColor3f::operator = (const Color3f &v)
    {
        r = sRGB2Linear(v.r);
        g = sRGB2Linear(v.g);
        b = sRGB2Linear(v.b);
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool LinearColor3f::operator == (const LinearColor3f &v) const
    {
        if(!math::IsNearlyEqual(r,v.r))return(false);
        if(!math::IsNearlyEqual(g,v.g))return(false);
        if(!math::IsNearlyEqual(b,v.b))return(false);
        return(true);
    }

    //--------------------------------------------------------------------------------------------------
    bool LinearColor3f::operator != (const LinearColor3f &v) const
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
    const LinearColor3f LinearBlackColor3f(0,0,0);
    const LinearColor3f LinearWhiteColor3f(1,1,1);
    const LinearColor3f LinearRedColor3f(1,0,0);
    const LinearColor3f LinearGreenColor3f(0,1,0);
    const LinearColor3f LinearBlueColor3f(0,0,1);
    const LinearColor3f LinearYellowColor3f(1,1,0);
}
