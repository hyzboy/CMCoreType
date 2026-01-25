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
        if(x<0)x=0; if(x>1)x=1;
        if(y<0)y=0; if(y>1)y=1;
        if(z<0)z=0; if(z>1)z=1;
        if(w<0)w=0; if(w>1)w=1;
    }

    //--------------------------------------------------------------------------------------------------
    CMYKf::CMYKf(const CMYKub &v)
    {
        x = float(v.x) / 255.0f;
        y = float(v.y) / 255.0f;
        z = float(v.z) / 255.0f;
        w = float(v.w) / 255.0f;
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
        w = 1.0f - max_rgb;

        if(w >= 1.0f)
        {
            x = y = z = 0.0f;
            w = 1.0f;
        }
        else
        {
            float inv_one_minus_k = 1.0f / (1.0f - w);
            x = (1.0f - rgb.r - w) * inv_one_minus_k;
            y = (1.0f - rgb.g - w) * inv_one_minus_k;
            z = (1.0f - rgb.b - w) * inv_one_minus_k;
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
        float inv_k = 1.0f - w;

        Color3f result;
        result.r = (1.0f - x) * inv_k;
        result.g = (1.0f - y) * inv_k;
        result.b = (1.0f - z) * inv_k;

        return result;
    }

    //--------------------------------------------------------------------------------------------------
    CMYKub CMYKf::toCMYKub() const
    {
        return CMYKub(uint8(x * 255.0f), uint8(y * 255.0f), uint8(z * 255.0f), uint8(w * 255.0f));
    }

    //--------------------------------------------------------------------------------------------------
    const CMYKf &CMYKf::operator=(const CMYKub &v)
    {
        x = float(v.x) / 255.0f;
        y = float(v.y) / 255.0f;
        z = float(v.z) / 255.0f;
        w = float(v.w) / 255.0f;
        clamp();
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool CMYKf::operator==(const CMYKf &v) const
    {
        if(!math::IsNearlyEqual(x, v.x))return(false);
        if(!math::IsNearlyEqual(y, v.y))return(false);
        if(!math::IsNearlyEqual(z, v.z))return(false);
        if(!math::IsNearlyEqual(w, v.w))return(false);

        return(true);
    }

    //--------------------------------------------------------------------------------------------------
    bool CMYKf::operator!=(const CMYKf &v) const
    {
        if(!math::IsNearlyEqual(x, v.x))return(true);
        if(!math::IsNearlyEqual(y, v.y))return(true);
        if(!math::IsNearlyEqual(z, v.z))return(true);
        if(!math::IsNearlyEqual(w, v.w))return(true);

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
