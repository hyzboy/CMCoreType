#pragma once

#include<hgl/CoreType.h>

namespace hgl
{
    /**
     * 星期每天枚举
     */
    enum class Weekday
    {
        Sunday=0,   ///<星期天
        Monday,     ///<星期一
        Tuesday,    ///<星期二
        Wednesday,  ///<星期三
        Thursday,   ///<星期四
        Friday,     ///<星期五
        Saturday,   ///<星期六

        BEGIN_RANGE =Sunday,
        END_RANGE   =Saturday,
        RANGE_SIZE  =END_RANGE-BEGIN_RANGE+1
    };

    /**
     * 月份枚举
     */
    enum class Month
    {
        January=1, ///<一月
        February,  ///<二月
        March,     ///<三月
        April,     ///<四月
        May,       ///<五月
        June,      ///<六月
        July,      ///<七月
        August,    ///<八月
        September, ///<九月
        October,   ///<十月
        November,  ///<十一月
        December,  ///<十二月

        BEGIN_RANGE =January,
        END_RANGE   =December,
        RANGE_SIZE  =END_RANGE-BEGIN_RANGE+1
    };//enum class Month

    // ==================== 纳秒/微秒/毫秒常量 ====================
    // (Nanosecond/Microsecond/Millisecond Constants)

    constexpr int64  HGL_NANO_SEC_PER_SEC       = 1000000000;       ///<纳秒每秒 (nanoseconds per second)
    constexpr int64  HGL_NANO_SEC_PER_MICRO     = 1000;             ///<纳秒每微秒 (nanoseconds per microsecond)
    constexpr int64  HGL_MICRO_SEC_PER_SEC      = 1000000;          ///<微秒每秒 (microseconds per second)
    constexpr int64  HGL_MILLI_SEC_PER_SEC      = 1000;             ///<毫秒每秒 (milliseconds per second)

    // ==================== 日历单位常量 ====================
    // (Calendar Unit Constants)
    // 注：这些常数在秒级时间常量中被使用

    constexpr int    HGL_HOUR_HALF_DAY          = 12;               ///<半天的小时数
    constexpr int    HGL_HOUR_ONE_DAY           = 24;               ///<一天的小时数 (hours per day)
    constexpr int    HGL_DAY_ONE_WEEK           = 7;                ///<一周的天数 (days per week)
    constexpr int    HGL_DAY_ONE_YEAR           = 365;              ///<一年的天数 (days per year, non-leap year)
    constexpr int    HGL_MONTH_ONE_YEAR         = 12;               ///<一年的月数 (months per year)

    // ==================== 秒级时间常量 ====================
    // (Second-based Time Constants)
    // 注：使用日历单位常数组合定义，保证一致性

    constexpr uint   HGL_TIME_ONE_SECOND        = 1;                ///<一秒
    constexpr uint   HGL_TIME_HALF_MINUTE       = 30;               ///<半分钟的秒数
    constexpr int    HGL_TIME_ONE_MINUTE        = 60;               ///<一分钟的秒数 (seconds per minute)
    constexpr int    HGL_TIME_HALF_HOUR         = 30 * HGL_TIME_ONE_MINUTE;     ///<半小时的秒数
    constexpr int    HGL_TIME_ONE_HOUR          = 60 * HGL_TIME_ONE_MINUTE;     ///<一小时的秒数 (seconds per hour)
    constexpr int    HGL_TIME_HALF_DAY          = HGL_HOUR_HALF_DAY * HGL_TIME_ONE_HOUR;       ///<半天的秒数
    constexpr int    HGL_TIME_ONE_DAY           = HGL_HOUR_ONE_DAY * HGL_TIME_ONE_HOUR;        ///<一天的秒数 (seconds per day)
    constexpr int    HGL_TIME_ONE_WEEK          = HGL_DAY_ONE_WEEK * HGL_TIME_ONE_DAY;         ///<一周的秒数
    constexpr int    HGL_TIME_ONE_YEAR          = HGL_DAY_ONE_YEAR * HGL_TIME_ONE_DAY;         ///<一年的秒数 (non-leap year)

    // ==================== Windows FILETIME 时间戳常量 ====================
    // (Windows FILETIME Constants)
    // Windows FILETIME: 100纳秒单位，从1601年1月1日00:00:00 UTC开始
    // Windows FILETIME: 100-nanosecond intervals since January 1, 1601 00:00:00 UTC

    constexpr int64  HGL_WIN_TICKS_PER_SEC      = 10000000;         ///<Windows时间刻度每秒 (100ns ticks per second)
    constexpr int64  HGL_WIN_TO_UNIX_EPOCH_SEC  = 11644473600LL;    ///<Windows纪元到Unix纪元的秒数差 (1601->1970)
                                                                     ///<计算方法: 369年 × 365.2425天/年 × 86400秒/天

    // ==================== UUIDv7 时间戳常量 ====================
    // (UUIDv7 Timestamp Constants)
    // UUIDv7: 毫秒时间戳，从Unix纪元开始
    // UUIDv7: millisecond timestamp since Unix epoch

    constexpr int    HGL_UUID7_TIMESTAMP_BITS   = 48;               ///<UUIDv7时间戳位数 (timestamp bits in UUIDv7)
    constexpr uint64 HGL_UUID7_TIMESTAMP_MASK   = 0x0000FFFFFFFFFFFFULL; ///<UUIDv7时间戳掩码 (48-bit mask)

}//namespace hgl
