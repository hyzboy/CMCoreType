#pragma once

#include<hgl/platform/Platform.h>

namespace hgl
{
    //==================================================================================================
    // 常量定义 / Constants Definition
    //==================================================================================================

    #define HGL_OFFICAL_WEB                "www.hyzgame.com"
    #define HGL_OFFICAL_WEB_U8     U8_TEXT("www.hyzgame.com")
    #define HGL_OFFICAL_WEB_OS     OS_TEXT("www.hyzgame.com")

    constexpr const u16char U16_FULL_WIDTH_SPACE=U16_TEXT('\u3000');      //全角空格
    constexpr const u32char U32_FULL_WIDTH_SPACE=U32_TEXT('\u3000');      //全角空格

    //==================================================================================================
    // 大小常量 / Size Constants
    //==================================================================================================

    constexpr const uint      HGL_SIZE_1KB    =1024;
    constexpr const uint      HGL_SIZE_1MB    =HGL_SIZE_1KB*1024;
    constexpr const uint      HGL_SIZE_1GB    =HGL_SIZE_1MB*1024;
    constexpr const uint64    HGL_SIZE_1TB    =HGL_SIZE_1GB*1024ULL;
    constexpr const uint64    HGL_SIZE_1PB    =HGL_SIZE_1TB*1024ULL;
    constexpr const uint64    HGL_SIZE_1EB    =HGL_SIZE_1PB*1024ULL;
//  constexpr const uint128    HGL_SIZE_1ZB   =HGL_SIZE_1EB*1024ULL;
//  constexpr const uint128    HGL_SIZE_1YB   =HGL_SIZE_1ZB*1024ULL;

    //==================================================================================================
    // 整数极值常量 / Integer Limit Constants
    //==================================================================================================

    constexpr const uint8     HGL_U8_MAX      =0xFF;
    constexpr const uint16    HGL_U16_MAX     =0xFFFF;
    constexpr const uint32    HGL_U32_MAX     =0xFFFFFFFF;
    constexpr const uint64    HGL_U64_MAX     =0xFFFFFFFFFFFFFFFFULL;

    constexpr const int8      HGL_S8_MAX      =0x7F;
    constexpr const int16     HGL_S16_MAX     =0x7FFF;
    constexpr const int32     HGL_S32_MAX     =0x7FFFFFFF;
    constexpr const int64     HGL_S64_MAX     =0x7FFFFFFFFFFFFFFFLL;

    constexpr const int8      HGL_S8_MIN      =(-0x80);
    constexpr const int16     HGL_S16_MIN     =(-0x8000);

    // `HGL_S32_MIN` / `HGL_S64_MIN` 一律用 `-MAX-1` 形式：
    // 写字面量 `-0x80000000` 时，字面量本身先溢出到无符号类型，取负再窄化回有符号
    // 在 constexpr 求值里是越界（MSVC 的 `i32` 自定义后缀 Clang 亦不认），因此不用
    // 平台分支，统一走恒为正的常量表达式。
    constexpr const int32     HGL_S32_MIN     =(-0x7FFFFFFF - 1);
    constexpr const int64     HGL_S64_MIN     =(-0x7FFFFFFFFFFFFFFFLL - 1);

    //==================================================================================================
    // 16进制字符 / Hexadecimal Characters
    //==================================================================================================

    constexpr char LowerHexChar[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};  ///<小写16进制字符
    constexpr char UpperHexChar[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};  ///<大写16进制字符
}//namespace hgl#pragma once
