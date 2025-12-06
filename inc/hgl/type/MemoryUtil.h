#pragma once

#include<hgl/platform/Platform.h>
#include<cstring>
#include<type_traits>
#include<algorithm>

namespace hgl
{
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

    //==================================================================================================
    // 内存操作 - 复制 / Memory Operations - Copy
    //==================================================================================================
    
    /**
     * 内存复制（单个对象）
     */
    template<typename T>
    inline void mem_copy(T &dst, const T &src)
    {
        //static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        std::memcpy(&dst, &src, sizeof(T));
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
     */
    template<typename T>
    inline void mem_copy(T *dst, const T *src, const size_t count)
    {
        //static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        if(!dst || !src || count == 0) return;
        std::memcpy(dst, src, count * sizeof(T));
    }

    /**
     * 内存复制（支持重叠区域）
     */
    template<typename T>
    inline void mem_move(T *dst, const T *src, const size_t count)
    {
        //static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        if(!dst || !src || count == 0) return;
        std::memmove(dst, src, count * sizeof(T));
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
     */
    template<typename T>
    inline void mem_fill_pattern(T *data, const T *pattern, const size_t count)
    {
        //static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        if(!data || !pattern || count == 0) return;

        for(size_t i = 0; i < count; i++)
        {
            std::memcpy(data, pattern, sizeof(T));
            ++data;
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
}//namespace hgl
