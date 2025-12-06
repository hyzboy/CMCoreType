#pragma once
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME       OS_TEXT("GNU C/C++")
#define HGL_LIB_COMPILER_NAME   OS_TEXT("GCC")

/*
 Require C++20:
 - If the compiler reports the language standard via __cplusplus, require C++20 (__cplusplus >= 202002L).
 - Otherwise require GCC 10 or later as a fallback and use __VERSION__ as the compiler version string.
*/
#if defined(__cplusplus)
    #if __cplusplus < 202002L
        #error Please compile with C++20 support (use the compiler flag -std=c++20 or newer).
    #endif

    #define HGL_LIB_COMPILER_VERSION    OS_TEXT("C++20")
#elif defined(__GNUC__)
    #if __GNUC__ < 10
        #error Please upgrade your compiler or development tools to GNU C/C++ 10 or later, or enable C++20 mode.
    #else
        #define HGL_LIB_COMPILER_VERSION __VERSION__
    #endif
#else
    #error Unsupported compiler. GNU C/C++ with C++20 is required.
#endif

#define HGL_THREAD_LOCAL_STORAGE        __thread                                //线程本地储存
//--------------------------------------------------------------------------------------------------
#define HGL_LIB_FRONT   HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_"

#define HGL_LIB_END     ".a"
//--------------------------------------------------------------------------------------------------
#include<hgl/CoreType.h>
#include<hgl/platform/compiler/Property.h>
//--------------------------------------------------------------------------------------------------
