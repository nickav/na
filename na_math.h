/*
    na_math.h - v0.04
    Nick Aversano's C++ math helper library

    This is a single header file with a bunch of useful math functions.
===========================================================================

USAGE
    Define this in your code:

    #include "na_math.h"

LICENSE
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

CREDITS
    Written by Nick Aversano
    Credits are much appreciated but not required.

VERSION HISTORY
    0.04  - add C support and better base types
    0.03  - added matrix functions, RGB to linear conversions, extra clamps,
            distance, height, aspect ratio helpers
    0.02  - Breaking API changes
    0.01  - Initial version
*/

#ifndef NA_MATH_H
#define NA_MATH_H

//
// TODO(nick):
// - SIMD?
//

#include <math.h>

#define PI  3.14159265359f
#define TAU 6.28318530717958647692f

#define EPSILON_F32 1.1920929e-7f
#define EPSILON_F64 2.220446e-16

#define SQRT_2 0.70710678118

#ifndef INFINITY
#define INFINITY 1e5000f
#endif

#define Mod(x, y) fmodf(x, y)
#define AbsF32(x) fabsf(x)
#define Sqrt(x) sqrtf(x)
#define Sin(x) sinf(x)
#define Cos(x) cosf(x)
#define Tan(x) tanf(x)
#define Pow(x, e) powf(x, e)

#define v2f(x, y)       v2((f32)(x), (f32)(y))
#define v3f(x, y, z)    v3((f32)(x), (f32)(y), (f32)(z))
#define v4f(x, y, z, w) v4((f32)(x), (f32)(y), (f32)(z), (f32)(w))

//
// NOTE(nick): shared imports to make this standalone
//

#define function static

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Clamp(value, lower, upper) (Max(Min(value, upper), lower))
#define ClampTop(a, b) Min(a, b)
#define ClampBot(a, b) Max(a, b)
#define Sign(x) (((x) > 0) - ((x) < 0))
#define Abs(x) (((x) < 0) ? (0u - x) : (0u + x))

#if !defined(__cplusplus) && !defined(bool)
    #define bool int
    #define true 1
    #define false 0
#endif

#ifndef U8_MAX

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef i8        b8;
typedef i16       b16;
typedef i32       b32;
typedef i64       b64;
typedef float     f32;
typedef double    f64;
typedef void VoidFunction(void);

#define U8_MAX  0xff
#define I8_MAX  0x7f
#define I8_MIN  0x80
#define U16_MAX 0xffff
#define I16_MAX 0x7fff
#define I16_MIN 0x8000
#define U32_MAX 0xffffffff
#define I32_MAX 0x7fffffff
#define I32_MIN 0x80000000
#define U64_MAX 0xffffffffffffffff
#define I64_MAX 0x7fffffffffffffff
#define I64_MIN 0x8000000000000000
#define F32_MIN 1.17549435e-38f
#define F32_MAX 3.40282347e+38f
#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623157e+308

#endif // U8_MAX

//
// Types
//

typedef union Vector2 Vector2;
union Vector2
{
    struct {
        f32 x, y;
    };

    struct {
        f32 width, height;
    };

    f32 e[2];
};

typedef union Vector2i Vector2i;
union Vector2i
{
    struct {
        i32 x, y;
    };

    struct {
        i32 width, height;
    };

    i32 e[2];
};

typedef union Vector3 Vector3;
union Vector3
{
    struct {
        f32 x, y, z;
    };

    struct {
        f32 r, g, b;
    };
    
    struct {
        f32 h, s, v;
    };

    struct {
        Vector2 xy;
        f32 __ignored1;
    };

    struct {
        f32 __ignored2;
        Vector2 yz;
    };

    f32 e[3];
};

typedef union Vector3i Vector3i;
union Vector3i
{
    struct {
        i32 x, y, z;
    };

    struct {
        i32 r, g, b;
    };

    struct {
        Vector2i xy;
        i32 __ignored1;
    };

    struct {
        i32 __ignored2;
        Vector2i yz;
    };

    i32 e[3];
};

typedef union Vector4 Vector4;
union Vector4
{
    struct {
        union {
            Vector3 xyz;
            struct {
                f32 x, y, z;
            };
        };

        f32 w;
    };

    struct {
        union {
            Vector3 rgb;
            struct {
                f32 r, g, b;
            };
        };

        f32 a;
    };

    struct {
        f32 red, green, blue, alpha;
    };

    struct {
        f32 left, top, right, bottom;
    };

    struct {
        Vector2 xy;
        f32 __ignored0;
        f32 __ignored1;
    };

    struct {
        f32 __ignored2;
        Vector2 yz;
        f32 __ignored3;
    };

    struct {
        f32 __ignored4;
        f32 __ignored5;
        Vector2 zw;
    };

    f32 e[4];
};

typedef union Vector4i Vector4i;
union Vector4i
{
    struct {
        union {
            Vector3i xyz;
            struct {
                i32 x, y, z;
            };
        };

        i32 w;
    };

    struct {
        union {
            Vector3i rgb;
            struct {
                i32 r, g, b;
            };
        };

        i32 a;
    };

    struct {
        Vector2i xy;
        i32 __ignored0;
        i32 __ignored1;
    };

    struct {
        i32 __ignored2;
        Vector2i yz;
        i32 __ignored3;
    };

    struct {
        i32 __ignored4;
        i32 __ignored5;
        Vector2i zw;
    };

    i32 e[4];
};

typedef struct Rectangle2 Rectangle2;
struct Rectangle2
{
    union {
        Vector2 p0;
        Vector2 u;
        Vector2 min;

        struct {
            f32 x0, y0;
        };
    };

    union {
        Vector2 p1;
        Vector2 v;
        Vector2 max;

        struct {
            f32 x1, y1;
        };
    };
};

typedef struct Rectangle2i Rectangle2i;
struct Rectangle2i
{
    union {
        Vector2i p0;

        struct {
            i32 x0, y0;
        };
    };

    union {
        Vector2i p1;

        struct {
            i32 x1, y1;
        };
    };
};

typedef struct Rectangle3 Rectangle3;
struct Rectangle3
{
    union {
        Vector3 p0;

        struct {
            f32 x0, y0, z0;
        };
    };

    union {
        Vector3 p1;

        struct {
            f32 x1, y1, z1;
        };
    };
};

typedef union Matrix3 Matrix3;
union Matrix3
{
    Vector3 rows[3];

    // NOTE(nick): These are stored row major - e[row][coulumn]
    f32 e[3][3];
};

typedef union Matrix4 Matrix4;
union Matrix4
{
    Vector4 rows[4];

    // NOTE(nick): These are stored row major - e[row][coulumn]
    f32 e[4][4];
};

typedef union Quaternion Quaternion;
union Quaternion
{
    struct {
        f32 x, y, z, w;
    };

    f32 e[4];
};

//
// Constants
//

const Vector2 v2_zero = {0, 0};
const Vector2 v2_one  = {1, 1};
const Vector2 v2_half = {0.5, 0.5};
const Vector2 v2_center = {0.5, 0.5};

const Vector3 v3_zero = {0, 0, 0};
const Vector3 v3_one  = {1, 1, 1};
const Vector3 v3_half = {0.5, 0.5, 0.5};
const Vector3 v3_center = {0.5, 0.5, 0.5};

const Vector4 v4_zero = {0, 0, 0, 0};
const Vector4 v4_one  = {1, 1, 1, 1};
const Vector4 v4_half = {0.5, 0.5, 0.5, 0.5};
const Vector4 v4_center = {0.5, 0.5, 0.5, 0.5};

const Vector4 v4_black   = {0, 0, 0, 1};
const Vector4 v4_white   = {1, 1, 1, 1};
const Vector4 v4_red     = {1, 0, 0, 1};
const Vector4 v4_green   = {0, 1, 0, 1};
const Vector4 v4_blue    = {0, 0, 1, 1};
const Vector4 v4_cyan    = {0, 1, 1, 1};
const Vector4 v4_magenta = {1, 0, 1, 1};
const Vector4 v4_yellow  = {1, 1, 0, 1};

const Rectangle2 r2_zero = {0, 0, 0, 0};
const Rectangle2 r2_one  = {1, 1, 1, 1};

//
// Basic Functions
//

function i32 min_i32(i32 a, i32 b) { return Min(a, b); }
function u32 min_u32(u32 a, u32 b) { return Min(a, b); }
function i64 min_i64(i64 a, i64 b) { return Min(a, b); }
function u64 min_u64(u64 a, u64 b) { return Min(a, b); }
function f32 min_f32(f32 a, f32 b) { return Min(a, b); }
function f64 min_f64(f64 a, f64 b) { return Min(a, b); }

function i32 max_i32(i32 a, i32 b) { return Max(a, b); }
function u32 max_u32(u32 a, u32 b) { return Max(a, b); }
function i64 max_i64(i64 a, i64 b) { return Max(a, b); }
function u64 max_u64(u64 a, u64 b) { return Max(a, b); }
function f32 max_f32(f32 a, f32 b) { return Max(a, b); }
function f64 max_f64(f64 a, f64 b) { return Max(a, b); }

function f32 min3_f32(f32 a, f32 b, f32 c) { return Min(Min(a, b), c); }
function f32 min3_f64(f64 a, f64 b, f64 c) { return Min(Min(a, b), c); }
function f32 max3_f32(f32 a, f32 b, f32 c) { return Max(Max(a, b), c); }
function f32 max3_f64(f64 a, f64 b, f64 c) { return Max(Max(a, b), c); }
function f32 min4_f32(f32 a, f32 b, f32 c, f32 d) { return Min(Min(a, b), Min(c, d)); }
function f32 min4_f64(f64 a, f64 b, f64 c, f32 d) { return Min(Min(a, b), Min(c, c)); }
function f32 max4_f32(f32 a, f32 b, f32 c, f32 d) { return Max(Max(a, b), Max(c, d)); }
function f32 max4_f64(f64 a, f64 b, f64 c, f32 d) { return Max(Max(a, b), Max(c, d)); }

function i32 clamp_i32(i32 value, i32 lower, i32 upper) { return Clamp(value, lower, upper); }
function u32 clamp_u32(u32 value, u32 lower, u32 upper) { return Clamp(value, lower, upper); }
function u64 clamp_u64(u64 value, u64 lower, u64 upper) { return Clamp(value, lower, upper); } 
function f32 clamp_f32(f32 value, f32 lower, f32 upper) { return Clamp(value, lower, upper); }
function f64 clamp_f64(f64 value, f64 lower, f64 upper) { return Clamp(value, lower, upper); }

function f32 clamp_top_f32(f32 value, f32 upper) { return ClampTop(value, upper); }
function f32 clamp_bot_f32(f32 value, f32 lower) { return ClampBot(value, lower); }
function f32 clamp_01_f32(f32 value) { return clamp_f32(value, 0, 1); }
function f32 clamp_01_f64(f64 value) { return clamp_f64(value, 0, 1); }

function f32 wrap_f32(f32 value, f32 lower, f32 upper) {
    f32 range = upper - lower;
    while (value > upper) value -= range;
    while (value < lower) value += range;
    return value;
}

function i32 sign_i32(i32 a) { return Sign(a); }
function i32 sign_i64(i64 a) { return Sign(a); }
function f32 sign_f32(f32 a) { return (f32)Sign(a); }
function f64 sign_f64(f64 a) { return Sign(a); }

function i32 abs_i32(i32 a) { return Abs(a); }
function i32 abs_i64(i64 a) { return (i32)Abs(a); }
function f32 abs_f32(f32 x) {
    union {
        f32 f;
        u32 i;
    } u = {x};
    u.i &= 0x7fffffff;
    return u.f;
}
function f64 abs_f64(f64 x) {
    union {
        f64 f;
        u64 i;
    } u = {x};
    u.i &= 0x7fffffffffffffff;
    return u.f;
}

function i32 round_i32(f32 x) {
    return (x >= 0) ? (i32)(x + 0.5) : (i32)(x - 0.5);
}

function i64 round_i64(f64 x) {
    return (x >= 0) ? (i64)(x + 0.5) : (i64)(x - 0.5);
}

function f32 round_f32(f32 x) { return (f32)round_i32(x); }
function f64 round_f64(f64 x) { return (f64)round_i64(x); }

function i32 floor_i32(f32 x) {
    return (i32)x - (x < 0);
}

function i64 floor_i64(f64 x) {
    return (i64)x - (x < 0);
}

function f32 floor_f32(f32 x) { return (f32)floor_i32(x); }
function f64 floor_f64(f64 x) { return (f64)floor_i64(x); }

function i32 ceil_i32(f32 x) {
    return (i32)(x + 1 - EPSILON_F32);
}

function i64 ceil_i64(f64 x) {
    return (i64)(x + 1 - EPSILON_F64);
}

function f32 ceil_f32(f32 x) { return (f32)ceil_i32(x); }
function f64 ceil_f64(f64 x) { return (f64)ceil_i64(x); }

function f32 mod_f32(f32 x, f32 y) { return (f32)Mod(x, y); }
function f64 mod_f64(f64 x, f64 y) { return (f64)Mod(x, y); }

function b32 f32_is_zero(f32 x) {
    return abs_f32(x) <= EPSILON_F32;
}

//
// Integers
//

// -fsanitize=signed-integer-overflow in clang

// -fwrapv in gcc and clang
//   > Treat signed integer overflow as two’s complement


#if 0
int add_safe(int a, int b)
{
    if (a > 0 && b > INT_MAX - a) { ERROR: overflow happens }
    else if (a < 0 && b < INT_MIN - a) { ERROR: underflow happens }
    return a + b; // now it is OK
}
#endif

// C specifies signed integer representation as using 1 of 3 forms: sign and magnitude, two’s complement, or ones’ complement.
// Given these forms, only division by 0 and two’s complement division of INT_MIN/-1 may overflow.

#if C_VERSION_23
#include <stdckdint.h>
#endif

#if defined(__has_builtin) && __has_builtin(__builtin_add_overflow)
    #define __checked_add_overflow(a, b, r) __builtin_add_overflow(a, b, r)
    #define __checked_sub_overflow(a, b, r) __builtin_sub_overflow(a, b, r)
    #define __checked_mul_overflow(a, b, r) __builtin_mul_overflow(a, b, r)
#elif C_VERSION_23
    #define __checked_add_overflow(r, a, b) ckd_add(r, a, b)
    #define __checked_sub_overflow(r, a, b) ckd_sub(r, a, b)
    #define __checked_mul_overflow(r, a, b) ckd_mul(r, a, b)
#endif


function bool checked_add_i16(i16 a, i16 b, i16 *result) {
#ifdef __checked_add_overflow
    return __checked_add_overflow(a, b, result);
#else
    *result = (i16)((u16)a + (u16)b);
    return (b < 0) ? (a < I16_MIN - b) : (a > I16_MAX - b);
#endif
}

function bool checked_add_i32(i32 a, i32 b, i32 *result) {
#ifdef __checked_add_overflow
    return __checked_add_overflow(a, b, result);
#else
    *result = (i32)((u32)a + (u32)b);
    return (b < 0) ? (a < I32_MIN - b) : (a > I32_MAX - b);
#endif
}

function bool checked_add_i64(i64 a, i64 b, i64 *result) {
#ifdef __checked_add_overflow
    return __checked_add_overflow(a, b, result);
#else
    *result = (i64)((u64)a + (u64)b);
    return (b < 0) ? (a < I64_MIN - b) : (a > I64_MAX - b);
#endif
}

function bool checked_add_u16(u16 a, u16 b, u16 *result) {
#ifdef __checked_add_overflow
    return __checked_add_overflow(a, b, result);
#else
    u16 r = a + b;
    *result = r;
    return r < a;
#endif
}

function bool checked_add_u32(u32 a, u32 b, u32 *result) {
#ifdef __checked_add_overflow
    return __checked_add_overflow(a, b, result);
#else
    u32 r = a + b;
    *result = r;
    return r < a;
#endif
}

function bool checked_add_u64(u64 a, u64 b, u64 *result) {
#ifdef __checked_add_overflow
    return __checked_add_overflow(a, b, result);
#else
    u64 r = a + b;
    *result = r;
    return r < a;
#endif
}

// Saturating addition - limits result between [I32_MIN, I32_MAX]
function i32 saturated_add_i32(i32 a, i32 b) {
#ifdef __checked_add_overflow
    i32 res;
    if (!__checked_add_overflow(a, b, &res)) return res;
    return res >= 0 ? I32_MIN : I32_MAX;
#else
    i64 r = (i64)a + (i64)b;
    return r < I32_MIN ? I32_MIN : r > I32_MAX ? I32_MAX : (i32)r;
#endif
}

function i16 add_i16(i16 a, i16 b) {
    i16 result = 0;
    checked_add_i16(a, b, &result);
    return result;
}

function i32 add_i32(i32 a, i32 b) {
    i32 result = 0;
    checked_add_i32(a, b, &result);
    return result;
}

function i64 add_i64(i64 a, i64 b) {
    i64 result = 0;
    checked_add_i64(a, b, &result);
    return result;
}

// TODO(nick): subtraction
// TODO(nick): multiplication
// TODO(nick): division

function bool checked_div_i32(i32 a, i32 b, i32 *result)
{
    if (b == 0) { return 1; }

    #if INT_MIN != -INT_MAX
    if (a == I32_MIN && b == -1) { return 1; }
    #endif

    *result = a / b;
    return 0;
}

function bool checked_div_i64(i64 a, i64 b, i64 *result)
{
    if (b == 0) { return 1; }
    if (a == I64_MIN && b == -1) { return 1; }
    
    *result = a / b;
    return 0;
}

function i32 div_i32(i32 a, i32 b)
{
    i32 result = 0;
    checked_div_i32(a, b, &result);
    return result;
}

function i32 div_i64(i64 a, i64 b)
{
    i64 result = 0;
    checked_div_i64(a, b, &result);
    return result;
}

function i16 truncate_i32_to_i16(i32 x)
{
    assert(x <= I16_MAX);
    i16 result = (i16)x;
    return result;
}

function u8 truncate_u64_to_u8(u64 x)
{
    assert(x <= U8_MAX);
    u8 result = (u8)x;
    return result;
}

function u16 truncate_u64_to_u16(u64 x)
{
    assert(x <= U16_MAX);
    u16 result = (u16)x;
    return result;
}

function u32 truncate_u64_to_u32(u64 x)
{
    assert(x <= U32_MAX);
    u32 result = (u32)x;
    return result;
}

//
// Floats
//

// x^(-1/2) with monic 2nd iteration, error = 4.639856 * 10^(-7)
// TODO(nick): test against hardware sqrt?
function f32 fast_sqrt_f32(f32 x)
{
    u32 X = *(u32 *)&x;
    u32 Y = 0x5F5FFF00 - (X >> 1);
    f32 y = *(f32 *)&Y;
    y *= 0.9439607f - x*y*y*0.19755164f;
    y *= 1.8898820f - x*y*y;
    return y;
}

function f32 sqrt_f32(f32 x)
{
    return (f32)Sqrt(x);
}

// fast sin function; maximum error is 0.001
function f32 sin_f32(f32 x) {
    x = x * (1 / PI);
    i32 k = (i32)round_i32(x);
    x = x - k;

    f32 y = (4 - 4 * abs_f32(x)) * x;

    y = 0.225f * (y * abs_f32(y) - y) + y;

    return (k & 1) ? -y : y;
}

function f32 cos_f32(f32 x) {
    return sin_f32((PI / 2) - x);
}

function f32 tan_f32(f32 x) {
    return sin_f32(x) / cos_f32(x);
}

function f32 atan2_f32(f32 y, f32 x) {
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
    // Volkan SALMA

    f32 r, angle;
    f32 abs_y = abs_f32(y) + 1e-10f; // kludge to prevent 0/0 condition

    if (x < 0.0f) {
        r = (x + abs_y) / (abs_y - x);
        angle = 0.75 * PI;
    } else {
        r = (x - abs_y) / (x + abs_y);
        angle = 0.25 * PI;
    }

    angle += (0.1963f * r * r - 0.9817f) * r;

    return y < 0 ? -angle : angle;
}

function f32 acos_f32(f32 x) {
    f32 negate = (f32)(x < 0);
    x = AbsF32(x);
    f32 ret = -0.0187293f;
    ret = ret * x;
    ret = ret + 0.0742610f;
    ret = ret * x;
    ret = ret - 0.2121144f;
    ret = ret * x;
    ret = ret + 1.5707288f;
    ret = ret * Sqrt(1.0f - x);
    ret = ret - 2 * negate * ret;
    return negate * PI + ret;
}

function f32 lerp_f32(f32 a, f32 b, f32 t) {
    return (1 - t) * a + b * t;
}

function f32 unlerp_f32(f32 a, f32 b, f32 v) {
    return (v - a) / (b - a);
}

function f32 remap_f32(f32 in_min, f32 in_max, f32 out_min, f32 out_max, f32 v)
{
    f32 t = unlerp_f32(in_min, in_max, v);
    return lerp_f32(out_min, out_max, t);
}

function f32 move_f32(f32 a, f32 b, f32 amount) {
    return (a > b) ? max_f32(a - amount, b) : min_f32(a + amount, b);
}

function f32 approach_f32(f32 a, f32 b, f32 inc, f32 dec) {
    return (a > b) ? max_f32(a - dec, b) : min_f32(a + inc, b);
}

function f32 snap_f32(f32 value, f32 grid) {
    return round_f32(value / grid) * grid;
}

function f32 degrees_to_radians(f32 degrees)
{
    return degrees * (PI / 180.0f);
}

function f32 radians_to_degrees(f32 radians)
{
    return radians * (180.0f / PI);
}

function f32 angle_between(Vector2 p0, Vector2 p1) {
    return atan2(p1.y - p0.y, p1.x - p0.x);
}

function f32 direction(Vector2 p0) {
    return -atan2(-p0.y, p0.x);
}

//
// Vector2
//

function Vector2 v2(f32 x, f32 y) {
    Vector2 result = {x, y};
    return result;
}

function Vector2 v2_from_v2i(Vector2i v) {
    Vector2 result = {(f32)v.x, (f32)v.y};
    return result;
}

function b32 v2_equals(Vector2 a, Vector2 b) {
    return abs_f32(a.x - b.x) <= EPSILON_F32 && abs_f32(a.y - b.y) <= EPSILON_F32;
}

function b32 v2_is_zero(Vector2 a) {
    return v2_equals(a, v2_zero);
}

function Vector2 v2_add(Vector2 a, Vector2 b) {
    Vector2 result = {a.x + b.x, a.y + b.y};
    return result;
}

function Vector2 v2_sub(Vector2 a, Vector2 b) {
    Vector2 result = {a.x - b.x, a.y - b.y};
    return result;
}

function Vector2 v2_inv(Vector2 a) {
    Vector2 result = {-a.x, -a.y};
    return result;
}

function Vector2 v2_mul(Vector2 a, Vector2 b) {
    Vector2 result = {a.x * b.x, a.y * b.y};
    return result;
}

function Vector2 v2_mulf(Vector2 a, f32 s) {
    Vector2 result = {a.x * s, a.y * s};
    return result;
}

function Vector2 v2_divf(Vector2 a, f32 s) {
    Vector2 result = {a.x / s, a.y / s};
    return result;
}

function Vector2 v2_div(Vector2 a, Vector2 b) {
    Vector2 result = {
        a.x / b.x,
        a.y / b.y,
    };
    return result;
}

function f32 v2_dot(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

function Vector2 v2_perp(Vector2 a) {
    Vector2 result = {-a.y, a.x};
    return result;
}

function f32 v2_length_squared(Vector2 a) {
    return v2_dot(a, a);
}

function f32 v2_length(Vector2 a) {
    return Sqrt(v2_length_squared(a));
}

function f32 v2_distance_squared(Vector2 a, Vector2 b) {
    return v2_length_squared(v2_sub(a, b));
}

function f32 v2_distance(Vector2 a, Vector2 b) {
    return v2_length(v2_sub(a, b));
}

function Vector2 v2_normalize(Vector2 a) {
    Vector2 result = {0};
    f32 len = v2_length(a);
    if (len != 0.0f)
    {
        result.x = a.x * (1.0f / len);
        result.y = a.y * (1.0f / len);
    }
    return result;
}

function Vector2 v2_arm(f32 angle) {
    Vector2 result = {Cos(angle), Sin(angle)};
    return result;
}

function f32 v2_triangle_area(Vector2 a, Vector2 b, Vector2 c)
{
    f32 abx = b.x - a.x;
    f32 aby = b.y - a.y;
    f32 acx = c.x - a.x;
    f32 acy = c.y - a.y;
    return (acx*aby - abx*acy) * 0.5;
}

// Functions:

function Vector2 lerp_v2(Vector2 a, Vector2 b, f32 t) {
    Vector2 result = {
        lerp_f32(a.x, b.x, t),
        lerp_f32(a.y, b.y, t),
    };
    return result;
}

function Vector2 min_v2(Vector2 a, Vector2 b) {
    Vector2 result = {
        min_f32(a.x, b.x),
        min_f32(a.y, b.y),
    };
    return result;
}

function Vector2 max_v2(Vector2 a, Vector2 b) {
    Vector2 result = {max_f32(a.x, b.x), max_f32(a.y, b.y)};
    return result;
}

function Vector2 abs_v2(Vector2 a) {
    Vector2 result = {abs_f32(a.x), abs_f32(a.y)};
    return result;
}

function Vector2 floor_v2(Vector2 a) {
    Vector2 result = {floor_f32(a.x), floor_f32(a.y)};
    return result;
}

function Vector2 round_v2(Vector2 a) {
    Vector2 result = {round_f32(a.x), round_f32(a.y)};
    return result;
}

function Vector2 ceil_v2(Vector2 a) {
    Vector2 result = {ceil_f32(a.x), ceil_f32(a.y)};
    return result;
}

function Vector2 sign_v2(Vector2 a) {
    Vector2 result = {(f32)Sign(a.x), (f32)Sign(a.y)};
    return result;
}

function Vector2 move_v2(Vector2 a, Vector2 b, f32 amount) {
    Vector2 result = {move_f32(a.x, b.x, amount), move_f32(a.y, b.y, amount)};
    return result;
}

function Vector2 approach_v2(Vector2 a, Vector2 b, f32 inc, f32 dec) {
    Vector2 result = {approach_f32(a.x, b.x, inc, dec), approach_f32(a.y, b.y, inc, dec)};
    return result;
}

function Vector2 snap_v2(Vector2 pos, Vector2 grid) {
    Vector2 result = {snap_f32(pos.x, grid.x), snap_f32(pos.y, grid.y)};
    return result;
}

function Vector2 clamp_v2(Vector2 a, Vector2 min, Vector2 max) {
    Vector2 result = {clamp_f32(a.x, min.x, max.x), clamp_f32(a.y, min.y, max.y)};
    return result;
}

#if LANG_CPP

function Vector2 operator+(Vector2 a, Vector2 b) {
    return v2_add(a, b);
}

function Vector2 operator-(Vector2 a) {
    return v2_inv(a);
}

function Vector2 &operator+=(Vector2 &a, Vector2 b) {
    return a = a + b;
}

function Vector2 operator-(Vector2 a, Vector2 b) {
    return v2_sub(a, b);
}

function Vector2 &operator-=(Vector2 &a, Vector2 b) {
    return a = a - b;
}

function Vector2 operator*(f32 a, Vector2 b) {
    return v2_mulf(b, a);
}

function Vector2 operator*(Vector2 a, f32 b) {
    return b * a;
}

function Vector2 operator*(Vector2 a, Vector2 b) {
    return v2_mul(a, b);
}

function Vector2 &operator*=(Vector2 &a, f32 b) {
    return a = a * b;
}

function Vector2 &operator*=(Vector2 &a, Vector2 b) {
    return a = a * b;
}

function Vector2 operator/(Vector2 a, f32 b) {
    return v2_divf(a, b);
}

function Vector2 operator/(f32 a, Vector2 b) {
    return {a / b.x, a / b.y};
}

function Vector2 operator/(Vector2 a, Vector2 b) {
    return v2_div(a, b);
}

function Vector2 &operator/=(Vector2 &a, Vector2 b) {
    return a = a / b;
}

function Vector2 &operator/=(Vector2 &a, f32 b) {
    return a = a / b;
}

#endif // LANG_CPP

//
// Vector2i
//

function Vector2i v2i(i32 x, i32 y) {
    Vector2i result = {x, y};
    return result;
}

function Vector2i v2i_from_v2(Vector2 v) {
    Vector2i result = {(i32)v.x, (i32)v.y};
    return result;
}

function b32 v2i_equals(Vector2i a, Vector2i b) {
    return a.x == b.x && a.y == b.y;
}

function b32 v2i_is_zero(Vector2i a) {
    return a.x == 0 && a.y == 0;
}

function Vector2i v2i_add(Vector2i a, Vector2i b) {
    Vector2i result = {a.x + b.x, a.y + b.y};
    return result;
}

function Vector2i v2i_sub(Vector2i a, Vector2i b) {
    Vector2i result = {a.x - b.x, a.y - b.y};
    return result;
}

function Vector2i v2i_inv(Vector2i a) {
    Vector2i result = {-a.x, -a.y};
    return result;
}

function Vector2i v2i_mulf(Vector2i a, f32 b) {
    Vector2i result = {(i32)(a.x * b), (i32)(a.y * b)};
    return result;
}

function Vector2i v2i_muli(Vector2i a, i32 b) {
    Vector2i result = {(a.x * b), (a.y * b)};
    return result;
}

function Vector2i v2i_mul(Vector2i a, Vector2i b) {
    Vector2i result = {(a.x * b.x), (a.y * b.y)};
    return result;
}

#if LANG_CPP

function Vector2i operator+(Vector2i a, Vector2i b) {
    return v2i_add(a, b);
}

function Vector2i operator-(Vector2i a, Vector2i b) {
    return v2i_sub(a, b);
}

function Vector2i &operator+=(Vector2i &a, Vector2i b) {
    return a = a + b;
}

function Vector2i operator*(Vector2i a, f32 b) {
    return v2i_mulf(a, b);
}

function Vector2i &operator*=(Vector2i &a, f32 b) {
    return a = a * b;
}

function Vector2i operator*(Vector2i a, Vector2i b) {
    return v2i_mul(a, b);
}

#endif // LANG_CPP

//
// Vector3
//

function Vector3 v3(f32 x, f32 y, f32 z) {
    Vector3 result = {x, y, z};
    return result;
}

function Vector3 v3_from_v3i(Vector3i v) {
    Vector3 result = {(f32)v.x, (f32)v.y, (f32)v.z};
    return result;
}

function Vector3 v3_from_v2f(Vector2 xy, f32 z) {
    Vector3 result = {(f32)xy.x, (f32)xy.y, (f32)z};
    return result;
}

function b32 v3_equals(Vector3 a, Vector3 b) {
    return (
        abs_f32(a.x - b.x) <= EPSILON_F32 &&
        abs_f32(a.y - b.y) <= EPSILON_F32 &&
        abs_f32(a.z - b.z) <= EPSILON_F32
    );
}

function b32 v3_is_zero(Vector3 a) {
    return v3_equals(a, v3_zero);
}

function Vector3 v3_add(Vector3 a, Vector3 b) {
    Vector3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

function Vector3 v3_sub(Vector3 a, Vector3 b) {
    Vector3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

function Vector3 v3_inv(Vector3 a) {
    Vector3 result = {-a.x, -a.y, -a.z};
    return result;
}

function Vector3 v3_mul(Vector3 a, Vector3 b) {
    Vector3 result = {a.x * b.x, a.y * b.y, a.z * b.z};
    return result;
}

function Vector3 v3_mulf(Vector3 a, f32 s) {
    Vector3 result = {a.x * s, a.y * s, a.z * s};
    return result;
}

function Vector3 v3_divf(Vector3 a, f32 s) {
    Vector3 result = {a.x / s, a.y / s, a.z / s};
    return result;
}

function Vector3 v3_div(Vector3 a, Vector3 b) {
    Vector3 result = {
        a.x / b.x,
        a.y / b.y,
        a.z / b.z,
    };
    return result;
}

function f32 v3_dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

function f32 v3_length_squared(Vector3 a) {
    return v3_dot(a, a);
}

function f32 v3_length(Vector3 a) {
    return Sqrt(v3_length_squared(a));
}

function f32 v3_distance_squared(Vector3 a, Vector3 b) {
    return v3_length_squared(v3_sub(a, b));
}

function f32 v3_distance(Vector3 a, Vector3 b) {
    return v3_length(v3_sub(a, b));
}

function Vector3 v3_normalize(Vector3 a) {
    Vector3 result = {0};
    f32 len = v3_length(a);
    if (len != 0.0f)
    {
        result.x = a.x * (1.0f / len);
        result.y = a.y * (1.0f / len);
        result.z = a.z * (1.0f / len);
    }
    return result;
}

function Vector3 v3_cross(Vector3 a, Vector3 b) {
    Vector3 result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
    return result;
}

// Functions:

function Vector3 lerp_v3(Vector3 a, Vector3 b, f32 t) {
    Vector3 result = {
        lerp_f32(a.x, b.x, t),
        lerp_f32(a.y, b.y, t),
        lerp_f32(a.z, b.z, t),
    };
    return result;
}

function Vector3 clamp_v3(Vector3 value, Vector3 lower, Vector3 upper) {
    Vector3 result = {
        clamp_f32(value.x, lower.x, upper.x),
        clamp_f32(value.y, lower.y, upper.y),
        clamp_f32(value.z, lower.z, upper.z),
    };
    return result;
}

function Vector3 min_v3(Vector3 a, Vector3 b) {
    Vector3 result = {
        min_f32(a.x, b.x),
        min_f32(a.y, b.y),
        min_f32(a.z, b.z),
    };
    return result;
}

function Vector3 max_v3(Vector3 a, Vector3 b) {
    Vector3 result = {
        max_f32(a.x, b.x),
        max_f32(a.y, b.y),
        max_f32(a.z, b.z),
    };
    return result;
}

function Vector3 sign_v3(Vector3 a) {
    Vector3 result = {
        sign_f32(a.x),
        sign_f32(a.y),
        sign_f32(a.z),
    };
    return result;
}

function Vector3 abs_v3(Vector3 a) {
    Vector3 result = {
        abs_f32(a.x),
        abs_f32(a.y),
        abs_f32(a.z),
    };
    return result;
}

function Vector3 round_v3(Vector3 a) {
    Vector3 result = {
        round_f32(a.x),
        round_f32(a.y),
        round_f32(a.z),
    };
    return result;
}

function Vector3 floor_v3(Vector3 a) {
    Vector3 result = {
        floor_f32(a.x),
        floor_f32(a.y),
        floor_f32(a.z),
    };
    return result;
}

function Vector3 ceil_v3(Vector3 a) {
    Vector3 result = {
        ceil_f32(a.x),
        ceil_f32(a.y),
        ceil_f32(a.z),
    };
    return result;
}

#if LANG_CPP

function Vector3 operator+(Vector3 a, Vector3 b) {
    return v3_add(a, b);
}

function Vector3 &operator+=(Vector3 &a, Vector3 b) {
    return a = a + b;
}

function Vector3 operator-(Vector3 a, Vector3 b) {
    return v3_sub(a, b);
}

function Vector3 &operator-=(Vector3 &a, Vector3 b) {
    return a = a - b;
}

function Vector3 operator-(Vector3 a) {
    return v3_inv(a);
}

function Vector3 operator*(f32 a, Vector3 b) {
    return v3_mulf(b, a);
}

function Vector3 operator*(Vector3 a, f32 b) {
    return b * a;
}

function Vector3 &operator*=(Vector3 &a, f32 b) {
    return a = a * b;
}

function Vector3 operator*(Vector3 a, Vector3 b) {
    return v3_mul(a, b);
}

function Vector3 operator/(Vector3 a, f32 b) {
    return v3_divf(a, b);
}

function Vector3 &operator/=(Vector3 &a, f32 b) {
    return a = a / b;
}

function Vector3 operator/(Vector3 a, Vector3 b) {
    return v3_div(a, b);
}

#endif // LANG_CPP

//
// Vector3i
//

function Vector3i v3i(i32 x, i32 y, i32 z) {
    Vector3i result = {x, y, z};
    return result;
}

function Vector3i v3i_from_v3(Vector3 v) {
    Vector3i result = {(i32)v.x, (i32)v.y, (i32)v.z};
    return result;
}

function b32 v3i_equals(Vector3i a, Vector3i b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

function b32 v3i_is_zero(Vector3i a) {
    return a.x == 0 && a.y == 0 && a.z == 0;
}

function Vector3i v3i_add(Vector3i a, Vector3i b) {
    Vector3i result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

function Vector3i v3i_sub(Vector3i a, Vector3i b) {
    Vector3i result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

#if LANG_CPP

function Vector3i operator+(Vector3i a, Vector3i b) {
    return v3i_add(a, b);
}

function Vector3i &operator+=(Vector3i &a, Vector3i b) {
    return a = a + b;
}

function Vector3i operator-(Vector3i a, Vector3i b) {
    return v3i_sub(a, b);
}

function Vector3i &operator-=(Vector3i &a, Vector3i b) {
    return a = a - b;
}

#endif // LANG_CPP

//
// Vector4
//

function Vector4 v4(f32 x, f32 y, f32 z, f32 w) {
    Vector4 result = {x, y, z, w};
    return result;
}

function Vector4 v4_from_v2(Vector2 a, Vector2 b) {
    Vector4 result = {a.x, a.y, b.x, b.y};
    return result;
}

function Vector4 v4_from_v3f(Vector3 v, f32 w) {
    Vector4 result = {v.x, v.y, v.z, w};
    return result;
}

function Vector4 rgba(f32 r, f32 g, f32 b, f32 a) {
    assert(r >= 0 && r <= 255);
    assert(b >= 0 && b <= 255);
    assert(g >= 0 && g <= 255);
    assert(a >= 0 && a <= 1);

    Vector4 result = {r/255.0f, g/255.0f, b/255.0f, a};
    return result;
}

function Vector4 rgb(f32 r, f32 g, f32 b) {
    return rgba(r, g, b, 1);
}

function b32 v4_equals(Vector4 a, Vector4 b) {
    return (
        abs_f32(a.x - b.x) <= EPSILON_F32 &&
        abs_f32(a.y - b.y) <= EPSILON_F32 &&
        abs_f32(a.z - b.z) <= EPSILON_F32 &&
        abs_f32(a.w - b.w) <= EPSILON_F32
    );
}

function b32 v4_is_zero(Vector4 a) {
    return v4_equals(a, v4_zero);
}

function Vector4 v4_add(Vector4 a, Vector4 b) {
    Vector4 result = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return result;
}

function Vector4 v4_sub(Vector4 a, Vector4 b) {
    Vector4 result = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return result;
}

function Vector4 v4_inv(Vector4 a) {
    Vector4 result = {-a.x, -a.y, -a.z, -a.w};
    return result;
}

function Vector4 v4_mul(Vector4 a, Vector4 b) {
    Vector4 result = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    return result;
}

function Vector4 v4_mulf(Vector4 a, f32 f) {
    Vector4 result = {a.x * f, a.y * f, a.z * f, a.w * f};
    return result;
}

function Vector4 v4_div(Vector4 a, Vector4 b) {
    Vector4 result = {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
    return result;
}

function Vector4 v4_divf(Vector4 a, f32 f) {
    Vector4 result = {a.x / f, a.y / f, a.z / f, a.w / f};
    return result;
}

function f32 v4_dot(Vector4 a, Vector4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

function f32 v4_length_squared(Vector4 a) {
    return v4_dot(a, a);
}

function f32 v4_length(Vector4 a) {
    return Sqrt(v4_length_squared(a));
}

function Vector4 v4_normalize(Vector4 a) {
    Vector4 result = {0};
    f32 len = v4_length(a);
    if (len != 0.0f)
    {
        result.x = a.x * (1.0f / len);
        result.y = a.y * (1.0f / len);
        result.z = a.z * (1.0f / len);
        result.w = a.w * (1.0f / len);
    }
    return result;
}

// Functions:

function Vector4 lerp_v4(Vector4 a, Vector4 b, f32 t) {
    Vector4 result = {
        lerp_f32(a.x, b.x, t),
        lerp_f32(a.y, b.y, t),
        lerp_f32(a.z, b.z, t),
        lerp_f32(a.w, b.w, t),
    };
    return result;
}

function Vector4 clamp_v4(Vector4 value, Vector4 lower, Vector4 upper) {
    Vector4 result = {
        clamp_f32(value.x, lower.x, upper.x),
        clamp_f32(value.y, lower.y, upper.y),
        clamp_f32(value.z, lower.z, upper.z),
        clamp_f32(value.w, lower.w, upper.w),
    };
    return result;
}

function Vector4 min_v4(Vector4 a, Vector4 b) {
    Vector4 result = {
        min_f32(a.x, b.x),
        min_f32(a.y, b.y),
        min_f32(a.z, b.z),
        min_f32(a.w, b.w),
    };
    return result;
}

function Vector4 max_v4(Vector4 a, Vector4 b) {
    Vector4 result = {
        max_f32(a.x, b.x),
        max_f32(a.y, b.y),
        max_f32(a.z, b.z),
        max_f32(a.w, b.w),
    };
    return result;
}

function Vector4 sign_v4(Vector4 a) {
    Vector4 result = {
        sign_f32(a.x),
        sign_f32(a.y),
        sign_f32(a.z),
        sign_f32(a.w),
    };
    return result;
}

function Vector4 abs_v4(Vector4 a) {
    Vector4 result = {
        abs_f32(a.x),
        abs_f32(a.y),
        abs_f32(a.z),
        abs_f32(a.w),
    };
    return result;
}

function Vector4 round_v4(Vector4 a) {
    Vector4 result = {
        round_f32(a.x),
        round_f32(a.y),
        round_f32(a.z),
        round_f32(a.w),
    };
    return result;
}

function Vector4 floor_v4(Vector4 a) {
    Vector4 result = {
        floor_f32(a.x),
        floor_f32(a.y),
        floor_f32(a.z),
        floor_f32(a.w),
    };
    return result;
}

function Vector4 ceil_v4(Vector4 a) {
    Vector4 result = {
        ceil_f32(a.x),
        ceil_f32(a.y),
        ceil_f32(a.z),
        ceil_f32(a.w),
    };
    return result;
}

#if LANG_CPP

function Vector4 operator+(Vector4 a, Vector4 b) {
    return v4_add(a, b);
}

function Vector4 &operator+=(Vector4 &a, Vector4 b) {
    return a = a + b;
}

function Vector4 operator-(Vector4 a, Vector4 b) {
    return v4_sub(a, b);
}

function Vector4 &operator-=(Vector4 &a, Vector4 b) {
    return a = a - b;
}

function Vector4 operator-(Vector4 a) {
    return v4_inv(a);
}

function Vector4 operator*(f32 a, Vector4 b) {
    return v4_mulf(b, a);
}

function Vector4 operator*(Vector4 a, f32 b) {
    return b * a;
}

function Vector4 &operator*=(Vector4 &a, f32 b) {
    return a = a * b;
}

function Vector4 operator*(Vector4 a, Vector4 b) {
    return v4_mul(a, b);
}

function Vector4 operator*=(Vector4 &a, Vector4 b) {
    return a = a * b;
}

#endif // LANG_CPP

//
// Vector4i
//

function Vector4i v4i(i32 x, i32 y, i32 z, i32 w) {
    Vector4i result = {x, y, z, w};
    return result;
}

function b32 v4i_equals(Vector4i a, Vector4i b) {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

function b32 v4i_is_zero(Vector4i a) {
    return a.x == 0 && a.y == 0 && a.z == 0 && a.w == 0;
}

//
// Rectangle2
// 

function Rectangle2 r2(Vector2 min, Vector2 max) {
    Rectangle2 result = {min, max};
    return result;
}

function Rectangle2 r2_from_f32(f32 x0, f32 y0, f32 x1, f32 y1) {
    Rectangle2 result = {{x0, y0}, {x1, y1}};
    return result;
}

function Rectangle2 r2_from_r2i(Rectangle2i r) {
    Rectangle2 result = {{(f32)r.x0, (f32)r.y0}, {(f32)r.x1, (f32)r.y1}};
    return result;
}

function Rectangle2 r2_from_v2(Vector2 size) {
    Rectangle2 result = {v2(0,0), size};
    return result;
}

function Rectangle2 r2_from_v4(Vector4 r) {
    return *(Rectangle2 *)&r;
}

function b32 r2_intersects(Rectangle2 r0, Rectangle2 r1) {
    return (r0.x0 < r1.x1 && r0.x1 > r1.x0 && r0.y0 < r1.y1 && r0.y1 > r1.y0);
}

function b32 r2_contains(Rectangle2 r, Vector2 p) {
    return (p.x >= r.x0 && p.x < r.x1 && p.y >= r.y0 && p.y < r.y1);
}

function b32 r2_equals(Rectangle2 a, Rectangle2 b) {
    return (a.x0 == b.x0 && a.y0 == b.y0 && a.x1 == b.x1 && a.y1 == b.y1);
}

function Vector2 r2_size(Rectangle2 r) {
    return v2(r.x1 - r.x0, r.y1 - r.y0);
}

function f32 r2_width(Rectangle2 r) {
    return r.x1 - r.x0;
}

function f32 r2_height(Rectangle2 r) {
    return r.y1 - r.y0;
}

function f32 r2_aspect_ratio(Rectangle2 r)
{
    Vector2 size = r2_size(r);
    return size.width/size.height;
}

function Rectangle2 r2_scale_to_fit(Rectangle2 r01, Rectangle2 dest) {
    Rectangle2 result = {0};
    result.p0 = v2_add(dest.p0, v2_mul(r01.p0, r2_size(dest)));
    result.p1 = v2_add(dest.p0, v2_mul(r01.p1, r2_size(dest)));
    return result;
}

function Vector2 r2_center(Rectangle2 r) {
    Vector2 s = r2_size(r);
    Vector2 result = {r.x0 + s.x / 2, r.y0 + s.y / 2};
    return result;
}

function Vector2 r2_top_left(Rectangle2 r) {
    return r.p0;
}

function Vector2 r2_top_right(Rectangle2 r) {
    return v2(r.x1, r.y1);
}

function Vector2 r2_bottom_left(Rectangle2 r) {
    return v2(r.x0, r.y1);
}

function Vector2 r2_bottom_right(Rectangle2 r) {
    return r.p1;
}

function Rectangle2 r2_split_from_bottom(Rectangle2 rect, f32 amount) {
    return r2(v2(rect.x0, rect.y1 - amount), rect.p1);
}

function Rectangle2 r2_split_from_top(Rectangle2 rect, f32 amount) {
    return r2(rect.p0, v2(rect.x1, rect.y0 + amount));
}

function Rectangle2 r2_split_from_left(Rectangle2 rect, f32 amount) {
    return r2(rect.p0, v2(rect.x0 + amount, rect.y1));
}

function Rectangle2 r2_split_from_right(Rectangle2 rect, f32 amount) {
    return r2(v2(rect.x1 - amount, rect.y0), rect.p1);
}

function Rectangle2 r2_trim_from_top(Rectangle2 rect, f32 amount) {
    return r2(v2(rect.x0, rect.y0 + amount), rect.p1);
}

function Rectangle2 r2_trim_from_left(Rectangle2 rect, f32 amount) {
    return r2(v2(rect.x0 + amount, rect.y0), rect.p1);
}

// Functions:

function Rectangle2 abs_r2(Rectangle2 rect) {
    Vector2 p0 = {min_f32(rect.x0, rect.x1), min_f32(rect.y0, rect.y1)};
    Vector2 p1 = {max_f32(rect.x0, rect.x1), max_f32(rect.y0, rect.y1)};
    return r2(p0, p1);
}

function Rectangle2 r2_union(Rectangle2 a, Rectangle2 b) {
    a = abs_r2(a);
    b = abs_r2(b);
    return r2(min_v2(a.p0, b.p0), max_v2(a.p1, b.p1));
}

function Rectangle2 r2_intersection(Rectangle2 a, Rectangle2 b) {
    a = abs_r2(a);
    b = abs_r2(b);
    return r2(max_v2(a.p0, b.p0), min_v2(a.p1, b.p1));
}

function Rectangle2 r2_clip(Rectangle2 a, Rectangle2 b) {
    a = abs_r2(a);
    b = abs_r2(b);
    return r2(max_v2(a.p0, b.p0), min_v2(a.p1, b.p1));
}

function Rectangle2 r2_shift(Rectangle2 a, Vector2 b) {
    return r2(v2_add(a.p0, b), v2_add(a.p1, b));
}

function Rectangle2 r2_pad(Rectangle2 rect, Vector2 padding) {
    return r2(v2_sub(rect.p0, padding), v2_add(rect.p1, padding));
}

function Rectangle2 r2f_pad(Rectangle2 rect, f32 padding) {
    return r2_pad(rect, v2(padding, padding));
}

function Rectangle2 r2_pad_inner_v2(Rectangle2 rect, Vector2 padding)
{
    return r2(v2_add(rect.p0, padding), v2_sub(rect.p1, padding));
}

function Rectangle2 r2_pad_inner_v4(Rectangle2 rect, Vector4 padding)
{
    Rectangle2 pad = r2_from_v4(padding);
    return r2_from_f32(rect.x0 + pad.x0, rect.y0 + pad.y0, rect.x1 - pad.x1, rect.y1 - pad.y1);
}

function Rectangle2 r2_pad_outer_v4(Rectangle2 rect, Vector4 padding)
{
    Rectangle2 pad = r2_from_v4(padding);
    return r2_from_f32(rect.x0 - pad.x0, rect.y0 - pad.y0, rect.x1 + pad.x1, rect.y1 + pad.y1);
}

function Rectangle2 r2_bounds(Vector2 pos, Vector2 size, Vector2 anchor, Vector2 scale)
{
    Vector2 ss = v2_mul(size, scale);
    Vector2 p0 = v2_sub(pos, v2_mul(anchor, ss));
    return abs_r2(r2(p0, v2_add(p0, ss)));
}

#if LANG_CPP

function Rectangle2 operator+(Rectangle2 a, Vector2 b) {
    return r2_shift(a, b);
}

function Rectangle2 operator+(Vector2 b, Rectangle2 a) {
    return r2_shift(a, b);
}

#endif // LANG_CPP

//
// Rectangle2i
// 

function Rectangle2i r2i(i32 x0, i32 y0, i32 x1, i32 y1) {
    Rectangle2i result = {x0, y0, x1, y1};
    return result;
}

function Vector2i r2i_size(Rectangle2i r) {
    Vector2i result = {r.x1 - r.x0, r.y1 - r.y0};
    return result;
}

function i32 r2i_width(Rectangle2i r) {
    return r.x1 - r.x0;
}

function i32 r2i_height(Rectangle2i r) {
    return r.y1 - r.y0;
}

function Rectangle2i aspect_ratio_fit(u32 src_width, u32 src_height, u32 dest_width, u32 dest_height)
{
    Rectangle2i result = {0};

    // can't divide by zero!
    if (src_width != 0 && src_height != 0 && dest_width != 0 && dest_height != 0)
    {
        f32 optimal_window_width = (f32)dest_height * ((f32)src_width / (f32)src_height);
        f32 optimal_window_height = (f32)dest_width * ((f32)src_height / (f32)src_width);

        if (optimal_window_width > (f32)dest_width) {
            // NOTE(casey): width-constrained display - top and bottom black bars
            result.x0 = 0;
            result.x1 = dest_width;

            f32 empty = (f32)dest_height - optimal_window_height;
            i32 half_empty = round_i32(0.5f * empty);
            i32 use_height = round_i32(optimal_window_height);

            result.y0 = half_empty;
            result.y1 = result.y0 + use_height;
        } else {
            // NOTE(casey): Height-constrained display - left and right black bars
            result.y0 = 0;
            result.y1 = dest_height;

            f32 empty = (f32)dest_width - optimal_window_width;
            i32 half_empty = round_i32(0.5f * empty);
            i32 use_width = round_i32(optimal_window_width);

            result.x0 = half_empty;
            result.x1 = result.x0 + use_width;
        }
    }

    return result;
}

function Rectangle2i aspect_ratio_fit_pixel_perfect(u32 src_width, u32 src_height, u32 dest_width, u32 dest_height)
{
    Rectangle2i result = {0};

    // can't divide by zero!
    if (src_width != 0 && src_height != 0 && dest_width != 0 && dest_height != 0)
    {
        u32 max_scalex = dest_width / src_width;
        u32 max_scaley = dest_height / src_height;

        u32 max_scale = Min(max_scalex, max_scaley);

        Vector2i scaled_size = v2i(max_scale * src_width, max_scale * src_height);
        i32 centerx = (dest_width - scaled_size.x) / 2;
        i32 centery = (dest_height - scaled_size.y) / 2;
        result.x0 = centerx;
        result.y0 = centery;
        result.x1 = result.x0 + scaled_size.x;
        result.y1 = result.y0 + scaled_size.y;
    }

    return result;
}

function Rectangle2i aspect_ratio_fill(u32 src_width, u32 src_height, u32 dest_width, u32 dest_height)
{
    Rectangle2i result = {0};

    // can't divide by zero!
    if (src_width != 0 && src_height != 0 && dest_width != 0 && dest_height != 0)
    {
        f32 optimal_window_width = (f32)dest_height * ((f32)src_width / (f32)src_height);
        f32 optimal_window_height = (f32)dest_width * ((f32)src_height / (f32)src_width);

        if (optimal_window_width > (f32)dest_width) {
            // NOTE(nick): width-overflow - clip left and right sides
            result.y0 = 0;
            result.y1 = dest_height;

            f32 overflow = (f32)(optimal_window_width - dest_width);
            result.x0 = -round_i32(0.5f * overflow);
            result.x1 = dest_width + round_i32(0.5 * overflow);
        } else {
            // NOTE(nick): height-overflow - clip top and bottom
            result.x0 = 0;
            result.x1 = dest_width;

            f32 overflow = (f32)(optimal_window_height - dest_height);
            result.y0 = -round_i32(0.5f * overflow);
            result.y1 = dest_height + round_i32(0.5 * overflow);
        }
    }

    return result;
}

//
// Rectangle3
//

function Rectangle3 r3(Vector3 p0, Vector3 p1) {
    Rectangle3 result = {p0, p1};
    return result;
}

function Vector3 r3_size(Rectangle3 r) {
    Vector3 result = {r.x1 - r.x0, r.y1 - r.y0, r.z1 - r.z0};
    return result;
}

function Vector3 r3_center(Rectangle3 r) {
    Vector3 s = r3_size(r);
    Vector3 result = {r.x0 + s.x / 2, r.y0 + s.y / 2, r.z0 + s.z / 2};
    return result;
}

function b32 r3_intersects(Rectangle3 a, Rectangle3 b) {
    return (
        (b.x1 > a.x0) && (b.x0 < a.x1) &&
        (b.y1 > a.y0) && (b.y0 < a.y1) &&
        (b.z1 > a.z0) && (b.z0 < a.z1)
    );
}

function b32 r3_contains(Rectangle3 rect, Vector3 p) {
    return (
        p.x >= rect.x0 && p.x <= rect.x1 &&
        p.y >= rect.y0 && p.y <= rect.y1 &&
        p.z >= rect.z0 && p.z <= rect.z1
    );
}

//
// Matrix3
// 

function Matrix3 matrix3_identity() {
    Matrix3 result = {{
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
    }};
    return result;
}

function Vector3 matrix3_column(Matrix3 *it, u32 i) {
    assert(i >= 0 && i < 3);
    Vector3 result = {it->e[0][i], it->e[1][i], it->e[2][i]};
    return result;
}

function Matrix3 matrix3_transpose(Matrix3 a) {
    f32 tmp;

    tmp = a.e[1][0];
    a.e[1][0] = a.e[0][1];
    a.e[0][1] = tmp;

    tmp = a.e[2][0];
    a.e[2][0] = a.e[0][2];
    a.e[0][2] = tmp;

    tmp = a.e[2][1];
    a.e[2][1] = a.e[1][2];
    a.e[1][2] = tmp;

    return a;
}

function Vector3 matrix3_mulv3(Matrix3 a, Vector3 p) {
    Vector3 result = {
        v3_dot(p, a.rows[0]),
        v3_dot(p, a.rows[1]),
        v3_dot(p, a.rows[2]),
    };
    return result;
}

function Vector3 matrix3_transform_v3(Matrix3 a, Vector3 p) {
    return matrix3_mulv3(a, p);
}

//
// Matrix4
//

function Matrix4 matrix4_identity() {
    Matrix4 result = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

function Matrix4 matrix4_diagonal(f32 d) {
    Matrix4 result = {{
        {d, 0, 0, 0},
        {0, d, 0, 0},
        {0, 0, d, 0},
        {0, 0, 0, d},
    }};
    return result;
}

function Matrix4 matrix4_from_v4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3) {
    Matrix4 result = {{
        row0,
        row1,
        row2,
        row3,
    }};
    return result;
}

function Vector4 matrix4_column(Matrix4 *it, u32 i) {
    assert(i >= 0 && i < 4);
    Vector4 result = {it->e[0][i], it->e[1][i], it->e[2][i], it->e[3][i]};
    return result;
}

function Vector4 matrix4_mulv4(Matrix4 a, Vector4 p) {
    Vector4 result = {
        v4_dot(p, a.rows[0]),
        v4_dot(p, a.rows[1]),
        v4_dot(p, a.rows[2]),
        v4_dot(p, a.rows[3]),
    };
    return result;
}

function Vector3 matrix4_mulv3(Matrix4 a, Vector3 p) {
    return matrix4_mulv4(a, v4_from_v3f(p, 1.0f)).xyz;
}

function Vector3 matrix4_transform_v3(Matrix4 a, Vector3 p) {
    return matrix4_mulv3(a, p);
}

function Vector4 matrix4_transform_v4(Matrix4 a, Vector4 p) {
    return matrix4_mulv4(a, p);
}

function Vector2 matrix4_transform_v2(Matrix4 m, Vector2 src) {
    f32 x = (m.e[0][0] * src.x) + (m.e[0][1] * src.y) + (0) + (m.e[0][3]);
    f32 y = (m.e[1][0] * src.x) + (m.e[1][1] * src.y) + (0) + (m.e[1][3]);
    return v2(x, y);
}

function Matrix4 matrix4_mul(Matrix4 a, Matrix4 b) {
    Vector4 row0 = a.rows[0];
    Vector4 row1 = a.rows[1];
    Vector4 row2 = a.rows[2];
    Vector4 row3 = a.rows[3];

    Vector4 col0 = matrix4_column(&b, 0);
    Vector4 col1 = matrix4_column(&b, 1);
    Vector4 col2 = matrix4_column(&b, 2);
    Vector4 col3 = matrix4_column(&b, 3);

    Matrix4 result = {{
        {v4_dot(row0, col0), v4_dot(row0, col1), v4_dot(row0, col2), v4_dot(row0, col3)},
        {v4_dot(row1, col0), v4_dot(row1, col1), v4_dot(row1, col2), v4_dot(row1, col3)},
        {v4_dot(row2, col0), v4_dot(row2, col1), v4_dot(row2, col2), v4_dot(row2, col3)},
        {v4_dot(row3, col0), v4_dot(row3, col1), v4_dot(row3, col2), v4_dot(row3, col3)},
    }};
    return result;
}

function Matrix4 matrix4_mulf(Matrix4 a, f32 s) {
    Matrix4 result = {{
        v4_mulf(a.rows[0], s),
        v4_mulf(a.rows[1], s),
        v4_mulf(a.rows[2], s),
        v4_mulf(a.rows[3], s),
    }};
    return result;
}

function Matrix4 matrix4_rotation_x(f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    Matrix4 result = {{
        {1, 0, 0, 0},
        {0, c, -s, 0},
        {0, s, c, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

function Matrix4 matrix4_rotation_y(f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    Matrix4 result = {{
        {c, 0, s, 0},
        {0, 1, 0, 0},
        {-s, 0, c, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

function Matrix4 matrix4_rotation_z(f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    Matrix4 result = {{
        {c, -s, 0, 0},
        {s, c, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

function Matrix4 matrix4_translation(Vector3 t) {
    Matrix4 result = {{
        {1, 0, 0, t.x},
        {0, 1, 0, t.y},
        {0, 0, 1, t.z},
        {0, 0, 0, 1},
    }};
    return result;
}

function Matrix4 matrix4_translation_2d(Vector2 t) {
    Matrix4 result = {{
        {1, 0, 0, t.x},
        {0, 1, 0, t.y},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

function void matrix4_translate(Matrix4 *a, Vector3 t) {
    a->e[0][3] += t.x;
    a->e[1][3] += t.y;
    a->e[2][3] += t.z;
}

function void matrix4_translate_2d(Matrix4 *a, Vector2 t) {
    matrix4_translate(a, v3_from_v2f(t, 0));
}

function Matrix4 matrix4_scale(Vector3 s) {
    Matrix4 result = {{
        {s.x, 0, 0, 0},
        {0, s.y, 0, 0},
        {0, 0, s.z, 0},
        {0, 0, 0, 1},
    }};
    return result;
}

function Matrix4 matrix4_scale_2d(Vector2 s) {
    return matrix4_scale(v3_from_v2f(s, 1.0));
}

function Matrix4 matrix4_scale_uniform(f32 s) {
    return matrix4_scale(v3(s, s, s));
}

function Matrix4 matrix4_transpose(Matrix4 a) {
    f32 tmp;

    tmp = a.e[1][0];
    a.e[1][0] = a.e[0][1];
    a.e[0][1] = tmp;

    tmp = a.e[2][0];
    a.e[2][0] = a.e[0][2];
    a.e[0][2] = tmp;

    tmp = a.e[2][1];
    a.e[2][1] = a.e[1][2];
    a.e[1][2] = tmp;

    tmp = a.e[3][0];
    a.e[3][0] = a.e[0][3];
    a.e[0][3] = tmp;

    tmp = a.e[3][1];
    a.e[3][1] = a.e[1][3];
    a.e[1][3] = tmp;

    tmp = a.e[3][2];
    a.e[3][2] = a.e[2][3];
    a.e[2][3] = tmp;

    return a;
}

function f32 matrix4_determinant(Matrix4 m) {
    f32 m0 = m.e[0][0], m1 = m.e[0][1], m2 = m.e[0][2], m3 = m.e[0][3],
        m4 = m.e[1][0], m5 = m.e[1][1], m6 = m.e[1][2], m7 = m.e[1][3],
        m8 = m.e[2][0], m9 = m.e[2][1], m10 = m.e[2][2], m11 = m.e[2][3],
        m12 = m.e[3][0], m13 = m.e[3][1], m14 = m.e[3][2], m15 = m.e[3][3];

    return (
        m12 * m9 * m6 * m3 - m8 * m13 * m6 * m3 - m12 * m5 * m10 * m3 +
        m4 * m13 * m10 * m3 + m8 * m5 * m14 * m3 - m4 * m9 * m14 * m3 -
        m12 * m9 * m2 * m7 + m8 * m13 * m2 * m7 + m12 * m1 * m10 * m7 -
        m0 * m13 * m10 * m7 - m8 * m1 * m14 * m7 + m0 * m9 * m14 * m7 +
        m12 * m5 * m2 * m11 - m4 * m13 * m2 * m11 - m12 * m1 * m6 * m11 +
        m0 * m13 * m6 * m11 + m4 * m1 * m14 * m11 - m0 * m5 * m14 * m11 -
        m8 * m5 * m2 * m15 + m4 * m9 * m2 * m15 + m8 * m1 * m6 * m15 -
        m0 * m9 * m6 * m15 - m4 * m1 * m10 * m15 + m0 * m5 * m10 * m15
    );
}

function Matrix4 matrix4_inverse(Matrix4 m) {
    Matrix4 t;

    f32 m0 = m.e[0][0], m1 = m.e[0][1], m2 = m.e[0][2], m3 = m.e[0][3],
        m4 = m.e[1][0], m5 = m.e[1][1], m6 = m.e[1][2], m7 = m.e[1][3],
        m8 = m.e[2][0], m9 = m.e[2][1], m10 = m.e[2][2], m11 = m.e[2][3],
        m12 = m.e[3][0], m13 = m.e[3][1], m14 = m.e[3][2], m15 = m.e[3][3];

    t.e[0][0] = (m9 * m14 * m7 - m13 * m10 * m7 + m13 * m6 * m11 - m5 * m14 * m11 - m9 * m6 * m15 + m5 * m10 * m15);
    t.e[0][1] = (m13 * m10 * m3 - m9 * m14 * m3 - m13 * m2 * m11 + m1 * m14 * m11 + m9 * m2 * m15 - m1 * m10 * m15);
    t.e[0][2] = (m5 * m14 * m3 - m13 * m6 * m3 + m13 * m2 * m7 - m1 * m14 * m7 - m5 * m2 * m15 + m1 * m6 * m15);
    t.e[0][3] = (m9 * m6 * m3 - m5 * m10 * m3 - m9 * m2 * m7 + m1 * m10 * m7 + m5 * m2 * m11 - m1 * m6 * m11);

    t.e[1][0] = (m12 * m10 * m7 - m8 * m14 * m7 - m12 * m6 * m11 + m4 * m14 * m11 + m8 * m6 * m15 - m4 * m10 * m15);
    t.e[1][1] = (m8 * m14 * m3 - m12 * m10 * m3 + m12 * m2 * m11 - m0 * m14 * m11 - m8 * m2 * m15 + m0 * m10 * m15);
    t.e[1][2] = (m12 * m6 * m3 - m4 * m14 * m3 - m12 * m2 * m7 + m0 * m14 * m7 + m4 * m2 * m15 - m0 * m6 * m15);
    t.e[1][3] = (m4 * m10 * m3 - m8 * m6 * m3 + m8 * m2 * m7 - m0 * m10 * m7 - m4 * m2 * m11 + m0 * m6 * m11);

    t.e[2][0] = (m8 * m13 * m7 - m12 * m9 * m7 + m12 * m5 * m11 - m4 * m13 * m11 - m8 * m5 * m15 + m4 * m9 * m15);
    t.e[2][1] = (m12 * m9 * m3 - m8 * m13 * m3 - m12 * m1 * m11 + m0 * m13 * m11 + m8 * m1 * m15 - m0 * m9 * m15);
    t.e[2][2] = (m4 * m13 * m3 - m12 * m5 * m3 + m12 * m1 * m7 - m0 * m13 * m7 - m4 * m1 * m15 + m0 * m5 * m15);
    t.e[2][3] = (m8 * m5 * m3 - m4 * m9 * m3 - m8 * m1 * m7 + m0 * m9 * m7 + m4 * m1 * m11 - m0 * m5 * m11);

    t.e[3][0] = (m12 * m9 * m6 - m8 * m13 * m6 - m12 * m5 * m10 + m4 * m13 * m10 + m8 * m5 * m14 - m4 * m9 * m14);
    t.e[3][1] = (m8 * m13 * m2 - m12 * m9 * m2 + m12 * m1 * m10 - m0 * m13 * m10 - m8 * m1 * m14 + m0 * m9 * m14);
    t.e[3][2] = (m12 * m5 * m2 - m4 * m13 * m2 - m12 * m1 * m6 + m0 * m13 * m6 + m4 * m1 * m14 - m0 * m5 * m14);
    t.e[3][3] = (m4 * m9 * m2 - m8 * m5 * m2 + m8 * m1 * m6 - m0 * m9 * m6 - m4 * m1 * m10 + m0 * m5 * m10);

    f32 d = matrix4_determinant(m);
    return matrix4_mulf(t, (1.0f / d));
}

function Matrix4 matrix4_rotate(f32 angle_degrees, Vector3 axis)
{
    Matrix4 result = matrix4_identity();
    
    axis = v3_normalize(axis);
    
    f32 sin_theta = Sin(degrees_to_radians(angle_degrees));
    f32 cos_theta = Cos(degrees_to_radians(angle_degrees));
    f32 cos_value = 1.0f - cos_theta;
    
    result.e[0][0] = (axis.x * axis.x * cos_value) + cos_theta;
    result.e[1][0] = (axis.x * axis.y * cos_value) + (axis.z * sin_theta);
    result.e[2][0] = (axis.x * axis.z * cos_value) - (axis.y * sin_theta);
    
    result.e[0][1] = (axis.y * axis.x * cos_value) - (axis.z * sin_theta);
    result.e[1][1] = (axis.y * axis.y * cos_value) + cos_theta;
    result.e[2][1] = (axis.y * axis.z * cos_value) + (axis.x * sin_theta);
    
    result.e[0][2] = (axis.z * axis.x * cos_value) + (axis.y * sin_theta);
    result.e[1][2] = (axis.z * axis.y * cos_value) - (axis.x * sin_theta);
    result.e[2][2] = (axis.z * axis.z * cos_value) + cos_theta;
    
    return result;
}

// NOTE(nick): reasonable values are perspective_proj(aspect_ratio, 1.0f, 0.1f, 100.0f);
function Matrix4 matrix4_perspective_camera(f32 aspect_width_over_height, f32 focal_length, f32 near_clip_plane, f32 far_clip_plane) {
    f32 a = 1.0f;
    f32 b = aspect_width_over_height;
    f32 c = focal_length; // NOTE(casey): This should really be called "film back distance"

    f32 n = near_clip_plane; // NOTE(casey): Near clip plane _distance_
    f32 f = far_clip_plane;  // NOTE(casey): Far clip plane _distance_

    // NOTE(casey): These are the perspective corRectangle terms, for when you divide by -z
    f32 d = (n + f) / (n - f);
    f32 e = (2 * f * n) / (n - f);

    Matrix4 result = {{
        {a * c, 0, 0, 0},
        {0, b * c, 0, 0},
        {0, 0, d, e},
        {0, 0, -1, 0},
    }};
    return result;
}

function Matrix4 matrix4_perspective(f32 fov, f32 aspect_ratio, f32 near_z, f32 far_z)
{
    Matrix4 result = matrix4_identity();

    f32 TanThetaOver2 = Tan(fov * (PI / 360.0f));
    
    result.e[0][0] = 1.0f / TanThetaOver2;
    result.e[1][1] = aspect_ratio / TanThetaOver2;
    result.e[3][2] = -1.0f;
    result.e[2][2] = (near_z + far_z) / (near_z - far_z);
    result.e[2][3] = (2.0f * near_z * far_z) / (near_z - far_z);
    result.e[3][3] = 0.0f;

    return result;
}

function Matrix4 matrix4_orthographic_camera(f32 aspect_width_over_height, f32 near_clip_plane, f32 far_clip_plane) {
    f32 a = 1.0f;
    f32 b = aspect_width_over_height;

    f32 n = near_clip_plane; // NOTE(casey): Near clip plane _distance_
    f32 f = far_clip_plane;  // NOTE(casey): Far clip plane _distance_

    // NOTE(casey): These are the non-perspective corRectangleed terms, for orthographic
    f32 d = 2.0f / (n - f);
    f32 e = (n + f) / (n - f);

    Matrix4 result = {{
        {a, 0, 0, 0},
        {0, b, 0, 0},
        {0, 0, d, e},
        {0, 0, 0, 1},
    }};
    return result;
}

// NOTE(nick): typically near = -1.0f, far = 1.0f
function Matrix4 matrix4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z) {
    Matrix4 result = matrix4_identity();
    result.e[0][0] = 2.0f / (right - left);
    result.e[1][1] = 2.0f / (top - bottom);
    result.e[2][2] = 2.0f / (near_z - far_z);
    result.e[0][3] = (right + left) / (left - right);
    result.e[1][3] = (top + bottom) / (bottom - top);
    result.e[2][3] = (far_z + near_z) / (near_z - far_z);
    return result;
}

// NOTE(nick): left-handed coordinates
function Matrix4 matrix4_window(Vector2 screen)
{
    return matrix4_orthographic(0, screen.width, screen.height, 0, -1, 1);
}

function Matrix4 matrix4_lookat(Vector3 eye, Vector3 center, Vector3 up)
{
    Matrix4 result = {0};

    Vector3 f = v3_normalize(v3_sub(center, eye));
    Vector3 s = v3_normalize(v3_cross(f, up));
    Vector3 u = v3_cross(s, f);

    result.e[0][0] = s.x;
    result.e[1][0] = u.x;
    result.e[2][0] = -f.x;

    result.e[0][1] = s.y;
    result.e[1][1] = u.y;
    result.e[2][1] = -f.y;

    result.e[0][2] = s.z;
    result.e[1][2] = u.z;
    result.e[2][2] = -f.z;

    result.e[0][3] = -v3_dot(s, eye);
    result.e[1][3] = -v3_dot(u, eye);
    result.e[2][3] = v3_dot(f, eye);
    result.e[3][3] = 1.0f;

    return result;
}

#if LANG_CPP

function Matrix4 operator*(Matrix4 a, Matrix4 b) {
    return matrix4_mul(a, b);
}

function Vector3 operator*(Matrix4 a, Vector3 p) {
    return matrix4_mulv3(a, p);
}

function Vector4 operator*(Matrix4 a, Vector4 p) {
    return matrix4_mulv4(a, p);
}

function Matrix4 operator*(Matrix4 a, f32 s) {
    return matrix4_mulf(a, s);
}

#endif // LANG_CPP

//
// Quaternion
//

function Quaternion quat(Vector4 a) {
    Quaternion result = {a.x, a.y, a.z, a.w};
    return result;
}

function Quaternion quaternion_from_angle(f32 angle, Vector3 v) {
    Vector3 a = v3_mulf(v, Sin(angle / 2));
    Quaternion result = {a.x, a.y, a.z, Cos(angle / 2)};
    return result;
}

function Quaternion quaternion_identity() {
    Quaternion result = {0, 0, 0, 1};
    return result;
}

function Quaternion quaternion_invert(Quaternion q) {
    Quaternion result = {-q.x, -q.y, -q.z, q.w};
    return result;
}

function Quaternion quaternion_mul(Quaternion q1, Quaternion q2) {
    Quaternion result;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    return result;
}

function Matrix4 quaternion_to_rotation_matrix(Quaternion q) {
    Matrix4 result = matrix4_identity();

    result.e[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
    result.e[0][1] = 2 * q.x * q.y - 2 * q.z * q.w;
    result.e[0][2] = 2 * q.x * q.z + 2 * q.y * q.w;

    result.e[1][0] = 2 * q.x * q.y + 2 * q.z * q.w;
    result.e[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
    result.e[1][2] = 2 * q.y * q.z - 2 * q.x * q.w;

    result.e[2][0] = 2 * q.x * q.z - 2 * q.y * q.w;
    result.e[2][1] = 2 * q.y * q.z + 2 * q.x * q.w;
    result.e[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;

    return result;
}

function Vector3 quaternion_vector3_mul(Quaternion q, Vector3 v) {
    return matrix4_transform_v3(quaternion_to_rotation_matrix(q), v);
}

function Quaternion quaternion_from_euler(f32 pitch, f32 yaw, f32 roll) {
    f32 p = pitch / 2.0f;
    f32 y = yaw / 2.0f;
    f32 r = roll / 2.0f;

    f32 sinp = Sin(p);
    f32 siny = Sin(y);
    f32 sinr = Sin(r);
    f32 cosp = Cos(p);
    f32 cosy = Cos(y);
    f32 cosr = Cos(r);

    f32 vx = sinr * cosp * cosy - cosr * sinp * siny;
    f32 vy = cosr * sinp * cosy + sinr * cosp * siny;
    f32 vz = cosr * cosp * siny - sinr * sinp * cosy;
    f32 vw = cosr * cosp * cosy + sinr * sinp * siny;

    Quaternion result = {vx, vy, vz, vw};
    return result;
}

#if LANG_CPP

function Quaternion operator*(Quaternion a, Quaternion b) {
    return quaternion_mul(a, b);
}

function Quaternion operator-(Quaternion a) {
    return quaternion_invert(a);
}

function Vector3 operator*(Quaternion a, Vector3 b) {
    return quaternion_vector3_mul(a, b);
}

#endif // LANG_CPP

//
// Shorthands
//

#if 0
function f32 min(f32 a, f32 b) { return min_f32(a, b); }
function f32 max(f32 a, f32 b) { return max_f32(a, b); }
function f32 clamp(f32 value, f32 lower, f32 upper) { return clamp_f32(value, lower, upper); }
function f32 sign(f32 x) { return sign_f32(x); }
function f32 abs(f32 x) { return abs_f32(x); }
function f32 round(f32 x) { return round_f32(x); }
function f32 floor(f32 x) { return floor_f32(x); }
function f32 ceil(f32 x) { return ceil_f32(x); }

function f32 sin(f32 x) { return sin_f32(x); }
function f32 cos(f32 x) { return cos_f32(x); }
function f32 tan(f32 x) { return tan_f32(x); }
function f32 atan2(f32 y, f32 x) { return atan2_f32(y, x); }
function f32 acos(f32 x) { return acos_f32(x); }
#endif

function f32 min3(f32 a, f32 b, f32 c) { return min3_f32(a, b, c); }
function f32 max3(f32 a, f32 b, f32 c) { return max3_f32(a, b, c); }
function f32 lerp(f32 a, f32 b, f32 t) { return lerp_f32(a, b, t); }
function f32 unlerp(f32 a, f32 b, f32 v) { return unlerp_f32(a, b, v); }
function f32 move(f32 x, f32 target, f32 speed) { return move_f32(x, target, speed); }
function f32 snap(f32 value, f32 grid) { return snap_f32(value, grid); }

// TODO(nick): shorthands for vector types
#if LANG_CPP

// Vector2
function Vector2 min(Vector2 a, Vector2 b) { return min_v2(a, b); }
function Vector2 max(Vector2 a, Vector2 b) { return max_v2(a, b); }
function Vector2 clamp(Vector2 value, Vector2 lower, Vector2 upper) { return clamp_v2(value, lower, upper); }
function Vector2 sign(Vector2 a) { return sign_v2(a); }
function Vector2 abs(Vector2 a) { return abs_v2(a); }
function Vector2 round(Vector2 a) { return round_v2(a); }
function Vector2 floor(Vector2 a) { return floor_v2(a); }
function Vector2 ceil(Vector2 a) { return ceil_v2(a); }
function Vector2 lerp(Vector2 a, Vector2 b, f32 t) { return lerp_v2(a, b, t); }

function Vector2 move(Vector2 x, Vector2 target, f32 speed) { return move_v2(x, target, speed); }
function Vector2 snap(Vector2 pos, Vector2 grid) { return snap_v2(pos, grid); }

// Vector3
function Vector3 min(Vector3 a, Vector3 b) { return min_v3(a, b); }
function Vector3 max(Vector3 a, Vector3 b) { return max_v3(a, b); }
function Vector3 clamp(Vector3 value, Vector3 lower, Vector3 upper) { return clamp_v3(value, lower, upper); }
function Vector3 sign(Vector3 a) { return sign_v3(a); }
function Vector3 abs(Vector3 a) { return abs_v3(a); }
function Vector3 round(Vector3 a) { return round_v3(a); }
function Vector3 floor(Vector3 a) { return floor_v3(a); }
function Vector3 ceil(Vector3 a) { return ceil_v3(a); }
function Vector3 lerp(Vector3 a, Vector3 b, f32 t) { return lerp_v3(a, b, t); }

// Vector4
function Vector4 min(Vector4 a, Vector4 b) { return min_v4(a, b); }
function Vector4 max(Vector4 a, Vector4 b) { return max_v4(a, b); }
function Vector4 clamp(Vector4 value, Vector4 lower, Vector4 upper) { return clamp_v4(value, lower, upper); }
function Vector4 sign(Vector4 a) { return sign_v4(a); }
function Vector4 abs(Vector4 a) { return abs_v4(a); }
function Vector4 round(Vector4 a) { return round_v4(a); }
function Vector4 floor(Vector4 a) { return floor_v4(a); }
function Vector4 ceil(Vector4 a) { return ceil_v4(a); }
function Vector4 lerp(Vector4 a, Vector4 b, f32 t) { return lerp_v4(a, b, t); }

#endif // LANG_CPP

//
// Colors
//

function Vector3 hsv_from_rgb(Vector3 rgb)
{
    f32 r = rgb.r;
    f32 g = rgb.g;
    f32 b = rgb.b;
    f32 k = 0.f;
    if (g < b)
    {
        f32 swap = b;
        b = g;
        g = swap;
        k = -1.f;
    }
    if (r < g)
    {
        f32 swap = r;
        r = g;
        g = swap;
        k = -2.f / 6.f - k;
    }
    f32 chroma = r - (g < b ? g : b);
    Vector3 result;
    result.x = AbsF32(k + (g - b) / (6.f * chroma + 1e-20f));
    result.y = chroma / (r + 1e-20f);
    result.z = r;
    return result;
}

function Vector3 rgb_from_hsv(Vector3 hsv)
{
    Vector3 rgb = {0};
    
    if (hsv.y == 0.0f)
    {
        rgb.r = rgb.g = rgb.b = hsv.z;
    }
    else
    {
        f32 h = hsv.x;
        f32 s = hsv.y;
        f32 v = hsv.z;
        
        if (h >= 1.f)
        {
            h -= 10 * 1e-6f;
        }
        
        if (s >= 1.f)
        {
            s -= 10 * 1e-6f;
        }
        
        if (v >= 1.f)
        {
            v -= 10 * 1e-6f;
        }
        
        h = Mod(h, 1.f) / (60.f/360.f);
        int i = (int)h;
        f32 f = h - (f32)i;
        f32 p = v * (1.0f - s);
        f32 q = v * (1.0f - s * f);
        f32 t = v * (1.0f - s * (1.0f - f));
        
        switch (i)
        {
            case 0: { rgb.r = v; rgb.g = t; rgb.b = p; } break;
            case 1: { rgb.r = q; rgb.g = v; rgb.b = p; } break;
            case 2: { rgb.r = p; rgb.g = v; rgb.b = t; } break;
            case 3: { rgb.r = p; rgb.g = q; rgb.b = v; } break;
            case 4: { rgb.r = t; rgb.g = p; rgb.b = v; } break;
            case 5: default: { rgb.r = v; rgb.g = p; rgb.b = q; } break;
        }
    }
    
    return rgb;
}

function Vector4 rgba_from_hsv(Vector3 hsv)
{
    return v4_from_v3f(rgb_from_hsv(hsv), 1.0);
}

function Vector4 v4_argb_from_u32(u32 hex)
{
    Vector4 result = {0};
    result.a = (f32)((hex & 0xff000000) >> 24) / 255.f;
    result.r = (f32)((hex & 0x00ff0000) >> 16) / 255.f;
    result.b = (f32)((hex & 0x0000ff00) >>  8) / 255.f;
    result.g = (f32)((hex & 0x000000ff) >>  0) / 255.f;
    return result;
}

function u32 u32_argb_from_v4(Vector4 v)
{
    u32 result =
        ((u32)(v.a * 255.0f) & 0xff) << 24 |
        ((u32)(v.r * 255.0f) & 0xff) << 16 |
        ((u32)(v.g * 255.0f) & 0xff) << 8  |
        ((u32)(v.b * 255.0f) & 0xff) << 0;
    return result;
}

function Vector4 v4_rgba_from_u32(u32 hex)
{
    Vector4 result = {0};
    result.a = (f32)((hex & 0xff000000) >> 24) / 255.f;
    result.b = (f32)((hex & 0x00ff0000) >> 16) / 255.f;
    result.g = (f32)((hex & 0x0000ff00) >>  8) / 255.f;
    result.r = (f32)((hex & 0x000000ff) >>  0) / 255.f;
    return result;
}

function Vector4 v4_rgba_from_u8(u8 r, u8 g, u8 b, u8 a)
{
    Vector4 result = {0};
    result.r = (f32)(r) / 255.f;
    result.g = (f32)(g) / 255.f;
    result.b = (f32)(b) / 255.f;
    result.a = (f32)(a) / 255.f;
    return result;
}

function Vector4 v4_rgb_from_u8(u8 r, u8 g, u8 b)
{
    Vector4 result = {0};
    result.r = (f32)(r) / 255.f;
    result.g = (f32)(g) / 255.f;
    result.b = (f32)(b) / 255.f;
    result.a = 1;
    return result;
}

function u32 u32_rgba_from_v4(Vector4 v)
{
    u32 result =
        ((u32)(v.a * 255.0f) & 0xff) << 24 |
        ((u32)(v.b * 255.0f) & 0xff) << 16 |
        ((u32)(v.g * 255.0f) & 0xff) << 8  |
        ((u32)(v.r * 255.0f) & 0xff) << 0;
    return result;
}

function u32 rgba_u32_from_u8(u8 r, u8 g, u8 b, u8 a)
{
    u32 result =
        ((u32)(a)) << 24 |
        ((u32)(b)) << 16 |
        ((u32)(g)) << 8  |
        ((u32)(r)) << 0;
    return result;
}

function Vector4 linear_rgb_to_srgb(Vector4 linear_rgb)
{
    Vector4 result = {0};
    result.r = Pow(linear_rgb.r, 1.0 / 2.2);
    result.g = Pow(linear_rgb.g, 1.0 / 2.2);
    result.b = Pow(linear_rgb.b, 1.0 / 2.2);
    result.a = linear_rgb.a;
    return result;
}

function Vector4 srgb_to_linear_rgb(Vector4 srgb)
{
    Vector4 result = {0};
    result.r = Pow(srgb.r, 2.2);
    result.g = Pow(srgb.g, 2.2);
    result.b = Pow(srgb.b, 2.2);
    result.a = srgb.a;
    return result;
}

function Vector4 v4_premultiply_alpha(Vector4 color)
{
    f32 a = color.a;
    return v4(color.r * a, color.g * a, color.b * a, 1.0);
}

//
// String Conversions
//

#if defined(BASE_STRINGS_H) && LANG_CPP

function String to_string(Vector2 a) {
    return sprint("Vector2 {%f, %f}", a.x, a.y);
}

function String to_string(Vector2i a) {
    return sprint("Vector2i {%d, %d}", a.x, a.y);
}

function String to_string(Vector3 a) {
    return sprint("Vector3 {%f, %f, %f}", a.x, a.y, a.z);
}

function String to_string(Vector3i a) {
    return sprint("Vector3i {%d, %d, %d}", a.x, a.y, a.z);
}

function String to_string(Vector4 a) {
    return sprint("Vector4 {%f, %f, %f, %f}", a.x, a.y, a.z, a.w);
}

function String to_string(Rectangle2 a) {
    return sprint("Rectangle2 {{%f, %f}, {%f, %f}}", a.x0, a.y0, a.x1, a.y1);
}

function String to_string(Rectangle2i a) {
    return sprint("Rectangle2i {{%d, %d}, {%d, %d}}", a.x0, a.y0, a.x1, a.y1);
}

inline String to_string(Rectangle3 a) {
  return sprint("Rectangle3 {{%f, %f, %f}, {%f, %f, %f}}", a.x0, a.y0, a.z0, a.x1, a.y1, a.z1);
}

function String to_string(Matrix3 a) {
    return sprint(
        "Matrix3 {\n  {%f, %f, %f},\n  {%f, %f, %f},\n  {%f, %f, %f},\n}",
        a.rows[0].x, a.rows[0].y, a.rows[0].z,
        a.rows[1].x, a.rows[1].y, a.rows[1].z,
        a.rows[2].x, a.rows[2].y, a.rows[2].z
    );
}

function String to_string(Matrix4 a) {
    return sprint(
        "Matrix4 {\n  {%f, %f, %f, %f},\n  {%f, %f, %f, %f},\n  {%f, %f, %f, %f},\n  {%f, %f, %f, %f}\n}",
        a.rows[0].x, a.rows[0].y, a.rows[0].z, a.rows[0].w,
        a.rows[1].x, a.rows[1].y, a.rows[1].z, a.rows[1].w,
        a.rows[2].x, a.rows[2].y, a.rows[2].z, a.rows[2].w,
        a.rows[3].x, a.rows[3].y, a.rows[3].z, a.rows[3].w
    );
}

function String to_string(Quaternion a) {
    return sprint("Quaternion {%f, %f, %f, %f}", a.x, a.y, a.z, a.w);
}

#endif // BASE_STRINGS_H

//
// Collisions
//

function b32 rectangle_circle_collision(Rectangle2 rect, Vector2 circle, f32 radius) {
    f32 dx = circle.x - clamp_f32(circle.x, rect.x0, rect.x1);
    f32 dy = circle.y - clamp_f32(circle.y, rect.y0, rect.y1);
    return dx * dx + dy * dy < radius * radius;
}

function b32 circle_intersects(Vector2 c0, f32 r0, Vector2 c1, f32 r1) {
    f32 radius = r0 + r1;
    return v2_length_squared(v2_sub(c0, c1)) <= (radius * radius);
}

function b32 line_intersects_line(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 *return_point) {
    f32 s1_x = p1.x - p0.x;
    f32 s1_y = p1.y - p0.y;
    f32 s2_x = p3.x - p2.x;
    f32 s2_y = p3.y - p2.y;

    f32 s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
    f32 t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (return_point) *return_point = v2_add(p0, v2_mulf(p1, t));

    return s >= 0 && s <= 1 && t >= 0 && t <= 1;
}

function b32 line_intersects_rectangle(Vector2 p0, Vector2 p1, Rectangle2 rect)
{
    Vector2 tl = v2(rect.x0, rect.y0);
    Vector2 tr = v2(rect.x1, rect.y0);
    Vector2 bl = v2(rect.x0, rect.y1);
    Vector2 br = v2(rect.x1, rect.y1);

    if (line_intersects_line(p0, p1, tl, tr, 0)) return true;
    if (line_intersects_line(p0, p1, tr, br, 0)) return true;
    if (line_intersects_line(p0, p1, bl, br, 0)) return true;
    if (line_intersects_line(p0, p1, tl, bl, 0)) return true;
    return false;
}

function b32 line_intersects_circle(Vector2 p1, Vector2 p2, Vector2 circle, f32 radius) {
    Vector2 ac = v2(circle.x - p1.x, circle.y - p1.y);
    Vector2 ab = v2(p2.x - p1.x, p2.y - p1.y);
    f32 ab2 = v2_dot(ab, ab);
    f32 acab = v2_dot(ac, ab);
    f32 t = acab / ab2;

    t = (t < 0) ? 0 : t;
    t = (t > 1) ? 1 : t;

    Vector2 h = v2((ab.x * t + p1.x) - circle.x, (ab.y * t + p1.y) - circle.y);
    f32 h2 = v2_dot(h, h);
    return h2 <= radius * radius;
}

function Vector3 screen_to_ndc(Vector2 window, Vector2 screen)
{
    f32 x = (2.0f * screen.x) / window.width - 1.0f;
    f32 y = 1.0f - (2.0f * screen.y) / window.height;
    return v3(x, y, 1.0f);
}

function Vector3 camera_ray_from_point(Matrix4 projection_matrix, Matrix4 view_matrix, Vector2 window, Vector2 point)
{
    // http://antongerdelan.net/opengl/raycasting.html
    // normalized device coordinates
    f32 x = (2.0f * point.x) / window.width - 1.0f;
    f32 y = 1.0f - (2.0f * point.y) / window.height;
    f32 z = 1.0f;
    Vector3 ray_nds = v3(x, y, z);
    // homogenous clip coordinates
    Vector4 ray_clip = v4(ray_nds.x, ray_nds.y, -1.0, 1.0);

    // 4d eye (camera) coordinates
    Vector4 ray_eye = matrix4_mulv4(matrix4_inverse(projection_matrix), ray_clip);
    ray_eye = v4(ray_eye.x, ray_eye.y, -1.0, 0.0); // forwards, not a point

    // 4d world coordinates
    Vector3 ray_world = matrix4_mulv4(matrix4_inverse(view_matrix), ray_eye).xyz;
    // don't forget to normalise the vector at some point
    return v3_normalize(ray_world);
}

function Vector3 world_to_ndc(Matrix4 proj, Vector3 world)
{
    // homogenous = P * M * world
    Vector4 h = matrix4_transform_v4(proj, v4_from_v3f(world, 1.0f));

    // Convert from homogenous coordinates to [-1, 1] cube
    return v3_divf(h.xyz, h.w);
}

// https://stackoverflow.com/questions/8491247/c-opengl-convert-world-coords-to-screen2d-coords
function Vector3 world_to_screen(Matrix4 proj, Vector3 world, Vector2 window)
{
    // homogenous = P * M * world
    Vector4 h = matrix4_transform_v4(proj, v4_from_v3f(world, 1.0f));

    // Convert from homogenous coordinates to [-1, 1] cube
    Vector3 ndc = v3_divf(h.xyz, h.w);

    // Convert from cube to screen position
    Vector2 view_offset = v2(0, 0);
    Vector2 view_size =   window;

    f32 x = ((ndc.x + 1.0f) / 2.0f) * view_size.x + view_offset.x;
    f32 y = ((1.0f - ndc.y) / 2.0f) * view_size.y + view_offset.y;
    //f32 z = ((ndc.z + 1.0) / 2.0);
    f32 z = ndc.z;

    return v3(x, y, z);
}

function b32 point_in_triangle(Vector2 point, Vector2 v1, Vector2 v2, Vector2 v3)
{
    #define triangle_sign(p1, p2, p3) (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y)

    f32 d1 = triangle_sign(point, v1, v2);
    f32 d2 = triangle_sign(point, v2, v3);
    f32 d3 = triangle_sign(point, v3, v1);

    #undef triangle_sign

    b32 has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    b32 has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

#endif // NA_MATH_H
