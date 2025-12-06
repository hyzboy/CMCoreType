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
        c = uint8(v.c * 255.0f);
        m = uint8(v.m * 255.0f);
        y = uint8(v.y * 255.0f);
        k = uint8(v.k * 255.0f);
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    void CMYKub::setf(float fc, float fm, float fy, float fk)
    {
        c = uint8(fc * 255.0f);
        m = uint8(fm * 255.0f);
        y = uint8(fy * 255.0f);
        k = uint8(fk * 255.0f);
        clamp();
    }

    //--------------------------------------------------------------------------------------------------
    CMYKf CMYKub::toCMYKf() const
    {
        return CMYKf(float(c) / 255.0f, float(m) / 255.0f, float(y) / 255.0f, float(k) / 255.0f);
    }

    //--------------------------------------------------------------------------------------------------
    const CMYKub &CMYKub::operator=(const CMYKf &v)
    {
        c = uint8(v.c * 255.0f);
        m = uint8(v.m * 255.0f);
        y = uint8(v.y * 255.0f);
        k = uint8(v.k * 255.0f);
        clamp();
        return *this;
    }

    //--------------------------------------------------------------------------------------------------
    bool CMYKub::operator==(const CMYKub &v) const
    {
        if(c != v.c)return(false);
        if(m != v.m)return(false);
        if(y != v.y)return(false);
        if(k != v.k)return(false);

        return(true);
    }

    //--------------------------------------------------------------------------------------------------
    bool CMYKub::operator!=(const CMYKub &v) const
    {
        if(c != v.c)return(true);
        if(m != v.m)return(true);
        if(y != v.y)return(true);
        if(k != v.k)return(true);

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
