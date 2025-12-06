#pragma once

#include<hgl/platform/Platform.h>
#include<bit>
#include<concepts>

namespace hgl
{
    //==================================================================================================
    // 位操作 / Bit Operations (C++20)
    //==================================================================================================
    
    // Concepts 定义
    template<typename T>
    concept Integral = std::is_integral_v<T>;
    
    #define HGL_BIT(n)      (1<<(n))
    #define HGL_64BIT(n)    (1LL<<(n))

    /**
     * 检测指定位是否为1
     */
    template<Integral T>
    constexpr bool is_bit_set(const T value, int offset) noexcept
    {
        return value & (T(1) << offset);
    }

    /**
     * 检测指定位是否为0
     */
    template<Integral T>
    constexpr bool is_bit_clear(const T value, int offset) noexcept
    {
        return !(value & (T(1) << offset));
    }

    /**
     * 设置指定位为1
     */
    template<Integral T>
    constexpr void set_bit(T &value, int offset) noexcept
    {
        value |= (T(1) << offset);
    }

    /**
     * 设置指定位为0
     */
    template<Integral T>
    constexpr void clear_bit(T &value, int offset) noexcept
    {
        value &= ~(T(1) << offset);
    }

    /**
     * 设置指定位为指定值
     */
    template<Integral T>
    constexpr void set_bit_value(T &value, int offset, bool b) noexcept
    {
        if(b)
            set_bit(value, offset);
        else
            clear_bit(value, offset);
    }

    /**
     * 翻转指定位
     */
    template<Integral T>
    constexpr void toggle_bit(T &value, int offset) noexcept
    {
        value ^= (T(1) << offset);
    }

    /**
     * 计算值中1的个数（使用C++20标准库）
     */
    template<Integral T>
    constexpr int popcount(const T value) noexcept
    {
        if constexpr (std::unsigned_integral<T>) {
            return std::popcount(value);
        } else {
            using UT = std::make_unsigned_t<T>;
            return std::popcount(static_cast<UT>(value));
        }
    }

    /**
     * 计算值中指定范围内1的个数
     */
    template<Integral T>
    constexpr int popcount(const T value, int bit_count) noexcept
    {
        if (bit_count >= sizeof(T) * 8) {
            return popcount(value);
        }
        
        const T mask = (T(1) << bit_count) - 1;
        return popcount(value & mask);
    }

    /**
     * 获取最低位1的位置（从0开始，使用C++20标准库）
     * 如果没有1则返回-1
     */
    template<Integral T>
    constexpr int bit_offset(const T value) noexcept
    {
        if (value == 0) return -1;
        
        if constexpr (std::unsigned_integral<T>) {
            return std::countr_zero(value);
        } else {
            using UT = std::make_unsigned_t<T>;
            return std::countr_zero(static_cast<UT>(value));
        }
    }

    /**
     * 获取最高位1的位置（从0开始）
     * 如果没有1则返回-1
     */
    template<Integral T>
    constexpr int bit_width(const T value) noexcept
    {
        if (value == 0) return 0;
        
        if constexpr (std::unsigned_integral<T>) {
            return std::bit_width(value);
        } else {
            using UT = std::make_unsigned_t<T>;
            return std::bit_width(static_cast<UT>(value));
        }
    }

    /**
     * 检查是否只有一位为1（是否为2的幂）
     */
    template<Integral T>
    constexpr bool has_single_bit(const T value) noexcept
    {
        if constexpr (std::unsigned_integral<T>) {
            return std::has_single_bit(value);
        } else {
            using UT = std::make_unsigned_t<T>;
            return value > 0 && std::has_single_bit(static_cast<UT>(value));
        }
    }

    /**
     * 获取不大于value的最大2的幂（向下取整到2的幂）
     */
    template<Integral T>
    constexpr T bit_floor(const T value) noexcept
    {
        if constexpr (std::unsigned_integral<T>) {
            return std::bit_floor(value);
        } else {
            using UT = std::make_unsigned_t<T>;
            return static_cast<T>(std::bit_floor(static_cast<UT>(value)));
        }
    }

    /**
     * 获取不小于value的最小2的幂（向上取整到2的幂）
     */
    template<Integral T>
    constexpr T bit_ceil(const T value) noexcept
    {
        if constexpr (std::unsigned_integral<T>) {
            return std::bit_ceil(value);
        } else {
            using UT = std::make_unsigned_t<T>;
            return static_cast<T>(std::bit_ceil(static_cast<UT>(value)));
        }
    }

    /**
     * 循环左移
     */
    template<Integral T>
    constexpr T rotl(const T value, int shift) noexcept
    {
        if constexpr (std::unsigned_integral<T>) {
            return std::rotl(value, shift);
        } else {
            using UT = std::make_unsigned_t<T>;
            return static_cast<T>(std::rotl(static_cast<UT>(value), shift));
        }
    }

    /**
     * 循环右移
     */
    template<Integral T>
    constexpr T rotr(const T value, int shift) noexcept
    {
        if constexpr (std::unsigned_integral<T>) {
            return std::rotr(value, shift);
        } else {
            using UT = std::make_unsigned_t<T>;
            return static_cast<T>(std::rotr(static_cast<UT>(value), shift));
        }
    }

    // 保留旧API的别名（向后兼容）
    template<Integral T>
    [[deprecated("Use is_bit_set instead")]]
    constexpr bool hgl_is_one(const T value, int off) noexcept
    {
        return is_bit_set(value, off);
    }

    template<Integral T>
    [[deprecated("Use is_bit_clear instead")]]
    constexpr bool hgl_is_zero(const T value, int off) noexcept
    {
        return is_bit_clear(value, off);
    }

    template<Integral T>
    [[deprecated("Use set_bit instead")]]
    constexpr void hgl_set_one(T &value, int off) noexcept
    {
        set_bit(value, off);
    }

    template<Integral T>
    [[deprecated("Use clear_bit instead")]]
    constexpr void hgl_set_zero(T &value, int off) noexcept
    {
        clear_bit(value, off);
    }

    template<Integral T>
    [[deprecated("Use set_bit_value instead")]]
    constexpr void hgl_set_bit(T &value, int off, bool b) noexcept
    {
        set_bit_value(value, off, b);
    }

    template<Integral T>
    [[deprecated("Use popcount instead")]]
    constexpr int hgl_bit_count(const T value) noexcept
    {
        return popcount(value);
    }

    template<Integral T>
    [[deprecated("Use popcount instead")]]
    constexpr int hgl_bit_count(const T value, int size) noexcept
    {
        return popcount(value, size);
    }

    template<Integral T>
    [[deprecated("Use bit_offset instead")]]
    constexpr int GetBitOffset(const T value) noexcept
    {
        return bit_offset(value);
    }
}//namespace hgl
