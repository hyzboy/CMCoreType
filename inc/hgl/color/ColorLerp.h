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
     * @brief Color interpolation utilities
     *
     * This module provides various interpolation methods for color types.
     * All functions are semantically consistent with CMMath's Lerp3D.h color functions.
     */

    //==================================================================================================
    // Linear Interpolation (existing in base classes, but also available as free functions)
    //==================================================================================================

    /**
     * @brief Linear interpolation between two Color3f values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
     * @brief Linear interpolation between two Color4f values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
     * @brief Linear interpolation between two Color3ub values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
     * @brief Linear interpolation between two Color4ub values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
    // Smooth (Cosine) Interpolation
    //==================================================================================================

    /**
     * @brief Smooth (cosine) interpolation between two Color3f values
     * Uses cosine interpolation for smoother transitions than linear
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
     */
    inline Color3f ColorLerpSmooth(const Color3f &a, const Color3f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        // Cosine interpolation: (1 - cos(t * π)) / 2
        float smooth_t = (1.0f - cosf(t * std::numbers::pi_v<float>)) * 0.5f;

        return Color3f(
            a.r + (b.r - a.r) * smooth_t,
            a.g + (b.g - a.g) * smooth_t,
            a.b + (b.b - a.b) * smooth_t
        );
    }

    /**
     * @brief Smooth (cosine) interpolation between two Color4f values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
     * @brief Smooth (cosine) interpolation between two Color3ub values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
     * @brief Smooth (cosine) interpolation between two Color4ub values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
    // Cubic Interpolation
    //==================================================================================================

    /**
     * @brief Cubic interpolation between two Color3f values
     * Uses cubic Hermite interpolation (smoothstep)
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
     */
    inline Color3f ColorLerpCubic(const Color3f &a, const Color3f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        // Cubic Hermite: 3t² - 2t³
        float cubic_t = t * t * (3.0f - 2.0f * t);

        return Color3f(
            a.r + (b.r - a.r) * cubic_t,
            a.g + (b.g - a.g) * cubic_t,
            a.b + (b.b - a.b) * cubic_t
        );
    }

    /**
     * @brief Cubic interpolation between two Color4f values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
     * @brief Cubic interpolation between two Color3ub values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
     * @brief Cubic interpolation between two Color4ub values
     * @param a Start color (t=0)
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color
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
    // Bezier Curve Interpolation (Quadratic)
    //==================================================================================================

    /**
     * @brief Quadratic Bezier curve interpolation for Color3f
     * @param a Start color (t=0)
     * @param control Control point color
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color along Bezier curve
     */
    inline Color3f ColorLerpBezier(const Color3f &a, const Color3f &control, const Color3f &b, float t)
    {
        if(t <= 0) return a;
        if(t >= 1) return b;

        // Quadratic Bezier: (1-t)²·P0 + 2(1-t)t·P1 + t²·P2
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
     * @brief Quadratic Bezier curve interpolation for Color4f
     * @param a Start color (t=0)
     * @param control Control point color
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color along Bezier curve
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
     * @brief Quadratic Bezier curve interpolation for Color3ub
     * @param a Start color (t=0)
     * @param control Control point color
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color along Bezier curve
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
     * @brief Quadratic Bezier curve interpolation for Color4ub
     * @param a Start color (t=0)
     * @param control Control point color
     * @param b End color (t=1)
     * @param t Interpolation parameter [0, 1]
     * @return Interpolated color along Bezier curve
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
