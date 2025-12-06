#pragma once

#include<hgl/CoreType.h>
#include<bit>
#include<cmath>
#include<limits>

namespace hgl::math
{
    /**
     * 浮点数验证函数 - 使用 C++20 标准库和 std::bit_cast
     * 支持 half_float, float, double 三种类型
     */

    // IsNaN - 检测是否为NaN（使用标准库）
    constexpr bool IsNaN(const half_float hf) noexcept
    {
        return ((hf & 0x7C00) == 0x7C00) && ((hf & 0x03FF) != 0);
    }

    constexpr bool IsNaN(const float value) noexcept
    {
        return std::isnan(value);
    }

    constexpr bool IsNaN(const double value) noexcept
    {
        return std::isnan(value);
    }

    // IsInfinite - 检测是否为无穷大（使用标准库）
    constexpr bool IsInfinite(const half_float hf) noexcept
    {
        return ((hf & 0x7C00) == 0x7C00) && ((hf & 0x03FF) == 0);
    }

    constexpr bool IsInfinite(const float value) noexcept
    {
        return std::isinf(value);
    }

    constexpr bool IsInfinite(const double value) noexcept
    {
        return std::isinf(value);
    }

    // IsPositiveInfinite - 检测是否为正无穷大
    constexpr bool IsPositiveInfinite(const half_float hf) noexcept
    {
        return (hf & 0x7FFF) == 0x7C00;
    }

    constexpr bool IsPositiveInfinite(const float value) noexcept
    {
        return std::bit_cast<uint32>(value) == 0x7F800000;
    }

    constexpr bool IsPositiveInfinite(const double value) noexcept
    {
        return std::bit_cast<uint64>(value) == 0x7FF0000000000000;
    }

    // IsNegativeInfinite - 检测是否为负无穷大
    constexpr bool IsNegativeInfinite(const half_float hf) noexcept
    {
        return hf == 0xFC00;
    }

    constexpr bool IsNegativeInfinite(const float value) noexcept
    {
        return std::bit_cast<uint32>(value) == 0xFF800000;
    }

    constexpr bool IsNegativeInfinite(const double value) noexcept
    {
        return std::bit_cast<uint64>(value) == 0xFFF0000000000000;
    }

    // IsZero - 检测是否为零（正负）
    constexpr bool IsZero(const half_float hf) noexcept
    {
        return (hf & 0x7FFF) == 0;
    }

    constexpr bool IsZero(const float value) noexcept
    {
        return (std::bit_cast<uint32>(value) & 0x7FFFFFFF) == 0;
    }

    constexpr bool IsZero(const double value) noexcept
    {
        return (std::bit_cast<uint64>(value) & 0x7FFFFFFFFFFFFFFF) == 0;
    }

    // IsPositiveZero - 检测是否为正零
    constexpr bool IsPositiveZero(const half_float hf) noexcept
    {
        return hf == 0;
    }

    constexpr bool IsPositiveZero(const float value) noexcept
    {
        return std::bit_cast<uint32>(value) == 0;
    }

    constexpr bool IsPositiveZero(const double value) noexcept
    {
        return std::bit_cast<uint64>(value) == 0;
    }

    // IsNegativeZero - 检测是否为负零
    constexpr bool IsNegativeZero(const half_float hf) noexcept
    {
        return hf == 0x8000;
    }

    constexpr bool IsNegativeZero(const float value) noexcept
    {
        return std::bit_cast<uint32>(value) == 0x80000000;
    }

    constexpr bool IsNegativeZero(const double value) noexcept
    {
        return std::bit_cast<uint64>(value) == 0x8000000000000000;
    }

    // IsFinite - 检测是否为有限数（使用标准库）
    constexpr bool IsFinite(const half_float hf) noexcept
    {
        return (hf & 0x7C00) != 0x7C00;
    }

    constexpr bool IsFinite(const float value) noexcept
    {
        return std::isfinite(value);
    }

    constexpr bool IsFinite(const double value) noexcept
    {
        return std::isfinite(value);
    }

    // IsDenormalized - 检测是否为非规格化数
    constexpr bool IsDenormalized(const half_float hf) noexcept
    {
        return ((hf & 0x7C00) == 0) && ((hf & 0x03FF) != 0);
    }

    constexpr bool IsDenormalized(const float value) noexcept
    {
        return std::fpclassify(value) == FP_SUBNORMAL;
    }

    constexpr bool IsDenormalized(const double value) noexcept
    {
        return std::fpclassify(value) == FP_SUBNORMAL;
    }

    // IsNormalized - 检测是否为规格化数
    constexpr bool IsNormalized(const half_float hf) noexcept
    {
        uint exp = (hf & 0x7C00) >> 10;
        return exp > 0 && exp < 0x1F;
    }

    constexpr bool IsNormalized(const float value) noexcept
    {
        return std::fpclassify(value) == FP_NORMAL;
    }

    constexpr bool IsNormalized(const double value) noexcept
    {
        return std::fpclassify(value) == FP_NORMAL;
    }

    // IsValid - 检测浮点数是否有效（不是NaN、无穷大）
    constexpr bool IsValid(const half_float hf) noexcept
    {
        return (hf & 0x7C00) != 0x7C00;
    }

    constexpr bool IsValid(const float value) noexcept
    {
        return !std::isnan(value) && !std::isinf(value);
    }

    constexpr bool IsValid(const double value) noexcept
    {
        return !std::isnan(value) && !std::isinf(value);
    }
}//namespace hgl::math
