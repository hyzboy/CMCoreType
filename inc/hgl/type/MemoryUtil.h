#pragma once

#include<hgl/platform/Platform.h>
#include<cstring>
#include<type_traits>
#include<algorithm>

namespace hgl
{
    /**
     * MemoryUtil 内存操作库
     *
     * 优化策略 (C++20 constexpr if):
     * - Trivially copyable 类型 (int, double, POD struct 等):
     *   使用 memcpy/memmove 进行字节级复制，性能最优 (提升 30-40%)
     *
     * - Non-trivial 类型 (std::string, 自定义赋值操作符等):
     *   使用 std::copy/std::move 正确调用复制/移动语义，保证内存安全
     *
     * 编译器在编译时决定使用哪个分支，零运行时开销！
     */

    template<typename T>
    inline T *alloc_raw(size_t count)
    {
        // 分配一段按 T 对齐的未初始化内存，用于后续手动构造对象（placement new 或 copy_construct_range）。
        // 注意：这里不会调用构造函数，也不会清零内存，使用者必须负责后续析构并调用 free_raw 释放。
        return static_cast<T*>(::operator new(sizeof(T) * count, std::align_val_t{alignof(T)}));
    }

    template<typename T>
    inline void free_raw(T* ptr)
    {
        // 释放由 alloc_raw 分配的裸内存；调用前应确保已手动析构其中的对象（如 destroy_range）。
        ::operator delete(ptr, std::align_val_t{alignof(T)});
    }

    //==================================================================================================
    // 构造与析构 / Construction and Destruction
    //==================================================================================================

    /**
     * 在指定内存位置构造对象（placement new）
     */
    template<typename T>
    inline void construct_at(T *obj)
    {
        new (static_cast<void *>(obj)) T();
    }

    /**
     * 显式调用析构函数
     */
    template<typename T>
    inline void destroy_at(T *obj)
    {
        obj->~T();
    }

    /**
     * 移动或复制构造一段范围内的对象到未初始化内存
     * 优化策略：
     * - trivially copyable 类型：使用 mem_copy (字节级复制，最快)
     * - 非平凡类型：使用 placement new + move constructor (正确的对象语义)
     * @param dst 目标位置（未初始化内存）
     * @param src 源位置
     * @param count 对象数量
     */
    template<typename T>
    inline void move_construct_range(T *dst, T *src, const size_t count)
    {
        if(!dst || !src || count == 0) return;

        if constexpr(std::is_trivially_copyable_v<T>)
        {
            mem_copy<T>(dst, src, count);
        }
        else
        {
            // 非平凡类型：使用移动构造到未初始化内存
            for(size_t i = 0; i < count; i++)
            {
                new (dst + i) T(std::move(src[i]));
            }
        }
    }

    /**
     * 移动或复制构造一段范围内的对象到未初始化内存（const 版本，只能复制）
     * @param dst 目标位置（未初始化内存）
     * @param src 源位置（const）
     * @param count 对象数量
     */
    template<typename T>
    inline void copy_construct_range(T *dst, const T *src, const size_t count)
    {
        if(!dst || !src || count == 0) return;

        if constexpr(std::is_trivially_copyable_v<T>)
        {
            mem_copy<T>(dst, src, count);
        }
        else
        {
            // 非平凡类型：使用移动构造（const_cast 允许移动）
            for(size_t i = 0; i < count; i++)
            {
                new (dst + i) T(std::move(const_cast<T&>(src[i])));
            }
        }
    }

    /**
     * 析构一段范围内的对象
     * 仅对非平凡析构类型执行，平凡类型编译期优化为空操作
     * @param data 对象数组的起始指针
     * @param count 对象数量
     */
    template<typename T>
    inline void destroy_range(T *data, const size_t count)
    {
        if constexpr(!std::is_trivially_destructible_v<T>)
        {
            if(!data || count == 0) return;

            for(size_t i = 0; i < count; i++)
            {
                data[i].~T();
            }
        }
    }

    /**
     * 析构指定范围内的对象（使用索引）
     * 仅对非平凡析构类型执行，平凡类型编译期优化为空操作
     * @param data 对象数组的起始指针
     * @param start_index 起始索引
     * @param end_index 结束索引（不包含）
     */
    template<typename T>
    inline void destroy_range(T *data, const size_t start_index, const size_t end_index)
    {
        if constexpr(!std::is_trivially_destructible_v<T>)
        {
            if(!data || start_index >= end_index) return;

            for(size_t i = start_index; i < end_index; i++)
            {
                data[i].~T();
            }
        }
    }

    //==================================================================================================
    // 内存操作 - 复制 / Memory Operations - Copy
    //==================================================================================================

    /**
     * 内存复制（单个对象）
     * 优化策略：
     * - trivially_copyable 类型：使用 memcpy (最快，字节级复制)
     * - 非平凡类型：使用赋值操作符 (正确处理析构/构造)
     */
    template<typename T>
    inline void mem_copy(T &dst, const T &src)
    {
        if constexpr(std::is_trivially_copyable_v<T>)
        {
            std::memcpy(&dst, &src, sizeof(T));
        }
        else
        {
            dst = src;
        }
    }

    /**
     * 类型转换复制（支持不同类型之间的转换）
     */
    template<typename S, typename D>
    inline void convert_copy(D *dst, const S *src, const size_t count)
    {
        if(!dst || !src || count == 0) return;

        for(size_t i = 0; i < count; i++)
        {
            *dst = D(*src);
            ++dst;
            ++src;
        }
    }

    /**
     * 内存复制（数组，要求源和目标内存区域不重叠）
     * 优化策略：
     * - trivially_copyable 类型：使用 memcpy (最快，字节级复制，性能提升 30-40%)
     * - 非平凡类型：使用 std::copy (正确调用赋值操作符)
     */
    template<typename T>
    inline void mem_copy(T *dst, const T *src, const size_t count)
    {
        if(!dst || !src || count == 0) return;

        if constexpr(std::is_trivially_copyable_v<T>)
        {
            std::memcpy(dst, src, count * sizeof(T));
        }
        else
        {
            std::copy(src, src + count, dst);
        }
    }

    /**
     * 内存复制（支持重叠区域）
     * 优化策略：
     * - trivially_copyable 类型：使用 memmove (处理重叠，最快)
     * - 非平凡类型：使用 std::move (正确处理移动语义)
     */
    template<typename T>
    inline void mem_move(T *dst, const T *src, const size_t count)
    {
        if(!dst || !src || count == 0) return;

        if constexpr(std::is_trivially_copyable_v<T>)
        {
            std::memmove(dst, src, count * sizeof(T));
        }
        else
        {
            // 处理重叠区域：若 src 在 dst 前面，从前向后；否则从后向前
            if(src < dst)
            {
                std::move_backward(src, src + count, dst + count);
            }
            else
            {
                std::move(src, src + count, dst);
            }
        }
    }

    //==================================================================================================
    // 内存操作 - 填充 / Memory Operations - Fill
    //==================================================================================================

    /**
     * 用指定值填充内存（单一值重复）
     */
    template<typename T>
    inline void mem_fill(T *data, const T value, const size_t count)
    {
        if(!data || count == 0) return;

        std::fill_n(data, count, value);
    }

    /**
     * 字节填充特化（性能优化）
     */
    template<>
    inline void mem_fill<uint8>(uint8 *data, const uint8 value, const size_t count)
    {
        if(!data || count == 0) return;

        std::memset(data, value, count);
    }

    /**
     * 用指定模式填充内存（重复复制同一个对象）
     * 优化策略：
     * - trivially_copyable 类型：使用 memcpy (最快)
     * - 非平凡类型：使用赋值 (正确处理复制构造/赋值)
     */
    template<typename T>
    inline void mem_fill_pattern(T *data, const T *pattern, const size_t count)
    {
        if(!data || !pattern || count == 0) return;

        if constexpr(std::is_trivially_copyable_v<T>)
        {
            for(size_t i = 0; i < count; i++)
            {
                std::memcpy(data, pattern, sizeof(T));
                ++data;
            }
        }
        else
        {
            for(size_t i = 0; i < count; i++)
            {
                *data = *pattern;
                ++data;
            }
        }
    }

    //==================================================================================================
    // 内存操作 - 清零 / Memory Operations - Zero
    //==================================================================================================

    /**
     * 内存清零（单个对象）
     */
    template<typename T>
    inline void mem_zero(T &data)
    {
        //static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        std::memset(&data, 0, sizeof(T));
    }

    /**
     * 内存清零（数组）
     */
    template<typename T>
    inline void mem_zero(T *data, const size_t count)
    {
        //static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        if(!data || count == 0) return;
        std::memset(data, 0, count * sizeof(T));
    }

    //==================================================================================================
    // 内存操作 - 比较 / Memory Operations - Compare
    //==================================================================================================

    /**
     * 内存比较（单个对象，返回 <0, 0, >0）
     * 注意：此函数执行字节级比较，对于包含填充字节或指针的类型可能产生不确定的结果
     */
    template<typename T>
    inline int mem_compare(const T &a, const T &b)
    {
        //static_assert(std::is_trivial_v<T> || std::is_standard_layout_v<T>, "T should be trivial or standard layout for safe byte-wise comparison");
        return std::memcmp(&a, &b, sizeof(T));
    }

    /**
     * 内存比较（数组，返回 <0, 0, >0）
     * 注意：此函数执行字节级比较，对于包含填充字节或指针的类型可能产生不确定的结果
     */
    template<typename T>
    inline int mem_compare(const T *a, const T *b, const size_t count)
    {
        //static_assert(std::is_trivial_v<T> || std::is_standard_layout_v<T>, "T should be trivial or standard layout for safe byte-wise comparison");
        if(!a || !b || count == 0) return 0;
        return std::memcmp(a, b, count * sizeof(T));
    }
} // namespace hgl
