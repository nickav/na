/*
    na_math.h - v0.02
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
    0.02  - Breaking API changes
    0.01  - Initial version
*/

#ifndef NA_MATH_H
#define NA_MATH_H

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

#ifndef NULL
    #define NULL 0
#endif

#ifndef null
    #define null nullptr
#endif

#if LANG_C
    #define bool int
    #define true 1
    #define false 0
#endif

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

//
// end of shared imports
//

#include <math.h>

#define PI  3.14159265359f
#define TAU 6.28318530717958647692f

#define EPSILON_F32 1.1920929e-7f
#define EPSILON_F64 2.220446e-16

#define SQRT_2 0.70710678118

#define Mod(x, y) fmodf(x, y)
#define AbsF32(x) fabsf(x)
#define Sqrt(x) sqrtf(x)
#define Sin(x) sinf(x)
#define Cos(x) cosf(x)
#define Tan(x) tanf(x)
#define Pow(x, e) powf(x, e)

#define v2f(x, y)       v2((f32)(x), (f32)(y))
#define v3f(x, y, z)    v2((f32)(x), (f32)(y), (f32)(z))
#define v4f(x, y, z, w) v2((f32)(x), (f32)(y), (f32)(z), (f32)(w))


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

union Vector3
{
    struct {
        f32 x, y, z;
    };

    struct {
        f32 r, g, b;
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

struct Rectangle2
{
    union {
        Vector2 p0;
        Vector2 u;

        struct {
            f32 x0, y0;
        };
    };

    union {
        Vector2 p1;
        Vector2 v;

        struct {
            f32 x1, y1;
        };
    };
};

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

union Matrix3
{
    Vector3 rows[3];

    // NOTE(nick): These are stored row major - e[row][coulumn]
    f32 e[3][3];
};

union Matrix4
{
    Vector4 rows[4];

    // NOTE(nick): These are stored row major - e[row][coulumn]
    f32 e[4][4];
};

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

const Vector3 v3_zero = {0, 0, 0};
const Vector3 v3_one  = {1, 1, 1};

const Vector4 v4_zero = {0, 0, 0, 0};
const Vector4 v4_one  = {1, 1, 1, 1};

const Vector4 v4_black   = {0, 0, 0, 1};
const Vector4 v4_white   = {1, 1, 1, 1};
const Vector4 v4_red     = {1, 0, 0, 1};
const Vector4 v4_green   = {0, 1, 0, 1};
const Vector4 v4_blue    = {0, 0, 1, 1};
const Vector4 v4_cyan    = {0, 1, 1, 1};
const Vector4 v4_magenta = {1, 0, 1, 1};
const Vector4 v4_yellow  = {1, 1, 0, 1};

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

function i32 clamp_i32(i32 value, i32 lower, i32 upper) { return Clamp(value, lower, upper); }
function u32 clamp_u32(u32 value, u32 lower, u32 upper) { return Clamp(value, lower, upper); }
function u64 clamp_u64(u64 value, u64 lower, u64 upper) { return Clamp(value, lower, upper); } 
function f32 clamp_f32(f32 value, f32 lower, f32 upper) { return Clamp(value, lower, upper); }
function f64 clamp_f64(f64 value, f64 lower, f64 upper) { return Clamp(value, lower, upper); }

function f32 wrap_f32(f32 value, f32 lower, f32 upper) {
    f32 range = upper - lower;
    while (value > upper) value -= range;
    while (value < lower) value += range;
    return value;
}

function i32 sign_i32(i32 a) { return Sign(a); }
function i32 sign_i64(i64 a) { return Sign(a); }
function f32 sign_f32(f32 a) { return Sign(a); }
function f64 sign_f64(f64 a) { return Sign(a); }

function i32 abs_i32(i32 a) { return Abs(a); }
function i32 abs_i64(i64 a) { return Abs(a); }
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

function f32 move_f32(f32 a, f32 b, f32 amount) {
    return (a > b) ? max_f32(a - amount, b) : min_f32(a + amount, b);
}

function f32 snap_f32(f32 value, f32 grid) {
    return round_f32(value / grid) * grid;
}

//
// Vector2
//

function Vector2 v2(f32 x, f32 y) {
    return {x, y};
}

function Vector2 v2_from_v2i(Vector2i v) {
    return {(f32)v.x, (f32)v.y};
}

function b32 v2_equals(Vector2 a, Vector2 b) {
    return abs_f32(a.x - b.x) <= EPSILON_F32 && abs_f32(a.y - b.y) <= EPSILON_F32;
}

function b32 v2_is_zero(Vector2 a) {
    return v2_equals(a, v2_zero);
}

function Vector2 v2_add(Vector2 a, Vector2 b) {
    return {a.x + b.x, a.y + b.y};
}

function Vector2 v2_sub(Vector2 a, Vector2 b) {
    return {a.x - b.x, a.y - b.y};
}

function Vector2 v2_inv(Vector2 a) {
    return {-a.x, -a.y};
}

function Vector2 v2_mul(Vector2 a, Vector2 b) {
    return {a.x * b.x, a.y * b.y};
}

function Vector2 v2_mulf(Vector2 a, f32 s) {
    return {a.x * s, a.y * s};
}

function Vector2 v2_divf(Vector2 a, f32 s) {
    return {a.x / s, a.y / s};
}

function Vector2 v2_div(Vector2 a, Vector2 b) {
    return {
        a.x / b.x,
        a.y / b.y,
    };
}

function f32 v2_dot(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

function Vector2 v2_perp(Vector2 a) {
    return {-a.y, a.x};
}

function f32 v2_length_squared(Vector2 a) {
    return v2_dot(a, a);
}

function f32 v2_length(Vector2 a) {
    return Sqrt(v2_length_squared(a));
}

function Vector2 v2_normalize(Vector2 a) {
    f32 f = 1.0f / v2_length(a);
    return {a.x * f, a.y * f};
}

// Functions:

function Vector2 lerp_v2(Vector2 a, Vector2 b, f32 t) {
    return {
        lerp_f32(a.x, b.x, t),
        lerp_f32(a.y, b.y, t),
    };
}

function Vector2 min_v2(Vector2 a, Vector2 b) {
    return {
        min_f32(a.x, b.x),
        min_f32(a.y, b.y),
    };
}

function Vector2 max_v2(Vector2 a, Vector2 b) {
    return {max_f32(a.x, b.x), max_f32(a.y, b.y)};
}

function Vector2 abs_v2(Vector2 a) {
    return {abs_f32(a.x), abs_f32(a.y)};
}

function Vector2 floor_v2(Vector2 a) {
    return {floor_f32(a.x), floor_f32(a.y)};
}

function Vector2 round_v2(Vector2 a) {
    return {round_f32(a.x), round_f32(a.y)};
}

function Vector2 ceil_v2(Vector2 a) {
    return {ceil_f32(a.x), ceil_f32(a.y)};
}

function Vector2 sign_v2(Vector2 a) {
    return {(f32)Sign(a.x), (f32)Sign(a.y)};
}

function Vector2 move_v2(Vector2 a, Vector2 b, f32 amount) {
    return {move_f32(a.x, b.x, amount), move_f32(a.y, b.y, amount)};
}

function Vector2 snap_v2(Vector2 pos, Vector2 grid) {
    return {snap_f32(pos.x, grid.x), snap_f32(pos.y, grid.y)};
}

function Vector2 clamp_v2(Vector2 a, Vector2 min, Vector2 max) {
    return {clamp_f32(a.x, min.x, max.x), clamp_f32(a.y, min.y, max.y)};
}

#if LANG_CPP

function Vector2 operator+(Vector2 a, Vector2 b) {
    return v2_add(a, b);
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

function Vector2 operator/(Vector2 a, f32 b) {
    return v2_divf(a, b);
}

function Vector2 operator/(f32 a, Vector2 b) {
    return {a / b.x, a / b.y};
}

function Vector2 operator/(Vector2 a, Vector2 b) {
    return v2_div(a, b);
}

function Vector2 &operator/=(Vector2 &a, f32 b) {
    return a = a / b;
}

#endif // LANG_CPP

//
// Vector2i
//

function Vector2i v2i(i32 x, i32 y) {
    return {x, y};
}

function Vector2i v2i_from_v2(Vector2 v) {
    return {(i32)v.x, (i32)v.y};
}

function b32 v2i_equals(Vector2i a, Vector2i b) {
    return a.x == b.x && a.y == b.y;
}

function b32 v2i_is_zero(Vector2i a) {
    return a.x == 0 && a.y == 0;
}

function Vector2i v2i_add(Vector2i a, Vector2i b) {
    return {a.x + b.x, a.y + b.y};
}

function Vector2i v2i_sub(Vector2i a, Vector2i b) {
    return {a.x - b.x, a.y - b.y};
}

function Vector2i v2i_inv(Vector2i a) {
    return {-a.x, -a.y};
}

function Vector2i v2i_mulf(Vector2i a, f32 b) {
    return {(i32)(a.x * b), (i32)(a.y * b)};
}

function Vector2i v2i_muli(Vector2i a, i32 b) {
    return {(a.x * b), (a.y * b)};
}

function Vector2i v2i_mul(Vector2i a, Vector2i b) {
    return {(a.x * b.x), (a.y * b.y)};
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
    return {x, y, z};
}

function Vector3 v3_from_v3i(Vector3i v) {
    return {(f32)v.x, (f32)v.y, (f32)v.z};
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
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

function Vector3 v3_sub(Vector3 a, Vector3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

function Vector3 v3_inv(Vector3 a) {
    return {-a.x, -a.y, -a.z};
}

function Vector3 v3_mul(Vector3 a, Vector3 b) {
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}

function Vector3 v3_mulf(Vector3 a, f32 s) {
    return {a.x * s, a.y * s, a.z * s};
}

function Vector3 v3_divf(Vector3 a, f32 s) {
    return {a.x / s, a.y / s, a.z / s};
}

function Vector3 v3_div(Vector3 a, Vector3 b) {
    return {
        a.x / b.x,
        a.y / b.y,
        a.z / b.z,
    };
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

function Vector3 v3_normalize(Vector3 a) {
    f32 f = 1.0f / v3_length(a);
    return {
        a.x * f,
        a.y * f,
        a.z * f,
    };
}

function Vector3 v3_cross(Vector3 a, Vector3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

// Functions:

function Vector3 lerp_v3(Vector3 a, Vector3 b, f32 t) {
    return {
        lerp_f32(a.x, b.x, t),
        lerp_f32(a.y, b.y, t),
        lerp_f32(a.z, b.z, t),
    };
}

function Vector3 clamp_v3(Vector3 value, Vector3 lower, Vector3 upper) {
    return {
        clamp_f32(value.x, lower.x, upper.x),
        clamp_f32(value.y, lower.y, upper.y),
        clamp_f32(value.z, lower.z, upper.z),
    };
}

function Vector3 min_v3(Vector3 a, Vector3 b) {
    return {
        min_f32(a.x, b.x),
        min_f32(a.y, b.y),
        min_f32(a.z, b.z),
    };
}

function Vector3 max_v3(Vector3 a, Vector3 b) {
    return {
        max_f32(a.x, b.x),
        max_f32(a.y, b.y),
        max_f32(a.z, b.z),
    };
}

function Vector3 sign_v3(Vector3 a) {
    return {
        sign_f32(a.x),
        sign_f32(a.y),
        sign_f32(a.z),
    };
}

function Vector3 abs_v3(Vector3 a) {
    return {
        abs_f32(a.x),
        abs_f32(a.y),
        abs_f32(a.z),
    };
}

function Vector3 round_v3(Vector3 a) {
    return {
        round_f32(a.x),
        round_f32(a.y),
        round_f32(a.z),
    };
}

function Vector3 floor_v3(Vector3 a) {
    return {
        floor_f32(a.x),
        floor_f32(a.y),
        floor_f32(a.z),
    };
}

function Vector3 ceil_v3(Vector3 a) {
    return {
        ceil_f32(a.x),
        ceil_f32(a.y),
        ceil_f32(a.z),
    };
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
    return {x, y, z};
}

function Vector3i v3i_from_v3(Vector3 v) {
    return {(i32)v.x, (i32)v.y, (i32)v.z};
}

function b32 v3i_equals(Vector3i a, Vector3i b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

function b32 v3i_is_zero(Vector3i a) {
    return a.x == 0 && a.y == 0 && a.z == 0;
}

function Vector3i v3i_add(Vector3i a, Vector3i b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

function Vector3i v3i_sub(Vector3i a, Vector3i b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
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
    return {x, y, z, w};
}

function Vector4 v4_from_v2(Vector2 a, Vector2 b) {
    return {a.x, a.y, b.x, b.y};
}

function Vector4 v4_from_v3f(Vector3 v, f32 w) {
    return {v.x, v.y, v.z, w};
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
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

function Vector4 v4_sub(Vector4 a, Vector4 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

function Vector4 v4_inv(Vector4 a) {
    return {-a.x, -a.y, -a.z, -a.w};
}

function Vector4 v4_mul(Vector4 a, Vector4 b) {
    return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

function Vector4 v4_mulf(Vector4 a, f32 f) {
    return {a.x * f, a.y * f, a.z * f, a.w * f};
}

function Vector4 v4_div(Vector4 a, Vector4 b) {
    return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

function Vector4 v4_divf(Vector4 a, f32 f) {
    return {a.x / f, a.y / f, a.z / f, a.w / f};
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
    f32 f = 1.0f / v4_length(a);
    return {
        a.x * f,
        a.y * f,
        a.z * f,
        a.w * f,
    };
}

// Functions:

function Vector4 lerp_v4(Vector4 a, Vector4 b, f32 t) {
    return {
        lerp_f32(a.x, b.x, t),
        lerp_f32(a.y, b.y, t),
        lerp_f32(a.z, b.z, t),
        lerp_f32(a.w, b.w, t),
    };
}

function Vector4 clamp_v4(Vector4 value, Vector4 lower, Vector4 upper) {
    return {
        clamp_f32(value.x, lower.x, upper.x),
        clamp_f32(value.y, lower.y, upper.y),
        clamp_f32(value.z, lower.z, upper.z),
        clamp_f32(value.w, lower.w, upper.w),
    };
}

function Vector4 min_v4(Vector4 a, Vector4 b) {
    return {
        min_f32(a.x, b.x),
        min_f32(a.y, b.y),
        min_f32(a.z, b.z),
        min_f32(a.w, b.w),
    };
}

function Vector4 max_v4(Vector4 a, Vector4 b) {
    return {
        max_f32(a.x, b.x),
        max_f32(a.y, b.y),
        max_f32(a.z, b.z),
        max_f32(a.w, b.w),
    };
}

function Vector4 sign_v4(Vector4 a) {
    return {
        sign_f32(a.x),
        sign_f32(a.y),
        sign_f32(a.z),
        sign_f32(a.w),
    };
}

function Vector4 abs_v4(Vector4 a) {
    return {
        abs_f32(a.x),
        abs_f32(a.y),
        abs_f32(a.z),
        abs_f32(a.w),
    };
}

function Vector4 round_v4(Vector4 a) {
    return {
        round_f32(a.x),
        round_f32(a.y),
        round_f32(a.z),
        round_f32(a.w),
    };
}

function Vector4 floor_v4(Vector4 a) {
    return {
        floor_f32(a.x),
        floor_f32(a.y),
        floor_f32(a.z),
        floor_f32(a.w),
    };
}

function Vector4 ceil_v4(Vector4 a) {
    return {
        ceil_f32(a.x),
        ceil_f32(a.y),
        ceil_f32(a.z),
        ceil_f32(a.w),
    };
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

#endif // LANG_CPP

//
// Vector4i
//

function Vector4i v4i(i32 x, i32 y, i32 z, i32 w) {
    return {x, y, z, w};
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
    return {min, max};
}

function Rectangle2 r2_from_4f(f32 x0, f32 y0, f32 x1, f32 y1) {
    return {{x0, y0}, {x1, y1}};
}

function b32 r2_intersects(Rectangle2 r0, Rectangle2 r1) {
    return (r0.x0 < r1.x1 && r0.x1 > r1.x0 && r0.y0 < r1.y1 && r0.y1 > r1.y0);
}

function b32 r2_contains(Rectangle2 r, Vector2 p) {
    return (p.x >= r.x0 && p.x < r.x1 && p.y >= r.y0 && p.y < r.y1);
}

function Vector2 r2_size(Rectangle2 r) {
    return v2(r.x1 - r.x0, r.y1 - r.y0);
}

function Vector2 r2_center(Rectangle2 r) {
    Vector2 s = r2_size(r);
    return {r.x0 + s.x / 2, r.y0 + s.y / 2};
}

function Rectangle2 r2_split_from_bottom(Rectangle2 rect, f32 amount) {
    return r2(v2(rect.x0, rect.y1 - amount), rect.p1);
}

function Rectangle2 r2_split_from_top(Rectangle2 rect, f32 amount) {
    return r2(rect.p0, v2(rect.x1, rect.y0 + amount));
}

function Rectangle2 r2_trim_from_top(Rectangle2 rect, f32 amount) {
    return r2(v2(rect.x0, rect.y0 + amount), rect.p1);
}

function Rectangle2 r2_split_from_left(Rectangle2 rect, f32 amount) {
    return r2(rect.p0, v2(rect.x0 + amount, rect.y1));
}

function Rectangle2 r2_split_from_right(Rectangle2 rect, f32 amount) {
    return r2(v2(rect.x1 - amount, rect.y0), rect.p1);
}

// Functions:

function Rectangle2 abs_r2(Rectangle2 rect) {
    Vector2 pos = {min_f32(rect.x0, rect.x1), min_f32(rect.y0, rect.y1)};
    return {pos, pos + abs_v2(r2_size(rect))};
}

function Rectangle2 union_r2(Rectangle2 a, Rectangle2 b) {
    a = abs_r2(a);
    b = abs_r2(b);
    return {min_v2(a.p0, b.p0), max_v2(a.p1, b.p1)};
}

function Rectangle2 intersection_r2(Rectangle2 a, Rectangle2 b) {
    a = abs_r2(a);
    b = abs_r2(b);
    return {max_v2(a.p0, b.p0), min_v2(a.p1, b.p1)};
}

function Rectangle2 shift_r2(Rectangle2 a, Vector2 b) {
    return {a.p0 + b, a.p1 + b};
}

function Rectangle2 pad_r2(Rectangle2 rect, Vector2 padding) {
    return r2(rect.p0 - padding, rect.p1 + padding);
}

function Rectangle2 pad_r2f(Rectangle2 rect, f32 padding) {
    return pad_r2(rect, v2(padding, padding));
}

function Rectangle2 r2_bounds(Vector2 pos, Vector2 size, Vector2 anchor, Vector2 scale)
{
    Vector2 ss = size * scale;
    Vector2 p0 = pos - anchor * ss;
    return abs_r2(Rectangle2{p0, p0 + ss});
}

#if LANG_CPP

function Rectangle2 operator+(Rectangle2 a, Vector2 b) {
    return shift_r2(a, b);
}

function Rectangle2 operator+(Vector2 b, Rectangle2 a) {
    return shift_r2(a, b);
}

#endif // LANG_CPP

//
// Rectangle2i
// 

function Rectangle2i r2i(i32 x0, i32 y0, i32 x1, i32 y1) {
    return {{x0, y0}, {x1, y1}};
}

function Vector2i r2i_size(Rectangle2i r) {
    return {r.x1 - r.x0, r.y1 - r.y0};
}

//
// Rectangle3
//

function Rectangle3 r3(Vector3 p0, Vector3 p1) {
    return {p0, p1};
}

function Vector3 r3_size(Rectangle3 r) {
    return {r.x1 - r.x0, r.y1 - r.y0, r.z1 - r.z0};
}

function Vector3 r3_center(Rectangle3 r) {
    Vector3 s = r3_size(r);
    return {r.x0 + s.x / 2, r.y0 + s.y / 2, r.z0 + s.z / 2};
}

b32 r3_intersects(Rectangle3 a, Rectangle3 b) {
    return (
        (b.x1 > a.x0) && (b.x0 < a.x1) &&
        (b.y1 > a.y0) && (b.y0 < a.y1) &&
        (b.z1 > a.z0) && (b.z0 < a.z1)
    );
}

b32 r3_contains(Rectangle3 rect, Vector3 p) {
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
    return {{
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
    }};
}

function Vector3 matrix3_column(Matrix3 *it, u32 i) {
    assert(i >= 0 && i < 3);
    return {it->e[0][i], it->e[1][i], it->e[2][i]};
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

function Vector3 matrix3_transform_v3(Matrix3 a, Vector3 p) {
    return {
        v3_dot(p, a.rows[0]),
        v3_dot(p, a.rows[1]),
        v3_dot(p, a.rows[2]),
    };
}

//
// Matrix4
//

function Matrix4 matrix4_identity() {
    return {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};
}

function Vector4 matrix4_column(Matrix4 *it, u32 i) {
    assert(i >= 0 && i < 4);
    return {it->e[0][i], it->e[1][i], it->e[2][i], it->e[3][i]};
}

function Vector4 matrix4_transform_v4(Matrix4 a, Vector4 p) {
    return {
        v4_dot(p, a.rows[0]),
        v4_dot(p, a.rows[1]),
        v4_dot(p, a.rows[2]),
        v4_dot(p, a.rows[3]),
    };
}

function Vector3 matrix4_transform_v3(Matrix4 a, Vector3 p) {
    return matrix4_transform_v4(a, v4_from_v3f(p, 1.0f)).xyz;
}

function Vector2 matrix4_transform_v2(Matrix4 m, Vector2 src) {
    f32 x = (m.e[0][0] * src.x) + (m.e[0][1] * src.y) + (0) + (m.e[0][3]);
    f32 y = (m.e[1][0] * src.x) + (m.e[1][1] * src.y) + (0) + (m.e[1][3]);
    return {x, y};
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

    return {{
        {v4_dot(row0, col0), v4_dot(row0, col1), v4_dot(row0, col2), v4_dot(row0, col3)},
        {v4_dot(row1, col0), v4_dot(row1, col1), v4_dot(row1, col2), v4_dot(row1, col3)},
        {v4_dot(row2, col0), v4_dot(row2, col1), v4_dot(row2, col2), v4_dot(row2, col3)},
        {v4_dot(row3, col0), v4_dot(row3, col1), v4_dot(row3, col2), v4_dot(row3, col3)},
    }};
}

function Matrix4 matrix4_mulf(Matrix4 a, f32 s) {
    return {{
        a.rows[0] * s,
        a.rows[1] * s,
        a.rows[2] * s,
        a.rows[3] * s,
    }};
}

function Matrix4 matrix4_rotation_x(f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    return {{
        {1, 0, 0, 0},
        {0, c, -s, 0},
        {0, s, c, 0},
        {0, 0, 0, 1},
    }};
}

function Matrix4 matrix4_rotation_y(f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    return {{
        {c, 0, s, 0},
        {0, 1, 0, 0},
        {-s, 0, c, 0},
        {0, 0, 0, 1},
    }};
}

function Matrix4 matrix4_rotation_z(f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    return {{
        {c, -s, 0, 0},
        {s, c, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};
}

function Matrix4 matrix4_translation(Vector3 t) {
    return {{
        {1, 0, 0, t.x},
        {0, 1, 0, t.y},
        {0, 0, 1, t.z},
        {0, 0, 0, 1},
    }};
}

function Matrix4 matrix4_translate(Matrix4 a, Vector3 t) {
    a.e[0][3] += t.x;
    a.e[1][3] += t.y;
    a.e[2][3] += t.z;
    return a;
}

function Matrix4 matrix4_scaling(Vector3 s) {
    return {{
        {s.x, 0, 0, 0},
        {0, s.y, 0, 0},
        {0, 0, s.z, 0},
        {0, 0, 0, 1},
    }};
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

// NOTE(nick): reasonable values are perspective_proj(aspect_ratio, 1.0f, 0.1f, 100.0f);
function Matrix4 matrix4_perspective_proj(f32 aspect_width_over_height, f32 focal_length, f32 near_clip_plane, f32 far_clip_plane) {
    f32 a = 1.0f;
    f32 b = aspect_width_over_height;
    f32 c = focal_length; // NOTE(casey): This should really be called "film back distance"

    f32 n = near_clip_plane; // NOTE(casey): Near clip plane _distance_
    f32 f = far_clip_plane;  // NOTE(casey): Far clip plane _distance_

    // NOTE(casey): These are the perspective corRectangle terms, for when you divide by -z
    f32 d = (n + f) / (n - f);
    f32 e = (2 * f * n) / (n - f);

    return {{
        {a * c, 0, 0, 0},
        {0, b * c, 0, 0},
        {0, 0, d, e},
        {0, 0, -1, 0},
    }};
}

function Matrix4 matrix4_orthographic_proj(f32 aspect_width_over_height, f32 near_clip_plane, f32 far_clip_plane) {
    f32 a = 1.0f;
    f32 b = aspect_width_over_height;

    f32 n = near_clip_plane; // NOTE(casey): Near clip plane _distance_
    f32 f = far_clip_plane;  // NOTE(casey): Far clip plane _distance_

    // NOTE(casey): These are the non-perspective corRectangleed terms, for orthographic
    f32 d = 2.0f / (n - f);
    f32 e = (n + f) / (n - f);

    return {{
        {a, 0, 0, 0},
        {0, b, 0, 0},
        {0, 0, d, e},
        {0, 0, 0, 1},
    }};
}

// NOTE(nick): typically z_near = -1.0f, z_far = 1.0f
function Matrix4 matrix4_orthographic_proj2d(f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far) {
    Matrix4 result = matrix4_identity();
    result.e[0][0] = (f32)(2) / (right - left);
    result.e[1][1] = (f32)(2) / (top - bottom);
    result.e[2][2] = -(f32)(2) / (z_far - z_near);
    result.e[0][3] = -(right + left) / (right - left);
    result.e[1][3] = -(top + bottom) / (top - bottom);
    result.e[2][3] = -(z_far + z_near) / (z_far - z_near);
    return result;
}

#if LANG_CPP

function Matrix4 operator*(Matrix4 a, Matrix4 b) {
    return matrix4_mul(a, b);
}

function Vector3 operator*(Matrix4 a, Vector3 p) {
    return matrix4_transform_v3(a, p);
}

function Vector4 operator*(Matrix4 a, Vector4 p) {
    return matrix4_transform_v4(a, p);
}

function Matrix4 operator*(Matrix4 a, f32 s) {
    return matrix4_mulf(a, s);
}

#endif // LANG_CPP

//
// Quaternion
//

function Quaternion quaternion_make(Vector4 a) {
    return {a.x, a.y, a.z, a.w};
}

function Quaternion quaternion_from_angle(f32 angle, Vector3 v) {
    Vector3 a = v * Sin(angle / 2);
    return {a.x, a.y, a.z, Cos(angle / 2)};
}

function Quaternion quaternion_identity() {
    return {0, 0, 0, 1};
}

function Quaternion quaternion_invert(Quaternion q) {
    return {-q.x, -q.y, -q.z, q.w};
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
    return quaternion_to_rotation_matrix(q) * v;
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

    return {vx, vy, vz, vw};
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
// Misc
//

function f32 angle_between(Vector2 p0, Vector2 p1) {
    return atan2(p1.y - p0.y, p1.x - p0.x);
}

function f32 direction(Vector2 p0) {
    return -atan2(-p0.y, p0.x);
}

function Vector2 arm2(f32 angle) {
    return {Cos(angle), Sin(angle)};
}

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

function Vector4 v4_from_u32(u32 hex)
{
    Vector4 result =
    {
        (f32)((hex & 0xff000000) >> 24) / 255.f,
        (f32)((hex & 0x00ff0000) >> 16) / 255.f,
        (f32)((hex & 0x0000ff00) >>  8) / 255.f,
        (f32)((hex & 0x000000ff) >>  0) / 255.f,
    };
    return result;
}

// NOTE(nick): untested!
function u32 u32_from_v4(Vector4 v)
{
    u32 result =
        ((u32)(v.a * 255.0f)) << 24 |
        ((u32)(v.r * 255.0f)) << 16 |
        ((u32)(v.g * 255.0f)) << 8  |
        ((u32)(v.b * 255.0f)) << 0;
    return result;
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
        "Matrix3 {\n  {%f, %f, %f},\n  {%f, %f, %f},\n  {%f, %f, %f},\n  {%f, %f, %f}\n}",
        a.rows[0].x, a.rows[0].y, a.rows[0].z,
        a.rows[1].x, a.rows[1].y, a.rows[1].z,
        a.rows[2].x, a.rows[2].y, a.rows[2].z,
        a.rows[3].x, a.rows[3].y, a.rows[3].z
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
    f32 dx = circle.x - clamp(circle.x, rect.x0, rect.x1);
    f32 dy = circle.y - clamp(circle.y, rect.y0, rect.y1);
    return dx * dx + dy * dy < radius * radius;
}

function b32 circle_intersects(Vector2 c0, f32 r0, Vector2 c1, f32 r1) {
    f32 radius = r0 + r1;
    return v2_length_squared(c0 - c1) <= (radius * radius);
}

function b32 line_intersects_line(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 *return_point) {
    f32 s1_x = p1.x - p0.x;
    f32 s1_y = p1.y - p0.y;
    f32 s2_x = p3.x - p2.x;
    f32 s2_y = p3.y - p2.y;

    f32 s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
    f32 t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (return_point) *return_point = p0 + p1 * t;

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
    float x = (2.0f * point.x) / window.width - 1.0f;
    float y = 1.0f - (2.0f * point.y) / window.height;
    float z = 1.0f;
    Vector3 ray_nds = v3(x, y, z);
    // homogenous clip coordinates
    Vector4 ray_clip = v4(ray_nds.x, ray_nds.y, -1.0, 1.0);

    // 4d eye (camera) coordinates
    Vector4 ray_eye = matrix4_inverse(projection_matrix) * ray_clip;
    ray_eye = v4(ray_eye.x, ray_eye.y, -1.0, 0.0); // forwards, not a point

    // 4d world coordinates
    Vector3 ray_world = (matrix4_inverse(view_matrix) * ray_eye).xyz;
    // don't forget to normalise the vector at some point
    return v3_normalize(ray_world);
}

function Vector3 world_to_ndc(Matrix4 &proj, Vector3 world)
{
    // homogenous = P * M * world
    Vector4 h = matrix4_transform_v4(proj, v4_from_v3f(world, 1.0f));

    // Convert from homogenous coordinates to [-1, 1] cube
    return h.xyz / h.w;
}

// https://stackoverflow.com/questions/8491247/c-opengl-convert-world-coords-to-screen2d-coords
function Vector3 world_to_screen(Matrix4 &proj, Vector3 world, Vector2 window)
{
    // homogenous = P * M * world
    Vector4 h = matrix4_transform_v4(proj, v4_from_v3f(world, 1.0f));

    // Convert from homogenous coordinates to [-1, 1] cube
    Vector3 ndc = h.xyz / h.w;

    // Convert from cube to screen position
    Vector2 view_offset = v2(0, 0);
    Vector2 view_size =   window;

    f32 x = ((ndc.x + 1.0) / 2.0) * view_size.x + view_offset.x;
    f32 y = ((1.0 - ndc.y) / 2.0) * view_size.y + view_offset.y;
    //f32 z = ((ndc.z + 1.0) / 2.0);
    f32 z = ndc.z;

    return v3(x, y, z);
}

#endif // NA_MATH_H
