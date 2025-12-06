#pragma once

#include<hgl/platform/Platform.h>
#include<bit>
#include<concepts>

#if __cplusplus >= 202302L  //C++23
#include <memory>
#endif//

namespace hgl
{
    //==================================================================================================
    // 对齐计算 / Alignment Calculation (C++20)
    //==================================================================================================
    
    template<typename T>
    concept UnsignedIntegral = std::unsigned_integral<T>;

    /**
     * 求数值对齐后的值（通用版本）
     */
    template<UnsignedIntegral T>
    constexpr T align_to(const T value, const T alignment) noexcept
    {    
        return ((value + alignment - 1) / alignment) * alignment;
    }

    /**
     * 求数值对齐后的值（仅支持2次幂，使用位运算优化）
     */
    template<UnsignedIntegral T>
    constexpr T align_to_pow2(const T value, const T alignment) noexcept
    {
    #if defined(__cpp_lib_align) && __cpp_lib_align >= 202306L
        return std::align_up(value, alignment);
    #else
        const T align_mask = alignment - 1;
        return (value + align_mask) & (~align_mask);
    #endif
    }

    /**
     * 向下取整到2的幂（使用 C++20 std::bit_floor）
     */
    template<UnsignedIntegral T>
    constexpr T floor_to_pow2(const T value) noexcept
    {
        return std::bit_floor(value);
    }

    /**
     * 向上对齐到alignment的倍数（仅支持2次幂）
     */
    template<UnsignedIntegral T>
    constexpr T align_up(const T value, const T alignment) noexcept
    {
        return (value + alignment - T(1)) & ~(alignment - T(1));
    }

    /**
     * 向下对齐到alignment的倍数（仅支持2次幂）
     */
    template<UnsignedIntegral T>
    constexpr T align_down(const T value, const T alignment) noexcept
    {
        return value & ~(alignment - T(1));
    }

    /**
     * 计算需要多少个y大小的块来容纳x（向上取整除法）
     */
    template<UnsignedIntegral T>
    constexpr T divide_ceil(const T x, const T y) noexcept
    {
        return (x + y - T(1)) / y;
    }

    /**
     * 检查值是否已对齐
     */
    template<UnsignedIntegral T>
    constexpr bool is_aligned(const T value, const T alignment) noexcept
    {
        if (std::has_single_bit(alignment)) {
            return (value & (alignment - 1)) == 0;
        }
        return (value % alignment) == 0;
    }

    // 向后兼容的别名
    template<std::integral T>
    [[deprecated("Use align_to instead")]]
    constexpr T hgl_align(const T value, const T alignment) noexcept {
        using UnsignedT = std::make_unsigned_t<T>;
        return static_cast<T>(align_to(static_cast<UnsignedT>(value), static_cast<UnsignedT>(alignment)));
    }

    template<std::integral T>
    [[deprecated("Use align_to_pow2 instead")]]
    constexpr T hgl_align_pow2(const T value, const T alignment) noexcept {
        using UnsignedT = std::make_unsigned_t<T>;
        return static_cast<T>(align_to_pow2(static_cast<UnsignedT>(value), static_cast<UnsignedT>(alignment)));
    }

    // power_to_2 需要支持有符号和无符号类型
    template<std::integral T>
    [[deprecated("Use std::bit_ceil instead")]]
    constexpr T power_to_2(const T value) noexcept {
        using UnsignedT = std::make_unsigned_t<T>;
        return static_cast<T>(std::bit_ceil(static_cast<UnsignedT>(value)));
    }

    template<std::integral T>
    [[deprecated("Use floor_to_pow2 instead")]]
    constexpr T power_to_2_down(const T value) noexcept {
        using UnsignedT = std::make_unsigned_t<T>;
        return static_cast<T>(floor_to_pow2(static_cast<UnsignedT>(value)));
    }

    template<std::integral T>
    [[deprecated("Use divide_ceil instead")]]
    constexpr T divide_rounding_up(const T x, const T y) noexcept {
        using UnsignedT = std::make_unsigned_t<T>;
        return static_cast<T>(divide_ceil(static_cast<UnsignedT>(x), static_cast<UnsignedT>(y)));
    }
}//namespace hgl
