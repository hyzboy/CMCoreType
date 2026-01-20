#pragma once

#include<type_traits>
#include<new>
#include<utility>

namespace hgl
{
    /**
     * 非平凡类型专用内存管理工具
     * 提供显式构造/析构、内存分配等底层操作
     *
     * 注意：这些工具函数专为手动内存管理设计，适用于 ObjectArray 等容器。
     * 对于平凡类型（trivially copyable），请使用 MemoryUtil.h 中的高性能函数。
     */

    //==================================================================================================
    // 原始内存分配 / Raw Memory Allocation
    //==================================================================================================

    /**
     * 分配原始内存（不构造对象，带对齐）
     * @param count 对象数量
     * @return 未初始化的内存指针
     */
    template<typename T>
    inline T* allocate_raw_memory(int count)
    {
        if(count <= 0) return nullptr;
        return static_cast<T*>(::operator new(count * sizeof(T), std::align_val_t{alignof(T)}));
    }

    /**
     * 释放原始内存（不调用析构）
     * @param pointer 由 allocate_raw_memory 分配的内存指针
     */
    template<typename T>
    inline void deallocate_raw_memory(T* pointer)
    {
        if(pointer) ::operator delete(pointer, std::align_val_t{alignof(T)});
    }

    //==================================================================================================
    // 构造函数显式调用 / Explicit Construction
    //==================================================================================================

    /**
     * 在指定位置默认构造对象（placement new）
     * @param destination 未初始化内存位置
     */
    template<typename T>
    inline void construct_at(T* destination)
    {
        new (static_cast<void*>(destination)) T();
    }

    /**
     * 在指定位置复制构造对象
     * @param destination 未初始化内存位置
     * @param value 要复制的对象
     */
    template<typename T>
    inline void construct_at_copy(T* destination, const T& value)
    {
        new (static_cast<void*>(destination)) T(value);
    }

    /**
     * 在指定位置移动构造对象
     * @param destination 未初始化内存位置
     * @param value 要移动的对象
     */
    template<typename T>
    inline void construct_at_move(T* destination, T&& value)
    {
        new (static_cast<void*>(destination)) T(std::move(value));
    }

    //==================================================================================================
    // 析构函数显式调用 / Explicit Destruction
    //==================================================================================================

    /**
     * 显式调用析构函数
     * @param pointer 对象指针
     */
    template<typename T>
    inline void destroy_at(T* pointer)
    {
        if(pointer) pointer->~T();
    }

    /**
     * 销毁范围内的所有对象
     * @param first 起始指针
     * @param last 结束指针（不包含）
     */
    template<typename T>
    inline void destroy_range(T* first, T* last)
    {
        if constexpr(!std::is_trivially_destructible_v<T>)
        {
            for(T* pointer = first; pointer != last; ++pointer)
                destroy_at(pointer);
        }
    }

    /**
     * 销毁指定数量的对象
     * @param data 起始指针
     * @param count 对象数量
     */
    template<typename T>
    inline void destroy_range(T* data, int count)
    {
        if(data && count > 0)
            destroy_range(data, data + count);
    }

    //==================================================================================================
    // 批量构造 / Batch Construction
    //==================================================================================================

    /**
     * 在未初始化内存中批量复制构造对象
     * @param destination 目标位置（未初始化内存）
     * @param source 源数据指针
     * @param count 对象数量
     */
    template<typename T>
    inline void copy_construct_range(T* destination, const T* source, int count)
    {
        if(!destination || !source || count <= 0) return;

        for(int i = 0; i < count; ++i)
            construct_at_copy(destination + i, source[i]);
    }

    /**
     * 在未初始化内存中批量移动构造对象
     * @param destination 目标位置（未初始化内存）
     * @param source 源数据指针
     * @param count 对象数量
     */
    template<typename T>
    inline void move_construct_range(T* destination, T* source, int count)
    {
        if(!destination || !source || count <= 0) return;

        for(int i = 0; i < count; ++i)
            construct_at_move(destination + i, std::move(source[i]));
    }

    /**
     * 在未初始化内存中批量默认构造对象
     * @param destination 目标位置（未初始化内存）
     * @param count 对象数量
     */
    template<typename T>
    inline void default_construct_range(T* destination, int count)
    {
        if(!destination || count <= 0) return;

        for(int i = 0; i < count; ++i)
            construct_at(destination + i);
    }

    //==================================================================================================
    // 内存重新分配 / Memory Reallocation
    //==================================================================================================

    /**
     * 重新分配内存并移动现有对象
     * @param old_data 旧内存指针
     * @param old_count 旧对象数量
     * @param new_capacity 新分配容量
     * @return 新内存指针
     */
    template<typename T>
    inline T* reallocate_and_move(T* old_data, int old_count, int new_capacity)
    {
        if(new_capacity <= 0) return nullptr;

        T* new_data = allocate_raw_memory<T>(new_capacity);

        if(old_data && old_count > 0)
        {
            // 移动构造到新内存
            move_construct_range(new_data, old_data, old_count);

            // 销毁旧对象
            destroy_range(old_data, old_count);

            // 释放旧内存
            deallocate_raw_memory(old_data);
        }

        return new_data;
    }

}//namespace hgl
