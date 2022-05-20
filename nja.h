#ifndef NJA_H
#define NJA_H

//
// Basic
//

#ifndef NULL
  #define NULL 0
#endif

#ifndef null
  #define null nullptr
#endif

#ifndef print
  #include <stdio.h>
  #define print(...) do { printf(__VA_ARGS__); fflush(stdout); } while(0)
#endif

#ifndef print_to_buffer
  #include <stdio.h>
  #define print_to_buffer(data, max_length, format, args) vsnprintf(data, max_length, format, args)
#endif

#if DEBUG
#ifndef debug_breakpoint
  #if defined(_MSC_VER)
    #if _MSC_VER < 1300
    #define debug_breakpoint() __asm int 3 /* Trap to debugger! */
    #else
    #define debug_breakpoint() __debugbreak()
    #endif
  #else
    #define debug_breakpoint() __builtin_trap()
  #endif
#endif
#else
  #define debug_breakpoint()
#endif

#ifndef assert
#ifdef DEBUG
  #ifndef assert_break
  #define assert_break debug_breakpoint
  #endif

  void nja_assert_handler(char *prefix, char *expr, char *file, long int line, char *msg) {
    print("%s(%ld): %s: ", file, line, prefix);
    if (expr) {
      print( "`%s` ", expr);
    }
    if (msg) {
      print("- %s", msg);
    }
    print("\n");
  }

  #define assert(expr) do { if (!(expr)) { nja_assert_handler("Assertion Failed", #expr, __FILE__, __LINE__, NULL); assert_break(); } } while (0)
#else
  #define assert(...)
#endif
#endif

#ifndef dump
#if DEBUG
  #define dump(var) print("%s: %s\n", #var, string_to_cstr(to_string(var)))
#else
  #define dump(...)
#endif
#endif

#ifndef STATIC_ASSERT
#define STATIC_ASSERT(expr) static_assert(expr, "")
#endif

#ifndef nja_inline
  #if defined(__GNUC__) && (__GNUC__ >= 4)
    #define nja_inline __attribute__((always_inline)) inline
  #elif defined(__llvm__)
    #define nja_inline __attribute__((always_inline)) inline
  #elif defined(_MSC_VER)
    #define nja_inline __forceinline
  #else
    #define nja_inline inline
  #endif
#endif

#ifndef nja_extern
  #define nja_extern extern "C"
#endif

#ifndef nja_dll_export
#if defined(_WIN32)
  #define nja_dll_export nja_extern __declspec(dllexport)
  #define nja_dll_import nja_extern __declspec(dllimport)
#else
  #define nja_dll_export nja_extern __attribute__((visibility("default")))
  #define nja_dll_import nja_extern
#endif
#endif

#if !defined(nja_thread_local)
  #if defined(_MSC_VER) && _MSC_VER >= 1300
    #define nja_thread_local __declspec(thread)
  #elif defined(__GNUC__)
    #define nja_thread_local __thread
  #else
    #define nja_thread_local thread_local
  #endif
#endif

#ifndef cast
#define cast(type) (type)
#endif

#ifndef size_of
#define size_of(x) ((isize)sizeof(x))
#endif

#ifndef count_of
#define count_of(array) (sizeof(array) / sizeof((array)[0]))
#endif

#ifndef offset_of
#define offset_of(Type, member) ((uint64_t) & (((Type *)0)->member))
#endif

#ifndef align_of
#define align_of(Type) ((isize)alignof(Type))
#endif

#ifndef nja_global
#define nja_global        static // Global variables
#define nja_internal      static // Internal linkage
#define nja_local_persist static // Local Persisting variables
#endif

#ifndef FOUR_CC
#define FOUR_CC(a, b, c, d) \
  (((u32)(a) << 0) | ((u32)(b) << 8) | ((u32)(c) << 16) | ((u32)(d) << 24))
#endif

#ifndef SWAP
#define SWAP(Type, a, b) do { Type tmp = (a); (a) = (b); (b) = tmp; } while (0)
#endif

#ifndef CONCAT
#define CONCAT_HELPER(x, y) x##y
#define CONCAT(x, y) CONCAT_HELPER(x, y)
#endif

#ifndef defer
// Defer macro/thing.
template<typename T>
struct ExitScope {
  T lambda;
  ExitScope(T lambda) : lambda(lambda) {}
  ~ExitScope() { lambda(); }
  ExitScope(const ExitScope&);
private:
  ExitScope& operator =(const ExitScope&);
};

class ExitScopeHelp {
public:
  template<typename T>
  ExitScope<T> operator+(T t) { return t; }
};

#define defer const auto& CONCAT(defer__, __LINE__) = ExitScopeHelp() + [&]()
#endif

#ifndef lambda
#define lambda(...) [](__VA_ARGS__)
#endif

#ifndef set_flag
#define set_flag(flags, flag, enable) do { if (enable) { flags |= (flag); } else { flags &= ~(flag); } } while(0)
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define OS_WINDOWS 1
#elif defined(__APPLE__)
  #define OS_MACOS 1
#elif defined(__unix__)

  #if defined(__linux__)
    #define OS_LINUX 1
  #else
    #error This UNIX operating system is not supported!
  #endif

#else
  #error [OS] Unsupported operating system!
#endif

#ifndef OS_WINDOWS
#define OS_WINDOWS 0
#endif

#ifndef OS_MACOS
#define OS_MACOS 0
#endif

#ifndef OS_LINUX
#define OS_LINUX 0
#endif

#if defined(_MSC_VER)
  #define COMPILER_MSVC 1
#elif defined(__GNUC__)
  #define COMPILER_GCC 1
#elif defined(__clang__)
  #define COMPILER_CLANG 1
#else
  #error Unknown compiler!
#endif

#ifndef COMPILER_MSVC
#define COMPILER_MSVC 0
#endif

#ifndef COMPILER_GCC
#define COMPILER_GCC 0
#endif

#ifndef COMPILER_CLANG
#define COMPILER_CLANG 0
#endif

#if OS_WINDOWS
#include <intrin.h>
#endif

//
// Types
//

#include <stdint.h>
#include <stddef.h>

typedef uint8_t   u8;
typedef int8_t    i8;
typedef uint16_t  u16;
typedef int16_t   i16;
typedef uint32_t  u32;
typedef int32_t   i32;
typedef uint64_t  u64;
typedef int64_t   i64;
typedef float     f32;
typedef double    f64;
typedef size_t    usize;
typedef ptrdiff_t isize;

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

#define USIZE_MIX U64_MIN
#define USIZE_MAX U64_MAX
#define ISIZE_MIX I64_MIN
#define ISIZE_MAX I64_MAX

STATIC_ASSERT(sizeof(u8)  == sizeof(i8));
STATIC_ASSERT(sizeof(u16) == sizeof(i16));
STATIC_ASSERT(sizeof(u32) == sizeof(i32));
STATIC_ASSERT(sizeof(u64) == sizeof(i64));

STATIC_ASSERT(sizeof(f32) == 4);
STATIC_ASSERT(sizeof(f64) == 8);

STATIC_ASSERT(sizeof(usize) == sizeof(isize));

STATIC_ASSERT(sizeof(u8)  == 1);
STATIC_ASSERT(sizeof(u16) == 2);
STATIC_ASSERT(sizeof(u32) == 4);
STATIC_ASSERT(sizeof(u64) == 8);

//
// Helpers
//

bool nja_is_power_of_two(isize x) {
  return !(x & (x - 1));
}

u64 nja_next_power_of_two(u64 x) {
  assert(x != 0);

  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  x |= x >> 32;

  return ++x;
}

u64 nja_previous_power_of_two(u64 x) {
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  x |= x >> 32;
  return x - (x >> 1);
}

inline u32 nja_rotate_left(u32 value, i32 amount) {
#if COMPILER_MSVC
  u32 result = _rotl(value, amount);
#else
  // TODO(casey): Actually port this to other compiler platforms!
  amount &= 31;
  u32 result = ((value << amount) | (value >> (32 - amount)));
#endif

  return result;
}

inline u32 nja_rotate_right(u32 value, i32 amount) {
#if COMPILER_MSVC
  u32 result = _rotr(value, amount);
#else
  // TODO(casey): Actually port this to other compiler platforms!
  amount &= 31;
  u32 result = ((value >> amount) | (value << (32 - amount)));
#endif

  return result;
}

void *nja_align_forward(void *ptr, isize alignment) {
  usize p;

  assert(alignment >= 1);
  assert(nja_is_power_of_two(alignment));

  p = cast(usize)ptr;
  return cast(void *)((p + (alignment - 1)) & ~(alignment - 1));
}

u64 nja_align_offset(void *ptr, isize alignment) {
  usize base_address = (usize)ptr;

  assert(alignment >= 1);
  assert(nja_is_power_of_two(alignment));

  u64 align_offset = alignment - (base_address & (alignment - 1));
  align_offset &= (alignment - 1);

  return align_offset;
}

inline void *nja_pointer_add(void *ptr, isize bytes) {
  return cast(void *)(cast(u8 *)ptr + bytes);
}

inline void *nja_pointer_sub(void *ptr, isize bytes) {
  return cast(void *)(cast(u8 *)ptr - bytes);
}

inline isize nja_pointer_diff(void *begin, void *end) {
  return cast(isize)(cast(u8 *)end - cast(u8 *)begin);
}

//
// Functions
//

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

#define kilobytes(value) (value * 1024LL)
#define megabytes(value) (value * 1024LL * 1024LL)
#define gigabytes(value) (value * 1024LL * 1024LL * 1024LL)
#define terabytes(value) (value * 1024LL * 1024LL * 1024LL * 1024LL)

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
inline f32 sign_f32(f32 a) { return Sign(a); }
inline f64 sign_f64(f64 a) { return Sign(a); }

inline i32 abs_i32(i32 a) { return Abs(a); }
inline f32 abs_f32(f32 a) { return Abs(a); }
inline f64 abs_f64(f64 a) { return Abs(a); }

inline f32 lerp(f32 a, f32 b, f32 t) {
  return (1 - t) * a + b * t;
}

//
// Memory
//

void *memory_copy(void *from, void *to, u64 size) {
  if (to == NULL) {
    return NULL;
  }

#if OS_WINDOWS
  // @Incomplete: is this good enough?
  __movsb((u8 *)to, (u8 *)from, size);
#else
  // @Speed: make this way faster!

  u8 *src = cast(u8 *)from;
  u8 *dest = cast(u8 *)to;

  while (size--) { *dest++ = *src++; }
#endif

  return to;
}

bool memory_equals(void *a_in, void *b_in, u64 size) {
  // @Speed: make this faster!
  u8 *a = cast(u8 *)a_in;
  u8 *b = cast(u8 *)b_in;

  while (size--) {
    if (*a++ != *b++) {
      return false;
    }
  }

  return true;
}

void memory_set(void *ptr, u8 value, u64 size) {
  // @Speed: heavily optimize!
  u8 *at = cast(u8 *)ptr;
  while (size--) { *at++ = value; }
}

void memory_zero(void *ptr, u64 size) {
  memory_set(ptr, 0, size);
}

void *memory_move(void *from, void *to, u64 size) {
  // @Speed: heavily optimize!
  char *d = (char *)to;
  char *s = (char *)from;

  if (d < s) {
    while (size--) {
      *d++ = *s++;
    }
  } else {
    char *lasts = s + (size - 1);
    char *lastd = d + (size - 1);

    while (size--) {
      *lastd-- = *lasts--;
    }
  }

  return to;
}

void memory_swap(void *i, void *j, isize size) {
  if (i == j) return;

  if (size == 4) {
    SWAP(u32, *cast(u32 *)i, *cast(u32 *)j);
  } else if (size == 8) {
    SWAP(u64, *cast(u64 *)i, *cast(u64 *)j);
  } else if (size < 8) {
    u8 *a = cast(u8 *)i;
    u8 *b = cast(u8 *)j;

    if (a != b) {
      while (size--) {
        SWAP(u8, *a, *b);
        a++, b++;
      }
    }
  } else {
    char buffer[256];

    // TODO(bill): Is the recursion ever a problem?
    while (size > size_of(buffer)) {
      memory_swap(i, j, size_of(buffer));
      i = nja_pointer_add(i, size_of(buffer));
      j = nja_pointer_add(j, size_of(buffer));
      size -= size_of(buffer);
    }

    memory_copy(i, buffer, size);
    memory_copy(j, i, size);
    memory_copy(buffer, j, size);
  }
}

//
// Arenas
//

enum Allocator_Mode {
  ALLOCATOR_MODE_ALLOC    = 0,
  ALLOCATOR_MODE_RESIZE   = 1,
  ALLOCATOR_MODE_FREE     = 2,
  ALLOCATOR_MODE_FREE_ALL = 3,
};

#define ALLOCATOR_PROC(name) void *name(Allocator_Mode mode, u64 requested_size, u64 old_size, void *old_memory_pointer, void *allocator_data, u32 alignment)
typedef ALLOCATOR_PROC(Allocator_Proc);

struct Allocator {
  Allocator_Proc *proc;
  void *data;
};

void *allocator_alloc(Allocator allocator, u64 size);

#ifndef DEFAULT_MEMORY_ALIGNMENT
#define DEFAULT_MEMORY_ALIGNMENT 16
#endif

#ifndef DEFAULT_MEMORY_BLOCK_SIZE
#define DEFAULT_MEMORY_BLOCK_SIZE kilobytes(4)
#endif

struct Memory {
  void *(*reserve) (u64 size);
  bool  (*commit)  (void *ptr, u64 size);
  bool  (*decommit)(void *ptr, u64 size);
  bool  (*release) (void *ptr, u64 size);
};

struct Arena {
  Memory backing;

  u8 *data;
  u64 offset;
  u64 size;
  u64 prev_offset;

  u64 commit_position;
  u64 block_size;
};

struct Arena_Mark {
  u64 offset;
  u64 prev_offset;
};

Arena arena_make(u8 *data, u64 size) {
  Arena result = {};
  result.data = data;
  result.size = size;
  return result;
}

void arena_init_from_backing_memory(Arena *arena, Memory memory, u64 total_size, u64 block_size = DEFAULT_MEMORY_BLOCK_SIZE) {
  *arena = {};

  assert(total_size % block_size == 0 && "block_size must be a multiple of total_size!");

  arena->backing    = memory;
  arena->data       = cast(u8 *)memory.reserve(total_size);
  arena->size       = total_size;
  arena->block_size = block_size;

  arena->commit_position = 0;
}

Arena arena_make_from_backing_memory(Memory memory, u64 total_size, u64 block_size = DEFAULT_MEMORY_BLOCK_SIZE) {
  Arena result = {};
  arena_init_from_backing_memory(&result, memory, total_size, block_size);
  return result;
}

void arena_init(Arena *arena, u8 *data, u64 size) {
  *arena = {};
  arena->data = data;
  arena->size = size;
}

void arena_free(Arena *arena) {
  if (arena->backing.release) {
    if (arena->data) {
      arena->backing.release(arena->data, arena->size);
      arena->data = NULL;
    }
  }
}

void *arena_push(Arena *arena, u64 size) {
  void *result = NULL;

  if (arena->offset + size < arena->size) {
    result = &arena->data[arena->offset];
    arena->prev_offset = arena->offset;
    arena->offset += size;

    if (arena->backing.commit) {
      if (arena->offset > arena->commit_position)
      {
        u64 commit_size = AlignUpPow2(arena->offset - arena->commit_position, arena->block_size);
        u64 bytes_remaining = arena->size - arena->offset;
        commit_size = ClampTop(commit_size, bytes_remaining);

        if (arena->backing.commit(arena->data + arena->commit_position, commit_size)) {
          arena->commit_position += commit_size;
          memory_zero(result, size);
        } else {
          result = NULL;
        }
      }
    } else {
      memory_zero(result, size);
    }
  }

  return result;
}

void arena_pop_to(Arena *arena, u64 pos) {
  if (pos < arena->offset) {
    arena->prev_offset = arena->offset; // @Robustness: test this with realloc!
    arena->offset = pos;

    if (arena->backing.decommit) {
      u64 prev_commit_position = arena->commit_position;
      u64 next_commit_position = AlignUpPow2(arena->offset, arena->block_size);

      if (next_commit_position < prev_commit_position) {
        u64 decommit_size = prev_commit_position - next_commit_position;

        if (arena->backing.decommit(arena->data + next_commit_position, decommit_size)) {
          arena->commit_position = next_commit_position;
        }
      }
    }
  }
}

void arena_pop(Arena *arena, u64 size) {
  assert(size <= arena->offset);

  arena_pop_to(arena, arena->offset - size);
}

void arena_free_pointer(Arena *arena, void *ptr) {
  if (arena->data + arena->prev_offset == ptr) {
    arena_pop_to(arena, arena->prev_offset);
  }

  // NO-OP
}

void arena_reset(Arena *arena) {
  arena->offset = 0;
  arena->prev_offset = 0;

  if (arena->backing.decommit) {
    if (arena->commit_position > 0) {
      arena->backing.decommit(arena->data, arena->commit_position);
      arena->commit_position = 0;
    }
  }
}

void arena_set_alignment(Arena *arena, u64 alignment) {
  u64 align_offset = nja_align_offset(arena->data + arena->offset, alignment);
  if (align_offset > 0) {
    arena_push(arena, align_offset);

    // NOTE(nick): make sure our data is aligned properly
    assert((u64)(arena->data + arena->offset) % alignment == 0);
  }
}

void *arena_alloc_aligned(Arena *arena, u64 size, u64 alignment) {
  arena_set_alignment(arena, alignment);
  return arena_push(arena, size);
}

void *arena_alloc(Arena *arena, u64 size) {
  return arena_alloc_aligned(arena, size, DEFAULT_MEMORY_ALIGNMENT);
}

bool arena_contains_pointer(Arena *arena, void *ptr) {
  return ptr >= arena->data && ptr < arena->data + arena->size;
}

void *arena_realloc_aligned(Arena *arena, u64 size, u64 old_size, void *old_memory, u16 alignment) {
  if (old_memory == NULL || old_size == 0 || arena->offset < arena->prev_offset) {
    return arena_alloc_aligned(arena, size, alignment);
  }

  if (!arena_contains_pointer(arena, old_memory)) {
    assert(!"Memory is out of bounds in this arena");
    return NULL;
  }

  if (arena->data + arena->prev_offset == old_memory) {
    arena->offset = arena->prev_offset + size;
    assert(arena_contains_pointer(arena, arena->data + arena->offset));

    if (arena->backing.commit) {
      if (arena->offset > arena->commit_position)
      {
        u64 commit_size = AlignUpPow2(arena->offset - arena->commit_position, arena->block_size);
        u64 bytes_remaining = arena->size - arena->offset;
        commit_size = ClampTop(commit_size, bytes_remaining);

        if (arena->backing.commit(arena->data + arena->commit_position, commit_size)) {
          arena->commit_position += commit_size;
        }
      }
    }

    i64 delta_size = cast(i64)(size - old_size);
    if (delta_size > 0) memory_zero(cast(u8 *)old_memory + old_size, delta_size);

    // The assumption is that the alignment will not change between allocations.
    assert(nja_align_offset(old_memory, alignment) == 0);

    return old_memory;
  }

  void *new_memory = arena_alloc_aligned(arena, size, alignment);

  i64 delta_size = cast(i64)(size - old_size);
  memory_copy(old_memory, new_memory, min_i64(old_size, size));
  memory_zero(cast(u8 *)new_memory + old_size, max_i64(delta_size, 0));

  return new_memory;
}

void *arena_realloc(Arena *arena, u64 size, u64 old_size, void *old_memory) {
  return arena_realloc_aligned(arena, size, old_size, old_memory, DEFAULT_MEMORY_ALIGNMENT);
}

#define push_struct(arena, Struct)  \
  (Struct *)arena_push(arena, sizeof(Struct))

#define push_array(arena, Struct, count)  \
  (Struct *)arena_push(arena, (count) * sizeof(Struct))

Arena_Mark arena_get_position(Arena *arena) {
  Arena_Mark result = {};

  result.offset      = arena->offset;
  result.prev_offset = arena->prev_offset;

  return result;
}

void arena_set_position(Arena *arena, Arena_Mark mark) {
  arena_pop_to(arena, mark.offset);

  arena->offset      = mark.offset;
  arena->prev_offset = mark.prev_offset;
}

//
// Sorting
//

#define COMPARE_PROC(name) int name(void *a, void *b)
typedef COMPARE_PROC(Compare_Proc);

// TODO(bill): Make user definable?
#define GB__SORT_STACK_SIZE            64
#define GB__SORT_INSERT_SORT_THRESHOLD  8

#define GB__SORT_PUSH(_base, _limit) do { \
  stack_ptr[0] = (_base); \
  stack_ptr[1] = (_limit); \
  stack_ptr += 2; \
} while (0)

#define GB__SORT_POP(_base, _limit) do { \
  stack_ptr -= 2; \
  (_base)  = stack_ptr[0]; \
  (_limit) = stack_ptr[1]; \
} while (0)

void nja_sort(void *base_, isize count, isize size, Compare_Proc cmp) {
  u8 *i, *j;
  u8 *base = cast(u8 *)base_;
  u8 *limit = base + count*size;
  isize threshold = GB__SORT_INSERT_SORT_THRESHOLD * size;

  // NOTE(bill): Prepare the stack
  u8 *stack[GB__SORT_STACK_SIZE] = {0};
  u8 **stack_ptr = stack;

  for (;;) {
    if ((limit-base) > threshold) {
      // NOTE(bill): Quick sort
      i = base + size;
      j = limit - size;

      memory_swap(((limit-base)/size/2) * size + base, base, size);
      if (cmp(i, j) > 0)    memory_swap(i, j, size);
      if (cmp(base, j) > 0) memory_swap(base, j, size);
      if (cmp(i, base) > 0) memory_swap(i, base, size);

      for (;;) {
        do i += size; while (cmp(i, base) < 0);
        do j -= size; while (cmp(j, base) > 0);
        if (i > j) break;
        memory_swap(i, j, size);
      }

      memory_swap(base, j, size);

      if (j - base > limit - i) {
        GB__SORT_PUSH(base, j);
        base = i;
      } else {
        GB__SORT_PUSH(i, limit);
        limit = j;
      }
    } else {
      // NOTE(bill): Insertion sort
      for (j = base, i = j+size;
           i < limit;
           j = i, i += size) {
        for (; cmp(j, j+size) > 0; j -= size) {
          memory_swap(j, j+size, size);
          if (j == base) break;
        }
      }

      if (stack_ptr == stack) break; // NOTE(bill): Sorting is done!
      GB__SORT_POP(base, limit);
    }
  }
}

#undef GB__SORT_PUSH
#undef GB__SORT_POP

isize nja_binary_search(void *base, isize count, isize size, void *key, Compare_Proc cmp) {
  isize start = 0;
  isize end = count;

  while (start < end) {
    isize mid = start + (end-start)/2;
    isize result = cmp(key, cast(u8 *)base + mid*size);
    if (result < 0)
      end = mid;
    else if (result > 0)
      start = mid+1;
    else
      return mid;
  }

  return -1;
}

//
// Threads
//

struct Thread_Context {
  Arena temporary_storage;
};

void *os_thread_get_context();
void os_thread_set_context(void *ptr);

void *os_alloc(u64 size);
void os_free(void *ptr);

#if !OS_WINDOWS
#include <pthread.h>
#endif

inline u32 thread_get_id() {
  u32 result;

#if OS_WINDOWS

  u8 *ThreadLocalStorage = (u8 *)__readgsqword(0x30);
  result = *(u32 *)(ThreadLocalStorage + 0x48);
#elif OS_MACOS
  result = pthread_mach_thread_np(pthread_self());
#elif OS_LINUX
  result = gettid();
#else
  #error Unsupported architecture for thread_get_id()
#endif

  return result;
}

inline u64 rdtsc(void) {
#if defined(COMPILER_MSVC) && !defined(__clang__)
  return __rdtsc();
#elif defined(__i386__)
  u64 x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
  return x;
#elif defined(__x86_64__)
  u32 hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return (cast(u64)lo) | ((cast(u64)hi)<<32);
#endif
}

#if COMPILER_MSVC
  #define read_barrier() _ReadBarrier()
  #define write_barrier() _WriteBarrier()
#else
  #define read_barrier() asm volatile("" ::: "memory")
  #define write_barrier() asm volatile("" ::: "memory")
#endif

#if COMPILER_MSVC
inline u32 atomic_compare_exchange_u32(u32 volatile *value, u32 New, u32 Expected) {
  u32 result = _InterlockedCompareExchange((long volatile *)value, New, Expected);
  return (result);
}

inline u64 atomic_exchange_u64(u64 volatile *value, u64 New) {
  u64 result = _InterlockedExchange64((__int64 volatile *)value, New);
  return (result);
}

inline u64 atomic_add_u64(u64 volatile *value, u64 Addend) {
  // NOTE(casey): Returns the original value _prior_ to adding
  u64 result = _InterlockedExchangeAdd64((__int64 volatile *)value, Addend);
  return (result);
}
#else // COMPILER_MSVC
inline u32 atomic_compare_exchange_u32(u32 volatile *value, u32 New, u32 Expected) {
  u32 result = __sync_val_compare_and_swap(value, Expected, New);
  return (result);
}

inline u64 atomic_exchange_u64(u64 volatile *value, u64 New) {
  u64 result = __sync_lock_test_and_set(value, New);
  return (result);
}

inline u64 atomic_add_u64(u64 volatile *value, u64 Addend) {
  // NOTE(casey): Returns the original value _prior_ to adding
  u64 result = __sync_fetch_and_add(value, Addend);
  return (result);
}
#endif // COMPILER_MSVC

Memory os_virtual_memory();

void thread_context_init(u64 temporary_storage_size) {
  Thread_Context *ctx = (Thread_Context *)os_alloc(sizeof(Thread_Context));

  arena_init_from_backing_memory(&ctx->temporary_storage, os_virtual_memory(), temporary_storage_size, DEFAULT_MEMORY_BLOCK_SIZE);

  os_thread_set_context(ctx);
}

void thread_context_free() {
  Thread_Context *ctx = (Thread_Context *)os_thread_get_context();

  os_free(&ctx->temporary_storage.data);
  os_free(ctx);
}

Arena *thread_get_temporary_arena() {
  Thread_Context *ctx = (Thread_Context *)os_thread_get_context();
  assert(ctx);
  return &ctx->temporary_storage;
}

void *talloc(u64 size) {
  return arena_alloc(thread_get_temporary_arena(), size);
}

void tpop(u64 size) {
  arena_pop(thread_get_temporary_arena(), size);
}

void reset_temporary_storage() {
  arena_reset(thread_get_temporary_arena());
}

struct Scratch_Memory {
  Arena *arena;
  Arena_Mark restore_point;
};

Scratch_Memory begin_scratch_memory() {
  Scratch_Memory result = {};

  Arena *arena = thread_get_temporary_arena();

  result.arena         = arena;
  result.restore_point = arena_get_position(arena);

  return result;
}

void end_scratch_memory(Scratch_Memory scratch) {
  arena_set_position(scratch.arena, scratch.restore_point);
}


//
// String
//

struct String {
  i64 count;
  u8 *data;

  u8 &operator[](i64 i) {
    assert(i >= 0 && i < count);
    return data[i];
  }
};

#define S(x) String{sizeof(x) - 1, cast(u8 *)x}

#define LIT(str) (int)str.count, (char *)str.data

nja_inline bool char_is_alpha(char ch) {
  return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

nja_inline bool char_is_digit(char ch) {
  return ch >= '0' && ch <= '9';
}

nja_inline bool char_is_numeric(char ch) {
  return char_is_digit(ch) || ch == '.' || ch == '-';
}

nja_inline bool char_is_whitespace(char ch) {
  return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}

nja_inline char char_to_upper(char ch) {
  return ch >= 'a' && ch <= 'z' ? ch - ('a' - 'A') : ch;
}

nja_inline char char_to_lower(char ch) {
  return ch >= 'A' && ch <= 'Z' ? ch + ('a' - 'A') : ch;
}

String make_string(void *data, i64 count) {
  return String{count, (u8 *)data};
}

char *string_to_cstr(Arena *arena, String str) {
  if (!str.count || !str.data) {
    return NULL;
  }

  char *result = push_array(arena, char, str.count + 1); // size for null character
  memory_copy(str.data, result, str.count);
  result[str.count] = 0;
  return result;
}

char *string_to_cstr(String str) {
  return string_to_cstr(thread_get_temporary_arena(), str);
}

i64 cstr_length(char *str) {
  char *at = str;

  // @Speed: this can be made wide
  if (at != NULL) {
    while (*at != 0) {
      at ++;
    }
  }

  return at - str;
}

bool cstr_equals(char *a, char *b) {
  if (a == b) {
    return true;
  }

  i64 length = cstr_length(a);
  return b[length] == (char)NULL && memory_equals(a, b, length);
}

String string_from_cstr(char *data) {
  String result = {};
  i64 length = cstr_length(data);

  if (length > 0) {
    result = {(i64)length, (u8 *)data};
  }

  return result;
}

bool string_equals(String a, String b) {
  return a.count == b.count && memory_equals(a.data, b.data, a.count);
}

bool string_starts_with(String str, String prefix) {
  return str.count >= prefix.count && memory_equals(str.data, prefix.data, prefix.count);
}

bool string_ends_with(String str, String postfix) {
  return str.count >= postfix.count && memory_equals(str.data + (str.count - postfix.count), postfix.data, postfix.count);
}

String string_slice(String str, i64 start_index, i64 end_index) {
  assert(start_index >= 0 && start_index <= str.count);
  assert(end_index >= 0 && end_index <= str.count);
  return make_string(str.data + start_index, end_index - start_index);
}

String string_slice(String str, i64 start_index) {
  return string_slice(str, start_index, str.count);
}

String string_range(u8 *at, u8 *end) {
  assert(end >= at);
  return make_string(at, (end - at));
}

i64 string_index(String str, String search, i64 start_index = 0) {
  for (i64 i = start_index; i < str.count; i += 1) {
    if (memory_equals(str.data + i, search.data, search.count)) {
      return i;
    }
  }

  return -1;
}

bool string_contains(String str, String search) {
  return string_index(str, search) >= 0;
}

String string_copy(Arena *arena, String other) {
  u8 *data = push_array(arena, u8, other.count);
  String copy = make_string(data, other.count);
  memory_copy(other.data, copy.data, other.count);
  return copy;
}

String string_concat(Arena *arena, String a, String b) {
  u8 *data = push_array(arena, u8, a.count + b.count);

  memory_copy(a.data, data + 0,       a.count);
  memory_copy(b.data, data + a.count, b.count);

  return make_string(data, a.count + b.count);
}

String string_concat(String a, String b) {
  return string_concat(thread_get_temporary_arena(), a, b);
}

String string_concat(Arena *arena, String a, String b, String c) {
  u8 *data = push_array(arena, u8, a.count + b.count + c.count);

  memory_copy(a.data, data + 0,                 a.count);
  memory_copy(b.data, data + a.count,           b.count);
  memory_copy(c.data, data + a.count + b.count, c.count);

  return make_string(data, a.count + b.count + c.count);
}

String string_concat(String a, String b, String c) {
  return string_concat(thread_get_temporary_arena(), a, b, c);
}

void string_advance(String *str, i64 amount) {
  assert(str->count >= amount);

  str->count -= amount;
  str->data  += amount;
}

String string_trim_whitespace(String str) {
  while (str.count > 0 && char_is_whitespace(str.data[0])) {
    str.data ++;
    str.count --;
  }

  while (str.count > 0 && char_is_whitespace(str.data[str.count - 1])) {
    str.count --;
  }

  return str;
}

bool string_eat_whitespace(String *str) {
  u64 start_count = str->count;

  while (str->count > 0 && char_is_whitespace((*str)[0])) {
    str->data ++;
    str->count --;
  }

  return start_count != str->count;
}

#include <stdarg.h>

String string_printv(Arena *arena, const char *format, va_list args) {
  // in case we need to try a second time
  va_list args2;
  va_copy(args2, args);

  i64 buffer_size = 1024;
  u8 *buffer = push_array(arena, u8, buffer_size);

  String result = {};
  i64 actual_size = print_to_buffer((char *)buffer, buffer_size, format, args);

  if (actual_size > 0) {
    if (actual_size < buffer_size) {
      arena_pop(arena, buffer_size - actual_size - 1);
      result = make_string(buffer, actual_size);
    } else {
      arena_pop(arena, buffer_size);
      u8 *fixed_buffer = push_array(arena, u8, actual_size + 1);
      i64 final_size = print_to_buffer((char *)fixed_buffer, actual_size + 1, format, args2);
      result = make_string(fixed_buffer, final_size);
    }
  }

  va_end(args2);

  return result;
}

String string_print(Arena *arena, const char *format, ...) {
  String result = {};

  va_list args;
  va_start(args, format);
  result = string_printv(arena, format, args);
  va_end(args);

  return result;
}

char *cstr_print(Arena *arena, const char *format, ...) {
  String result = {};

  va_list args;
  va_start(args, format);
  result = string_printv(arena, format, args);
  va_end(args);

  char *null_terminator = (char *)arena_push(arena, 1);
  *null_terminator = 0;

  return (char *)result.data;
}

#define sprint(...) string_print(thread_get_temporary_arena(), __VA_ARGS__)
#define cprint(...) cstr_print(thread_get_temporary_arena(), __VA_ARGS__)

// NOTE(nick): The path_* functions assume that we are working with a normalized (unix-like) path string.
// All paths should be normalized at the OS interface level, so we can make that assumption here.

String path_join(String path1, String path2) {
  if (!path1.count) return path2;
  if (!path2.count) return path1;
  return sprint("%.*s/%.*s", path1.count, path1.data, path2.count, path2.data);
}

String path_join(String path1, String path2, String path3) {
  if (!path1.count) return path_join(path2, path3);
  if (!path2.count) return path_join(path1, path3);
  if (!path3.count) return path_join(path2, path3);
  return sprint("%.*s/%.*s/%.*s", path1.count, path1.data, path2.count, path2.data, path3.count, path3.data);
}

String path_dirname(String filename) {
  for (i32 i = filename.count - 1; i >= 0; i--) {
    if (filename.data[i] == '/') {
      return string_slice(filename, 0, i);
    }
  }

  return sprint("./");
}

String path_filename(String path) {
  for (i32 i = path.count - 1; i >= 0; i--) {
    char ch = path.data[i];
    if (ch == '/') {
      return string_slice(path, i + 1, path.count);
    }
  }

  return path;
}

String path_strip_extension(String path) {
  for (i32 i = path.count - 1; i >= 0; i--) {
    char ch = path.data[i];
    if (ch == '.') {
      return string_slice(path, 0, i);
    }
  }

  return path;
}

String path_get_extension(String path) {
  for (i32 i = path.count - 1; i >= 0; i--) {
    char ch = path.data[i];
    if (ch == '.') {
      return string_slice(path, i, path.count);
    }
  }

  return {};
}

bool path_is_absolute(String path) {
  if (path.count == 0) { return false; }

  char first_char = path[0];

  return (
    first_char == '/' ||
    // Windows drive root:
    (path.count > 2 && char_is_alpha(first_char) && path[1] == ':' && path[2] == '/')
  );
}

String to_string(bool x)   { if (x) return S("true"); return S("false"); }
String to_string(char x)   { return sprint("%c", x); }
String to_string(char *x)  { return string_from_cstr(x); }
String to_string(i8 x)     { return sprint("%d", x); }
String to_string(u8 x)     { return sprint("%d", x); }
String to_string(i16 x)    { return sprint("%d", x); }
String to_string(u16 x)    { return sprint("%d", x); }
String to_string(i32 x)    { return sprint("%d", x); }
String to_string(u32 x)    { return sprint("%d", x); }
String to_string(i64 x)    { return sprint("%lld", x); }
String to_string(u64 x)    { return sprint("%llu", x); }
/*
String to_string(isize x)  { return sprint("%lld", x); }
String to_string(usize x)  { return sprint("%llu", x); }
*/
String to_string(f32 x)    { return sprint("%.2f", x); }
String to_string(f64 x)    { return sprint("%.4f", x); }
String to_string(void *x)  { return sprint("%p", x); }
String to_string(String x) { return x; }

//
// String conversions
//

struct String16 {
  i64 count;
  u16 *data;
};

struct String32 {
  i64 count;
  u32 *data;
};

struct String_Decode {
  u32 codepoint;
  u8 size; // 1 - 4
};

String16 make_string16(void *data, i64 count) {
  return String16{count, (u16 *)data};
}

String_Decode string_decode_utf8(u8 *str, u32 capacity) {
  static u8 high_bits_to_count[] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 3, 3, 4, 0,
  };

  String_Decode result = {'?', 1};

  u8 count = high_bits_to_count[str[0] >> 3];

  // @Speed: this could be made branchless
  if (capacity >= count) {
    switch (count) {
      case 1: {
        result.size = 1;
        result.codepoint = str[0] & 0x7f;
      } break;

      case 2: {
        result.size = 2;
        result.codepoint = ((str[0] & 0x1f) << 6) | (str[1] & 0x3f);
      } break;

      case 3: {
        result.size = 3;
        result.codepoint = ((str[0] & 0x0f) << 12) | ((str[1] & 0x3f) << 6) | (str[2] & 0x3f);
      } break;

      case 4: {
        result.size = 4;
        result.codepoint = ((str[0] & 0x09) << 18) | ((str[1] & 0x3f) << 12) | ((str[2] & 0x3f) << 6) | (str[3] & 0x3f);
      } break;
    }
  }

  return result;
}

u32 string_encode_utf8(u8 *dest, u32 codepoint) {
  u32 size = 0;

  if (codepoint <= 0x7f) {
    size = 1;
    dest[0] = codepoint;
  }
  else if (codepoint <= 0x7ff) {
    size = 2;
    dest[0] = 0xC0 | (codepoint >> 6);
    dest[1] = 0x80 | (codepoint & 0x3f);
  }
  else if (codepoint <= 0xffff) {
    size = 3;
    dest[0] = 0xE0 | (codepoint >> 12);
    dest[1] = 0x80 | ((codepoint >> 6) & 0x3f);
    dest[2] = 0x80 | (codepoint & 0x3f);
  }
  else if (codepoint <= 0x10FFFF) {
    size = 4;
    dest[0] = 0xF0 | (codepoint >> 18);
    dest[1] = 0x80 | ((codepoint >> 12) & 0x3f);
    dest[2] = 0x80 | ((codepoint >> 6) & 0x3f);
    dest[3] = 0x80 | (codepoint & 0x3f);
  }
  else {
    dest[0] = '?';
    size = 1;
  }

  return size;
}

String_Decode string_decode_utf16(u16 *str, u32 capacity) {
  String_Decode result = {'?', 1};

  u16 x = str[0];

  if (x < 0xD800 || 0xDFFF < x) {
    result.codepoint = x;
    result.size = 1;
  } else if (capacity >= 2) {
    u16 y = str[1];

    if (0xD800 <= x && x < 0xDC00 && 0xDC00 <= y && y < 0xE000) {
      result.codepoint = (((x - 0xD800) << 10) | (y - 0xDC00)) + 0x10000;
      result.size = 2;
    }
  }

  return result;
}

u32 string_encode_utf16(u16 *dest, u32 codepoint) {
  u32 size = 0;

  if (codepoint < 0x10000) {
    dest[0] = codepoint;
    size = 1;
  } else {
    u32 x = (codepoint - 0x10000);
    dest[0] = (x >> 10) + 0xD800;
    dest[1] = (x & 0x3FF) + 0xDC00;
    size = 2;
  }

  return size;
}

String32 string32_from_string(Arena *arena, String str) {
  u32 *memory = push_array(arena, u32, str.count);;

  u8 *p0 = str.data;
  u8 *p1 = str.data + str.count;
  u32 *at = memory;
  i64 remaining = str.count;

  while (p0 < p1) {
    auto decode = string_decode_utf8(p0, remaining);

    *at = decode.codepoint;
    p0 += decode.size;
    at += 1;
    remaining -= decode.size;
  }

  i64 alloc_count = str.count;
  i64 string_count = cast(u64)(at - memory);
  i64 unused_count = string_count - alloc_count;

  arena_pop(arena, unused_count * sizeof(u32));

  String32 result = {string_count, memory};
  return result;
}

String string_from_string32(Arena *arena, String32 str) {
  u8 *memory = push_array(arena, u8, str.count * 4);

  u32 *p0 = str.data;
  u32 *p1 = str.data + str.count;
  u8 *at = memory;

  while (p0 < p1) {
    auto size = string_encode_utf8(at, *p0);

    p0 += 1;
    at += size;
  }

  i64 alloc_count = str.count * 4;
  i64 string_count = cast(u64)(at - memory);
  i64 unused_count = alloc_count - string_count;

  arena_pop(arena, unused_count);

  String result = {string_count, memory};
  return result;
}

String16 string16_from_string(Arena *arena, String str) {
  u16 *data = push_array(arena, u16, str.count * 2 + 1);

  u16 *at = data;
  u8 *p0 = str.data;
  u8 *p1 = str.data + str.count;

  while (p0 < p1) {
    auto decode = string_decode_utf8(p0, (u64)(p1 - p0));
    u32 encode_size = string_encode_utf16(at, decode.codepoint);

    p0 += decode.size;
    at += encode_size;
  }

  *at = 0;

  i64 alloc_count = str.count + 1;
  i64 string_count = cast(u64)(at - data);
  i64 unused_count = alloc_count - string_count - 1;

  arena_pop(arena, unused_count * sizeof(u16));

  String16 result = {string_count, data};
  return result;
}

String string_from_string16(Arena *arena, String16 str) {
  String result = {};
  result.data = push_array(arena, u8, str.count * 3 + 1);

  u16 *p0 = str.data;
  u16 *p1 = str.data + str.count;
  u8 *at = result.data;

  while (p0 < p1) {
    auto decode = string_decode_utf16(p0, cast(u64)(p1 - p0));
    u32 encode_size = string_encode_utf8(at, decode.codepoint);

    p0 += decode.size;
    at += encode_size;
  }

  result.count = at - result.data;

  u64 alloc_count = str.count * 3 + 1;
  u64 string_count = result.count;
  u64 unused_count = alloc_count - string_count;
  arena_pop(arena, unused_count);

  return result;
}

//
// Random
//
struct Random_Lcg {
  u32 state;
};

inline Random_Lcg random_seed_lcg(u32 state) {
  return {state};
}

inline u32 random_next_u32(Random_Lcg *series) {
  return series->state = (series->state * 1103515245 + 12345) & U32_MAX;
}

inline f32 random(Random_Lcg *series) {
  f32 divisor = 1.0f / (f32)U32_MAX;
  return divisor * (f32)random_next_u32(series);
}

inline f32 random_between(Random_Lcg *series, f32 min, f32 max) {
  return lerp(min, max, random(series));
}

inline i32 random_int_between(Random_Lcg *series, i32 min, i32 max) {
  assert(max >= min);
  return min + (i32)(random_next_u32(series) % (max - min + 1));
}

struct Random_Pcg {
  u64 state;
  u64 selector;
};

inline Random_Pcg random_seed_pcg(u64 state, u64 selector) {
  Random_Pcg result;

  result.state = state;
  result.selector = (selector << 1) | 1;

  return result;
}

inline u32 random_next_u32(Random_Pcg *series) {
  u64 state = series->state;
  state = state * 6364136223846793005ULL + series->selector;
  series->state = state;

  u32 pre_rotate = (u32)((state ^ (state >> 18)) >> 27);
  u32 result = nja_rotate_right(pre_rotate, (i32)(state >> 59));

  return result;
}

inline f32 random(Random_Pcg *series) {
  f32 divisor = 1.0f / (f32)U32_MAX;
  return divisor * (f32)random_next_u32(series);
}

inline f32 random_between(Random_Pcg *series, f32 min, f32 max) {
  return lerp(min, max, random(series));
}

inline i32 random_int_between(Random_Pcg *series, i32 min, i32 max) {
  assert(max >= min);
  return min + (i32)(random_next_u32(series) % (max - min + 1));
}

//
// OS
//

struct File {
  void *handle;
  bool has_errors;
  u64 offset;
};

struct File_Info {
  u64 date;
  u64 size;
  String name;
  bool is_directory;
};

enum File_Mode {
  FILE_MODE_READ   = 0x1,
  FILE_MODE_WRITE  = 0x2,
  FILE_MODE_APPEND = 0x4,
};

struct File_Lister;

#define THREAD_PROC(name) u32 name(void *data)
typedef THREAD_PROC(Thread_Proc);

struct Thread {
  u32 id;
  void *handle;
};

struct Thread_Params {
  Thread_Proc *proc;
  void *data;
};

bool os_init();

void *os_alloc(u64 size);
void os_free(void *ptr);
void *os_memory_reserve(u64 size);
bool os_memory_commit(void *ptr, u64 size);
bool os_memory_decommit(void *ptr, u64 size);
bool os_memory_release(void *ptr);

void os_thread_set_context(void *ptr);
void *os_thread_get_context();

f64 os_time_in_miliseconds();
f64 os_time_utc_now();
void os_sleep(f64 miliseconds);

void os_set_high_process_priority(bool enable);

String os_read_entire_file(Allocator allocator, String path);
String os_read_entire_file(String path);
bool os_write_entire_file(String path, String contents);

bool os_make_directory(String path);
bool os_delete_file(String path);
bool os_delete_directory(String path);

File os_file_open(String path, u32 mode_flags);
void os_file_read(File *file, u64 offset, u64 size, void *dest);
void os_file_write(File *file, u64 offset, u64 size, void *data);
u64  os_file_get_size(File *file);
void os_file_append(File *file, u64 size, void *data);
void os_file_append(File *file, String str);
void os_file_close(File *file);

File_Lister os_file_list_begin(Arena *arena, String path);
bool os_file_list_next(File_Lister *iter, File_Info *info);
void os_file_list_end(File_Lister *iter);

File_Info os_get_file_info(Arena *arena, String path);
File_Info os_get_file_info(String path);

Thread os_create_thread(u64 stack_size, Thread_Proc *proc, void *data);
void os_detatch_thread(Thread thread);
u32 os_await_thread(Thread thread);

String os_get_executable_path();
String os_get_current_directory();
String os_get_executable_directory();

bool os_clipboard_set_text(String text);
String os_clipboard_get_text();

#if OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#include <windows.h>

#pragma comment(lib, "user32")

static u64 win32_ticks_per_second = 1;
static u64 win32_counter_offset = 0;

static DWORD win32_thread_context_index = 0;

static bool did_init_os = false;

struct File_Lister {
  Arena *arena;
  WCHAR *find_path;

  HANDLE handle;
  WIN32_FIND_DATAW data;
};

bool os_init() {
  if (did_init_os) return false;

  HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!stdout_handle) {
    AttachConsole(ATTACH_PARENT_PROCESS);
  }

  LARGE_INTEGER perf_frequency = {};
  if (QueryPerformanceFrequency(&perf_frequency)) {
    win32_ticks_per_second = perf_frequency.QuadPart;
  }
  LARGE_INTEGER perf_counter = {};
  if (QueryPerformanceCounter(&perf_counter)) {
    win32_counter_offset = perf_counter.QuadPart;
  }

  win32_thread_context_index = TlsAlloc();

  thread_context_init(megabytes(32));

  did_init_os = true;
  return true;
}

void os_thread_set_context(void *ptr) {
  TlsSetValue(win32_thread_context_index, ptr);
}

void *os_thread_get_context() {
  void *result = TlsGetValue(win32_thread_context_index);
  return result;
}

f64 os_time_in_miliseconds() {
  f64 result = 0;

  LARGE_INTEGER perf_counter;
  if (QueryPerformanceCounter(&perf_counter)) {
    perf_counter.QuadPart -= win32_counter_offset;
    result = (f64)(perf_counter.QuadPart * 1000) / win32_ticks_per_second;
  }

  return result;
}

void os_sleep(f64 miliseconds) {
  LARGE_INTEGER ft;
  ft.QuadPart = -(10 * (__int64)(miliseconds * 1000));

  HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}

void os_set_high_process_priority(bool enable) {
  if (enable) {
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
  } else {
    SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
  }
}

void *os_memory_reserve(u64 size) {
  return VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
}

bool os_memory_commit(void *ptr, u64 size) {
  return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != NULL;
}

bool os_memory_decommit(void *ptr, u64 size) {
  return VirtualFree(ptr, size, MEM_DECOMMIT) != NULL;
}

bool os_memory_release(void *ptr, u64 size) {
  // According to the docs, the size should be 0 when using MEM_RELEASE
  return VirtualFree(ptr, 0, MEM_RELEASE) != NULL;
}

void *os_alloc(u64 size) {
  // Memory allocated by this function is automatically initialized to zero.
  return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void os_free(void *ptr) {
  if (ptr) {
    VirtualFree(ptr, 0, MEM_RELEASE);
    ptr = 0;
  }
}

bool os_file_rename(String from, String to) {
  auto scratch = begin_scratch_memory();

  String16 from16 = string16_from_string(scratch.arena, from);
  String16 to16   = string16_from_string(scratch.arena, to);

  BOOL result = MoveFileW((WCHAR *)from16.data, (WCHAR *)to16.data);
  end_scratch_memory(scratch);
  return result;
}

void win32_file_error(File *file, char *message, String file_name = {}) {
#if DEBUG
  if (file_name.data) {
    print("[file] %s: %.*s\n", message, LIT(file_name));
  } else {
    print("[file] %s\n", message);
  }
#endif

  file->has_errors = true;
}

File os_file_open(String path, u32 mode_flags) {
  File result = {};

  DWORD permissions = 0;
  DWORD creation = 0;

  if (mode_flags & FILE_MODE_READ) {
    permissions |= GENERIC_READ;
    creation = OPEN_EXISTING;
  }

  if (mode_flags & FILE_MODE_APPEND) {
    permissions |= GENERIC_WRITE;
    creation = OPEN_ALWAYS;
  }

  if (mode_flags & FILE_MODE_WRITE) {
    permissions |= GENERIC_WRITE;
    creation = CREATE_ALWAYS;
  }

  auto scratch = begin_scratch_memory();

  String16 path_w = string16_from_string(scratch.arena, path);
  HANDLE handle = CreateFileW(cast(WCHAR *)path_w.data, permissions, FILE_SHARE_READ, 0, creation, 0, 0);
  result.handle = (void *)handle;

  end_scratch_memory(scratch);

  if (handle == INVALID_HANDLE_VALUE) {
    win32_file_error(&result, "Failed to open file", path);
  }

  if ((mode_flags & FILE_MODE_APPEND) && !(mode_flags & FILE_MODE_WRITE)) {
    LARGE_INTEGER file_size;
    GetFileSizeEx(handle, &file_size);
    u64 size = cast(u64)file_size.QuadPart;

    result.offset = size;
  }

  return result;
}

void os_file_read(File *file, u64 offset, u64 size, void *dest) {
  if (file->has_errors) { return; }

  HANDLE handle = (HANDLE)file->handle;

  // :Win32_64BitFileIO
  assert(size <= U32_MAX);
  u32 size32 = cast(u32)size;

  OVERLAPPED overlapped = {};
  overlapped.Offset = (u32)((offset >> 0) & 0xFFFFFFFF);
  overlapped.OffsetHigh = (u32)((offset >> 32) & 0xFFFFFFFF);

  DWORD bytes_read;
  if (ReadFile(handle, dest, size32, &bytes_read, &overlapped) && (size32 == bytes_read)) {
    // Success!
  } else {
    win32_file_error(file, "Failed to read file");
  }
}

void os_file_write(File *file, u64 offset, u64 size, void *data) {
  if (file->has_errors) { return; }

  HANDLE handle = (HANDLE)file->handle;

  // :Win32_64BitFileIO
  assert(size <= U32_MAX);
  u32 size32 = cast(u32)(size);

  OVERLAPPED overlapped = {};
  overlapped.Offset = (u32)((offset >> 0) & 0xFFFFFFFF);
  overlapped.OffsetHigh = (u32)((offset >> 32) & 0xFFFFFFFF);

  DWORD bytes_written;
  if (WriteFile(handle, data, size32, &bytes_written, &overlapped) && (size32 == bytes_written)) {
    // Success!
  } else {
    win32_file_error(file, "Failed to write file");
  }
}

u64 os_file_get_size(File *file) {
  HANDLE handle = (HANDLE)file->handle;

  LARGE_INTEGER file_size;
  GetFileSizeEx(handle, &file_size);
  u64 size = cast(u64)file_size.QuadPart;

  return size;
}

void os_file_close(File *file) {
  if (file->handle) {
    HANDLE handle = (HANDLE)file->handle;
    BOOL success = CloseHandle(handle);
    file->handle = NULL;

    if (!success) {
      win32_file_error(file, "Failed to close file");
    }
  }
}

bool os_delete_file(String path) {
  auto scratch = begin_scratch_memory();
  String16 str = string16_from_string(scratch.arena, path);
  BOOL success = DeleteFileW(cast(WCHAR *)str.data);
  end_scratch_memory(scratch);

  return success;
}

bool os_make_directory(String path) {
  auto scratch = begin_scratch_memory();
  String16 str = string16_from_string(scratch.arena, path);
  BOOL success = CreateDirectoryW(cast(WCHAR *)str.data, NULL);
  end_scratch_memory(scratch);

  return success;
}

bool os_delete_directory(String path) {
  auto scratch = begin_scratch_memory();
  String16 str = string16_from_string(scratch.arena, path);
  BOOL success = RemoveDirectoryW(cast(WCHAR *)str.data);
  end_scratch_memory(scratch);

  return success;
}

String string_from_wstr(Arena *arena, WCHAR *wstr) {
  String16 str16 = make_string16(wstr, wcslen(wstr));
  return string_from_string16(arena, str16);
}

File_Lister os_file_list_begin(Arena *arena, String path) {
  File_Lister result = {};

  String16 find_path = string16_from_string(arena, string_concat(path, S("\\*.*"))); // @Incomplete: use \\?\ prefix?

  result.find_path = cast(WCHAR *)find_path.data;
  result.arena     = arena;
  result.handle    = 0;

  return result;
}

bool os_file_list_next(File_Lister *iter, File_Info *info) {
  bool should_continue = true;

  if (!iter->handle) {
    iter->handle = FindFirstFileW(iter->find_path, &iter->data);
  } else {
    should_continue = FindNextFileW(iter->handle, &iter->data);
  }

  if (iter->handle != INVALID_HANDLE_VALUE) {
    String name = string_from_wstr(iter->arena, iter->data.cFileName);

    // Ignore . and .. "directories"
    while (string_equals(name, S(".")) || string_equals(name, S(".."))) {
      should_continue = FindNextFileW(iter->handle, &iter->data);

      if (!should_continue) {
        return false;
      }

      name = string_from_wstr(iter->arena, iter->data.cFileName);
    }

    *info = {};
    info->name         = name;
    info->date         = ((u64)iter->data.ftLastWriteTime.dwHighDateTime << (u64)32) | (u64)iter->data.ftLastWriteTime.dwLowDateTime;
    info->size         = ((u64)iter->data.nFileSizeHigh << (u64)32) | (u64)iter->data.nFileSizeLow;
    info->is_directory = iter->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
  } else {
    should_continue = false;
  }

  return should_continue;
}

void os_file_list_end(File_Lister *iter) {
  if (iter->handle) {
    // @Memory: restore arena to position when os_file_list_begin was called?
    FindClose(iter->handle);
    iter->handle = 0;
  }
}

File_Info os_get_file_info(Arena *arena, String path) {
  File_Info result = {};

  auto scratch = begin_scratch_memory();
  String16 str = string16_from_string(scratch.arena, path);

  WIN32_FILE_ATTRIBUTE_DATA data;
  if (GetFileAttributesExW(cast(WCHAR *)str.data, GetFileExInfoStandard, &data)) {
    result.name         = path_filename(path);
    result.date         = ((u64)data.ftLastWriteTime.dwHighDateTime << (u64)32) | (u64)data.ftLastWriteTime.dwLowDateTime;
    result.size         = ((u64)data.nFileSizeHigh << (u64)32) | (u64)data.nFileSizeLow;
    result.is_directory = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
  }

  end_scratch_memory(scratch);

  return result;
}

DWORD WINAPI win32_thread_proc(LPVOID lpParameter) {
  Thread_Params *params = (Thread_Params *)lpParameter;

  thread_context_init(megabytes(16));
  assert(params->proc);
  u32 result = params->proc(params->data);
  thread_context_free();

  os_free(params);

  return result;
}

Thread os_create_thread(u64 stack_size, Thread_Proc *proc, void *data) {
  Thread_Params *params = (Thread_Params *)os_alloc(sizeof(Thread_Params));
  params->proc = proc;
  params->data = data;

  DWORD thread_id;
  HANDLE handle = CreateThread(0, stack_size, win32_thread_proc, params, 0, &thread_id);

  Thread result = {};
  result.handle = handle;
  result.id = thread_id;
  return result;
}

void os_detatch_thread(Thread thread) {
  HANDLE handle = (HANDLE)thread.handle;
  CloseHandle(handle);
}

u32 os_await_thread(Thread thread) {
  HANDLE handle = (HANDLE)thread.handle;
  WaitForSingleObject(handle, INFINITE);
  DWORD result;
  GetExitCodeThread(handle, &result);
  // @MemoryLeak: free Thread_Params
  return result;
}

void win32_normalize_path(String path) {
  u8 *at = path.data;

  for (u64 i = 0; i < path.count; i++) {
    if (*at == '\\') {
      *at = '/';
    }

    at ++;
  }
}

String os_get_executable_path() {
  Arena *arena = thread_get_temporary_arena();

  u64 buffer_size = 2048;
  WCHAR *buffer = push_array(arena, WCHAR, buffer_size);

  DWORD length = GetModuleFileNameW(NULL, buffer, buffer_size);
  if (length == 0) {
    return {};
  }

  if (length == buffer_size && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
    bool success = false;

    for (int i = 0; i < 4; i ++) {
      arena_pop(arena, buffer_size * sizeof(WCHAR));

      buffer_size *= 2;
      buffer = push_array(arena, WCHAR, buffer_size);
      length = GetModuleFileNameW(NULL, buffer, buffer_size);

      if (!(length == buffer_size && GetLastError() == ERROR_INSUFFICIENT_BUFFER)) {
        success = true;
        break;
      }
    }

    if (!success) {
      return {};
    }
  }

  arena_pop(arena, (buffer_size - length) * sizeof(WCHAR));

  String result = string_from_string16(arena, make_string16(buffer, length));
  win32_normalize_path(result);

  return result;
}

String os_get_current_directory() {
  Arena *arena = thread_get_temporary_arena();

  DWORD length = GetCurrentDirectoryW(0, NULL);
  if (length == 0) {
    return {};
  }

  WCHAR *buffer = push_array(arena, WCHAR, length);
  DWORD bytes_written = GetCurrentDirectoryW(length, buffer);
  if (bytes_written + 1 != length) {
    return {};
  }

  String result = string_from_string16(arena, make_string16(buffer, length));
  win32_normalize_path(result);

  return result;
}

bool os_clipboard_set_text(String str) {
  int count;
  HANDLE handle;
  WCHAR* buffer;

  auto scratch = begin_scratch_memory();
  defer { end_scratch_memory(scratch); };

  char *cstr = string_to_cstr(scratch.arena, str);

  count = MultiByteToWideChar(CP_UTF8, 0, cstr, -1, NULL, 0);

  if (!count) {
    return false;
  }

  handle = GlobalAlloc(GMEM_MOVEABLE, count * sizeof(WCHAR));
  if (!handle) {
    print("[clipboard] Failed to allocated global handle for clipboard.");
    return false;
  }

  buffer = (WCHAR *)GlobalLock(handle);
  if (!buffer) {
    print("[clipboard] Failed to lock global handle.");
    GlobalFree(handle);
    return false;
  }

  MultiByteToWideChar(CP_UTF8, 0, cstr, -1, buffer, count);
  GlobalUnlock(handle);

  if (!OpenClipboard(NULL)) {
    print("[clipboard] Failed to open clipboard.");
    GlobalFree(handle);
    return false;
  }

  EmptyClipboard();
  SetClipboardData(CF_UNICODETEXT, handle);
  CloseClipboard();

  GlobalFree(handle);

  return true;
}

String os_clipboard_get_text() {
  if (!OpenClipboard(NULL)) {
    print("[clipboard] Failed to open clipboard.");
    return {};
  }

  HANDLE handle = GetClipboardData(CF_UNICODETEXT);
  if (!handle) {
    print("[clipboard] Failed to convert clipboard to string.");
    CloseClipboard();
    return {};
  }

  WCHAR *buffer = (WCHAR *)GlobalLock(handle);
  if (!buffer) {
    print("[clipboard] Failed to lock global handle.");
    CloseClipboard();
    return {};
  }

  u32 buffer_size = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
  String16 str16 = make_string16(buffer, buffer_size);

  auto arena = thread_get_temporary_arena();
  String result = string_from_string16(arena, str16);

  GlobalUnlock(handle);
  CloseClipboard();

  return result;
}

#endif // OS_WINDOWS

#if OS_MACOS

#include <mach/clock.h> // clock_get_time
#include <mach/mach_time.h> // mach_timebase_info, mach_absolute_time
#include <mach/mach_host.h> // mach_host_self
#include <mach/mach_port.h> // mach_port_deallocate
#include <mach-o/dyld.h> // _NSGetExecutablePath

#include <time.h>
#include <unistd.h> // unlink, usleep, getcwd

#include <pthread.h>
#include <stdlib.h> // realpath

#define PATH_MAX 1024 // #include <sys/syslimits.h>

static f64 macos_perf_frequency = 0;
static f64 macos_perf_counter = 0;

static pthread_key_t macos_thread_local_key;

bool os_init() {
  mach_timebase_info_data_t rate_nsec;
  mach_timebase_info(&rate_nsec);

  macos_perf_frequency = 1000000LL * rate_nsec.numer / rate_nsec.denom;
  macos_perf_counter = mach_absolute_time();

  pthread_key_create(&macos_thread_local_key, NULL);

  thread_context_init(megabytes(32));

  return true;
}

void os_thread_set_context(void *ptr) {
  int result = pthread_setspecific(macos_thread_local_key, ptr);
  assert(result == 0);
}

void *os_thread_get_context() {
  void *result = pthread_getspecific(macos_thread_local_key);
  return result;
}

f64 os_time_in_miliseconds() {
  f64 now = mach_absolute_time();
  return (now - macos_perf_counter) / macos_perf_frequency;
}

// @Incomplete: is this correct?
#if 0
f64 os_time_utc_now() {
  struct timespec t;

  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  t.tv_sec = mts.tv_sec;
  t.tv_nsec = mts.tv_nsec;

  return (u64)t.tv_sec * 1000000ull + t.tv_nsec/1000 + 11644473600000000ull;
}
#endif

void os_sleep(f64 miliseconds) {
  u32 ms = (u32)miliseconds;
  struct timespec req = {(time_t)ms / 1000, (long)((ms % 1000) * 1000000)};
  struct timespec rem = {0, 0};
  nanosleep(&req, &rem);
}

String os_get_executable_path() {
  u32 length = 0;
  _NSGetExecutablePath(0, &length);

  Arena *arena = thread_get_temporary_arena();
  char *buffer = (char *)arena_push(arena, length);

  if (_NSGetExecutablePath(buffer, &length) < 0) {
    return {};
  }

  u32 alloc_size = Max(length, PATH_MAX);

  char *normalized = (char *)arena_push(arena, alloc_size);
  if (realpath(buffer, normalized) != NULL)
  {
    auto result = make_string(normalized, length);
    u32 unused_size = alloc_size - result.count;
    arena_pop(arena, unused_size);
    return result;
  }

  return make_string(buffer, length);
}

String os_get_current_directory() {
  Arena *arena = thread_get_temporary_arena();

  char *buffer = (char *)arena_push(arena, PATH_MAX);
  getcwd(buffer, PATH_MAX);

  auto result = string_from_cstr(buffer);
  u32 unused_size = PATH_MAX - result.count;
  arena_pop(arena, unused_size);
  return result;
}

#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <objc/NSObjCRuntime.h>

struct NSString;
typedef NSString * NSPasteboardType;
nja_extern NSPasteboardType const NSPasteboardTypeString; // Available MacOS 10.6

#define objc_msgSend_id ((id (*)(id, SEL))objc_msgSend)
#define objc_method(ReturnType, ...) ((ReturnType (*)(__VA_ARGS__))objc_msgSend)

String os_clipboard_get_text() {
  // NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  id pasteboard = objc_msgSend_id((id)objc_getClass("NSPasteboard"), sel_registerName("generalPasteboard"));
  // NSString *string = [pasteboard stringForType:NSPasteboardTypeString];
  id string = objc_method(id, id, SEL, id)(pasteboard, sel_registerName("stringForType:"), (id)NSPasteboardTypeString);
  // char *text = [string UTF8String];
  char *text = objc_method(char*, id, SEL)(string, sel_registerName("UTF8String"));
  
  return string_copy(thread_get_temporary_arena(), string_from_cstr(text));
}

bool os_clipboard_set_text(String text) {
  auto scratch = begin_scratch_memory();
  char *str = string_to_cstr(scratch.arena, text);
  end_scratch_memory(scratch);

  // [NSString stringWithUTF8String: str];
  id string = objc_method(id, id, SEL, const char *)((id)objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"), str);
  // NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  id pasteboard = objc_msgSend_id((id)objc_getClass("NSPasteboard"), sel_registerName("generalPasteboard"));
  // [pasteboard clearContents];
  objc_msgSend(pasteboard, sel_registerName("clearContents"));
  // [pasteboard setString:str forType:NSPasteboardTypeString];
  BOOL result = objc_method(BOOL, id, SEL, id, id)(pasteboard, sel_registerName("setString:forType:"), string, (id)NSPasteboardTypeString);

  return result == YES;
}

#endif // OS_MACOS

#if OS_MACOS || OS_LINUX

#include <stdlib.h>   // calloc, free, fseek, ftell, fread, fclose
#include <dirent.h>   // opendir, readdir, closedir
#include <sys/stat.h> // stat

struct File_Lister {
  Arena *arena;
  char *find_path;

  DIR *handle;
};

void *os_alloc(u64 size) {
  return calloc(size, 1);
}

void os_free(void *memory) {
  if (memory) {
    free(memory);
    memory = 0;
  }
}

#include <sys/mman.h>

void *os_memory_reserve(u64 size) {
  void *result = 0;

  result = mmap(NULL, size, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  //msync(result, size, MS_SYNC | MS_INVALIDATE);

  return result;
}

bool os_memory_commit(void *ptr, u64 size) {
  int page_size = sysconf(_SC_PAGE_SIZE);

  isize p = (isize)ptr;
  isize p_aligned = AlignDownPow2(p, page_size);

  if (p != p_aligned) {
    isize delta = p - p_aligned;
    ptr = nja_pointer_sub(ptr, delta);
    size += delta;
  }

  size = AlignUpPow2(size, page_size);

  // NOTE(nick): ptr must be aligned to a page boundary.
  int result = mprotect(ptr, size, PROT_READ | PROT_WRITE);

  madvise(ptr, size, MADV_WILLNEED);

  return result == 0;
}

bool os_memory_decommit(void *ptr, u64 size) {
  int page_size = sysconf(_SC_PAGE_SIZE);

  isize p = (isize)ptr;
  isize p_aligned = AlignDownPow2(p, page_size);

  if (p != p_aligned) {
    isize delta = p - p_aligned;
    ptr = nja_pointer_sub(ptr, delta);
    size += delta;
  }

  size = AlignUpPow2(size, page_size);

  // NOTE(nick): ptr must be aligned to a page boundary.
  int result = mprotect(ptr, size, PROT_NONE);

  //madvise(ptr, size, MADV_DONTNEED); // is this too harsh?

  return result == 0;
}

bool os_memory_release(void *ptr, u64 size) {
  return munmap(ptr, size) == 0; // @Incomplete: can size be 0 like on windows?
}

u64 os_file_get_size(File *file) {
  FILE *f = (FILE *)file->handle;

  u64 prev_position = ftell(f);
  fseek(f, 0, SEEK_END);
  u64 size = ftell(f);
  fseek(f, prev_position, SEEK_SET);

  return size;
}

bool os_file_rename(String from, String to) {
  auto scratch = begin_scratch_memory();
  defer { end_scratch_memory(scratch); };

  char *from_cstr = string_to_cstr(scratch.arena, from);
  char *to_cstr   = string_to_cstr(scratch.arena, to);

  return rename(from_cstr, to_cstr) == 0;
}

File_Info os_get_file_info(Arena *arena, String path) {
  struct stat64 stat_info;

  auto mark = arena_get_position(arena);
  defer { arena_set_position(arena, mark); };

  char *cpath = string_to_cstr(arena, path);
  bool file_exists = stat64(cpath, &stat_info) == 0;

  File_Info result = {};

  if (file_exists) {
    result.name         = path_filename(path);
    result.date         = stat_info.st_mtime;
    result.size         = stat_info.st_size;
    result.is_directory = S_ISDIR(stat_info.st_mode);
  }

  return result;
}

void unix_file_error(File *file, char *message, String file_name = {}) {
#if DEBUG
  if (file_name.data) {
    print("[file] %s: %.*s\n", message, LIT(file_name));
  } else {
    print("[file] %s\n", message);
  }
#endif

  file->has_errors = true;
}

File os_file_open(String path, u32 mode_flags) {
  File result = {};

  char mode[4] = {};
  if ((mode_flags & FILE_MODE_READ) && (mode_flags & FILE_MODE_WRITE)) {
    mode[0] = 'r';
    mode[1] = 'b';
    mode[2] = '+';
  } else if (mode_flags & FILE_MODE_APPEND) {
    if (mode_flags & FILE_MODE_WRITE) {
      mode[0] = 'w';
      mode[1] = 'b';
      mode[2] = '+';
    } else {
      mode[0] = 'a';
      mode[1] = 'b';
      if (mode_flags & FILE_MODE_READ) {
        mode[2] = '+';
      }
    }
  } else if (mode_flags & FILE_MODE_WRITE) {
    mode[0] = 'w';
    mode[1] = 'b';
  } else {
    mode[0] = 'r';
    mode[1] = 'b';
  }

  auto scratch = begin_scratch_memory();

  FILE *f = fopen(string_to_cstr(scratch.arena, path), mode);
  result.handle = f;

  end_scratch_memory(scratch);

  if (!f) {
    unix_file_error(&result, "Failed to open file", path);
    return result;
  }

  if ((mode_flags & FILE_MODE_APPEND) && !(mode_flags & FILE_MODE_WRITE)) {
    fseek(f, 0, SEEK_END);
    i32 size = ftell(f);
    if (size != -1) {
      result.offset = size;
    }
  }

  return result;
}

void os_file_read(File *file, u64 offset, u64 size, void *dest) {
  if (file->has_errors) { return; }

  FILE *f = (FILE *)file->handle;

  if (offset != ftell(f)) {
    int seek_result = fseek(f, offset, SEEK_SET);
    if (seek_result != 0) { unix_file_error(file, "Failed to seek file"); }
  }

  size_t bytes_read = fread(dest, sizeof(char), size, f);
  if (bytes_read != sizeof(char) * size) {
    unix_file_error(file, "Failed to read file");
  }
}

void os_file_write(File *file, u64 offset, u64 size, void *data) {
  if (file->has_errors) { return; }

  FILE *f = (FILE *)file->handle;

  if (offset != ftell(f)) {
    int seek_result = fseek(f, offset, SEEK_SET);
    if (seek_result != 0) { unix_file_error(file, "Failed to seek file"); }
  }

  size_t bytes_written = fwrite(data, sizeof(char), size, f);
  if (bytes_written != sizeof(char) * size) {
    unix_file_error(file, "Failed to write to file");
  }
}

void os_file_close(File *file) {
  if (file->handle) {
    FILE *f = (FILE *)file->handle;
    file->handle = NULL;

    int close_result = fclose(f);
    if (close_result != 0) { unix_file_error(file, "Failed to close file"); }
  }
}

File_Lister os_file_list_begin(Arena *arena, String path) {
  File_Lister result = {};

  char *cpath = string_to_cstr(arena, path);
  DIR *handle = opendir(cpath);

  result.find_path = cpath;
  result.handle    = handle;
  result.arena     = arena;

  return result;
}

bool os_file_list_next(File_Lister *iter, File_Info *info) {
  if (!iter->handle) {
    return false;
  }

  struct dirent *data = readdir(iter->handle);

  if (data != NULL) {
    struct stat64 stat_info;
    stat64(iter->find_path, &stat_info);

    String name = string_from_cstr(data->d_name);

    // Ignore . and .. "directories"
    while (string_equals(name, S(".")) || string_equals(name, S(".."))) {
      data = readdir(iter->handle);
      if (data == NULL) {
        return false;
      }

      name = string_from_cstr(data->d_name);
    }

    *info = {};
    info->name         = string_copy(iter->arena, name);
    info->date         = stat_info.st_mtime;
    info->size         = data->d_reclen;
    info->is_directory = data->d_type == DT_DIR;
  }

  return data != NULL;
}

void os_file_list_end(File_Lister *iter) {
  if (iter->handle) {
    closedir(iter->handle);
    iter->handle = 0;
  }
}

bool os_make_directory(String path) {
  auto scratch = begin_scratch_memory();
  int result = mkdir(string_to_cstr(scratch.arena, path), 0700);
  end_scratch_memory(scratch);
  return result == 0;
}

bool os_delete_file(String path) {
  auto scratch = begin_scratch_memory();
  int result = unlink(string_to_cstr(scratch.arena, path));
  end_scratch_memory(scratch);
  return result == 0;
}

bool os_delete_directory(String path) {
  auto scratch = begin_scratch_memory();
  int result = rmdir(string_to_cstr(scratch.arena, path));
  end_scratch_memory(scratch);
  return result == 0;
}

#include <pthread.h>
#include <sys/resource.h> // setpriority

void *unix_thread_proc(void *data) {
  Thread_Params *params = (Thread_Params *)data;
  assert(params->proc);

  thread_context_init(megabytes(32));
  u32 result = params->proc(params->data);
  thread_context_free();
  os_free(params);

  return (void *)result;
}

Thread os_create_thread(u64 stack_size, Thread_Proc *proc, void *data) {
  pthread_attr_t attr;
  if (pthread_attr_init(&attr) != 0) {
    return {};
  }

  if (stack_size > 0) {
    if (pthread_attr_setstacksize(&attr, stack_size) != 0) {
      return {};
    }
  }

  Thread_Params *params = (Thread_Params *)os_alloc(sizeof(Thread_Params));
  params->proc = proc;
  params->data = data;

  pthread_t thread_id;
  pthread_create(&thread_id, &attr, unix_thread_proc, params);

  Thread result = {};
  result.handle = (void *)thread_id;
  return result;
}

void os_detatch_thread(Thread thread) {
  pthread_t tid = (pthread_t)thread.handle;
  pthread_detach(tid);
}

u32 os_await_thread(Thread thread) {
  pthread_t tid = (pthread_t)thread.handle;
  void *result = 0;
  pthread_join(tid, &result);
  // @MemoryLeak: free Thread_Params
  return *(u32 *)result;
}

void os_set_high_process_priority(bool enable) {
  if (enable) {
    setpriority(PRIO_PROCESS, getpid(), -20);
  } else {
    setpriority(PRIO_PROCESS, getpid(), 0);
  }
}

#endif // OS_MACOS || OS_LINUX

//
// DLLs
//

struct OS_Library {
  void *handle;
};

typedef void (*OS_Library_Proc)(void);

OS_Library os_lib_load(String path);
void os_lib_release(OS_Library lib);
OS_Library_Proc os_lib_get_proc(OS_Library lib, char *proc_name);

#if OS_WINDOWS

OS_Library os_lib_load(String path) {
  auto scratch = begin_scratch_memory();

  OS_Library result = {};
  result.handle     = LoadLibraryW((WCHAR *)string16_from_string(scratch.arena, path).data);

  end_scratch_memory(scratch);

  return result;
}

inline void os_lib_release(OS_Library lib) {
  if (lib.handle) {
    FreeLibrary((HMODULE)lib.handle);
    lib.handle = 0;
  }
}

inline OS_Library_Proc gb_dll_proc_address(OS_Library lib, char *proc_name) {
  return (OS_Library_Proc)GetProcAddress((HMODULE)lib.handle, proc_name);
}

#else // POSIX

#include <dlfcn.h>

OS_Library os_lib_load(String path) {
  auto scratch = begin_scratch_memory();

  OS_Library result = {};
  // TODO(bill): Should this be RTLD_LOCAL?
  result.handle     = dlopen(string_to_cstr(scratch.arena, path), RTLD_LAZY | RTLD_GLOBAL);

  end_scratch_memory(scratch);

  return result;
}

inline void os_lib_release(OS_Library lib) {
  if (lib.handle) {
    dlclose(lib.handle);
    lib.handle = 0;
  }
}

inline OS_Library_Proc os_lib_get_proc(OS_Library lib, char *proc_name) {
  return (OS_Library_Proc)dlsym(lib.handle, proc_name);
}

#endif

inline OS_Library_Proc os_lib_get_proc(OS_Library lib, String proc_name) {
  auto scratch = begin_scratch_memory();
  auto result = os_lib_get_proc(lib, string_to_cstr(scratch.arena, proc_name));
  end_scratch_memory(scratch);
  return result;
}

//
// Allocator
//

void *allocator_alloc(Allocator allocator, u64 size) {
  assert(allocator.proc);
  void *result = allocator.proc(ALLOCATOR_MODE_ALLOC, size, 0, NULL, allocator.data, DEFAULT_MEMORY_ALIGNMENT);
  return result;
}

void *allocator_alloc_aligned(Allocator allocator, u64 size, u32 alignment) {
  assert(allocator.proc);
  void *result = allocator.proc(ALLOCATOR_MODE_ALLOC, size, 0, NULL, allocator.data, alignment);
  return result;
}

void allocator_free(Allocator allocator, void *data) {
  assert(allocator.proc);
  if (data != NULL) {
    allocator.proc(ALLOCATOR_MODE_FREE, 0, 0, data, allocator.data, 0);
  }
}

void *allocator_realloc(Allocator allocator, void *data, u64 new_size, u64 old_size) {
  assert(allocator.proc);
  return allocator.proc(ALLOCATOR_MODE_RESIZE, new_size, old_size, data, allocator.data, DEFAULT_MEMORY_ALIGNMENT);
}

void *allocator_realloc_aligned(Allocator allocator, void *data, u64 new_size, u64 old_size, u32 alignment) {
  assert(allocator.proc);
  return allocator.proc(ALLOCATOR_MODE_RESIZE, new_size, old_size, data, allocator.data, alignment);
}

ALLOCATOR_PROC(os_allocator_proc) {
  switch (mode) {
    case ALLOCATOR_MODE_FREE: {
      os_free(old_memory_pointer);
      return NULL;
    }

    case ALLOCATOR_MODE_FREE_ALL: {
      // @Incomplete
      return NULL;
    }

    case ALLOCATOR_MODE_RESIZE:
    case ALLOCATOR_MODE_ALLOC: {
      u64 actual_size = requested_size + nja_align_offset(0, alignment);

      void *result = os_alloc(actual_size);

      if (result && old_memory_pointer && mode == ALLOCATOR_MODE_RESIZE) {
        // @Incomplete: provide os-level realloc function
        memory_copy(old_memory_pointer, result, Min(requested_size, old_size));
        os_free(old_memory_pointer);
      }

      return result;
    }

    default: {
      return NULL;
    }
  }
}

Allocator os_allocator() {
  return Allocator{os_allocator_proc, 0};
}

ALLOCATOR_PROC(arena_allocator_proc) {
  Arena *arena = cast(Arena *)allocator_data; 

  switch (mode) {
    case ALLOCATOR_MODE_ALLOC: {
      return arena_alloc_aligned(arena, requested_size, alignment);
    }

    case ALLOCATOR_MODE_RESIZE: {
      return arena_realloc_aligned(arena, requested_size, old_size, old_memory_pointer, alignment);
    }

    case ALLOCATOR_MODE_FREE: {
      arena_free_pointer(arena, old_memory_pointer);
      return NULL;
    }

    case ALLOCATOR_MODE_FREE_ALL: {
      arena_reset(arena);
      return NULL;
    }

    default: {
      return NULL;
    }
  }
}

Allocator arena_allocator(Arena *arena) {
  return Allocator{arena_allocator_proc, arena};
}

Arena arena_create_from_allocator(Allocator allocator, u64 size) {
  Arena result = {};
  result.data = cast(u8 *)allocator_alloc(allocator, size);
  result.size = size;
  return result;
}

//
// Array
//

#define For(array) for (auto it : array)

#define For_Array(it, array) for (auto it : array)

#define Forp(array) \
  auto CONCAT(__array, __LINE__) = (array); \
  for (auto it = CONCAT(__array, __LINE__).begin(); it < CONCAT(__array, __LINE__).end(); it++)

#define For_Index(array) for (i64 index = 0; index < cast(i64)(array).count; index++)

#define For_Index_Reverse(array) \
  for (i64 index = cast(i64)(array).count - 1; index >= 0; index--)

template <typename T>
struct Array {
  Allocator allocator = os_allocator();
  u64       capacity = 0;
  i64       count = 0;
  T *       data = NULL;

  T &operator[](i64 i) {
    assert(i >= 0 && i < count);
    return data[i];
  }

  T *begin() { return data ? &data[0] : NULL; }
  T *end()   { return data ? &data[count] : NULL; }
};

template <typename T>
void array_init(Array<T> &it, u64 initial_capacity = 16) {
  it.count = 0;
  it.data = NULL;
  array_resize(it, initial_capacity);
}

template <typename T>
void array_reset(Array<T> &it) {
  it.count = 0;
}

template <typename T>
void array_resize(Array<T> &it, u64 next_capacity) {
  if (it.data && it.capacity == next_capacity) return;

  u64 prev_size = it.capacity * sizeof(T);
  u64 next_size = next_capacity * sizeof(T);

  it.data = (T *)allocator_realloc(it.allocator, it.data, next_size, prev_size);
  assert(it.data);
  it.capacity = next_capacity;
}

template <typename T>
bool array_at_capacity(Array<T> &it) {
  return it.capacity <= it.count;
}

template <typename T>
T *array_peek(Array<T> &it) {
  return it.count > 0 ? &it.data[it.count - 1] : NULL;
}

template <typename T>
void *array_pop(Array<T> &it) {
  void *removed = array_peek(it);
  if (removed) it.count --;
  return removed;
}

template <typename T>
void array_remove_unordered(Array<T> &it, i64 index) {
  assert(index >= 0 && index < it.count);
  memory_copy(&it.data[it.count - 1], &it.data[index], sizeof(T));
  it.count--;
}

template <typename T>
void array_remove_ordered(Array<T> &it, i64 index, i64 num_to_remove = 1) {
  assert(index >= 0 && index < it.count);
  assert(num_to_remove > 0 && index + num_to_remove <= it.count);

  u64 i = index + num_to_remove;
  u64 remaining_count = it.count - i;

  memory_move(&it.data[i], &it.data[index], sizeof(T) * remaining_count);

  it.count -= num_to_remove;
}

template <typename T>
void array_free(Array<T> &it) {
  if (it.data) {
    allocator_free(it.allocator, it.data);
    it.data = NULL;
  }

  it.capacity = 0;
  it.count = 0;
}

template <typename T>
void array_reserve(Array<T> &it, u64 minimum_count) {
  if (it.capacity < minimum_count) {
    array_resize(it, minimum_count);
  }
}

template <typename T>
T *array_push(Array<T> &it) {
  if (it.count >= it.capacity) {
    array_resize(it, it.capacity ? it.capacity * 2 : 16);
  }

  memory_set(&it.data[it.count], 0, sizeof(T));
  return &it.data[it.count ++];
}

template <typename T>
T *array_push(Array<T> &it, T item) {
  if (it.count >= it.capacity) {
    array_resize(it, it.capacity ? it.capacity * 2 : 16);
  }

  it.data[it.count] = item;
  return &it.data[it.count ++];
}

template <typename T>
void array_concat(Array<T> &it, T *data, u64 count) {
  if (!count|| !data) return;

  array_reserve(it, it.count + count);
  memory_copy(data, &it.data[it.count], count * sizeof(T));
  it.count += count;
}

template <typename T>
void array_sort(Array<T> &it) {
  auto compare = [](void *a, void *b) -> i32 {
    if (*(T *)a == *(T *)b) return 0;
    return (*(T *)a < *(T *)b) ? -1 : 1;
  };

  nja_sort(it.data, it.count, sizeof(T), compare);
}

//
// Slice
//

template <typename T>
struct Slice {
  i64 count = 0;
  T * data = NULL;

  T &operator[](i64 i) {
    assert(i >= 0 && i < count);
    return data[i];
  }

  T *begin() { return data ? &data[0] : NULL; }
  T *end()   { return data ? &data[count] : NULL; }
};

template <typename T>
Slice<T> make_slice(T *data, i64 count) {
  Slice<T> result = {};

  result.data = data;
  result.count = count;

  return result;
}

template <typename T>
Slice<T> slice_array(Array<T> &array, i64 lo, i64 hi) {
  assert(lo >= 0 && lo <= hi && hi <= array.count);

  Slice<T> result = {};

  result.data  = array.data + lo;
  result.count = hi - lo;

  return result;
}

template <typename T>
Slice<T> slice_from_array(Array<T> &array) {
  Slice<T> result = {};

  result.data  = array.data;
  result.count = array.count;

  return result;
}

//
// Hash
//

u32 murmur32_seed(void const *data, isize len, u32 seed) {
  u32 const c1 = 0xcc9e2d51;
  u32 const c2 = 0x1b873593;
  u32 const r1 = 15;
  u32 const r2 = 13;
  u32 const m  = 5;
  u32 const n  = 0xe6546b64;

  isize i, nblocks = len / 4;
  u32 hash = seed, k1 = 0;
  u32 const *blocks = cast(u32 const*)data;
  u8 const *tail = cast(u8 const *)(data) + nblocks*4;

  for (i = 0; i < nblocks; i++) {
    u32 k = blocks[i];
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;

    hash ^= k;
    hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
  }

  switch (len & 3) {
  case 3:
    k1 ^= tail[2] << 16;
  case 2:
    k1 ^= tail[1] << 8;
  case 1:
    k1 ^= tail[0];

    k1 *= c1;
    k1 = (k1 << r1) | (k1 >> (32 - r1));
    k1 *= c2;
    hash ^= k1;
  }

  hash ^= len;
  hash ^= (hash >> 16);
  hash *= 0x85ebca6b;
  hash ^= (hash >> 13);
  hash *= 0xc2b2ae35;
  hash ^= (hash >> 16);

  return hash;
}

u64 murmur64_seed(void const *data_, isize len, u64 seed) {
  u64 const m = 0xc6a4a7935bd1e995ULL;
  i32 const r = 47;

  u64 h = seed ^ (len * m);

  u64 const *data = cast(u64 const *)data_;
  u8  const *data2 = cast(u8 const *)data_;
  u64 const* end = data + (len / 8);

  while (data != end) {
    u64 k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  switch (len & 7) {
  case 7: h ^= cast(u64)(data2[6]) << 48;
  case 6: h ^= cast(u64)(data2[5]) << 40;
  case 5: h ^= cast(u64)(data2[4]) << 32;
  case 4: h ^= cast(u64)(data2[3]) << 24;
  case 3: h ^= cast(u64)(data2[2]) << 16;
  case 2: h ^= cast(u64)(data2[1]) << 8;
  case 1: h ^= cast(u64)(data2[0]);
    h *= m;
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

u32 murmur32(void const *data, isize len) { return murmur32_seed(data, len, 0x9747b28c); }
u64 murmur64(void const *data, isize len) { return murmur64_seed(data, len, 0x9747b28c); }

u32 fnv32a(void const *data, isize len) {
  u8 const *bytes = cast(u8 const *)data;
  u32 h = 0x811c9dc5;
  
  for (; len >= 8; len -= 8, bytes += 8) {
    h = (h ^ bytes[0]) * 0x01000193;
    h = (h ^ bytes[1]) * 0x01000193;
    h = (h ^ bytes[2]) * 0x01000193;
    h = (h ^ bytes[3]) * 0x01000193;
    h = (h ^ bytes[4]) * 0x01000193;
    h = (h ^ bytes[5]) * 0x01000193;
    h = (h ^ bytes[6]) * 0x01000193;
    h = (h ^ bytes[7]) * 0x01000193;
  }

  while (len--) {
    h = (h ^ *bytes++) * 0x01000193;
  }
  return h;
}

u64 fnv64a(void const *data, isize len) {
  u8 const *bytes = cast(u8 const *)data;
  u64 h = 0xcbf29ce484222325ull;
  
  for (; len >= 8; len -= 8, bytes += 8) {
    h = (h ^ bytes[0]) * 0x100000001b3ull;
    h = (h ^ bytes[1]) * 0x100000001b3ull;
    h = (h ^ bytes[2]) * 0x100000001b3ull;
    h = (h ^ bytes[3]) * 0x100000001b3ull;
    h = (h ^ bytes[4]) * 0x100000001b3ull;
    h = (h ^ bytes[5]) * 0x100000001b3ull;
    h = (h ^ bytes[6]) * 0x100000001b3ull;
    h = (h ^ bytes[7]) * 0x100000001b3ull;
  }

  while (len--) {
    h = (h ^ *bytes++) * 0x100000001b3ull;
  }
  return h;
}

//
// Hash Table
//

#define For_Table(it, table) \
  for (auto it = (table).begin(); it < (table).end(); it++) \
    if (it->hash < TABLE_FIRST_VALID_HASH) continue; else

const int TABLE_NEVER_OCCUPIED_HASH = 0;
const int TABLE_REMOVED_HASH = 1;
const int TABLE_FIRST_VALID_HASH = 2;

const int TABLE_SIZE_MIN = 32;

template <typename K>
bool table_key_equals(const K &a, const K &b) {
  return memory_equals(a, b, sizeof(K));
}

bool table_key_equals(const char *&a, const char *&b) {
  return cstr_equals((char *)a, (char *)b); 
}

bool table_key_equals(const String &a, const String &b) {
  return string_equals(a, b);
}

template <typename K>
u32 table_key_hash(K key) {
  return murmur32(&key, sizeof(K));
}

u32 table_key_hash(char *key) {
  return fnv32a(&key, cstr_length(key));
}

u32 table_key_hash(String key) {
  return fnv32a(key.data, key.count);
}

template <typename K, typename V>
struct Hash_Table {
  struct Entry {
    u32 hash;
    K key;
    V value;
  };

  Allocator allocator = os_allocator();
  u32       capacity = 0;
  i32       count = 0;
  u32       slots_filled = 0;
  Entry *   data = 0;

  #if 0
  u32 add_collisions = 0;
  u32 find_collisions = 0;
  #endif

  Entry &operator[](i64 i) {
    assert(i >= 0 && i < capacity);
    return data[i];
  }

  Entry *begin() { return data ? &data[0] : NULL; }
  Entry *end()   { return data ? &data[capacity] : NULL; }
};

template <typename K, typename V>
void table_init(Hash_Table<K, V> &it, u32 table_size) {
  it.capacity = max_u32(nja_next_power_of_two(table_size), TABLE_SIZE_MIN);
  it.count = 0;
  it.slots_filled = 0;

  #if 0
  it.add_collisions = 0;
  it.find_collisions = 0;
  #endif

  assert(nja_is_power_of_two(it.capacity)); // Must be a power of two!

  it.data = (typename Hash_Table<K, V>::Entry *)allocator_alloc(it.allocator, it.capacity * sizeof(typename Hash_Table<K, V>::Entry));
  table_reset(it); // @Speed: Can be removed if data is initialized to zero!
}

template <typename K, typename V>
void table_reset(Hash_Table<K, V> &it) {
  it.count = 0;
  it.slots_filled = 0;

  if (it.data) {
    for (u32 i = 0; i < it.capacity; i++) {
      it.data[i].hash = 0;
    }
  }
}

template <typename K, typename V>
void table_free(Hash_Table<K, V> &it) {
  if (it.data) {
    allocator_free(it.allocator, it.data);
    it.data = NULL;
    it.capacity = 0;
    it.count = 0;
    it.slots_filled = 0;
  }
}

template <typename K, typename V>
void table_expand(Hash_Table<K, V> &it) {
  u32 next_capacity = it.capacity ? it.capacity * 2 : TABLE_SIZE_MIN;

  assert(nja_is_power_of_two(it.capacity)); // Must be a power of two!

  auto *old_data = it.data;
  u32 old_capacity = it.capacity;

  table_init(it, next_capacity);

  // count and slots_filled will be incremented by add.
  it.count        = 0;
  it.slots_filled = 0;

  for (u32 i = 0; i < old_capacity; i++) {
    auto *entry = &old_data[i];

    if (entry->hash >= TABLE_FIRST_VALID_HASH) {
      table_add(it, entry->key, entry->value);
    }
  }

  allocator_free(it.allocator, old_data);
}

// Sets the key-value pair, replacing it if it already exists.
template <typename K, typename V>
V *table_set(Hash_Table<K, V> &it, K key, V value) {
  if (it.data) {
    auto result = table_find_pointer(key);
    if (result) {
      *result = value;
      return result;
    }
  }

  return table_add(key, value);
}

// Adds the given key-value pair to the table, returns a pointer to the inserted item.
template <typename K, typename V>
V *table_add(Hash_Table<K, V> &it, K key, V value) {
  // The + 1 is here to handle the weird case where the table size is 1 and you add the first item
  // slots_filled / capacity >= 7 / 10 ...therefore:
  // slots_filled * 10 >= capacity * 7
  if ((it.slots_filled + 1) * 10 >= it.capacity * 7) table_expand(it);

  assert(it.slots_filled <= it.capacity);

  u32 hash = table_key_hash(key);
  if (hash < TABLE_FIRST_VALID_HASH) hash += TABLE_FIRST_VALID_HASH;

  u32 index = hash & (it.capacity - 1);

  while (it.data[index].hash) {
    #if 0
    it.add_collisions += 1;
    #endif

    index += 1;
    index &= (it.capacity - 1);
  }

  it.count ++;
  it.slots_filled ++;
  it.data[index] = {hash, key, value};

  return &it.data[index].value;
}

template <typename K, typename V>
V *table_find_pointer(Hash_Table<K, V> &it, K key) {
  if (it.data) {
    u32 hash = table_key_hash(key);
    if (hash < TABLE_FIRST_VALID_HASH) hash += TABLE_FIRST_VALID_HASH;
    u32 index = hash & (it.capacity - 1);

    while (it.data[index].hash) {
      auto entry = &it.data[index];

      if (entry->hash == hash && table_key_equals(&entry->key, &key)) {
        return &entry->value;
      }

      index += 1;
      index &= (it.capacity - 1);
    }
  }

  return NULL;
}

template <typename K, typename V>
bool table_remove(Hash_Table<K, V> &it, K key) {
  assert(it.data); // Must be initialized!

  u32 hash = table_key_hash(key);
  if (hash < TABLE_FIRST_VALID_HASH) hash += TABLE_FIRST_VALID_HASH;
  u32 index = hash & (it.capacity - 1);

  while (it.data[index].hash) {
    if (it.data[index].hash == hash) {
      it.data[index].hash = TABLE_REMOVED_HASH; // No valid entry will ever hash to TABLE_REMOVED_HASH.
      it.count --;
      return true;
    }

    index += 1;
    index &= (it.capacity - 1);
  }

  return false;
}

template <typename K, typename V>
V table_find(Hash_Table<K, V> &it, K key) {
  V *result = table_find_pointer(it, key);

  if (result) {
    return *result;
  }

  return {};
}

template <typename K, typename V>
V table_contains(Hash_Table<K, V> &it, K key) {
  return table_find_pointer(it, key) != NULL;
}

//
// Extended OS Functions
//

String os_read_entire_file(Allocator allocator, String path) {
  auto file = os_file_open(path, FILE_MODE_READ);
  auto size = os_file_get_size(&file);

  String result = {};
  result.data = cast(u8 *)allocator_alloc(allocator, size);
  result.count = size;

  os_file_read(&file, 0, size, result.data);
  os_file_close(&file);

  return result;
}

String os_read_entire_file(String path) {
  return os_read_entire_file(arena_allocator(thread_get_temporary_arena()), path);
}

bool os_write_entire_file(String path, String contents) {
  auto file = os_file_open(path, FILE_MODE_WRITE);
  os_file_write(&file, 0, contents.count, contents.data);
  os_file_close(&file);
  return !file.has_errors;
}

File_Info os_get_file_info(String path) {
  return os_get_file_info(thread_get_temporary_arena(), path);
}

File_Lister os_file_list_begin(String path) {
  return os_file_list_begin(thread_get_temporary_arena(), path);
}

void os_file_write(File *file, u64 offset, String str) {
  os_file_write(file, offset, str.count, str.data);
}

void os_file_append(File *file, u64 size, void *data) {
  os_file_write(file, file->offset, size, data);
  file->offset += size;
}

void os_file_append(File *file, String str) {
  os_file_write(file, file->offset, str.count, str.data);
  file->offset += str.count;
}

Memory os_virtual_memory() {
  Memory result = {};

  result.reserve  = &os_memory_reserve;
  result.commit   = &os_memory_commit;
  result.decommit = &os_memory_decommit;
  result.release  = &os_memory_release;

  return result;
}

String os_get_executable_directory() {
  String result = os_get_executable_path();
  return path_dirname(result);
}

bool os_delete_entire_directory(String path) {
  auto scratch = begin_scratch_memory();

  bool success = true;

  auto handle = os_file_list_begin(scratch.arena, path);

  File_Info info = {};
  while (os_file_list_next(&handle, &info)) {
    auto file_path = path_join(path, info.name);

    if (info.is_directory) {
      // @Speed: is the recursive ever a problem?
      success |= os_delete_entire_directory(file_path);
    } else {
      success |= os_delete_file(file_path);
    }
  }

  os_file_list_end(&handle);

  success |= os_delete_directory(path);

  end_scratch_memory(scratch);
  return success;
}

nja_inline bool os_file_info_exists(File_Info info) {
  return info.name.count > 0;
}

bool os_file_or_directory_exists(String path) {
  return os_file_info_exists(os_get_file_info(path));
}

bool os_make_directory_recursive(String path) {
  i64 index = string_index(path, S("/"));

  while (true) {
    index = string_index(path, S("/"), index + 1);
    if (index < 0) break;

    auto slice = string_slice(path, 0, index);

    if (!os_file_or_directory_exists(slice)) {
      bool success = os_make_directory(slice);
      if (!success) return false;
    }
  }

  if (!os_file_or_directory_exists(path)) {
    bool success = os_make_directory(path);
    if (!success) return false;
  }

  return true;
}

Array<File_Info *> os_scan_directory(Arena *arena, String path) {
  Array<File_Info *> result = {};
  result.allocator = arena_allocator(arena);

  array_init(result, 32); // 32 should be enough for anybody.

  auto handle = os_file_list_begin(arena, path);

  File_Info info = {};
  while (os_file_list_next(&handle, &info)) {
    auto ptr = array_push(result);

    auto it = push_struct(arena, File_Info);
    memory_copy(&info, it, sizeof(File_Info));
    it->name = string_copy(arena, info.name);

    *ptr = it;
  }

  os_file_list_end(&handle);

  return result;
}

Array<File_Info *> os_scan_directory(String path) {
  return os_scan_directory(thread_get_temporary_arena(), path);
}

Array<File_Info *> os_scan_files_recursive(Arena *arena, String path, i32 max_depth = 2048) {
  Array<File_Info *> results = {};
  Array<String> stack = {};

  results.allocator = arena_allocator(arena);
  stack.allocator   = arena_allocator(arena); // @Memory: these can be freed after the function ends

  array_push(stack, S(""));

  while (max_depth >= 0) {
    max_depth -= 1;

    u32 n = stack.count;

    for (u32 i = 0; i < n; i++) {
      auto dir = stack[i];
      auto infos = os_scan_directory(arena, path_join(path, dir));

      For (infos) {
        it->name = path_join(dir, it->name);

        if (it->is_directory) {
          array_push(stack, it->name);
        } else {
          array_push(results, it);
        }
      }
    }

    if (n > 0) {
      array_remove_ordered(stack, 0, n);
    }
  }

  return results;
}

Array<File_Info *> os_scan_files_recursive(String path, u32 max_depth = 1024) {
  return os_scan_files_recursive(thread_get_temporary_arena(), path, max_depth);
}

//
// Extended Strings
//

Array<String> string_split(Allocator allocator, String str, String split) {
  Array<String> result = {};
  result.allocator = allocator;

  u8 *at = str.data;
  u8 *word_first = at;
  u8 *str_end = str.data + str.count;

  for (; at < str_end; at += 1) {
    if (*at == split.data[0])
    {
      if (string_starts_with(string_range(at, str_end), split)) {
        array_push(result, string_range(word_first, at));
        at += split.count - 1;
        word_first = at + 1;
        continue;
      }
    }
  }

  array_push(result, string_range(word_first, str_end));

  return result;
}

Array<String> string_split(String str, String split) {
  return string_split(arena_allocator(thread_get_temporary_arena()), str, split);
}

String string_join(Allocator allocator, Array<String> list, String join) {
  u64 size = join.count * (list.count - 1);
  For (list) size += it.count;

  u8 *data = cast(u8 *)allocator_alloc(allocator, size);

  bool is_mid = false;
  u8 *at = data;

  For_Index (list) {
    auto it = list[index];

    if (is_mid) {
      memory_copy(join.data, at, join.count);
      at += join.count;
    }

    memory_copy(it.data, at, it.count);
    at += it.count;
    is_mid = index + 1 < list.count;
  }

  return make_string(data, size);
}

String string_join(Array<String> list, String join) {
  return string_join(arena_allocator(thread_get_temporary_arena()), list, join);
}

//
// Threading Primitives
//

struct Semaphore {
  void *handle;
};

Semaphore semaphore_create(u32 max_count);
void semaphore_signal(Semaphore *sem);
void semaphore_wait_for(Semaphore *sem, bool infinite);
void semaphore_destroy(Semaphore *sem);

struct Mutex {
  void *handle;
};

Mutex mutex_create(u32 spin_count = 0);
void  mutex_aquire_lock(Mutex *mutex);
bool  mutex_try_aquire_lock(Mutex *mutex);
void  mutex_release_lock(Mutex *mutex);
void  mutex_destroy(Mutex *mutex);

#define WORKER_PROC(name) void name(void *data)
typedef WORKER_PROC(Worker_Proc);

struct Work_Entry {
  Worker_Proc *callback;
  void *data;
};

struct Work_Queue {
  u32 volatile completion_goal;
  u32 volatile completion_count;

  u32 volatile next_entry_to_write;
  u32 volatile next_entry_to_read;
  Semaphore semaphore;

  Work_Entry entries[256];
};

struct Worker_Params {
  Work_Queue *queue;
};

void work_queue_create(Work_Queue *queue, u32 thread_count, Worker_Params *thread_params);
void work_queue_add_entry(Work_Queue *queue, Worker_Proc *callback, void *data);
void work_queue_complete_all_work(Work_Queue *queue);

#if OS_WINDOWS

Semaphore semaphore_create(u32 max_count) {
  Semaphore result = {};
  result.handle = CreateSemaphoreEx(0, 0, max_count, 0, 0, SEMAPHORE_ALL_ACCESS);
  return result;
}

void semaphore_signal(Semaphore *sem) {
  BOOL ok = ReleaseSemaphore(sem->handle, 1, 0);
  assert(ok);
}

void semaphore_wait_for(Semaphore *sem, bool infinite) {
  DWORD res;

  if (infinite) {
    res = WaitForSingleObject(sem->handle, INFINITE);
  } else {
    res = WaitForSingleObject(sem->handle, 50);
  }

  assert(res != WAIT_FAILED);
}

void semaphore_destroy(Semaphore *sem) {
  CloseHandle(sem->handle);
  sem->handle = 0;
}

Mutex mutex_create(u32 spin_count) {
  Mutex result = {};

  result.handle = os_alloc(sizeof(CRITICAL_SECTION));
  assert(result.handle);

  InitializeCriticalSection(cast(LPCRITICAL_SECTION)result.handle);

  if (spin_count) {
    SetCriticalSectionSpinCount(cast(LPCRITICAL_SECTION)result.handle, spin_count);
  }

  return result;
}

void mutex_aquire_lock(Mutex *mutex) {
  EnterCriticalSection(cast(LPCRITICAL_SECTION)mutex->handle);
}

bool mutex_try_aquire_lock(Mutex *mutex) {
  return TryEnterCriticalSection(cast(LPCRITICAL_SECTION)mutex->handle) != 0;
}

void mutex_release_lock(Mutex *mutex) {
  LeaveCriticalSection(cast(LPCRITICAL_SECTION)mutex->handle);
}

void mutex_destroy(Mutex *mutex) {
  // @Robustness: track if it's been released before deleting?
  DeleteCriticalSection(cast(LPCRITICAL_SECTION)mutex->handle);
  os_free(mutex->handle);
  mutex->handle = 0;
}

#endif // OS_WINDOWS

#if OS_MACOS

#undef internal
#include <mach/mach.h>
#include <mach/semaphore.h>
#include <mach/task.h>
#define SYNC_POLICY_PREPOST 0x4
#define internal static

// Make sure this fits into the handle pointer!
STATIC_ASSERT(sizeof(semaphore_t) <= sizeof(void *));

Semaphore semaphore_create(u32 max_count) {
  Semaphore result = {};

  mach_port_t self = mach_task_self();

  semaphore_t *handle = cast(semaphore_t *)os_alloc(sizeof(semaphore_t)); // @Memory @Cleanup
  result.handle = handle;

  kern_return_t ret = semaphore_create(self, handle, SYNC_POLICY_PREPOST, 1);
  assert(ret == KERN_SUCCESS);

  return result;
}

void semaphore_signal(Semaphore *sem) {
  auto handle = cast(semaphore_t *)sem->handle;
  kern_return_t ret = semaphore_signal(*handle);
  assert(ret == KERN_SUCCESS);
}

void semaphore_wait_for(Semaphore *sem, bool infinite) {
  kern_return_t ret;
  auto handle = cast(semaphore_t *)sem->handle;

  if (infinite) {
    ret = semaphore_wait(*handle);
  } else {
    // @Incomplete
    assert(!"Invalid code path");
  }

  assert(ret == KERN_SUCCESS);
}

void semaphore_destroy(Semaphore *sem) {
  mach_port_t self = mach_task_self();
  auto handle = cast(semaphore_t *)sem->handle;
  semaphore_destroy(self, *handle);
  os_free(handle); // @Memory @Cleanup
  handle = 0;
}

Mutex mutex_create(u32 spin_count) {
  Mutex result = {};

  result.handle = os_alloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(cast(pthread_mutex_t *)result.handle, NULL);

  // @Incomplete: should spin_count do anything?

  return result;
}

void mutex_aquire_lock(Mutex *mutex) {
  pthread_mutex_lock(cast(pthread_mutex_t *)mutex->handle);
}

bool mutex_try_aquire_lock(Mutex *mutex) {
  return pthread_mutex_trylock(cast(pthread_mutex_t *)mutex->handle) != 0;
}

void mutex_release_lock(Mutex *mutex) {
  pthread_mutex_unlock(cast(pthread_mutex_t *)mutex->handle);
}

void mutex_destroy(Mutex *mutex) {
  // @Robustness: track if it's been released before deleting?
  pthread_mutex_destroy(cast(pthread_mutex_t *)mutex->handle);
  os_free(mutex->handle);
  mutex->handle = 0;
}

#endif // OS_MACOS


nja_internal bool do_next_work_queue_entry(Work_Queue *queue) {
  bool we_should_sleep = false;

  u32 original_next_entry_to_read = queue->next_entry_to_read;
  u32 new_next_entry_to_read = (original_next_entry_to_read + 1) % count_of(queue->entries);

  if (original_next_entry_to_read != queue->next_entry_to_write) {
    u32 index = atomic_compare_exchange_u32(&queue->next_entry_to_read, new_next_entry_to_read, original_next_entry_to_read);

    if (index == original_next_entry_to_read) {
      Work_Entry entry = queue->entries[index];
      assert(entry.callback);
      entry.callback(entry.data);
      atomic_add_u64((u64 volatile *)&queue->completion_count, 1);
    }
  } else {
    we_should_sleep = true;
  }

  return we_should_sleep;
}

nja_internal u32 worker_thread_proc(void *data) {
  Worker_Params *params = (Worker_Params *)data;
  Work_Queue *queue = params->queue;

  for (;;) {
    bool we_should_sleep = do_next_work_queue_entry(queue);

    if (we_should_sleep) {
      semaphore_wait_for(&queue->semaphore, true);
    }
  }
}

void work_queue_create(Work_Queue *queue, u32 thread_count, Worker_Params *thread_params) {
  queue->completion_goal = 0;
  queue->completion_count = 0;

  queue->next_entry_to_write = 0;
  queue->next_entry_to_read = 0;

  queue->semaphore = semaphore_create(thread_count);

  for (u32 i = 0; i < thread_count; i++) {
    Worker_Params *params = thread_params + i;
    params->queue = queue;

    Thread thread = os_create_thread(megabytes(1), worker_thread_proc, params);
    os_detatch_thread(thread);
  }
}

void work_queue_add_entry(Work_Queue *queue, Worker_Proc *callback, void *data) {
  // TODO(casey): Switch to InterlockedCompareExchange eventually
  // so that any thread can add?
  u32 new_next_entry_to_write = (queue->next_entry_to_write + 1) % count_of(queue->entries);
  assert(new_next_entry_to_write != queue->next_entry_to_read);

  Work_Entry *entry = queue->entries + queue->next_entry_to_write;
  entry->callback = callback;
  entry->data = data;
  queue->completion_goal ++;

  write_barrier();

  queue->next_entry_to_write = new_next_entry_to_write;

  semaphore_signal(&queue->semaphore);
}

void work_queue_complete_all_work(Work_Queue *queue) {
  while (queue->completion_goal != queue->completion_count) {
    do_next_work_queue_entry(queue);
  }

  queue->completion_goal = 0;
  queue->completion_count = 0;
}


#endif // NJA_H
