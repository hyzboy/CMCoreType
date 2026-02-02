#pragma once

#include<stdarg.h>
#include<cstddef>
#include<stdio.h>

namespace hgl
{
    // 计算格式化字符串所需的长度（不包括 null 终止符）
    template<typename T> int vsprintf_length(const T *format, va_list va);

    // 格式化字符串到缓冲区
    template<typename T> int vsprintf(T *buffer, const size_t buf_max, const T *format, va_list va);

    // ==================== char 特化实现 ====================
    template<> inline int vsprintf_length<char>(const char *format, va_list va)
    {
#ifdef _MSC_VER
        return _vscprintf(format, va);  // Windows MSVC
#else
        return vsnprintf(NULL, 0, format, va);  // 标准 C99
#endif
    }

    template<> inline int vsprintf<char>(char *buffer, const size_t buf_max, const char *format, va_list va)
    {
#ifdef _MSC_VER
        return vsprintf_s(buffer, buf_max, format, va);
#else
        return vsnprintf(buffer, buf_max, format, va);
#endif
    }

    // ==================== wchar_t 特化实现 ====================
    template<> inline int vsprintf_length<wchar_t>(const wchar_t *format, va_list va)
    {
#ifdef _MSC_VER
        return _vscwprintf(format, va);  // Windows MSVC
#else
        return vswprintf(NULL, 0, format, va);  // 标准 C99
#endif
    }

    template<> inline int vsprintf<wchar_t>(wchar_t *buffer, const size_t buf_max, const wchar_t *format, va_list va)
    {
#ifdef _MSC_VER
        return vswprintf_s(buffer, buf_max, format, va);
#else
        return vswprintf(buffer, buf_max, format, va);
#endif
    }

    // ==================== C++20 char8_t 特化实现 ====================
#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
    template<> inline int vsprintf_length<char8_t>(const char8_t *format, va_list va)
    {
        // char8_t 底层与 char 兼容，直接 reinterpret_cast
        return vsprintf_length<char>(reinterpret_cast<const char*>(format), va);
    }

    template<> inline int vsprintf<char8_t>(char8_t *buffer, const size_t buf_max, const char8_t *format, va_list va)
    {
        // char8_t 底层与 char 兼容，直接 reinterpret_cast
        return vsprintf<char>(reinterpret_cast<char*>(buffer), buf_max,
                             reinterpret_cast<const char*>(format), va);
    }
#endif

    // ==================== char16_t 特化实现（非 Windows 平台）====================
#ifndef _MSC_VER
    // 非 Windows 平台上，wchar_t 通常是 4 字节，char16_t 是 2 字节，需要单独处理
    #if WCHAR_MAX > 0xFFFF
    // wchar_t 是 4 字节（UTF-32），char16_t 需要单独处理
    template<> inline int vsprintf_length<char16_t>(const char16_t *format, va_list va)
    {
        // char16_t 与 wchar_t 底层不兼容时，暂时返回 -1 表示不支持
        // 实际使用时建议转换为 wchar_t 或使用其他方式
        return -1;
    }

    template<> inline int vsprintf<char16_t>(char16_t *buffer, const size_t buf_max, const char16_t *format, va_list va)
    {
        // char16_t 与 wchar_t 底层不兼容时，暂时返回 -1 表示不支持
        // 实际使用时建议转换为 wchar_t 或使用其他方式
        return -1;
    }
    #else
    // wchar_t 是 2 字节（UTF-16），char16_t 可以与 wchar_t 互转
    template<> inline int vsprintf_length<char16_t>(const char16_t *format, va_list va)
    {
        return vswprintf(NULL, 0, reinterpret_cast<const wchar_t*>(format), va);
    }

    template<> inline int vsprintf<char16_t>(char16_t *buffer, const size_t buf_max, const char16_t *format, va_list va)
    {
        return vswprintf(reinterpret_cast<wchar_t*>(buffer), buf_max,
                        reinterpret_cast<const wchar_t*>(format), va);
    }
    #endif
#endif
}//namespace hgl
