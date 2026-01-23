/**
 * PhysicsConstants.h - 物理常量
 *
 * 包含常用的物理常量和比例
 */

#pragma once

namespace hgl
{
    // ==================== 数学比例 ====================

    inline constexpr double HGL_SILVER_RATIO     = 2.4142135623730950488;                                ///< 白银比例

    // ==================== 物理常量 ====================

    inline constexpr double HGL_SPEED_OF_SOUND                     = 331.3;         ///< 音速(米/秒, 0°C, 1 atm, 干燥空气)
    inline constexpr double HGL_SPEED_OF_LIGHT                     = 299792458;     ///< 光速(米/秒)
    inline constexpr double HGL_ABSOLUTE_ZERO                      = -273.15;       ///< 绝对零度(°C)
    inline constexpr double HGL_UNIVERSAL_GRAVITATION              = 6.67430e-11;   ///< 万有引力常数(m³/(kg·s²), CODATA 2018)

    // ==================== 地球相关常量 ====================

    inline constexpr double HGL_EARTH_GRAVITATIONAL_ACCELERATION   = 9.80665;       ///< 地球上的重力加速度(米/秒²)
    inline constexpr double HGL_EARTH_MASS                         = 5.9722e+24;    ///< 地球质量
    inline constexpr double HGL_EARTH_RADIUS                       = 6371000;       ///< 地球半径(米)
}//namespace hgl
