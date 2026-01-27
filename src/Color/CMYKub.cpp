#include<hgl/color/CMYKub.h>
#include<hgl/color/CMYKf.h>
namespace hgl
{
    //--------------------------------------------------------------------------------------------------
    void CMYKub::clamp()
    {
        // uint8 values are naturally clamped to 0-255 by type
    }

    //--------------------------------------------------------------------------------------------------
    CMYKub::CMYKub(const CMYKf &v)
    {
        x = uint8(v.x * 255.0f);
        y = uint8(v.y * 255.0f);
        z = uint8(v.z * 255.0f);
        w = uint8(v.w * 255.0f);
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    void CMYKub::setf(float fc, float fm, float fy, float fk)
    {
        x = uint8(fc * 255.0f);
        y = uint8(fm * 255.0f);
        z = uint8(fy * 255.0f);
        w = uint8(fk * 255.0f);
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    CMYKf CMYKub::toCMYKf() const
    {
        return CMYKf(float(x) / 255.0f, float(y) / 255.0f, float(z) / 255.0f, float(w) / 255.0f);
    }

    //--------------------------------------------------------------------------------------------------
    const CMYKub &CMYKub::operator=(const CMYKf &v)
    {
        x = uint8(v.x * 255.0f);
        y = uint8(v.y * 255.0f);
        z = uint8(v.z * 255.0f);
        w = uint8(v.w * 255.0f);
        clamp();
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool CMYKub::operator==(const CMYKub &v) const
    {
        if(x != v.x)return(false);
        if(y != v.y)return(false);
        if(z != v.z)return(false);
        if(w != v.w)return(false);

        return(true);
    }

    //--------------------------------------------------------------------------------------------------
    bool CMYKub::operator!=(const CMYKub &v) const
    {
        if(x != v.x)return(true);
        if(y != v.y)return(true);
        if(z != v.z)return(true);
        if(w != v.w)return(true);

        return(false);
    }

    //--------------------------------------------------------------------------------------------------
}

namespace hgl
{
    const CMYKub WhiteCMYKub(0, 0, 0, 0);       // No ink
    const CMYKub BlackCMYKub(0, 0, 0, 255);     // Pure black
    const CMYKub CyanCMYKub(255, 0, 0, 0);
    const CMYKub MagentaCMYKub(0, 255, 0, 0);
    const CMYKub YellowCMYKub(0, 0, 255, 0);
    const CMYKub RedCMYKub(0, 255, 255, 0);     // M + Y
    const CMYKub GreenCMYKub(255, 0, 255, 0);   // C + Y
    const CMYKub BlueCMYKub(255, 255, 0, 0);    // C + M
}
