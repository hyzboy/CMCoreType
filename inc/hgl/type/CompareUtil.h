#pragma once

#include<hgl/platform/Platform.h>
#include<algorithm>
#include<numeric>
#include<utility>
#include<concepts>

namespace hgl
{
    //==================================================================================================
    // 交换与比较 / Swap and Comparison (C++20)
    //==================================================================================================
    
    // Concepts 定义
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template<typename T>
    concept Comparable = requires(T a, T b) {
        { a < b } -> std::convertible_to<bool>;
        { a > b } -> std::convertible_to<bool>;
    };

    // 使用标准库 swap
    using std::swap;

    /**
     * 限制值在指定范围内（使用标准库 clamp）
     */
    template<Comparable T>
    constexpr T clamp(const T &value, const T &min_value, const T &max_value) noexcept
    {
        return std::clamp(value, min_value, max_value);
    }

    /**
     * 绝对值（使用标准库 abs）
     */
    template<Arithmetic T>
    constexpr T abs(const T &value) noexcept
    {
        if constexpr (std::is_unsigned_v<T>) {
            return value;
        } else {
            return std::abs(value);
        }
    }

    /**
     * 将a更新为a和b中的较小值
     */
    template<Comparable T>
    constexpr void update_min(T &a, const T &b) noexcept
    {
        a = std::min(a, b);
    }

    /**
     * 将a更新为a和b中的较大值
     */
    template<Comparable T>
    constexpr void update_max(T &a, const T &b) noexcept
    {
        a = std::max(a, b);
    }

    /**
     * 返回两个值中的较小值（使用标准库 min）
     */
    template<Comparable T>
    constexpr const T& min(const T &a, const T &b) noexcept
    {
        return std::min(a, b);
    }

    /**
     * 返回两个值中的较大值（使用标准库 max）
     */
    template<Comparable T>
    constexpr const T& max(const T &a, const T &b) noexcept
    {
        return std::max(a, b);
    }

    /**
     * 在数组中查找最小值
     */
    template<Comparable T>
    constexpr T min_element_value(const T *data, int count, T default_value) noexcept
    {
        if (!data || count <= 0) return default_value;
        return *std::min_element(data, data + count);
    }

    /**
     * 在数组中查找最大值
     */
    template<Comparable T>
    constexpr T max_element_value(const T *data, int count, T default_value) noexcept
    {
        if (!data || count <= 0) return default_value;
        return *std::max_element(data, data + count);
    }

    // 向后兼容的别名
    template<typename T>
    [[deprecated("Use std::swap instead")]]
    inline void hgl_swap(T &x, T &y) noexcept {
        std::swap(x, y);
    }

    template<typename T>
    [[deprecated("Use clamp instead")]]
    constexpr T hgl_clamp(const T &cur, const T &min_value, const T &max_value) noexcept {
        return clamp(cur, min_value, max_value);
    }

    template<typename T>
    [[deprecated("Use abs instead")]]
    constexpr T hgl_abs(const T &v) noexcept {
        return abs(v);
    }

    template<typename T>
    [[deprecated("Use update_min instead")]]
    constexpr void get_min(T &a, const T &b) noexcept {
        update_min(a, b);
    }

    template<typename T>
    [[deprecated("Use update_max instead")]]
    constexpr void get_max(T &a, const T &b) noexcept {
        update_max(a, b);
    }

    template<typename T>
    [[deprecated("Use min instead")]]
    constexpr T hgl_min(const T &a, const T &b) noexcept {
        return min(a, b);
    }

    template<typename T>
    [[deprecated("Use max instead")]]
    constexpr T hgl_max(const T &a, const T &b) noexcept {
        return max(a, b);
    }

    template<typename T>
    [[deprecated("Use min_element_value instead")]]
    constexpr T hgl_min(const T *data, int count, T min_value) noexcept {
        return min_element_value(data, count, min_value);
    }

    template<typename T>
    [[deprecated("Use max_element_value instead")]]
    constexpr T hgl_max(const T *data, int count, T max_value) noexcept {
        return max_element_value(data, count, max_value);
    }
}//namespace hgl
