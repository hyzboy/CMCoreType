#include<hgl/color/CMYKf.h>
#include<hgl/color/CMYKub.h>
#include<hgl/color/Color3f.h>
#include<hgl/math/FloatPrecision.h>
#include<algorithm>
namespace hgl
{
    //--------------------------------------------------------------------------------------------------
    void CMYKf::clamp()
    {
        if(c<0)c=0; if(c>1)c=1;
        if(m<0)m=0; if(m>1)m=1;
        if(y<0)y=0; if(y>1)y=1;
        if(k<0)k=0; if(k>1)k=1;
    }

    //--------------------------------------------------------------------------------------------------
    CMYKf::CMYKf(const CMYKub &v)
    {
        c = float(v.c) / 255.0f;
        m = float(v.m) / 255.0f;
        y = float(v.y) / 255.0f;
        k = float(v.k) / 255.0f;
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    /**
    * RGB to CMYK conversion formula:
    * K = 1 - max(R, G, B)
    * C = (1 - R - K) / (1 - K)
    * M = (1 - G - K) / (1 - K)
    * Y = (1 - B - K) / (1 - K)
    */
    CMYKf::CMYKf(const Color3f &rgb)
    {
        fromRGB(rgb);
    }

    //--------------------------------------------------------------------------------------------------
    void CMYKf::fromRGB(const Color3f &rgb)
    {
        float max_rgb = std::max({rgb.r, rgb.g, rgb.b});
        k = 1.0f - max_rgb;

        if(k >= 1.0f)
        {
            c = m = y = 0.0f;
            k = 1.0f;
        }
        else
        {
            float inv_one_minus_k = 1.0f / (1.0f - k);
            c = (1.0f - rgb.r - k) * inv_one_minus_k;
            m = (1.0f - rgb.g - k) * inv_one_minus_k;
            y = (1.0f - rgb.b - k) * inv_one_minus_k;
        }

        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    /**
    * CMYK to RGB conversion formula:
    * R = (1 - C) * (1 - K)
    * G = (1 - M) * (1 - K)
    * B = (1 - Y) * (1 - K)
    */
    Color3f CMYKf::toRGB() const
    {
        float inv_k = 1.0f - k;

        Color3f result;
        result.r = (1.0f - c) * inv_k;
        result.g = (1.0f - m) * inv_k;
        result.b = (1.0f - y) * inv_k;

        return result;
    }

    //--------------------------------------------------------------------------------------------------
    CMYKub CMYKf::toCMYKub() const
    {
        return CMYKub(uint8(c * 255.0f), uint8(m * 255.0f), uint8(y * 255.0f), uint8(k * 255.0f));
    }

    //--------------------------------------------------------------------------------------------------
    const CMYKf &CMYKf::operator=(const CMYKub &v)
    {
        c = float(v.c) / 255.0f;
        m = float(v.m) / 255.0f;
        y = float(v.y) / 255.0f;
        k = float(v.k) / 255.0f;
        clamp();
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool CMYKf::operator==(const CMYKf &v) const
    {
        if(!math::IsNearlyEqual(c, v.c))return(false);
        if(!math::IsNearlyEqual(m, v.m))return(false);
        if(!math::IsNearlyEqual(y, v.y))return(false);
        if(!math::IsNearlyEqual(k, v.k))return(false);

        return(true);
    }

    //--------------------------------------------------------------------------------------------------
    bool CMYKf::operator!=(const CMYKf &v) const
    {
        if(!math::IsNearlyEqual(c, v.c))return(true);
        if(!math::IsNearlyEqual(m, v.m))return(true);
        if(!math::IsNearlyEqual(y, v.y))return(true);
        if(!math::IsNearlyEqual(k, v.k))return(true);

        return(false);
    }

    //--------------------------------------------------------------------------------------------------
}

namespace hgl
{
    const CMYKf WhiteCMYKf(0, 0, 0, 0);     // No ink
    const CMYKf BlackCMYKf(0, 0, 0, 1);     // Pure black
    const CMYKf CyanCMYKf(1, 0, 0, 0);
    const CMYKf MagentaCMYKf(0, 1, 0, 0);
    const CMYKf YellowCMYKf(0, 0, 1, 0);
    const CMYKf RedCMYKf(0, 1, 1, 0);       // M + Y
    const CMYKf GreenCMYKf(1, 0, 1, 0);     // C + Y
    const CMYKf BlueCMYKf(1, 1, 0, 0);      // C + M
}
