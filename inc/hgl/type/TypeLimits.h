#pragma once

#include<hgl/type/Constants.h>
#include<limits>
#include<concepts>

namespace hgl
{
    //==================================================================================================
    // 类型极值模板函数 / Type Limit Template Functions (C++20)
    //==================================================================================================

    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    /**
     * 获取类型的最大值（使用 std::numeric_limits）
     */
    template<Numeric T>
    constexpr T numeric_max() noexcept
    {
        return std::numeric_limits<T>::max();
    }

    /**
     * 获取类型的最小值（使用 std::numeric_limits）
     */
    template<Numeric T>
    constexpr T numeric_min() noexcept
    {
        if constexpr (std::is_integral_v<T>) {
            return std::numeric_limits<T>::min();
        } else {
            // 对于浮点数，lowest() 返回最小负数
            return std::numeric_limits<T>::lowest();
        }
    }

    /**
     * 获取类型的最低值（最小负数或最小正数）
     */
    template<Numeric T>
    constexpr T numeric_lowest() noexcept
    {
        return std::numeric_limits<T>::lowest();
    }

    /**
     * 获取无符号整数类型的一半值
     */
    template<std::unsigned_integral T>
    constexpr T unsigned_half() noexcept
    {
        return std::numeric_limits<T>::max() >> 1;
    }

    /**
     * 获取类型的 epsilon（浮点数精度）
     */
    template<std::floating_point T>
    constexpr T numeric_epsilon() noexcept
    {
        return std::numeric_limits<T>::epsilon();
    }

    /**
     * 获取类型的无穷大值（仅浮点数）
     */
    template<std::floating_point T>
    constexpr T numeric_infinity() noexcept
    {
        return std::numeric_limits<T>::infinity();
    }

    /**
     * 获取类型的 quiet NaN（仅浮点数）
     */
    template<std::floating_point T>
    constexpr T numeric_quiet_nan() noexcept
    {
        return std::numeric_limits<T>::quiet_NaN();
    }

    /**
     * 检查类型是否有符号
     */
    template<Numeric T>
    constexpr bool is_signed() noexcept
    {
        return std::numeric_limits<T>::is_signed;
    }

    /**
     * 获取类型的位数
     */
    template<std::integral T>
    constexpr int bit_width() noexcept
    {
        return std::numeric_limits<T>::digits + (std::numeric_limits<T>::is_signed ? 1 : 0);
    }

    // 向后兼容的别名
    template<typename T>
    [[deprecated("Use numeric_max instead")]]
    constexpr T HGL_INTEGER_MAX() noexcept {
        return numeric_max<T>();
    }

    template<typename T>
    [[deprecated("Use numeric_min instead")]]
    constexpr T HGL_INTEGER_MIN() noexcept {
        return numeric_min<T>();
    }

    template<typename T>
    [[deprecated("Use unsigned_half instead")]]
    constexpr T HGL_UINTEGER_HALF() noexcept {
        return unsigned_half<T>();
    }
}//namespace hgl
