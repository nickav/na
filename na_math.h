/*
  na_math.h - v0.01 
  Nick Aversano's C++ math helper library

  This is a single header file with a bunch of useful math functions.
===========================================================================

LICENSE
  This software is dual-licensed to the public domain and under the following
  license: you are granted a perpetual, irrevocable license to copy, modify,
  publish, and distribute this file as you see fit.

CREDITS
  Written by Nick Aversano

VERSION HISTORY
  0.01  - Initial version
*/

#ifndef NA_MATH_H
#define NA_MATH_H

// NOTE(nick): shared with na.h
#ifndef NA_FUNCTIONS
#define NA_FUNCTIONS

#define PI  3.14159265359f
#define TAU 6.28318530717958647692f

#define EPSILON_F32 (1.1920929e-7f)
#define EPSILON_F64 (2.220446e-16)

#define SQRT_2 0.70710678118

#ifndef Min
#define Min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef Max
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef Clamp
#define Clamp(value, lower, upper) (Max(Min(value, upper), lower))
#endif

#ifndef ClampTop
#define ClampTop(a, b) Min(a, b)
#endif

#ifndef ClampBot
#define ClampBot(a, b) Max(a, b)
#endif

#ifndef Sign
#define Sign(x) (((x) > 0) - ((x) < 0))
#endif

#ifndef Abs
#define Abs(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef AlignUpPow2
#define AlignUpPow2(x, p) (((x) + (p) - 1) & ~((p) - 1))
#endif

#ifndef AlignDownPow2
#define AlignDownPow2(x, p) ((x) & ~((p) - 1))
#endif

inline i32 min_i32(i32 a, i32 b) { return Min(a, b); }
inline u32 min_u32(u32 a, u32 b) { return Min(a, b); }
inline i64 min_i64(i64 a, i64 b) { return Min(a, b); }
inline u64 min_u64(u64 a, u64 b) { return Min(a, b); }
inline f32 min_f32(f32 a, f32 b) { return Min(a, b); }
inline f64 min_f64(f64 a, f64 b) { return Min(a, b); }

inline i32 max_i32(i32 a, i32 b) { return Max(a, b); }
inline u32 max_u32(u32 a, u32 b) { return Max(a, b); }
inline i64 max_i64(i64 a, i64 b) { return Max(a, b); }
inline u64 max_u64(u64 a, u64 b) { return Max(a, b); }
inline f32 max_f32(f32 a, f32 b) { return Max(a, b); }
inline f64 max_f64(f64 a, f64 b) { return Max(a, b); }

inline i32 clamp_i32(i32 value, i32 lower, i32 upper) { return Clamp(value, lower, upper); }
inline u32 clamp_u32(u32 value, u32 lower, u32 upper) { return Clamp(value, lower, upper); }
inline u64 clamp_u64(u64 value, u64 lower, u64 upper) { return Clamp(value, lower, upper); } 
inline f32 clamp_f32(f32 value, f32 lower, f32 upper) { return Clamp(value, lower, upper); }
inline f64 clamp_f64(f64 value, f64 lower, f64 upper) { return Clamp(value, lower, upper); }

inline i32 sign_i32(i32 a) { return Sign(a); }
inline i32 sign_i64(i64 a) { return Sign(a); }
inline f32 sign_f32(f32 a) { return Sign(a); }
inline f64 sign_f64(f64 a) { return Sign(a); }

inline i32 abs_i32(i32 a) { return Abs(a); }
inline i32 abs_i64(i64 a) { return Abs(a); }
inline f32 abs_f32(f32 x) {
  union {
    f32 f;
    u32 i;
  } u = {x};
  u.i &= 0x7fffffff;
  return u.f;
}
inline f64 abs_f64(f64 x) {
  union {
    f64 f;
    u64 i;
  } u = {x};
  u.i &= 0x7fffffffffffffff;
  return u.f;
}

inline f32 lerp(f32 a, f32 b, f32 t) {
  return (1 - t) * a + b * t;
}

inline f32 unlerp(f32 a, f32 b, f32 v) {
  return (v - a) / (b - a);
}

#endif // NA_FUNCTIONS

#ifdef sin_f32

#include <math.h>
// @Incomplete

#else // NA_MATH_USE_STDLIB

#if defined(__APPLE__)
#include <xmmintrin.h>
#endif


#define EPSILON_F32 (1.1920929e-7f)
#define EPSILON_F64 (2.220446e-16)

na_inline f32 sqrt_f32(f32 x) {
  return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
}

na_inline i32 round_i32(f32 x) {
  return (x >= 0) ? (i32)(x + 0.5) : (i32)(x - 0.5);
}

na_inline i64 round_i64(f64 x) {
  return (x >= 0) ? (i64)(x + 0.5) : (i64)(x - 0.5);
}

na_inline f32 round_f32(f32 x) { return (f32)round_i32(x); }
na_inline f64 round_f64(f64 x) { return (f64)round_i64(x); }

na_inline i32 floor_i32(f32 x) {
  return (i32)x - (x < 0);
}

na_inline i64 floor_i64(f64 x) {
  return (i64)x - (x < 0);
}

na_inline f32 floor_f32(f32 x) { return (f32)floor_i32(x); }
na_inline f64 floor_f64(f64 x) { return (f64)floor_i64(x); }

na_inline i32 ceil_i32(f32 x) {
  return (i32)(x + 1 - EPSILON_F64);
}

na_inline i64 ceil_i64(f64 x) {
  return (i64)(x + 1 - EPSILON_F64);
}

na_inline f32 ceil_f32(f32 x) { return (f32)ceil_i32(x); }
na_inline f64 ceil_f64(f64 x) { return (f64)ceil_i64(x); }

f32 mod_f32(f32 a, f32 b) {
  return a - (i32)(a / b) * b;
}

// fast sin function; maximum error is 0.001
na_inline f32 sin_f32(f32 x) {
  x = x * (1 / PI);
  i32 k = (i32)round_i32(x);
  x = x - k;

  f32 y = (4 - 4 * abs_f32(x)) * x;

  y = 0.225f * (y * abs_f32(y) - y) + y;

  return (k & 1) ? -y : y;
}

na_inline f32 cos_f32(f32 x) {
  return sin_f32((PI / 2) - x);
}

na_inline f32 tan_f32(f32 x) {
  return sin_f32(x) / cos_f32(x);
}

na_inline f32 atan2_f32(f32 y, f32 x) {
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

na_inline f32 acos_f32(f32 x) {
  f32 negate = f32(x < 0);
  x = abs_f32(x);
  f32 ret = -0.0187293f;
  ret = ret * x;
  ret = ret + 0.0742610f;
  ret = ret * x;
  ret = ret - 0.2121144f;
  ret = ret * x;
  ret = ret + 1.5707288f;
  ret = ret * sqrt_f32(1.0f - x);
  ret = ret - 2 * negate * ret;
  return negate * PI + ret;
}

#endif // NA_MATH_USE_STDLIB


// @Incomplete: do we want to use these intrinsics?
na_inline u32 round_f32_to_u32(f32 x) {
  return cast(u32)_mm_cvtss_si32(_mm_set_ss(x));
}

na_inline f32 lengthdir_x(f32 length, f32 angle) {
  return cos_f32(angle) * length;
}

na_inline f32 lengthdir_y(f32 length, f32 angle) {
  return -sin_f32(angle) * length;
}

na_inline f32 snap(f32 value, f32 grid) {
  return round_f32(value / grid) * grid;
}

f32 smoothstep(f32 a, f32 b, f32 t) {
  // Scale, bias and saturate to 0..1 range
  f32 x = clamp_f32((t - a) / (b - a), 0.0f, 1.0f);
  // Evaluate polynomial
  return x * x * (3 - 2 * x);
}

f32 move(f32 a, f32 b, f32 amount) {
  // @Speed: this probably can be made faster
  if (a > b) {
    a -= amount;
    if (a < b) a = b;
  } else {
    a += amount;
    if (a > b) a = b;
  }

  return a;
}

f32 cubic_bezier(f32 p0, f32 p1, f32 p2, f32 p3, f32 t) {
  f32 nt = (1 - t);

  f32 s0 = nt * nt * nt;
  f32 s1 = 3.0 * nt * nt * t;
  f32 s2 = 3.0 * nt * t * t;
  f32 s3 = t * t * t;

  f32 result = (p0 * s0) + (p1 * s1) + (p2 * s2) + (p3 * s3);
  return result;
}


#define v2  make_vector2
#define v2u make_vector2u
#define v2i make_vector2i
#define v3  make_vector3
#define v3i make_vector3i
#define v4  make_vector4
#define r2  make_rectangle2
//#define r2u make_rectangle2u
//#define r2i make_rectangle2i
#define r3  make_rectangle3

#define v2f(x, y)        make_vector2((f32)(x), (f32)(y))
#define v3f(x, y, z)     make_vector2((f32)(x), (f32)(y), (f32)(z))
#define v4f(x, y, z, w)  make_vector2((f32)(x), (f32)(y), (f32)(z), (f32)(w))


#define v2_zero v2(0, 0)
#define v2_one  v2(1, 1)

#define v3_zero v3(0, 0, 0)
#define v3_one  v3(1, 1, 1)

#define v4_zero v4(0, 0, 0, 0)
#define v4_one  v3(1, 1, 1, 1)

#define v4_black   make_vector4(0, 0, 0, 1)
#define v4_white   make_vector4(1, 1, 1, 1)
#define v4_red     make_vector4(1, 0, 0, 1)
#define v4_green   make_vector4(0, 1, 0, 1)
#define v4_blue    make_vector4(0, 0, 1, 1)
#define v4_cyan    make_vector4(0, 1, 1, 1)
#define v4_magenta make_vector4(1, 0, 1, 1)
#define v4_yellow  make_vector4(1, 1, 0, 1)

union Vector2 {
  struct {
    f32 x, y;
  };

  struct {
    f32 width, height;
  };

  f32 e[2];
};

union Vector2u {
  struct {
    u32 x, y;
  };

  struct {
    u32 width, height;
  };

  u32 e[2];
};

union Vector2i {
  struct {
    i32 x, y;
  };

  struct {
    i32 width, height;
  };

  i32 e[2];
};

union Vector3 {
  struct {
    f32 x, y, z;
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

union Vector3i {
  struct {
    i32 x, y, z;
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

union Vector4 {
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

struct Rectangle2 {
  union {
    Vector2 p0;

    struct {
      f32 x0, y0;
    };
  };

  union {
    Vector2 p1;

    struct {
      f32 x1, y1;
    };
  };
};

struct Rectangle2u {
  union {
    Vector2u p0;

    struct {
      u32 x0, y0;
    };
  };

  union {
    Vector2u p1;

    struct {
      u32 x1, y1;
    };
  };
};

struct Rectangle2i {
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

union Matrix3 {
  Vector3 rows[3];

  // NOTE(nick): These are stored row major - e[row][coulumn]
  f32 e[3][3];
};

union Matrix4 {
  Vector4 rows[4];

  // NOTE(nick): These are stored row major - e[row][coulumn]
  f32 e[4][4];
};

struct Quaternion {
  f32 x, y, z, w;
};

//
// Vector2u
//

inline Vector2u make_vector2u(u32 x, u32 y) {
  return {x, y};
}

inline Vector2u make_vector2u(Vector2 v) {
  return {cast(u32)v.x, cast(u32)v.y};
}

inline Vector2u operator+(Vector2u a, Vector2u b) {
  return {a.x + b.x, a.y + b.y};
}

inline Vector2u operator*(u32 a, Vector2u b) {
  return {a * b.x, a * b.y};
}

inline Vector2u operator*(Vector2u a, f32 b) {
  return (u32)b * a;
}

inline Vector2u operator/(Vector2u a, f32 b) {
  return {(u32)(a.x / b), (u32)(a.y / b)};
}

//
// Vector2i
//

inline Vector2i make_vector2i(i32 x, i32 y) {
  return {x, y};
}

inline Vector2i make_vector2i(Vector2 v) {
  return {cast(i32)v.x, cast(i32)v.y};
}

inline Vector2i operator*(Vector2i a, f32 b) {
  return {cast(i32)(a.x * b), cast(i32)(a.y * b)};
}

inline Vector2i &operator*=(Vector2i &a, f32 b) {
  return a = a * b;
}

//
// Vector2
//

na_inline Vector2 make_vector2(f32 x, f32 y) {
  return {x, y};
}

na_inline Vector2 make_vector2(Vector2 v) {
  return {v.x, v.y};
}

na_inline Vector2 make_vector2(Vector2u v) {
  return {cast(f32)v.x, cast(f32)v.y};
}

na_inline Vector2 make_vector2(Vector2i v) {
  return {cast(f32)v.x, cast(f32)v.y};
}

inline bool vector2_equals(Vector2 a, Vector2 b) {
  return abs_f32(a.x - b.x) <= EPSILON_F32 && abs_f32(a.y - b.y) <= EPSILON_F32;
}

inline Vector2 hadamard(Vector2 a, Vector2 b) {
  return {a.x * b.x, a.y * b.y};
}

inline f32 dot(Vector2 a, Vector2 b) {
  return a.x * b.x + a.y * b.y;
}

inline Vector2 perp(Vector2 a) {
  return {-a.y, a.x};
}

inline f32 length_squared(Vector2 a) {
  return dot(a, a);
}

inline f32 length(Vector2 a) {
  return sqrt_f32(length_squared(a));
}

inline Vector2 normalize(Vector2 a) {
  f32 f = 1.0f / length(a);
  return {a.x * f, a.y * f};
}

inline Vector2 arm2(f32 angle) {
  return {cos_f32(angle), sin_f32(angle)};
}

inline Vector2 lerp(Vector2 a, Vector2 b, f32 t) {
  return {
    lerp(a.x, b.x, t),
    lerp(a.y, b.y, t),
  };
}

inline Vector2 min(Vector2 a, Vector2 b) {
  return {
    min_f32(a.x, b.x),
    min_f32(a.y, b.y),
  };
}

inline Vector2 max(Vector2 a, Vector2 b) {
  return {max_f32(a.x, b.x), max_f32(a.y, b.y)};
}

inline Vector2 abs(Vector2 a) {
  return {abs_f32(a.x), abs_f32(a.y)};
}

inline Vector2 floor(Vector2 a) {
  return {(f32)floor_f32(a.x), (f32)floor_f32(a.y)};
}

inline Vector2 round(Vector2 a) {
  return {(f32)round_f32(a.x), (f32)round_f32(a.y)};
}

inline Vector2 ceil(Vector2 a) {
  return {(f32)ceil_f32(a.x), (f32)ceil_f32(a.y)};
}

inline Vector2 sign(Vector2 a) {
  return {(f32)Sign(a.x), (f32)Sign(a.y)};
}

#if 0
inline Vector2 move(Vector2 a, Vector2 b, f32 amount) {
  return {move(a.x, b.x, amount), move(a.y, b.y, amount)};
}

inline Vector2 snap(Vector2 pos, Vector2 grid) {
  return {(f32)snap(pos.x, grid.x), (f32)snap(pos.y, grid.y)};
}

inline Vector2 clamp(Vector2 a, Vector2 min, Vector2 max) {
  return {clamp(a.x, min.x, max.x), clamp(a.y, min.y, max.y)};
}

na_inline f32 angle_between(Vector2 p0, Vector2 p1) {
  return atan2(p1.y - p0.y, p1.x - p0.x);
}

na_inline f32 direction(Vector2 p0) {
  return -atan2(-p0.y, p0.x);
}

na_inline f32 distance(Vector2 p0, Vector2 p1) {
  return length(p0 - p1);
}

na_inline f32 distance_squared(Vector2 p0, Vector2 p1) {
  return length_squared(p0 - p1);
}
#endif

inline Vector2 operator+(Vector2 a, Vector2 b) {
  return {a.x + b.x, a.y + b.y};
}

inline Vector2 &operator+=(Vector2 &a, Vector2 b) {
  return a = a + b;
}

inline Vector2 operator-(Vector2 a, Vector2 b) {
  return {a.x - b.x, a.y - b.y};
}

inline Vector2 &operator-=(Vector2 &a, Vector2 b) {
  return a = a - b;
}

inline Vector2 operator*(f32 a, Vector2 b) {
  return {a * b.x, a * b.y};
}

inline Vector2 operator*(Vector2 a, f32 b) {
  return b * a;
}

inline Vector2 operator*(Vector2 a, Vector2 b) {
  return hadamard(a, b);
}

inline Vector2 &operator*=(Vector2 &a, f32 b) {
  return a = a * b;
}

inline Vector2 operator/(Vector2 a, f32 b) {
  return {a.x / b, a.y / b};
}

inline Vector2 operator/(Vector2 a, Vector2 b) {
  return {
    a.x / b.x,
    a.y / b.y,
  };
}

inline Vector2 &operator/=(Vector2 &a, f32 b) {
  return a = a / b;
}

//
// Vector3
//

inline Vector3 make_vector3(f32 x, f32 y, f32 z) {
  return {x, y, z};
}

inline Vector3 make_vector3(Vector3 v) {
  return {v.x, v.y, v.z};
}

inline Vector3 make_vector3(Vector3i v) {
  return {(f32)v.x, (f32)v.y, (f32)v.z};
}

inline Vector3 make_vector3(Vector2 v, f32 z) {
  return {v.x, v.y, z};
}

inline bool vector3_equals(Vector3 a, Vector3 b) {
  return abs_f32(a.x - b.x) <= EPSILON_F32 && abs_f32(a.y - b.y) <= EPSILON_F32 && abs_f32(a.z - b.z) <= EPSILON_F32;
}

inline Vector3 hadamard(Vector3 a, Vector3 b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z};
}

inline Vector3 operator*(Vector3 a, Vector3 b) {
  return hadamard(a, b);
}

inline f32 dot(Vector3 a, Vector3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}


inline Vector3 &operator*=(Vector3 &a, Vector3 b) {
  return a = hadamard(a, b);
}

inline f32 length_squared(Vector3 a) {
  return dot(a, a);
}

inline f32 length(Vector3 a) {
  return sqrt_f32(length_squared(a));
}

inline Vector3 normalize(Vector3 a) {
  f32 f = 1.0f / length(a);
  return {
    a.x * f,
    a.y * f,
    a.z * f,
  };
}

inline Vector3 cross(Vector3 a, Vector3 b) {
  return {
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x,
  };
}

inline Vector3 lerp(Vector3 a, Vector3 b, f32 t) {
  return {
    lerp(a.x, b.x, t),
    lerp(a.y, b.y, t),
    lerp(a.z, b.z, t),
  };
}

inline Vector3 min(Vector3 a, Vector3 b) {
  return {
    min_f32(a.x, b.x),
    min_f32(a.y, b.y),
    min_f32(a.z, b.z),
  };
}

inline Vector3 max(Vector3 a, Vector3 b) {
  return {
    max_f32(a.x, b.x),
    max_f32(a.y, b.y),
    max_f32(a.z, b.z),
  };
}


inline Vector3 operator+(Vector3 a, Vector3 b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vector3 &operator+=(Vector3 &a, Vector3 b) {
  return a = a + b;
}

inline Vector3 operator-(Vector3 a) {
  return {-a.x, -a.y, -a.z};
}

inline Vector3 operator-(Vector3 a, Vector3 b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vector3 &operator-=(Vector3 &a, Vector3 b) {
  return a = a - b;
}

inline Vector3 operator*(f32 a, Vector3 b) {
  return {a * b.x, a * b.y, a * b.z};
}

inline Vector3 operator*(Vector3 a, f32 b) {
  return b * a;
}

inline Vector3 &operator*=(Vector3 &a, f32 b) {
  return a = a * b;
}

inline Vector3 operator/(Vector3 a, f32 b) {
  return (1.0f / b) * a;
}

inline Vector3 operator/(f32 a, Vector3 b) {
  return {
    a / b.x,
    a / b.y,
    a / b.z,
  };
}

inline Vector3 &operator/=(Vector3 &a, f32 b) {
  return a = a / b;
}


//
// Vector3i
//

inline Vector3i make_vector3i(i32 x, i32 y, i32 z) {
  return {x, y, z};
}

inline Vector3i make_vector3i(Vector3 v) {
  return {(i32)v.x, (i32)v.y, (i32)v.z};
}


inline Vector3i operator+(Vector3i a, Vector3i b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vector3i &operator+=(Vector3i &a, Vector3i b) {
  return a = a + b;
}

//
// Vector4
//

na_inline Vector4 make_vector4(f32 x, f32 y, f32 z, f32 w) {
  return {x, y, z, w};
}

na_inline Vector4 make_vector4(Vector2 v, f32 z, f32 w) {
  return {v.x, v.y, z, w};
}

na_inline Vector4 make_vector4(Vector2 a, Vector2 b) {
  return {a.x, a.y, b.x, b.y};
}

na_inline Vector4 make_vector4(Vector3 v, f32 w) {
  return {v.x, v.y, v.z, w};
}

na_inline Vector4 make_vector4(Vector4 v, f32 w) {
  return {v.x, v.y, v.z, w};
}

na_inline Vector4 make_vector4(Rectangle2 r) {
  return *cast(Vector4 *)&r;
}

inline bool vector4_equals(Vector4 a, Vector4 b) {
  return abs_f32(a.x - b.x) <= EPSILON_F32 && abs_f32(a.y - b.y) <= EPSILON_F32 && abs_f32(a.z - b.z) <= EPSILON_F32 && abs_f32(a.w - b.w) <= EPSILON_F32;
}


na_inline f32 dot(Vector4 a, Vector4 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

na_inline Vector4 hadamard(Vector4 a, Vector4 b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

na_inline f32 length_squared(Vector4 a) {
  return dot(a, a);
}

na_inline f32 length(Vector4 a) {
  return sqrt_f32(length_squared(a));
}

na_inline Vector4 normalize(Vector4 a) {
  f32 f = 1.0f / length(a);
  return {
    a.x * f,
    a.y * f,
    a.z * f,
    a.w * f,
  };
}

na_inline Vector4 lerp(Vector4 a, Vector4 b, f32 t) {
  return {
    lerp(a.x, b.x, t),
    lerp(a.y, b.y, t),
    lerp(a.z, b.z, t),
    lerp(a.w, b.w, t),
  };
}


inline Vector4 operator+(Vector4 a, Vector4 b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

inline Vector4 &operator+=(Vector4 &a, Vector4 b) {
  return a = a + b;
}

inline Vector4 operator-(Vector4 a, Vector4 b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

inline Vector4 &operator-=(Vector4 &a, Vector4 b) {
  return a = a - b;
}

inline Vector4 operator*(f32 a, Vector4 b) {
  return {a * b.x, a * b.y, a * b.z, a * b.w};
}

inline Vector4 operator*(Vector4 a, f32 b) {
  return b * a;
}

inline Vector4 &operator*=(Vector4 &a, f32 b) {
  return a = a * b;
}

inline Vector4 operator-(Vector4 a) {
  return {-a.x, -a.y, -a.z, -a.w};
}

//
// Rectangle2
// 

Rectangle2 make_rectangle2(f32 x0, f32 y0, f32 x1, f32 y1) {
  return {{x0, y0}, {x1, y1}};
}

Rectangle2 make_rectangle2(Vector2 min, Vector2 max) {
  return {min, max};
}

na_inline Rectangle2 make_rectangle2(Vector4 v) {
  return *cast(Rectangle2 *)&v;
}

f32 rect_width(Rectangle2 r) {
  return abs_f32(r.x0 - r.x1);
}

f32 rect_height(Rectangle2 r) {
  return abs_f32(r.y0 - r.y1);
}

Vector2 rect_size(Rectangle2 r) {
  return v2(r.x1 - r.x0, r.y1 - r.y0);
}

Vector2 rect_center(Rectangle2 r) {
  Vector2 size = v2((r.x1 - r.x0), (r.y1 - r.y0));
  return {r.x0 + size.x / 2, r.y0 + size.y / 2};
}

na_inline f32 width(Rectangle2 r) { return rect_width(r); }
na_inline f32 height(Rectangle2 r) { return rect_height(r); }
na_inline Vector2 size(Rectangle2 r) { return rect_size(r); }
na_inline Vector2 center(Rectangle2 r) { return rect_center(r); }

bool rect_contains(Vector2 rp0, Vector2 rp1, Vector2 p) {
  return (p.x >= rp0.x && p.x < rp1.x && p.y >= rp0.y && p.y < rp1.y);
}

Rectangle2 abs(Rectangle2 rect) {
  Vector2 pos = {min_f32(rect.x0, rect.x1), min_f32(rect.y0, rect.y1)};
  return {pos, pos + abs(rect_size(rect))};
}

inline Rectangle2 rect_union(Rectangle2 a, Rectangle2 b) {
  a = abs(a);
  b = abs(b);
  return {min(a.p0, b.p0), max(a.p1, b.p1)};
}

Rectangle2 rect_intersection(Rectangle2 a, Rectangle2 b) {
  a = abs(a);
  b = abs(b);
  return {max(a.p0, b.p0), min(a.p1, b.p1)};
}

inline Rectangle2 rect_shift(Rectangle2 a, Vector2 b) {
  return {a.p0 + b, a.p1 + b};
}

inline Rectangle2 apply_padding(Rectangle2 rect, Vector2 padding) {
  return r2(rect.p0 - padding, rect.p1 + padding);
}

inline Rectangle2 apply_padding(Rectangle2 rect, f32 padding) {
  return apply_padding(rect, v2(padding, padding));
}

Rectangle2 split_from_bottom(Rectangle2 rect, f32 amount) {
  return r2(v2(rect.x0, rect.y1 - amount), rect.p1);
}

Rectangle2 split_from_top(Rectangle2 rect, f32 amount) {
  return r2(rect.p0, v2(rect.x1, rect.y0 + amount));
}

Rectangle2 trim_from_top(Rectangle2 rect, f32 amount) {
  return r2(v2(rect.x0, rect.y0 + amount), rect.p1);
}

Rectangle2 split_from_left(Rectangle2 rect, f32 amount) {
  return r2(rect.p0, v2(rect.x0 + amount, rect.y1));
}

Rectangle2 split_from_right(Rectangle2 rect, f32 amount) {
  return r2(v2(rect.x1 - amount, rect.y0), rect.p1);
}

Rectangle2 center_in_rect(Rectangle2 rect, Vector2 size) {
  auto rs = rect_center(rect);
  return r2(rs - size * 0.5f, rs + size * 0.5f);
}

inline Rectangle2 operator+(Rectangle2 a, Vector2 b) {
  return rect_shift(a, b);
}

inline Rectangle2 operator+(Vector2 b, Rectangle2 a) {
  return rect_shift(a, b);
}

//
// Matrix4
//

na_inline Vector4 matrix4_col(Matrix4 &it, u32 i) {
  assert(i >= 0 && i < 4);
  return {it.e[0][i], it.e[1][i], it.e[2][i], it.e[3][i]};
}

na_inline Matrix4 matrix4_identity() {
  return {{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  }};
}

inline Vector4 transform(Matrix4 a, Vector4 p) {
  return {
    dot(p, a.rows[0]),
    dot(p, a.rows[1]),
    dot(p, a.rows[2]),
    dot(p, a.rows[3]),
  };
}

inline Vector3 transform(Matrix4 a, Vector3 p) {
  return transform(a, v4(p, 1.0f)).xyz;
}

inline Vector2 transform(Matrix4 m, Vector2 src) {
  f32 x = (m.e[0][0] * src.x) + (m.e[0][1] * src.y) + (0) + (m.e[0][3]);
  f32 y = (m.e[1][0] * src.x) + (m.e[1][1] * src.y) + (0) + (m.e[1][3]);
  return {x, y};
}

Matrix4 operator*(Matrix4 a, Matrix4 b) {
  Vector4 row0 = a.rows[0];
  Vector4 row1 = a.rows[1];
  Vector4 row2 = a.rows[2];
  Vector4 row3 = a.rows[3];

  Vector4 col0 = matrix4_col(b, 0);
  Vector4 col1 = matrix4_col(b, 1);
  Vector4 col2 = matrix4_col(b, 2);
  Vector4 col3 = matrix4_col(b, 3);

  return {{
    {dot(row0, col0), dot(row0, col1), dot(row0, col2), dot(row0, col3)},

    {dot(row1, col0), dot(row1, col1), dot(row1, col2), dot(row1, col3)},

    {dot(row2, col0), dot(row2, col1), dot(row2, col2), dot(row2, col3)},

    {dot(row3, col0), dot(row3, col1), dot(row3, col2), dot(row3, col3)},
  }};
}

inline Vector3 operator*(Matrix4 a, Vector3 p) {
  return transform(a, p);
}

inline Vector4 operator*(Matrix4 a, Vector4 p) {
  return transform(a, p);
}

inline Matrix4 operator*(Matrix4 a, f32 s) {
  return {{
    a.rows[0] * s,
    a.rows[1] * s,
    a.rows[2] * s,
    a.rows[3] * s,
  }};
}

inline Matrix4 rotation_x(f32 angle) {
  f32 c = cos_f32(angle);
  f32 s = sin_f32(angle);

  return {{
    {1, 0, 0, 0},
    {0, c, -s, 0},
    {0, s, c, 0},
    {0, 0, 0, 1},
  }};
}

inline Matrix4 rotation_y(f32 angle) {
  f32 c = cos_f32(angle);
  f32 s = sin_f32(angle);

  return {{
    {c, 0, s, 0},
    {0, 1, 0, 0},
    {-s, 0, c, 0},
    {0, 0, 0, 1},
  }};
}

inline Matrix4 rotation_z(f32 angle) {
  f32 c = cos_f32(angle);
  f32 s = sin_f32(angle);

  return {{
    {c, -s, 0, 0},
    {s, c, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  }};
}

inline Matrix4 translation(Vector3 t) {
  return {{
    {1, 0, 0, t.x},
    {0, 1, 0, t.y},
    {0, 0, 1, t.z},
    {0, 0, 0, 1},
  }};
}

inline Matrix4 translate(Matrix4 a, Vector3 t) {
  Matrix4 result = a;
  result.e[0][3] += t.x;
  result.e[1][3] += t.y;
  result.e[2][3] += t.z;
  return result;
}

inline Matrix4 scaling(Vector3 s) {
  return {{
    {s.x, 0, 0, 0},
    {0, s.y, 0, 0},
    {0, 0, s.z, 0},
    {0, 0, 0, 1},
  }};
}

inline Matrix4 transpose(Matrix4 a) {
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

inline f32 determinant(Matrix4 m) {
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
    m0 * m9 * m6 * m15 - m4 * m1 * m10 * m15 + m0 * m5 * m10 * m15);
}

inline Matrix4 inverse(Matrix4 m) {
  Matrix4 t;

  f32 m0 = m.e[0][0], m1 = m.e[0][1], m2 = m.e[0][2], m3 = m.e[0][3],
      m4 = m.e[1][0], m5 = m.e[1][1], m6 = m.e[1][2], m7 = m.e[1][3],
      m8 = m.e[2][0], m9 = m.e[2][1], m10 = m.e[2][2], m11 = m.e[2][3],
      m12 = m.e[3][0], m13 = m.e[3][1], m14 = m.e[3][2], m15 = m.e[3][3];

  t.e[0][0] =
    (m9 * m14 * m7 - m13 * m10 * m7 + m13 * m6 * m11 - m5 * m14 * m11 -
     m9 * m6 * m15 + m5 * m10 * m15);
  t.e[0][1] =
    (m13 * m10 * m3 - m9 * m14 * m3 - m13 * m2 * m11 + m1 * m14 * m11 +
     m9 * m2 * m15 - m1 * m10 * m15);
  t.e[0][2] =
    (m5 * m14 * m3 - m13 * m6 * m3 + m13 * m2 * m7 - m1 * m14 * m7 -
     m5 * m2 * m15 + m1 * m6 * m15);
  t.e[0][3] =
    (m9 * m6 * m3 - m5 * m10 * m3 - m9 * m2 * m7 + m1 * m10 * m7 +
     m5 * m2 * m11 - m1 * m6 * m11);

  t.e[1][0] =
    (m12 * m10 * m7 - m8 * m14 * m7 - m12 * m6 * m11 + m4 * m14 * m11 +
     m8 * m6 * m15 - m4 * m10 * m15);
  t.e[1][1] =
    (m8 * m14 * m3 - m12 * m10 * m3 + m12 * m2 * m11 - m0 * m14 * m11 -
     m8 * m2 * m15 + m0 * m10 * m15);
  t.e[1][2] =
    (m12 * m6 * m3 - m4 * m14 * m3 - m12 * m2 * m7 + m0 * m14 * m7 +
     m4 * m2 * m15 - m0 * m6 * m15);
  t.e[1][3] =
    (m4 * m10 * m3 - m8 * m6 * m3 + m8 * m2 * m7 - m0 * m10 * m7 -
     m4 * m2 * m11 + m0 * m6 * m11);

  t.e[2][0] =
    (m8 * m13 * m7 - m12 * m9 * m7 + m12 * m5 * m11 - m4 * m13 * m11 -
     m8 * m5 * m15 + m4 * m9 * m15);
  t.e[2][1] =
    (m12 * m9 * m3 - m8 * m13 * m3 - m12 * m1 * m11 + m0 * m13 * m11 +
     m8 * m1 * m15 - m0 * m9 * m15);
  t.e[2][2] =
    (m4 * m13 * m3 - m12 * m5 * m3 + m12 * m1 * m7 - m0 * m13 * m7 -
     m4 * m1 * m15 + m0 * m5 * m15);
  t.e[2][3] =
    (m8 * m5 * m3 - m4 * m9 * m3 - m8 * m1 * m7 + m0 * m9 * m7 + m4 * m1 * m11 -
     m0 * m5 * m11);

  t.e[3][0] =
    (m12 * m9 * m6 - m8 * m13 * m6 - m12 * m5 * m10 + m4 * m13 * m10 +
     m8 * m5 * m14 - m4 * m9 * m14);
  t.e[3][1] =
    (m8 * m13 * m2 - m12 * m9 * m2 + m12 * m1 * m10 - m0 * m13 * m10 -
     m8 * m1 * m14 + m0 * m9 * m14);
  t.e[3][2] =
    (m12 * m5 * m2 - m4 * m13 * m2 - m12 * m1 * m6 + m0 * m13 * m6 +
     m4 * m1 * m14 - m0 * m5 * m14);
  t.e[3][3] =
    (m4 * m9 * m2 - m8 * m5 * m2 + m8 * m1 * m6 - m0 * m9 * m6 - m4 * m1 * m10 +
     m0 * m5 * m10);

  f32 d = determinant(m);

  return t * (1.0f / d);
}

// NOTE(nick): reasonable values are perspective_proj(aspect_ratio, 1.0f, 0.1f, 100.0f);
inline Matrix4 perspective_proj(f32 aspect_width_over_height, f32 focal_length, f32 near_clip_plane, f32 far_clip_plane) {
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

inline Matrix4 orthographic_proj(f32 aspect_width_over_height, f32 near_clip_plane, f32 far_clip_plane) {
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

na_inline Matrix4 orthographic_proj2d(f32 left, f32 right, f32 bottom, f32 top, f32 zNear = -1.0f, f32 zFar = 1.0f) {
  Matrix4 result = matrix4_identity();
  result.e[0][0] = f32(2) / (right - left);
  result.e[1][1] = f32(2) / (top - bottom);
  result.e[2][2] = -f32(2) / (zFar - zNear);
  result.e[0][3] = -(right + left) / (right - left);
  result.e[1][3] = -(top + bottom) / (top - bottom);
  result.e[2][3] = -(zFar + zNear) / (zFar - zNear);
  return result;
}

//
// Quaternion
//

inline Quaternion make_quaternion(Vector4 a) {
  return {a.x, a.y, a.z, a.w};
}

inline Quaternion make_quaternion(f32 angle, Vector3 v) {
  Vector3 a = v * sin_f32(angle / 2);
  return {a.x, a.y, a.z, cos_f32(angle / 2)};
}

inline Quaternion quaternion_identity() {
  return {0, 0, 0, 1};
}

inline Quaternion quaternion_invert(Quaternion q) {
  return {-q.x, -q.y, -q.z, q.w};
}

Quaternion quaternion_mul(Quaternion q1, Quaternion q2) {
  Quaternion result;
  result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
  result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
  result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
  result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
  return result;
}

inline Matrix4 quaternion_to_rotation_matrix(Quaternion q) {
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

Vector3 quaternion_vector3_mul(Quaternion q, Vector3 v) {
  return quaternion_to_rotation_matrix(q) * v;
}

inline Quaternion quaternion_from_euler(f32 pitch, f32 yaw, f32 roll) {
  f32 p = pitch / 2.0f;
  f32 y = yaw / 2.0f;
  f32 r = roll / 2.0f;

  f32 sinp = sin_f32(p);
  f32 siny = sin_f32(y);
  f32 sinr = sin_f32(r);
  f32 cosp = cos_f32(p);
  f32 cosy = cos_f32(y);
  f32 cosr = cos_f32(r);

  f32 vx = sinr * cosp * cosy - cosr * sinp * siny;
  f32 vy = cosr * sinp * cosy + sinr * cosp * siny;
  f32 vz = cosr * cosp * siny - sinr * sinp * cosy;
  f32 vw = cosr * cosp * cosy + sinr * sinp * siny;

  return {vx, vy, vz, vw};
}

inline Quaternion operator*(Quaternion a, Quaternion b) {
  return quaternion_mul(a, b);
}

inline Vector3 operator*(Quaternion a, Vector3 b) {
  return quaternion_vector3_mul(a, b);
}


//
// String Extensions
//

inline String to_string(Vector2 a) {
  return sprint("Vector2 {%f, %f}", a.x, a.y);
}

inline String to_string(Vector2i a) {
  return sprint("Vector2i {%d, %d}", a.x, a.y);
}
inline String to_string(Vector2u a) {
  return sprint("Vector2u {%d, %d}", a.x, a.y);
}

inline String to_string(Vector3 a) {
  return sprint("Vector3 {%f, %f, %f}", a.x, a.y, a.z);
}

inline String to_string(Vector3i a) {
  return sprint("Vector3i {%d, %d, %d}", a.x, a.y, a.z);
}

inline String to_string(Vector4 a) {
  return sprint("Vector4 {%f, %f, %f, %f}", a.x, a.y, a.z, a.w);
}

inline String to_string(Rectangle2 a) {
  return sprint("Rectangle2 {%f, %f, %f, %f}", a.x0, a.y0, a.x1, a.y1);
}

inline String to_string(Rectangle2i a) {
  return sprint("Rectangle2i {%d, %d, %d, %d}", a.x0, a.y0, a.x1, a.y1);
}

inline String to_string(Rectangle2u a) {
  return sprint("Rectangle2u {%d, %d, %d, %d}", a.x0, a.y0, a.x1, a.y1);
}

inline String to_string(Matrix4 a) {
  return sprint(
    "Matrix4 {\n  {%f, %f, %f, %f},\n  {%f, %f, %f, %f},\n  {%f, %f, %f, %f},\n  {%f, %f, %f, %f}\n}",
    a.rows[0].x, a.rows[0].y, a.rows[0].z, a.rows[0].w,
    a.rows[1].x, a.rows[1].y, a.rows[1].z, a.rows[1].w,
    a.rows[2].x, a.rows[2].y, a.rows[2].z, a.rows[2].w,
    a.rows[3].x, a.rows[3].y, a.rows[3].z, a.rows[3].w
  );
}

inline String to_string(Quaternion a) {
  return sprint("Quaternion {%f, %f, %f, %f}", a.x, a.y, a.z, a.w);
}

//
// Packing
//

na_inline u32 vector2_pack_u32(Vector2 unpacked) {
  return (
    (round_f32_to_u32(unpacked.y * U16_MAX) << 16) |
    (round_f32_to_u32(unpacked.x * U16_MAX) << 0)
  );
}

na_inline u32 vector4_pack_u32(Vector4 unpacked) {
  // @Speed: this could be made wide
  // @Cleanup: make some math functions for these
  //unpacked *= 255.0f;

  return (
    (round_f32_to_u32(unpacked.a * U8_MAX) << 24) |
    (round_f32_to_u32(unpacked.b * U8_MAX) << 16) |
    (round_f32_to_u32(unpacked.g * U8_MAX) << 8) |
    (round_f32_to_u32(unpacked.r * U8_MAX) << 0)
  );
}

//
// Collisions
//

bool rectangle_intersects(Rectangle2 r0, Rectangle2 r1) {
  return (r0.x0 < r1.x1 && r0.x1 > r1.x0 && r0.y0 < r1.y1 && r0.y1 > r1.y0);
}

bool rectangle_touching(Rectangle2 r0, Rectangle2 r1) {
  return (r0.x0 <= r1.x1 && r0.x1 >= r1.x0 && r0.y0 <= r1.y1 && r0.y1 >= r1.y0);
}

bool rectangle_contains(Rectangle2 r0, Vector2 p) {
  return p.x >= r0.x0 && p.x <= r0.x1 && p.y >= r0.y0 && p.y <= r0.y1;
}

bool rectangle_circle_collision(Rectangle2 rect, Vector2 circle, f32 radius) {
  f32 dx = circle.x - clamp_f32(circle.x, rect.x0, rect.x1);
  f32 dy = circle.y - clamp_f32(circle.y, rect.y0, rect.y1);
  return dx * dx + dy * dy < radius * radius;
}

bool circle_intersects(Vector2 c0, f32 r0, Vector2 c1, f32 r1) {
  f32 radius = r0 + r1;
  return length_squared(c0 - c1) <= (radius * radius);
}

bool line_intersects_line(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3) {
  f32 s1_x = p1.x - p0.x;
  f32 s1_y = p1.y - p0.y;
  f32 s2_x = p3.x - p2.x;
  f32 s2_y = p3.y - p2.y;

  f32 s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
  f32 t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

  return s >= 0 && s <= 1 && t >= 0 && t <= 1;
}

bool line_intersects_line_point(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, Vector2 *return_point) {
  f32 s1_x = p1.x - p0.x;
  f32 s1_y = p1.y - p0.y;
  f32 s2_x = p3.x - p2.x;
  f32 s2_y = p3.y - p2.y;

  f32 s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
  f32 t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

  *return_point = p0 + p1 * t;

  return s >= 0 && s <= 1 && t >= 0 && t <= 1;
}

bool line_intersects_circle(Vector2 p1, Vector2 p2, Vector2 circle, f32 radius) {
  Vector2 ac = v2(circle.x - p1.x, circle.y - p1.y);
  Vector2 ab = v2(p2.x - p1.x, p2.y - p1.y);
  f32 ab2 = dot(ab, ab);
  f32 acab = dot(ac, ab);
  f32 t = acab / ab2;

  t = (t < 0) ? 0 : t;
  t = (t > 1) ? 1 : t;

  Vector2 h = v2((ab.x * t + p1.x) - circle.x, (ab.y * t + p1.y) - circle.y);
  f32 h2 = dot(h, h);
  return h2 <= radius * radius;
}

#endif // NA_MATH_H
