/**
 * MathConstants.h - 基础数学常量
 * 
 * 包含：
 * - 自然常数 e 及其对数
 * - 圆周率 π 及其派生常量
 * - 根号 2 及其派生常量
 * 
 * C++20 风格：使用 std::numbers 标准库常量
 */

#pragma once

#include <numbers>

namespace hgl::math
{
    // ==================== 自然常数 e ====================
    
    inline constexpr double e           = std::numbers::e;                      ///< 欧拉数(自然对数的底数)
    inline constexpr double log2e       = std::numbers::log2e;                  ///< log₂(e)
    inline constexpr double log10e      = std::numbers::log10e;                 ///< log₁₀(e)
    inline constexpr double ln2         = std::numbers::ln2;                    ///< ln(2)
    inline constexpr double ln10        = std::numbers::ln10;                   ///< ln(10)

    // ==================== 圆周率 π ====================
   
    inline constexpr double pi          = std::numbers::pi;                     ///< 圆周率 π
    inline constexpr float  pi_f        = std::numbers::pi_v<float>;            ///< 圆周率 π (float 版本)
    inline constexpr double pi_2        = std::numbers::pi / 2.0;               ///< π/2
    inline constexpr double pi_4        = std::numbers::pi / 4.0;               ///< π/4
    inline constexpr double pi_3_4      = std::numbers::pi * 3.0 / 4.0;         ///< 3π/4
    inline constexpr double inv_pi      = std::numbers::inv_pi;                 ///< 1/π
    inline constexpr double two_over_pi = 2.0 * std::numbers::inv_pi;           ///< 2/π
    inline constexpr double two_sqrtpi  = 2.0 * std::numbers::inv_sqrtpi;       ///< 2/√π

    // ==================== 根号常量 ====================
    
    inline constexpr double sqrt2       = std::numbers::sqrt2;                  ///< √2
    inline constexpr double sqrt3       = std::numbers::sqrt3;                  ///< √3
    inline constexpr double sqrt1_2     = 1.0 / std::numbers::sqrt2;            ///< √(1/2) = 1/√2

    // ==================== 黄金比例 ====================
    
    inline constexpr double phi         = std::numbers::phi;                    ///< 黄金比例 φ = (1 + √5)/2

    // ==================== 特殊数学常数 ====================
    
    inline constexpr double euler_gamma = std::numbers::egamma;                 ///< 欧拉-马斯凯罗尼常数 γ
    inline constexpr double catalan_g   = 0.91596559417721901505460351493238411077414937428167; ///< 卡塔兰常数 G (标准库暂无)
    inline constexpr double apery_zeta3 = 1.20205690315959428539973816151144999076498629234049; ///< 阿培里常数 ζ(3) (标准库暂无)

    // ==================== 常用三角值 ====================
    
    inline constexpr double sqrt2_over_2 = std::numbers::sqrt2 / 2.0;           ///< √2/2 = sin(45°) = cos(45°)
}//namespace hgl::math
