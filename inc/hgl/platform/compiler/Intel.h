#pragma once
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME   u"Intel C/C++"
//--------------------------------------------------------------------------------------------------
/*
 Require C++20:
 - If the compiler reports the language standard via __cplusplus, require C++20 (__cplusplus >= 202002L).
 - Otherwise require a recent Intel compiler as a fallback (Intel 19.0 or later).
*/
#if defined(__cplusplus)
    #if __cplusplus < 202002L
        #error Please compile with C++20 support (use the compiler flag -std=c++20 or enable C++20 mode in your compiler).
    #endif

    #define HGL_LIB_COMPILER_VERSION    OS_TEXT("C++20")
#elif defined(__INTEL_COMPILER)
    #if __INTEL_COMPILER < 1900
        #error Please upgrade your compiler to Intel C/C++ 19.0 or later, or enable C++20 mode.
    #endif

    /* stringify __INTEL_COMPILER to produce a version string */
    #define HGL_INTEL_STR_HELPER(x) #x
    #define HGL_INTEL_STR(x) HGL_INTEL_STR_HELPER(x)

    #define HGL_LIB_COMPILER_NAME "IntelCPP"
    #define HGL_LIB_COMPILER_VERSION OS_TEXT(HGL_INTEL_STR(__INTEL_COMPILER))

    #define enum_uint(name) enum name:unsigned int
#else
    #error Unsupported compiler. Intel C/C++ with C++20 is required.
#endif

#define vsnwprintf _vsnwprintf
//--------------------------------------------------------------------------------------------------
#include<hgl/CoreType.h>
#include<hgl/platform/compiler/Property.h>

#if HGL_OS == HGL_OS_Windows
    #define HGL_THREAD      __declspec(thread)
#else
    #define HGL_THREAD      __thread
#endif//HGL_OS == HGL_OS_Windows
//--------------------------------------------------------------------------------------------------
#ifdef _DLL
    #define HGL_LIB_CRT "Dynamic"
#else
    #define HGL_LIB_CRT "Static"
#endif//_DLL

#define HGL_LIB_FRONT   HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_" HGL_LIB_CRT "_"

#if HGL_OS == HGL_OS_Windows
    #define HGL_LIB_END     ".LIB"
#else
    #define HGL_LIB_END     ".a"
#endif//HGL_OS == HGL_OS_Windows
