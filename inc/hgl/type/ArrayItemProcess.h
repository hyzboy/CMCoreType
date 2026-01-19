#pragma once

#include<hgl/CoreType.h>
#include<hgl/type/MemoryUtil.h>
#include<memory>

namespace hgl
{
    /**
     * 阵列数据项处理回调
     */
    template<typename T> struct ArrayItemProcessCallback
    {
        virtual void Equal(T &dst,const T &src)=0;                  ///<将src赋值给dst

        virtual void Equal(T *dst,const T *src,const int count)=0;  ///<批量将src赋值给dst

        virtual int  Comp(const T &a,const T &b)=0;                 ///<数据比较，数值的话，如果纯整数比较，建议返回a-b
    };

    template<typename T> struct RawTypeArrayItemProcessCallback:public ArrayItemProcessCallback<T>
    {
        void Equal(T &dst,const T &src) override {dst=src;}

        void Equal(T *dst,const T *src,const int count) override {mem_copy(dst,src,count);}

        int Comp(const T &a,const T &b) override {return a-b;}
    };

    template<typename T> struct MemoryArrayItemProcessCallback:public ArrayItemProcessCallback<T>
    {
        void Equal(T &dst,const T &src) override {mem_copy<T>(dst,src);}

        void Equal(T *dst,const T *src,const int count) override {mem_copy(dst,src,count);}

        int Comp(const T &a,const T &b) override {return memcmp(&a,&b,sizeof(T));}
    };

    /**
     * 查找数据在无序阵列中的位置
     * @return 找到返回索引位置，未找到返回-1
     * 优化：对有operator==的类型使用==比较，否则使用memcmp
     */
    template<typename T> static int FindDataPositionInArray(const T *data_list,const int64 count,const T &data)
    {
        if(!data_list)return(-1);
        if(count<=0)return(-1);

        const T *p=data_list;

        for(int64 i=0;i<count;i++)
        {
            // 使用memcmp进行字节比较（适用于POD类型）
            if(!std::memcmp(p,&data,sizeof(T)))
                return i;

            ++p;
        }

        return -1;
    }

    template<typename T,typename O> static int64 FindDataPositionInArray(const T &data_list,const O &data)
    {
        return FindDataPositionInArray(data_list.GetData(),data_list.GetCount(),data);
    }

    /**
     * 查找数据在有序阵列中的位置（标准二分查找）
     * @return 找到返回索引位置，未找到返回-1
     * 时间复杂度：O(log n)
     * 优化：移除冗余的边界检查，使用标准二分算法
     */
    template<typename T> static int64 FindDataPositionInSortedArray(const T *data_array,const int64 count,const T &flag)
    {
        if(!data_array || count<=0)return(-1);

        int64 left=0, right=count-1;

        while(left<=right)
        {
            int64 mid=(left+right)>>1;  // 避免溢出：等价于(left+right)/2

            if(data_array[mid]==flag)
                return mid;
            else if(data_array[mid]<flag)
                left=mid+1;     // 搜索右半部分
            else
                right=mid-1;    // 搜索左半部分
        }

        return(-1);
    }

    template<typename T,typename O> static int64 FindDataPositionInSortedArray(const T &data_array,const O &flag)
    {
        return FindDataPositionInSortedArray(data_array.GetData(),data_array.GetCount(),flag);
    }

    /**
    * 在已排序的阵列中查找数据的插入位置（标准lower_bound算法）
    * @param pos 返回的插入位置：如果元素存在，返回第一次出现的位置；否则返回应该插入的位置
    * @param data_array 数据阵列（必须已排序）
    * @param count 数据数量
    * @param flag 要查找的数据
    * @return true=元素已存在，false=元素不存在（pos为插入位置）
    * 时间复杂度：O(log n)
    * 优化：使用标准lower_bound算法，避免越界，逻辑更清晰
    */
    template<typename T> static bool FindInsertPositionInSortedArray(int64 *pos,const T *data_array,const int64 count,const T &flag)
    {
        if(!pos)return(false);
        if(!data_array || count<=0)
        {
            *pos=0;
            return(false);
        }

        int64 left=0, right=count;

        // 标准lower_bound二分查找：找到第一个>=flag的位置
        while(left<right)
        {
            int64 mid=(left+right)>>1;

            if(data_array[mid]<flag)
                left=mid+1;
            else
                right=mid;
        }

        *pos=left;

        // 检查找到的位置是否恰好等于flag
        if(left<count && data_array[left]==flag)
            return(true);

        return(false);
    }

    template<typename T,typename O> static bool FindInsertPositionInSortedArray(int64 *pos,const T &data_array,const O &flag)
    {
        return FindInsertPositionInSortedArray(pos,data_array.GetData(),data_array.GetCount(),flag);
    }
}//namespace hgl
