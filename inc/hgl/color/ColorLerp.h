#pragma once

#include<hgl/color/Color3f.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color4ub.h>
#include<cmath>
#include<numbers>

namespace hgl
{
    /**
     * @brief 颜色插值工具
     *
     * 本模块提供多种颜色类型的插值方法。
     * 所有函数在语义上与 CMMath 的 Lerp3D.h 颜色函数保持一致。
     */

    //==================================================================================================
    // 线性插值（已存在于基类中，但也提供独立函数）
    //==================================================================================================

    /**
     * @brief 两个 Color3f 值之间的线性插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color3f ColorLerp(const Color3f &a, const Color3f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        return Color3f(
            a.r + (b.r - a.r) * t,
            a.g + (b.g - a.g) * t,
            a.b + (b.b - a.b) * t
        );
    }

    /**
     * @brief 两个 Color4f 值之间的线性插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color4f ColorLerp(const Color4f &a, const Color4f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        return Color4f(
            a.r + (b.r - a.r) * t,
            a.g + (b.g - a.g) * t,
            a.b + (b.b - a.b) * t,
            a.a + (b.a - a.a) * t
        );
    }

    /**
     * @brief 两个 Color3ub 值之间的线性插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color3ub ColorLerp(const Color3ub &a, const Color3ub &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        return Color3ub(
            uint8(a.r + (b.r - a.r) * t),
            uint8(a.g + (b.g - a.g) * t),
            uint8(a.b + (b.b - a.b) * t)
        );
    }

    /**
     * @brief 两个 Color4ub 值之间的线性插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color4ub ColorLerp(const Color4ub &a, const Color4ub &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        return Color4ub(
            uint8(a.r + (b.r - a.r) * t),
            uint8(a.g + (b.g - a.g) * t),
            uint8(a.b + (b.b - a.b) * t),
            uint8(a.a + (b.a - a.a) * t)
        );
    }

    //==================================================================================================
    // 平滑（余弦）插值
    //==================================================================================================

    /**
     * @brief 两个 Color3f 值之间的平滑（余弦）插值
     * 使用余弦插值实现比线性插值更平滑的过渡
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color3f ColorLerpSmooth(const Color3f &a, const Color3f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        // 余弦插值: (1 - cos(t * π)) / 2
        float smooth_t = (1.0f - cosf(t * std::numbers::pi_v<float>)) * 0.5f;

        return Color3f(
            a.r + (b.r - a.r) * smooth_t,
            a.g + (b.g - a.g) * smooth_t,
            a.b + (b.b - a.b) * smooth_t
        );
    }

    /**
     * @brief 两个 Color4f 值之间的平滑（余弦）插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color4f ColorLerpSmooth(const Color4f &a, const Color4f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float smooth_t = (1.0f - cosf(t * std::numbers::pi_v<float>)) * 0.5f;

        return Color4f(
            a.r + (b.r - a.r) * smooth_t,
            a.g + (b.g - a.g) * smooth_t,
            a.b + (b.b - a.b) * smooth_t,
            a.a + (b.a - a.a) * smooth_t
        );
    }

    /**
     * @brief 两个 Color3ub 值之间的平滑（余弦）插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color3ub ColorLerpSmooth(const Color3ub &a, const Color3ub &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float smooth_t = (1.0f - cosf(t * std::numbers::pi_v<float>)) * 0.5f;

        return Color3ub(
            uint8(a.r + (b.r - a.r) * smooth_t),
            uint8(a.g + (b.g - a.g) * smooth_t),
            uint8(a.b + (b.b - a.b) * smooth_t)
        );
    }

    /**
     * @brief 两个 Color4ub 值之间的平滑（余弦）插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color4ub ColorLerpSmooth(const Color4ub &a, const Color4ub &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float smooth_t = (1.0f - cosf(t * std::numbers::pi_v<float>)) * 0.5f;

        return Color4ub(
            uint8(a.r + (b.r - a.r) * smooth_t),
            uint8(a.g + (b.g - a.g) * smooth_t),
            uint8(a.b + (b.b - a.b) * smooth_t),
            uint8(a.a + (b.a - a.a) * smooth_t)
        );
    }

    //==================================================================================================
    // 立方插值
    //==================================================================================================

    /**
     * @brief 两个 Color3f 值之间的立方插值
     * 使用立方 Hermite 插值（smoothstep）
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color3f ColorLerpCubic(const Color3f &a, const Color3f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        // 立方 Hermite: 3t² - 2t³
        float cubic_t = t * t * (3.0f - 2.0f * t);

        return Color3f(
            a.r + (b.r - a.r) * cubic_t,
            a.g + (b.g - a.g) * cubic_t,
            a.b + (b.b - a.b) * cubic_t
        );
    }

    /**
     * @brief 两个 Color4f 值之间的立方插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color4f ColorLerpCubic(const Color4f &a, const Color4f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float cubic_t = t * t * (3.0f - 2.0f * t);

        return Color4f(
            a.r + (b.r - a.r) * cubic_t,
            a.g + (b.g - a.g) * cubic_t,
            a.b + (b.b - a.b) * cubic_t,
            a.a + (b.a - a.a) * cubic_t
        );
    }

    /**
     * @brief 两个 Color3ub 值之间的立方插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color3ub ColorLerpCubic(const Color3ub &a, const Color3ub &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float cubic_t = t * t * (3.0f - 2.0f * t);

        return Color3ub(
            uint8(a.r + (b.r - a.r) * cubic_t),
            uint8(a.g + (b.g - a.g) * cubic_t),
            uint8(a.b + (b.b - a.b) * cubic_t)
        );
    }

    /**
     * @brief 两个 Color4ub 值之间的立方插值
     * @param a 起始颜色 (t=0)
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color4ub ColorLerpCubic(const Color4ub &a, const Color4ub &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float cubic_t = t * t * (3.0f - 2.0f * t);

        return Color4ub(
            uint8(a.r + (b.r - a.r) * cubic_t),
            uint8(a.g + (b.g - a.g) * cubic_t),
            uint8(a.b + (b.b - a.b) * cubic_t),
            uint8(a.a + (b.a - a.a) * cubic_t)
        );
    }

    //==================================================================================================
    // 贝塞尔曲线插值（二次）
    //==================================================================================================

    /**
     * @brief Color3f 的二次贝塞尔曲线插值
     * @param a 起始颜色 (t=0)
     * @param control 控制点颜色
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 沿贝塞尔曲线的插值颜色
     */
    inline Color3f ColorLerpBezier(const Color3f &a, const Color3f &control, const Color3f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        // 二次贝塞尔: (1-t)²·P0 + 2(1-t)t·P1 + t²·P2
        float one_minus_t = 1.0f - t;
        float weight_a = one_minus_t * one_minus_t;
        float weight_control = 2.0f * one_minus_t * t;
        float weight_b = t * t;

        return Color3f(
            a.r * weight_a + control.r * weight_control + b.r * weight_b,
            a.g * weight_a + control.g * weight_control + b.g * weight_b,
            a.b * weight_a + control.b * weight_control + b.b * weight_b
        );
    }

    /**
     * @brief Color4f 的二次贝塞尔曲线插值
     * @param a 起始颜色 (t=0)
     * @param control 控制点颜色
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 沿贝塞尔曲线的插值颜色
     */
    inline Color4f ColorLerpBezier(const Color4f &a, const Color4f &control, const Color4f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float one_minus_t = 1.0f - t;
        float weight_a = one_minus_t * one_minus_t;
        float weight_control = 2.0f * one_minus_t * t;
        float weight_b = t * t;

        return Color4f(
            a.r * weight_a + control.r * weight_control + b.r * weight_b,
            a.g * weight_a + control.g * weight_control + b.g * weight_b,
            a.b * weight_a + control.b * weight_control + b.b * weight_b,
            a.a * weight_a + control.a * weight_control + b.a * weight_b
        );
    }

    /**
     * @brief Color3ub 的二次贝塞尔曲线插值
     * @param a 起始颜色 (t=0)
     * @param control 控制点颜色
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 沿贝塞尔曲线的插值颜色
     */
    inline Color3ub ColorLerpBezier(const Color3ub &a, const Color3ub &control, const Color3ub &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float one_minus_t = 1.0f - t;
        float weight_a = one_minus_t * one_minus_t;
        float weight_control = 2.0f * one_minus_t * t;
        float weight_b = t * t;

        return Color3ub(
            uint8(float(a.r) * weight_a + float(control.r) * weight_control + float(b.r) * weight_b),
            uint8(float(a.g) * weight_a + float(control.g) * weight_control + float(b.g) * weight_b),
            uint8(float(a.b) * weight_a + float(control.b) * weight_control + float(b.b) * weight_b)
        );
    }

    /**
     * @brief Color4ub 的二次贝塞尔曲线插值
     * @param a 起始颜色 (t=0)
     * @param control 控制点颜色
     * @param b 结束颜色 (t=1)
     * @param t 插值参数 [0, 1]
     * @return 沿贝塞尔曲线的插值颜色
     */
    inline Color4ub ColorLerpBezier(const Color4ub &a, const Color4ub &control, const Color4ub &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        float one_minus_t = 1.0f - t;
        float weight_a = one_minus_t * one_minus_t;
        float weight_control = 2.0f * one_minus_t * t;
        float weight_b = t * t;

        return Color4ub(
            uint8(float(a.r) * weight_a + float(control.r) * weight_control + float(b.r) * weight_b),
            uint8(float(a.g) * weight_a + float(control.g) * weight_control + float(b.g) * weight_b),
            uint8(float(a.b) * weight_a + float(control.b) * weight_control + float(b.b) * weight_b),
            uint8(float(a.a) * weight_a + float(control.a) * weight_control + float(b.a) * weight_b)
        );
    }
}//namespace hgl
