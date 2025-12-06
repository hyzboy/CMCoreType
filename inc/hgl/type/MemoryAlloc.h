#pragma once

#include<hgl/platform/Platform.h>
#include<hgl/type/MemoryUtil.h>
#include<memory>
#include<concepts>
#include<type_traits>

namespace hgl
{
    //==================================================================================================
    // 内存分配 / Memory Allocation (C++20)
    //==================================================================================================
    
    template<typename T>
    concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

    /**
     * 分配指定类型数据并清零（需要平凡可复制类型）
     */
    template<TriviallyCopyable T>
    inline T* zero_new()
    {
        T* data = new T;
        mem_zero(*data);
        return data;
    }

    /**
     * 分配指定类型数组并清零（需要平凡可复制类型）
     */
    template<TriviallyCopyable T>
    inline T* zero_new(const size_t count)
    {
        if (count == 0) return nullptr;

        T* data = new T[count];
        mem_zero(data, count);
        return data;
    }

    /**
     * 分配并复制数组（需要平凡可复制类型）
     */
    template<TriviallyCopyable T>
    inline T* new_copy(const T* src, const size_t count)
    {
        if (!src || count == 0) return nullptr;

        T* data = new T[count];
        mem_copy(data, src, count);
        return data;
    }

    /**
     * 分配并复制单个对象（需要平凡可复制类型）
     */
    template<TriviallyCopyable T>
    inline T* new_copy(const T* src)
    {
        if (!src) return nullptr;

        T* data = new T;
        mem_copy(*data, *src);
        return data;
    }

    //==================================================================================================
    // 智能指针版本 / Smart Pointer Versions
    //==================================================================================================
    
    /**
     * 创建零初始化的 unique_ptr
     */
    template<TriviallyCopyable T>
    inline std::unique_ptr<T> make_zero_unique()
    {
        auto ptr = std::make_unique<T>();
        mem_zero(*ptr);
        return ptr;
    }

    /**
     * 创建零初始化数组的 unique_ptr
     */
    template<TriviallyCopyable T>
    inline std::unique_ptr<T[]> make_zero_unique(const size_t count)
    {
        if (count == 0) return nullptr;
        
        auto ptr = std::make_unique<T[]>(count);
        mem_zero(ptr.get(), count);
        return ptr;
    }

    /**
     * 创建复制的 unique_ptr
     */
    template<TriviallyCopyable T>
    inline std::unique_ptr<T> make_copy_unique(const T* src)
    {
        if (!src) return nullptr;
        
        auto ptr = std::make_unique<T>();
        mem_copy(*ptr, *src);
        return ptr;
    }

    /**
     * 创建复制数组的 unique_ptr
     */
    template<TriviallyCopyable T>
    inline std::unique_ptr<T[]> make_copy_unique(const T* src, const size_t count)
    {
        if (!src || count == 0) return nullptr;
        
        auto ptr = std::make_unique<T[]>(count);
        mem_copy(ptr.get(), src, count);
        return ptr;
    }

    //==================================================================================================
    // 数组内存管理 / Array Memory Management
    //==================================================================================================
    
    template<typename T>
    inline T* array_alloc(const uint count)
    {
        return static_cast<T*>(hgl_malloc(count * sizeof(T)));
    }

    template<typename T>
    inline T* array_realloc(T* origin, const uint count)
    {
        return static_cast<T*>(hgl_realloc(origin, count * sizeof(T)));
    }

    template<typename T>
    inline void array_free(T* items)
    {
        hgl_free(items);
    }

    // 向后兼容的别名
    template<TriviallyCopyable T>
    [[deprecated("Use zero_new instead")]]
    inline T* hgl_zero_new() {
        return zero_new<T>();
    }

    template<TriviallyCopyable T>
    [[deprecated("Use zero_new instead")]]
    inline T* hgl_zero_new(const size_t count) {
        return zero_new<T>(count);
    }

    template<TriviallyCopyable T>
    [[deprecated("Use new_copy instead")]]
    inline T* hgl_new_copy(const T* src, const size_t count) {
        return new_copy(src, count);
    }

    template<TriviallyCopyable T>
    [[deprecated("Use new_copy instead")]]
    inline T* hgl_new_copy(const T* src) {
        return new_copy(src);
    }
}//namespace hgl
