#pragma once

#include<stdarg.h>
#include<cstddef>
namespace hgl
{
    template<typename T> int vsprintf(T *,const size_t,const T *,va_list va);
}//namespace hgl
