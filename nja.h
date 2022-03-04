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
#ifndef DEBUG_TRAP

  #if defined(_MSC_VER)
    #if _MSC_VER < 1300
    #define DEBUG_TRAP() __asm int 3 /* Trap to debugger! */
    #else
    #define DEBUG_TRAP() __debugbreak()
    #endif
  #else
    #define DEBUG_TRAP() __builtin_trap()
  #endif

#endif
#else
  #define DEBUG_TRAP()
#endif

#ifndef assert
#ifdef DEBUG
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

  #define assert(expr) do { if (!(expr)) { nja_assert_handler("Assertion Failed", #expr, __FILE__, __LINE__, NULL); DEBUG_TRAP(); } } while (0)
#else
  #define assert(...)
#endif
#endif

#if DEBUG
  #define dump(var) print("%s: %s\n", #var, string_to_cstr(to_string(var)))
#else
  #define dump(...)
#endif

#ifndef STATIC_ASSERT
#ifdef __cplusplus
  #define STATIC_ASSERT(expr) static_assert(expr, "")
#else
  #define STATIC_ASSERT(expr) _Static_assert(expr, "")
#endif
#endif

#if defined(__cplusplus)
  #define EXTERN extern "C"
#else
  #define EXTERN extern
#endif

#if defined(_WIN32)
  #define DLL_EXPORT EXTERN __declspec(dllexport)
  #define DLL_IMPORT EXTERN __declspec(dllimport)
#else
  #define DLL_EXPORT EXTERN __attribute__((visibility("default")))
  #define DLL_IMPORT EXTERN
#endif

#define cast(type) (type)

#define count_of(array) (sizeof(array) / sizeof((array)[0]))

#define offset_of(Type, member) ((uint64_t) & (((Type *)0)->member))

#define size_of(type) ((isize)sizeof(type))

#define SWAP(Type, a, b) do { Type tmp = (a); (a) = (b); (b) = tmp; } while (0)

#if defined(__cplusplus) && !defined(defer)
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

#define set_flag(flags, flag, enable) do { if (enable) { flags |= (flag); } else { flags &= ~(flag); } } while(0)

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
// Memory
//

#define kilobytes(value) (value * 1024LL)
#define megabytes(value) (value * 1024LL * 1024LL)
#define gigabytes(value) (value * 1024LL * 1024LL * 1024LL)
#define terabytes(value) (value * 1024LL * 1024LL * 1024LL * 1024LL)

void *memory_copy(void *from, void *to, u64 size) {
  u8 *src = cast(u8 *)from;
  u8 *dest = cast(u8 *)to;

  while (size--) { *dest++ = *src++; }

  return to;
}

bool memory_equals(void *a_in, void *b_in, u64 size) {
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
  u8 *at = cast(u8 *)ptr;
  while (size--) { *at++ = value; }
}

void memory_zero(void *ptr, u64 size) {
  u8 *at = cast(u8 *)ptr;
  while (size--) { *at++ = 0; }
}

//
// Arenas
//

bool nja_is_power_of_two(isize x) {
  return x > 0 && !(x & (x - 1));
}

u32 nja_next_power_of_two(u32 x) {
  assert(x != 0);

  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;

  return ++x;
}

u32 nja_previous_power_of_two(u32 x) {
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x - (x >> 1);
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

struct Arena {
  u8 *data;
  u64 offset;
  u64 size;
};

struct Arena_Mark {
  u64 offset;
};

Arena make_arena(u8 *data, u64 size) {
  Arena result = {};
  result.data = data;
  result.size = size;
  return result;
}

void arena_init(Arena *arena, u8 *data, u64 size) {
  *arena = {};
  arena->data = data;
  arena->size = size;
}

void *arena_alloc_aligned(Arena *arena, u64 size, u64 alignment) {
  void *result = NULL;

  u64 align_offset = nja_align_offset(arena->data + arena->offset, alignment);
  size += align_offset;

  if (arena->offset + size < arena->size) {
    result = arena->data + arena->offset + align_offset;
    arena->offset += size;

    // NOTE(nick): make sure our data is aligned properly
    assert((u64)result % alignment == 0);
  }

  return result;
}

void *arena_alloc(Arena *arena, u64 size) {
  return arena_alloc_aligned(arena, size, 8);
}

void arena_pop(Arena *arena, u64 size) {
  if (size > arena->offset) {
    arena->offset = 0;
  } else {
    arena->offset -= size;
  }
}

void arena_reset(Arena *arena) {
  arena->offset = 0;
  memory_zero(arena->data, arena->size);
}

void *arena_push(Arena *arena, u64 size) {
  void *result = 0;

  if (arena->offset + size < arena->size) {
    result = arena->data + arena->offset;
    arena->offset += size;
  }

  return result;
}

#define push_struct(arena, Struct)  \
  (Struct *)arena_push(arena, sizeof(Struct))

#define push_array(arena, Struct, count)  \
  (Struct *)arena_push(arena, count * sizeof(Struct))

Arena_Mark arena_get_position(Arena *arena) {
  Arena_Mark result = {};
  result.offset = arena->offset;
  return result;
}

void arena_set_position(Arena *arena, Arena_Mark mark) {
  arena->offset = mark.offset;
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
  u32 thread_id;

#if OS_WINDOWS
  thread_id = GetCurrentThreadId();
#elif OS_MACOS
  thread_id = pthread_mach_thread_np(pthread_self());
#elif OS_LINUX
  thread_id = gettid();
#else
  #error Unsupported architecture for thread_get_id()
#endif

  return thread_id;
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

void init_thread_context(u64 temporary_storage_size) {
  Thread_Context *ctx = (Thread_Context *)os_alloc(sizeof(Thread_Context));

  u8 *data = (u8 *)os_alloc(temporary_storage_size);
  arena_init(&ctx->temporary_storage, data, temporary_storage_size);

  os_thread_set_context(ctx);
}

void free_thread_context() {
  Thread_Context *ctx = (Thread_Context *)os_thread_get_context();

  os_free(&ctx->temporary_storage.data);
  os_free(ctx);
}

Arena *thread_get_temporary_arena() {
  Thread_Context *ctx = (Thread_Context *)os_thread_get_context();
  assert(ctx);
  return &ctx->temporary_storage;
}

// @Robustness: support overflowing
// @Robustness: support arbitrary temp storage size
void *talloc(u64 size) {
  Arena *arena = thread_get_temporary_arena();
  return arena_alloc(arena, size);
}

void tpop(u64 size) {
  Arena *arena = thread_get_temporary_arena();
  arena_pop(arena, size);
}

void reset_temporary_storage() {
  Arena *arena = thread_get_temporary_arena();
  arena_reset(arena);
}

//
// String
//

struct String {
  u64 count;
  u8 *data;

  u8 &operator[](u64 i) {
    assert(i >= 0 && i < count);
    return data[i];
  }
};

#define S(x) String{sizeof(x) - 1, cast(u8 *)x}

#define LIT(str) (int)str.count, (char *)str.data

String make_string(void *data, u64 count) {
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
  Arena *arena = thread_get_temporary_arena();
  return string_to_cstr(arena, str);
}

i64 cstr_length(char *str) {
  char *at = str;

  // @Speed: this can be made wide
  if (at) {
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
    assert(length <= I64_MAX);
    result = {(u64)length, (u8 *)data};
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
  return make_string(str.data + start_index, cast(u64)(end_index - start_index));
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

String string_join(Arena *arena, String a, String b) {
  u8 *data = push_array(arena, u8, a.count + b.count);

  memory_copy(a.data, data + 0,       a.count);
  memory_copy(b.data, data + a.count, b.count);

  return make_string(data, a.count + b.count);
}

String string_join(String a, String b) {
  Arena *arena = thread_get_temporary_arena();
  return string_join(arena, a, b);
}

String string_join(Arena *arena, String a, String b, String c) {
  u8 *data = push_array(arena, u8, a.count + b.count + c.count);

  memory_copy(a.data, data + 0,                 a.count);
  memory_copy(b.data, data + a.count,           b.count);
  memory_copy(c.data, data + a.count + b.count, c.count);

  return make_string(data, a.count + b.count + c.count);
}

String string_join(String a, String b, String c) {
  Arena *arena = thread_get_temporary_arena();
  return string_join(arena, a, b, c);
}

void string_advance(String *str, u64 amount) {
  assert(str->count >= amount);

  str->count -= amount;
  str->data  += amount;
}

bool char_is_whitespace(char ch) {
  return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
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

#include <stdarg.h>

String string_printv(Arena *arena, const char *format, va_list args) {
  // in case we need to try a second time
  va_list args2;
  va_copy(args2, args);

  u64 buffer_size = 1024;
  u8 *buffer = push_array(arena, u8, buffer_size);

  String result = {};
  u64 actual_size = print_to_buffer((char *)buffer, buffer_size, format, args);

  if (actual_size > 0) {
    if (actual_size < buffer_size) {
      arena_pop(arena, buffer_size - actual_size - 1);
      result = make_string(buffer, actual_size);
    } else {
      arena_pop(arena, buffer_size);
      u8 *fixed_buffer = push_array(arena, u8, actual_size + 1);
      u64 final_size = print_to_buffer((char *)fixed_buffer, actual_size + 1, format, args2);
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
  return sprint("%.*s/%.*s", path1.count, path1.data, path2.count, path2.data);
}

String path_join(String path1, String path2, String path3) {
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

String to_string(bool x) { if (x) return S("true"); return S("false"); }
String to_string(char x) { return sprint("%c", x); }
String to_string(char *x) { return string_from_cstr(x); }
String to_string(i8 x)  { return sprint("%d", x); }
String to_string(u8 x)  { return sprint("%d", x); }
String to_string(i16 x) { return sprint("%d", x); }
String to_string(u16 x) { return sprint("%d", x); }
String to_string(i32 x) { return sprint("%d", x); }
String to_string(u32 x) { return sprint("%d", x); }
String to_string(i64 x) { return sprint("%d", x); }
String to_string(u64 x) { return sprint("%llu", x); }
String to_string(f32 x)    { return sprint("%.2f", x); }
String to_string(f64 x)    { return sprint("%.4f", x); }
String to_string(String x) { return x; }

//
// String list
//

struct String_Node {
  String str;
  String_Node *next;
};

struct String_List {
  String_Node *first;
  String_Node *last;

  u64 count;
  u64 size_in_bytes;
};

String_List make_string_list() {
  String_List result = {};
  return result;
}

void string_list_push_explicit(String_List *list, String str, String_Node *node) {
  node->str = str;
  node->next = NULL;

  if (!list->first) list->first = node;
  if (list->last) list->last->next = node;
  list->last = node;

  list->count += 1;
  list->size_in_bytes += str.count;
}

void string_list_push(Arena *arena, String_List *list, String str) {
  String_Node *node = push_struct(arena, String_Node);
  string_list_push_explicit(list, str, node);
}

String_List string_list_split(Arena *arena, String str, String split) {
  String_List result = {};

  u8 *at = str.data;
  u8 *word_first = at;
  u8 *str_end = str.data + str.count;

  for (; at < str_end; at += 1) {
    if (*at == split.data[0])
    {
      if (string_starts_with(string_range(at, str_end), split)) {
        String slice = string_range(word_first, at);
        string_list_push(arena, &result, slice);

        at += split.count - 1;
        word_first = at + 1;
        continue;
      }
    }
  }

  String slice = string_range(word_first, str_end);
  string_list_push(arena, &result, slice);

  return result;
}

String string_list_join(Arena *arena, String_List *list, String join) {
  u64 size = join.count * (list->count - 1) + list->size_in_bytes;
  u8 *data = push_array(arena, u8, size);

  bool is_mid = false;
  u8 *at = data;
  for (String_Node *it = list->first; it != NULL; it = it->next) {
    if (is_mid) {
      memory_copy(join.data, at, join.count);
      at += join.count;
    }

    memory_copy(it->str.data, at, it->str.count);
    at += it->str.count;
    is_mid = it->next != NULL;
  }

  return make_string(data, size);
}

void string_list_print(Arena *arena, String_List *list, const char *format, ...) {
  va_list args;
  va_start(args, format);
  String result = string_printv(arena, format, args);
  va_end(args);

  string_list_push(arena, list, result);
}

//
// String conversions
//

struct String16 {
  u64 count;
  u16 *data;
};

struct String32 {
  u64 count;
  u32 *data;
};

struct String_Decode {
  u32 codepoint;
  u8 size; // 1 - 4
};

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

  if (x < 0xD800 || x > 0xDFFF) {
    result.codepoint = x;
    result.size = 1;
  } else if (capacity >= 2) {
    u16 y = str[1];

    // @Robustness: range check?
    result.codepoint = ((x - 0xD800) << 10 | (y - 0xDC00)) + 0x10000;
    result.size = 2;
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
    dest[1] = (x & 0x3ff) + 0xDC00;
    size = 2;
  }

  return size;
}

String32 string32_from_string(Arena *arena, String str) {
  u32 *memory = push_array(arena, u32, str.count);;

  u8 *p0 = str.data;
  u8 *p1 = str.data + str.count;
  u32 *at = memory;
  u64 remaining = str.count;

  while (p0 < p1) {
    auto decode = string_decode_utf8(p0, remaining);

    *at = decode.codepoint;
    p0 += decode.size;
    at += 1;
    remaining -= decode.size;
  }

  u64 alloc_count = str.count;
  u64 string_count = cast(u64)(at - memory);
  u64 unused_count = string_count - alloc_count;

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

  u64 alloc_count = str.count * 4;
  u64 string_count = cast(u64)(at - memory);
  u64 unused_count = alloc_count - string_count;

  arena_pop(arena, unused_count);

  String result = {string_count, memory};
  return result;
}

String16 string16_from_string(Arena *arena, String str) {
  u16 *memory = push_array(arena, u16, str.count + 1);

  u8 *p0 = str.data;
  u8 *p1 = str.data + str.count;
  u16 *at = memory;
  u64 remaining = str.count;

  while (p0 < p1) {
    auto decode = string_decode_utf8(p0, remaining);
    u32 encode_size = string_encode_utf16(at, decode.codepoint);

    at += encode_size;
    p0 += decode.size;
    remaining -= decode.size;
  }

  *at = 0;

  u64 alloc_count = str.count + 1;
  u64 string_count = cast(u64)(at - memory);
  u64 unused_count = alloc_count - string_count - 1;

  arena_pop(arena, unused_count * sizeof(u16));

  String16 result = {string_count, memory};
  return result;
}

String string_from_string16(Arena *arena, String16 str) {
  String result = {};
  result.data = push_array(arena, u8, str.count * 2);

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

  u64 unused_size = (result.count - str.count);
  arena_pop(arena, unused_size);

  return result;
}

//
// Functions
//

#define PI       3.14159265359f
#define TAU      6.28318530717958647692f
#define EPSILON  0.00001f
#define EPSILON2 (EPSILON * EPSILON)

#define SQRT_2 0.70710678118

#define MIN(a, b) ((a < b) ? (a) : (b))
#define MAX(a, b) ((a > b) ? (a) : (b))
#define CLAMP(value, lower, upper) (MAX(MIN(value, upper), lower))

#define SIGN(x) ((x > 0) - (x < 0))
#define ABS(x) ((x < 0) ? -(x) : (x))

inline i32 Min(i32 a, i32 b) { return MIN(a, b); }
inline u32 Min(u32 a, u32 b) { return MIN(a, b); }
inline u64 Min(u64 a, u64 b) { return MIN(a, b); }
inline f32 Min(f32 a, f32 b) { return MIN(a, b); }
inline f64 Min(f64 a, f64 b) { return MIN(a, b); }

inline i32 Max(i32 a, i32 b) { return MAX(a, b); }
inline u32 Max(u32 a, u32 b) { return MAX(a, b); }
inline u64 Max(u64 a, u64 b) { return MAX(a, b); }
inline f32 Max(f32 a, f32 b) { return MAX(a, b); }
inline f64 Max(f64 a, f64 b) { return MAX(a, b); }

inline i32 Clamp(i32 value, i32 lower, i32 upper) { return CLAMP(value, lower, upper); }
inline u32 Clamp(u32 value, u32 lower, u32 upper) { return CLAMP(value, lower, upper); }
inline u64 Clamp(u64 value, u64 lower, u64 upper) { return CLAMP(value, lower, upper); } 
inline f32 Clamp(f32 value, f32 lower, f32 upper) { return CLAMP(value, lower, upper); }
inline f64 Clamp(f64 value, f64 lower, f64 upper) { return CLAMP(value, lower, upper); }

inline i32 Sign(i32 a) { return SIGN(a); }
inline u32 Sign(u32 a) { return SIGN(a); }
inline u64 Sign(u64 a) { return SIGN(a); }
inline f32 Sign(f32 a) { return SIGN(a); }
inline f64 Sign(f64 a) { return SIGN(a); }

inline i32 Abs(i32 a) { return ABS(a); }
inline u32 Abs(u32 a) { return ABS(a); }
inline u64 Abs(u64 a) { return ABS(a); }
inline f32 Abs(f32 a) { return ABS(a); }
inline f64 Abs(f64 a) { return ABS(a); }

#define ClampTop Max
#define ClampBot Min

//
// Math
//

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

struct Scan_Result {
  File_Info *files;
  u32 count;
};

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

struct OS_Memory {
  void *(*reserve)(u64 size);
  void *(commit)(void *ptr, u64 size);
  void *(decommit)(void *ptr, u64 size);
  void *(release)(void *ptr);
};

bool os_init();

void *os_alloc(u64 size);
void os_free(void *ptr);
void *os_memory_reserve(u64 size);
void os_memory_commit(void *ptr, u64 size);
void os_memory_decommit(void *ptr, u64 size);
void os_memory_release(void *ptr);

void os_thread_set_context(void *ptr);
void *os_thread_get_context();

f64 os_time_in_miliseconds();
f64 os_time_utc_now();
void os_sleep(f64 miliseconds);

void os_set_high_process_priority(bool enable);

String os_read_entire_file(Arena *arena, String path);
String os_read_entire_file(String path);
bool os_write_entire_file(String path, String contents);
bool os_atomic_replace_file(String path, u64 size, void *data);
bool os_delete_file(String path);
bool os_file_exists(String path);
bool os_make_directory(String path);
bool os_delete_directory(String path);
bool os_delete_entire_directory(String path);
Scan_Result os_scan_directory(Arena *arena, String path);

File_Lister os_file_list_begin(Arena *arena, String path);
bool os_file_list_next(File_Lister *iter, File_Info *info);
void os_file_list_end(File_Lister *iter);
File_Info os_get_file_info(Arena *arena, String path);

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

static LARGE_INTEGER win32_perf_frequency;
static LARGE_INTEGER win32_perf_counter;

static DWORD win32_thread_context_index = 0;

static bool did_init_os = false;

bool os_init() {
  if (did_init_os) return false;

  AttachConsole(ATTACH_PARENT_PROCESS);

  QueryPerformanceFrequency(&win32_perf_frequency);
  QueryPerformanceCounter(&win32_perf_counter);

  win32_thread_context_index = TlsAlloc();

  init_thread_context(megabytes(32));

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
  assert(win32_perf_frequency.QuadPart);

  LARGE_INTEGER current_time;
  QueryPerformanceCounter(&current_time);

  LARGE_INTEGER elapsed;
  elapsed.QuadPart = current_time.QuadPart - win32_perf_counter.QuadPart;
  return (f64)(elapsed.QuadPart * 1000) / win32_perf_frequency.QuadPart;
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

void os_memory_commit(void *ptr, u64 size) {
  VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
}

void os_memory_decommit(void *ptr, u64 size) {
  VirtualFree(ptr, size, MEM_DECOMMIT);
}

void os_memory_release(void *ptr) {
  VirtualFree(ptr, 0, MEM_RELEASE);
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

String os_read_entire_file(Arena *arena, String path) {
  String result = {};

  // @Cleanup: do we always want to null-terminate String16 s?
  String16 str = string16_from_string(arena, path);
  HANDLE handle = CreateFileW(cast(WCHAR *)str.data, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

  if (handle == INVALID_HANDLE_VALUE) {
    print("[file] Error reading entire file: (error code: %d) for %.*s\n", GetLastError(), LIT(path));
    return result;
  }

  DWORD hi_size = 0;
  DWORD lo_size = GetFileSize(handle, &hi_size);

  u64 size = ((cast(u64)hi_size) << 32) | cast(u64)lo_size;

  result.data = cast(u8 *)arena_alloc(arena, size + 1); // space for null character.
  result.data[size] = 0;
  result.count = size;

  DWORD bytes_read = 0;
  BOOL success = ReadFile(handle, (void *)result.data, size, &bytes_read, 0);

  if (!success) {
    print("[file] Failed to read entire file: %.*s\n", LIT(path));
  } else if (bytes_read != size) {
    // @Robustness: should this keep reading until it reads everything?
    print("[file] Warning byte read mismatch, expected %llu but got %lu for file: %.*s\n", size, bytes_read, LIT(path));
  }

  CloseHandle(handle);

  return result;
}

bool os_write_entire_file(String path, String contents) {
  Arena *arena = thread_get_temporary_arena();

  // :ScratchMemory
  String16 str = string16_from_string(arena, path);
  HANDLE handle = CreateFileW(cast(WCHAR *)str.data, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

  if (handle == INVALID_HANDLE_VALUE) {
    print("[file] Error writing entire file: (error code: %d) for %.*s\n", GetLastError(), LIT(path));
    return false;
  }

  // @Robustness: handle > 32 byte content size
  assert(contents.count < U32_MAX);

  DWORD bytes_written;
  BOOL success = WriteFile(handle, contents.data, contents.count, &bytes_written, 0);

  if (!success) {
    print("[file] Failed to write entire file: %.*s\n", LIT(path));
    return false;
  } else if (bytes_written != contents.count) {
    // @Robustness: keep writing until everything is written?
    print("[file] Warning byte read mismatch, expected %llu but got %lu for file: %.*s\n", contents.count, bytes_written, LIT(path));
    return false;
  }

  CloseHandle(handle);

  return true;
}

bool os_atomic_replace_file(String path, u64 size, void *data) {
  bool success = false;

  char *temp_filename = cprint("%.*s.tmp", LIT(path));
  HANDLE file = CreateFileA(temp_filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

  if (file == INVALID_HANDLE_VALUE) {
    log("file", "Failed to open temporary file: %s\n", temp_filename);
  }

  if (file != INVALID_HANDLE_VALUE) {
    bool write_success = false;
    DWORD bytes_written = 0;
    if (WriteFile(file, data, safe_truncate_to_u32(size), &bytes_written, 0)) {
      write_success = bytes_written == size;
    }

    CloseHandle(file);

    if (write_success) {
      success = MoveFileExA(temp_filename, path.c_str(), MOVEFILE_REPLACE_EXISTING);
    } else {
      log("file", "Failed to atomically replace file: %s -> %s\n", temp_filename, path.c_str());
    }
  }

  if (!success) {
    DeleteFileA(temp_filename);
  }

  return success;
}

bool os_delete_file(String path) {
  Arena *arena = thread_get_temporary_arena();

  auto restore_point = arena_get_position(arena);
  String16 str = string16_from_string(arena, path);
  BOOL success = DeleteFileW(cast(WCHAR *)str.data);
  arena_set_position(arena, restore_point);

  return success;
}

bool os_make_directory(String path) {
  Arena *arena = thread_get_temporary_arena();

  auto restore_point = arena_get_position(arena);
  String16 str = string16_from_string(arena, path);
  BOOL success = CreateDirectoryW(cast(WCHAR *)str.data, NULL);
  arena_set_position(arena, restore_point);

  return success;
}

bool os_delete_directory(String path) {
  Arena *arena = thread_get_temporary_arena();

  auto restore_point = arena_get_position(arena);
  String16 str = string16_from_string(arena, path);
  BOOL success = RemoveDirectoryW(cast(WCHAR *)str.data);
  arena_set_position(arena, restore_point);

  return success;
}

bool os_delete_entire_directory(String path) {
  Arena *arena = thread_get_temporary_arena();

  auto restore_point = arena_get_position(arena);
  char *find_path = cstr_print(arena, "%.*s\\*.*", LIT(path));

  bool success = true;

  WIN32_FIND_DATA data;
  HANDLE handle = FindFirstFile(find_path, &data);

  if (handle != INVALID_HANDLE_VALUE) {
    do {
      String file_name = string_from_cstr(data.cFileName);

      if (string_equals(file_name, S(".")) || string_equals(file_name, S(".."))) continue;

      String file_path = string_join(path, S("/"), file_name);

      if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        success |= os_delete_entire_directory(file_path);
      } else {
        success |= os_delete_file(file_path);
      }

    } while(FindNextFile(handle, &data));
  }

  FindClose(handle);

  success |= os_delete_directory(path);

  arena_set_position(arena, restore_point);

  return success;
}

Scan_Result os_scan_directory(Arena *arena, String path) {
  Scan_Result result = {};

  // :ScratchMemory
  char *find_path = cstr_print(arena, "%.*s\\*.*", LIT(path));

  // @Speed: we could avoid doing this twice by assuming a small-ish number to start and only re-scanning if we're wrong
  // but then that wastes a bunch of memory
  // and is this block actually slow?
  u32 count = 0; 
  {
    WIN32_FIND_DATA data;
    HANDLE handle = FindFirstFile(find_path, &data);

    if (handle != INVALID_HANDLE_VALUE) {
      do {
        count += 1;
      } while (FindNextFile(handle, &data));
    }

    FindClose(handle);
  }

  result.files = push_array(arena, File_Info, count);

  WIN32_FIND_DATA data;
  HANDLE handle = FindFirstFile(find_path, &data);

  if (handle != INVALID_HANDLE_VALUE) {
    do {
      String name = string_from_cstr(data.cFileName);
      // Ignore . and .. "directories"
      if (string_equals(name, S(".")) || string_equals(name, S(".."))) continue;

      File_Info *info = &result.files[result.count];
      result.count += 1;

      *info = {};
      info->name         = string_copy(arena, name);
      info->date         = ((u64)data.ftLastWriteTime.dwHighDateTime << (u64)32) | (u64)data.ftLastWriteTime.dwLowDateTime;
      info->size         = ((u64)data.nFileSizeHigh << (u64)32) | (u64)data.nFileSizeLow;
      info->is_directory = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

    } while (FindNextFile(handle, &data));
  }

  FindClose(handle);

  return result;
}

struct File_Lister {
  char *find_path;

  HANDLE handle;
  WIN32_FIND_DATA data;
};

File_Lister os_file_list_begin(Arena *arena, String path) {
  File_Lister result = {};

  char *find_path = cstr_print(arena, "%.*s\\*.*", LIT(path));
  result.find_path = find_path;
  result.handle = 0;

  return result;
}

bool os_file_list_next(File_Lister *iter, File_Info *info) {
  bool should_continue = true;

  if (!iter->handle) {
    iter->handle = FindFirstFile(iter->find_path, &iter->data);
  } else {
    should_continue = FindNextFile(iter->handle, &iter->data);
  }

  if (iter->handle != INVALID_HANDLE_VALUE) {
    String name = string_from_cstr(iter->data.cFileName);

    // Ignore . and .. "directories"
    while (should_continue && (string_equals(name, S(".")) || string_equals(name, S("..")))) {
      should_continue = FindNextFile(iter->handle, &iter->data);
      name = string_from_cstr(iter->data.cFileName);
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
    FindClose(iter->handle);
    iter->handle = 0;
  }
}

File_Info os_get_file_info(Arena *arena, String path) {
  File_Info result = {};

  // :ScratchMemory
  char *cpath = string_to_cstr(arena, path);

  WIN32_FILE_ATTRIBUTE_DATA data;
  if (GetFileAttributesExA(cpath, GetFileExInfoStandard, &data)) {
    result.name         = path_filename(path);
    result.date         = ((u64)data.ftLastWriteTime.dwHighDateTime << (u64)32) | (u64)data.ftLastWriteTime.dwLowDateTime;
    result.size         = ((u64)data.nFileSizeHigh << (u64)32) | (u64)data.nFileSizeLow;
    result.is_directory = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
  }

  return result;
}

DWORD WINAPI win32_thread_proc(LPVOID lpParameter) {
  Thread_Params *params = (Thread_Params *)lpParameter;

  init_thread_context(megabytes(16));
  assert(params->proc);
  u32 result = params->proc(params->data);
  free_thread_context();

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
  WCHAR buffer[1024];

  DWORD length = GetModuleFileNameW(NULL, buffer, sizeof(buffer));
  if (length == 0) {
    return {};
  }

  String16 temp = {length, cast(u16 *)buffer};

  Arena *arena = thread_get_temporary_arena();
  String result = string_from_string16(arena, temp);
  win32_normalize_path(result);

  return result;
}

String os_get_current_directory() {
  WCHAR buffer[1024];

  DWORD length = GetCurrentDirectoryW(sizeof(buffer), buffer);
  if (length == 0) {
    return {};
  }

  String16 temp = {length, cast(u16 *)buffer};

  Arena *arena = thread_get_temporary_arena();
  String result = string_from_string16(arena, temp);
  win32_normalize_path(result);

  return result;
}

bool os_clipboard_set_text(String str) {
  int count;
  HANDLE handle;
  WCHAR* buffer;

  char *cstr = str.c_str();

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
  String result = utf8_from_utf16(buffer, buffer_size);

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

static f64 macos_perf_frequency = 0;
static f64 macos_perf_counter = 0;

bool os_init() {
  mach_timebase_info_data_t rate_nsec;
  mach_timebase_info(&rate_nsec);

  macos_perf_frequency = 1000000LL * rate_nsec.numer / rate_nsec.denom;
  macos_perf_counter = mach_absolute_time();

  return true;
}

void os_thread_set_context(void *ptr) {
  // @Incomplete
}

void *os_thread_get_context() {
  // @Incomplete
  return NULL;
}

f64 os_time_in_miliseconds() {
  f64 now = mach_absolute_time();
  return (now - macos_perf_counter) / macos_perf_frequency;
}

f64 os_time_utc_now() {
  struct timespec t;

  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  t.tv_sec = mts.tv_sec;
  t.tv_nsec = mts.tv_nsec;

  return cast(u64)t.tv_sec * 1000000ull + t.tv_nsec/1000 + 11644473600000000ull;
}

void os_sleep(f64 miliseconds) {
  u32 ms = (u32)miliseconds;
  struct timespec req = {(time_t)ms / 1000, (long)((ms % 1000) * 1000000)};
  struct timespec rem = {0, 0};
  nanosleep(&req, &rem);
}

String os_get_executable_path() {
  u32 length = 0;
  _NSGetExecutablePath(0, &length);

  char *buffer = (char *)talloc(sizeof(char) * length);

  if (_NSGetExecutablePath(buffer, &length) < 0) { return {}; }

  return make_string(buffer, length);
}

#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <objc/NSObjCRuntime.h>

struct NSString;
typedef NSString * NSPasteboardType;
EXTERN NSPasteboardType const NSPasteboardTypeString; // Available MacOS 10.6

#define objc_msgSend_id ((id (*)(id, SEL))objc_msgSend)

#define objc_method(ReturnType, ...) ((ReturnType (*)(__VA_ARGS__))objc_msgSend)

String os_clipboard_get_text() {
  // NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  id pasteboard = objc_msgSend_id((id)objc_getClass("NSPasteboard"), sel_registerName("generalPasteboard"));
  // NSString *string = pasteboard.string;
  id string = objc_method(id, id, SEL, id)(pasteboard, sel_registerName("stringForType:"), (id)NSPasteboardTypeString);
  // char *text = [string UTF8String];
  char *text = objc_method(char*, id, SEL)(string, sel_registerName("UTF8String"));
  return string_from_cstr(text);
}

bool os_clipboard_set_text(String text) {
  char *str = string_to_cstr(text);

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

void *os_alloc(u64 size) {
  return calloc(size, 1);
}

void os_free(void *memory) {
  if (memory) {
    free(memory);
    memory = 0;
  }
}

#include <stdio.h> // fopen, fseek, ftell, fread, fclose
#include <sys/stat.h> // stat

String os_read_entire_file(Arena *arena, String path) {
  String result = {};
  FILE *f = fopen(string_to_cstr(arena, path), "rb");

  if (f == NULL) {
    print("[file] Failed to read entire file: %.*s\n", LIT(path));
    return result;
  }

  fseek(f, 0, SEEK_END);
  u64 size = ftell(f);
  fseek(f, 0, SEEK_SET);

  result.data = cast(u8 *)arena_alloc(arena, size + 1); // space for null character.
  result.data[size] = 0;
  result.count = size;
  size_t bytes_read = fread(result.data, sizeof(char), size, f);

  if (bytes_read != size) {
    print("[file] Warning byte read mismatch, expected %llu but got %zu for file: %.*s\n", size, bytes_read, LIT(path));
  }

  fclose(f);

  return result;
}

bool os_atomic_replace_file(String path, u64 size, void *data) {
  bool success = false;

  char *cpath = string_to_cstr(path);
  char *temp_filename = cprint("%s.tmp", cpath);
  FILE *file = fopen(cpath, "wb");

  if (file) {
    size_t bytes_written = fwrite(data, sizeof(char), size, file);
    bool write_success = bytes_written == size;
    fclose(file);

    if (write_success) {
      int result = rename(temp_filename, cpath);
      success = result == 0;
    }
  }

  if (!success) {
    unlink(temp_filename);
  }

  return success;
}

bool os_file_exists(String path) {
  struct stat buffer;
  return stat(string_to_cstr(path), &buffer) == 0;
}

#include <pthread.h>
#include <sys/resource.h> // setpriority

void *unix_thread_proc(void *data) {
  Thread_Params *params = (Thread_Params *)data;
  assert(params->proc);

  init_thread_context(megabytes(32));
  u32 result = params->proc(params->data);
  free_thread_context();
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

String os_read_entire_file(String path) {
  Arena *arena = thread_get_temporary_arena();
  return os_read_entire_file(arena, path);
}

String os_get_executable_directory() {
  String result = os_get_executable_path();
  return path_dirname(result);
}

//
// DLLs
//

struct OS_Library {
  void *handle;
};

typedef void (*OS_Library_Proc)(void);

OS_Library os_lib_load(char *path);
void os_lib_unload(OS_Library lib);
OS_Library_Proc os_lib_get_proc(OS_Library lib, char *proc_name);

#if OS_WINDOWS

OS_Library os_lib_load(char *path) {
  OS_Library result = {};
  result.handle = LoadLibraryA(path);
  return result;
}

inline void os_lib_unload(OS_Library lib) {
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

OS_Library os_lib_load(char *path) {
  OS_Library result = {};
  // TODO(bill): Should this be RTLD_LOCAL?
  result.handle = dlopen(path, RTLD_LAZY|RTLD_GLOBAL);
  return result;
}

inline void os_lib_unload(OS_Library lib) {
  if (lib.handle) {
    dlclose(lib.handle);
    lib.handle = 0;
  }
}

inline OS_Library_Proc os_lib_get_proc(OS_Library lib, char *proc_name) {
  return (OS_Library_Proc)dlsym(lib.handle, proc_name);
}

#endif

//
// Allocator
//

enum Allocator_Mode {
  ALLOCATOR_MODE_ALLOC    = 0,
  ALLOCATOR_MODE_RESIZE   = 1,
  ALLOCATOR_MODE_FREE     = 2,
  ALLOCATOR_MODE_FREE_ALL = 3,
};

#define ALLOCATOR_PROC(name) void *name(Allocator_Mode mode, u64 requested_size, u64 old_size, void *old_memory_pointer, void *allocator_data);
typedef ALLOCATOR_PROC(Allocator_Proc);

struct Allocator {
  Allocator_Proc *proc;
  void *data;
};

void *Alloc(u64 size, Allocator allocator) {
  assert(allocator.proc);
  void *result = allocator.proc(ALLOCATOR_MODE_ALLOC, size, 0, NULL, allocator.data);
  return result;
}

void Free(void *data, Allocator allocator) {
  assert(allocator.proc);
  allocator.proc(ALLOCATOR_MODE_FREE, 0, 0, data, allocator.data);
}

void *Realloc(void *data, u64 new_size, u64 old_size, Allocator allocator) {
  assert(allocator.proc);
  return allocator.proc(ALLOCATOR_MODE_RESIZE, new_size, old_size, data, allocator.data);
}

void *os_allocator_proc(Allocator_Mode mode, u64 requested_size, u64 old_size, void *old_memory_pointer, void *allocator_data) {
  switch (mode) {
    default:
    case ALLOCATOR_MODE_FREE: {
      os_free(old_memory_pointer);
      return NULL;
    }

    case ALLOCATOR_MODE_RESIZE:
    case ALLOCATOR_MODE_ALLOC: {
      u64 alignment = 8;
      u64 extra = alignment - (requested_size & (alignment - 1));
      extra &= (alignment - 1);

      u64 actual_size = requested_size + extra;

      void *result = os_alloc(actual_size);

      if (result && old_memory_pointer && mode == ALLOCATOR_MODE_RESIZE) {
        // @Incomplete: provide os-level realloc function
        memory_copy(old_memory_pointer, result, Min(requested_size, old_size));
        os_free(old_memory_pointer);
      }

      return result;
    }
  }
}

Allocator os_allocator() {
  return Allocator{os_allocator_proc, 0};
}

//
// Array
//

#define For(array) for (auto it : array)

#define Forv(it, array) for (auto it : array)

#define Forp(array)   \
  auto CONCAT(__array, __LINE__) = (array);   \
  for (auto it = CONCAT(__array, __LINE__).begin_ptr(); it < CONCAT(__array, __LINE__).end_ptr(); it++)

#define For_Index(array) for (i64 index = 0; index < cast(i64)(array).count; index++)

#define For_Index_Reverse(array)                                               \
  for (i64 index = cast(i64)(array).count - 1; index >= 0; index--)

template <typename T> struct Array;

template <typename T>
struct Array_Iterator {
  const Array<T> *array;
  u64 index;

  Array_Iterator(const Array<T> *_array, u64 _index) : array(_array), index(_index) {};

  bool operator != (const Array_Iterator<T> &other) const {
    return index != other.index;
  }

  T &operator *() const {
    return array->data[index];
  }

  const Array_Iterator &operator++ () {
    index++;
    return *this;
  }
};

template <typename T>
struct Array {
  Allocator allocator = os_allocator();
  u64 capacity = 0;
  u64 count = 0;
  T *data = NULL;

  T &operator[](u64 i) {
    assert(i >= 0 && i < capacity);
    return data[i];
  }

  Array_Iterator<T> begin() const { return Array_Iterator<T>(this, 0); }
  Array_Iterator<T> end() const   { return Array_Iterator<T>(this, count); }

  T *begin_ptr() { return data ? &data[0] : NULL; }
  T *end_ptr()   { return data ? &data[count] : NULL; }
};

template <typename T>
void array_reset(Array<T> &it) {
  it.count = 0;
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
void array_remove_unordered(Array<T> &it, u64 index) {
  assert(index >= 0 && index < it.count);
  memory_copy(&it.data[it.count - 1], &it.data[index], sizeof(T));
  it.count--;
}

template <typename T>
void array_remove_while_keeping_order(Array<T> &it, u64 index, u64 num_to_remove = 1) {
  assert(index >= 0 && index < it.count);
  assert(num_to_remove > 0 && index + num_to_remove <= it.count);

  // @Speed: this could be replaced with memory_move, the problem is overlapping memory
  for (u64 i = index + num_to_remove; i < it.count; i++) {
    memory_copy(&it.data[i], &it.data[i - num_to_remove], sizeof(T));
  }

  it.count -= num_to_remove;
}

template <typename T>
void array_resize(Array<T> &it, u64 next_capacity) {
  if (it.data && it.capacity == next_capacity) return;

  u64 prev_size = it.capacity * sizeof(T);
  u64 next_size = next_capacity * sizeof(T);

  it.data = (T *)Realloc(it.data, next_size, prev_size, it.allocator);
  assert(it.data);
  it.capacity = next_capacity;
}

template <typename T>
void array_init(Array<T> &it, u64 initial_capacity = 16) {
  it.count = 0;
  it.data = NULL;
  array_resize(it, initial_capacity);
}

template <typename T>
void array_free(Array<T> &it) {
  if (it.data) {
    os_free(it.data);
    it.data = NULL;
    it.capacity = 0;
    it.count = 0;
  }
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

//
// Hash Table
//

#define For_Table(table)                                    \
  for (auto it = (table).begin_ptr(); it < (table).end_ptr(); it++) \
    if (it->hash < TABLE_FIRST_VALID_HASH) continue; else

const int TABLE_NEVER_OCCUPIED_HASH = 0;
const int TABLE_REMOVED_HASH = 1;
const int TABLE_FIRST_VALID_HASH = 2;

template <typename K, typename V> struct Hash_Table;
template <typename K, typename V> struct Hash_Table_Entry;

template <typename K, typename V>
struct Hash_Table_Iterator {
  const Hash_Table<K, V> *table;
  u64 index;

  Hash_Table_Iterator(const Hash_Table<K, V> *_table, u64 _index) : table(_table), index(_index) {};

  bool operator != (const Hash_Table_Iterator<K, V> &other) const {
    return index != other.index;
  }

  Hash_Table_Entry<K, V> &operator *() const {
    return table->data[index];
  }

  const Hash_Table_Iterator &operator++ () {
    index ++;

    while ((index < table->capacity) && (table->data[index].hash < TABLE_FIRST_VALID_HASH)) {
      index ++;
    }

    return *this;
  }
};

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
  return murmur64(&key, sizeof(K));
}

u32 table_key_hash(char *key) {
  return murmur64(&key, cstr_length(key));
}

u32 table_key_hash(String key) {
  return murmur64(key.data, key.count);
}

template <typename K, typename V>
struct Hash_Table_Entry {
  u32 hash;
  K key;
  V value;
};

template <typename K, typename V>
struct Hash_Table {
  Allocator allocator = os_allocator();
  u32 capacity = 0;
  u32 count = 0;
  u32 slots_filled = 0;
  Hash_Table_Entry<K, V> *data = 0;

  Hash_Table_Entry<K, V> &operator[](u64 i) {
    assert(i >= 0 && i < capacity);
    return data[i];
  }

  Hash_Table_Iterator<K, V> begin() const {
    u64 index = 0;

    while ((index < this->capacity) && (this->data[index].hash < TABLE_FIRST_VALID_HASH)) {
      index ++;
    }

    return Hash_Table_Iterator<K, V>(this, index); 
  }
  Hash_Table_Iterator<K, V> end() const   { return Hash_Table_Iterator<K, V>(this, capacity); }

  Hash_Table_Entry<K, V> *begin_ptr() { return data ? &data[0] : NULL; }
  Hash_Table_Entry<K, V> *end_ptr()   { return data ? &data[capacity] : NULL; }
};

template <typename K, typename V>
void table_init(Hash_Table<K, V> &it, u32 table_size) {
  it.capacity = nja_next_power_of_two(table_size);
  it.count = 0;
  it.slots_filled = 0;

  assert((it.capacity & (it.capacity - 1)) == 0); // Must be a power of two!

  it.data = (Hash_Table_Entry<K, V> *)Alloc(it.capacity * sizeof(Hash_Table_Entry<K, V>), it.allocator);
  table_reset(it);
}

template <typename K, typename V>
void table_reset(Hash_Table<K, V> &it) {
  it.count = 0;
  it.slots_filled = 0;

  if (it.data) {
    for (u32 i = 0; i < it.capacity; i++) { it.data[i].hash = 0; }
  }
}

template <typename K, typename V>
void table_free(Hash_Table<K, V> &it) {
  if (it.data) {
    Free(it.data, it.allocator);
    it.data = NULL;
    it.capacity = 0;
    it.count = 0;
    it.slots_filled = 0;
  }
}

template <typename K, typename V>
void table_expand(Hash_Table<K, V> &it) {
  u32 next_capacity = it.capacity ? it.capacity * 2 : 32;

  assert((next_capacity & (next_capacity - 1)) == 0); // Must be a power of two!

  auto *old_data = it.data;
  u32 old_capacity = it.capacity;

  table_init(it, next_capacity);

  // count and slots_filled will be incremented by add.
  it.count        = 0;
  it.slots_filled = 0;

  for (u32 i = 0; i < old_capacity; i++) {
    auto *entry = &old_data[i];

    // Note that if we removed some stuff we will over-allocate the new table.
    // Maybe we should count the number of clobbers and subtract that? I dunno.
    if (entry->hash >= TABLE_FIRST_VALID_HASH) table_add(it, entry->key, entry->value);
  }

  Free(old_data, it.allocator);
}

// Sets the key-value pair, replacing it if it already exists.
template <typename K, typename V>
V *table_set(Hash_Table<K, V> &it, K key, V value) {
  auto result = table_find_pointer(key);
  if (result) {
    *result = value;
    return result;
  } else {
    return table_add(key, value);
  }
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
    index = (index + 1) & (it.capacity - 1);
  }

  it.count ++;
  it.slots_filled ++;
  it.data[index] = {hash, key, value};

  return &it.data[index].value;
}

template <typename K, typename V>
V *table_find_pointer(Hash_Table<K, V> &it, K key) {
  if (!it.data) return NULL; // @Incomplete: do we want this extra branch hit here?

  assert(it.data); // Must be initialized!

  u32 hash = table_key_hash(key);
  if (hash < TABLE_FIRST_VALID_HASH) hash += TABLE_FIRST_VALID_HASH;
  u32 index = hash & (it.capacity - 1);

  while (it.data[index].hash) {
    auto entry = &it.data[index];

    if (entry->hash == hash && table_key_equals(&entry->key, &key)) {
      return &entry->value;
    }

    index = (index + 1) & (it.capacity - 1);
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

    index = (index + 1) & (it.capacity - 1);
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


#endif // NJA_H
