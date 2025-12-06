#pragma once
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME            OS_TEXT("Microsoft C/C++")
#define HGL_LIB_COMPILER_NAME        OS_TEXT("MSC")

/*
 Minimum required: C++20
 - Prefer checking _MSVC_LANG (language standard macro) and require >= C++20 (202002L)
 - If _MSVC_LANG is not available, require MSVC toolset corresponding to Visual C++ 2022 or newer (_MSC_VER >= 1930)
*/
#if defined(_MSVC_LANG)
    #if _MSVC_LANG < 202002L
        #error Please compile with C++20 support (use the compiler flag /std:c++20 or newer).
    #endif

    #define HGL_LIB_COMPILER_VERSION    OS_TEXT("C++20")
#else
    #if _MSC_VER < 1930                            // Visual C++ 2022 (MSVC 19.3x)
        #error Please upgrade your compiler or development tools to Visual C++ 2022 (MSVC 19.3) or later.
    #else
        #define HGL_LIB_COMPILER_VERSION    OS_TEXT("19.3")      //Visual C++ 2022
    #endif//_MSC_VER
#endif//_MSC_VER

#define HGL_THREAD_LOCAL_STORAGE        __declspec(thread)                              //线程本地储存
//--------------------------------------------------------------------------------------------------
#define HGL_FMT_I64            "%I64d"
#define HGL_FMT_U64            "%I64u"
//参考文档最后查阅支持版本为VC2013，网址：http://msdn.microsoft.com/en-us/library/tcxf1dw6.aspx
//--------------------------------------------------------------------------------------------------
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES                // 使用数学常数定义
#endif//_USE_MATH_DEFINES
//--------------------------------------------------------------------------------------------------
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif//

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif//

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif//

#ifdef _DLL
    #define HGL_LIB_CRT    "Dynamic"
#else
    #define HGL_LIB_CRT    "Static"
#endif//_DLL

#define HGL_LIB_FRONT    HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_" HGL_LIB_CRT "_"

#define HGL_LIB_END        ".LIB"
//--------------------------------------------------------------------------------------------------
#include<hgl/CoreType.h>
#include<hgl/platform/compiler/Property.h>
//--------------------------------------------------------------------------------------------------
