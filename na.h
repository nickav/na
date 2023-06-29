/*
    na.h - v0.04
    Nick Aversano's C++ helper library

    This is a single header file with a bunch of useful stuff
    to replace the C++ standard library.
===========================================================================

USAGE
    Define this in your source file:

    #include "na.h"

LICENSE
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

CREDITS
    Written by Nick Aversano
    Credits are much appreciated but not required.

VERSION HISTORY
    0.04  - breaking API changes, lots of new stuff!
    0.03  - arena improvements
    0.02  - replace nja with na, arena and thread clean up
    0.01  - Initial version
*/

#ifndef NA_H
#define NA_H

#ifndef BASE_CTX_CRACK_H
#define BASE_CTX_CRACK_H

//
// OS
//

#if defined(_WIN32)
    #define OS_WINDOWS 1
#elif defined(__APPLE__)
    #define OS_MACOS 1
#elif defined(__linux__)
    #define OS_LINUX 1
#endif

#if !defined(OS_WINDOWS)
    #define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
    #define OS_LINUX 0
#endif
#if !defined(OS_MACOS)
    #define OS_MACOS 0
#endif

//
// Language
//

#if defined(__cplusplus)
    #define LANG_CPP 1
#else
    #define LANG_C 1
#endif

#if !defined(LANG_CPP)
    #define LANG_CPP 0
#endif
#if !defined(LANG_C)
    #define LANG_C 0
#endif

//
// Compiler
//

#if defined(_MSC_VER)
    #define COMPILER_CL 1
#elif defined(__clang__)
    #define COMPILER_CLANG 1
#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_GCC 1
#endif

#if !defined(COMPILER_CL)
    #define COMPILER_CL 0
#endif
#if !defined(COMPILER_GCC)
    #define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
    #define COMPILER_CLANG 0
#endif

//
// Arch
//

// TODO(nick): implement!

#endif // BASE_CTX_CRACK_H

#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//
// Keywords
//

#define global         static
#define function       static
#define local_persist  static

#if LANG_CPP
    #if OS_WINDOWS
        #define import extern "C" __declspec(dllimport)
        #define export extern "C" __declspec(dllexport)
    #else
        #define import extern "C"
        #define export extern "C"
    #endif
#else
    #if OS_WINDOWS
        #define import __declspec(dllimport)
        #define export __declspec(dllexport)
    #else
        #define import
        #define export
    #endif
#endif

#if COMPILER_CL
    #define thread_local __declspec(thread)
#elif COMPILER_CLANG
    #define thread_local __thread
#elif COMPILER_GCC
    #define thread_local __thread
#endif

#if COMPILER_CL && _MSC_VER < 1900 // COMPILER_CL_YEAR < 2015
    #define __function__ "unknown"
#else
    #define __function__ __func__
#endif

#define fallthrough

#if OS_WINDOWS
    #pragma section(".roglob", read)
    #define read_only __declspec(allocate(".roglob"))
#else
    #define read_only
#endif

#ifndef force_inline
    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define force_inline __attribute__((always_inline)) inline
    #elif defined(__llvm__)
        #define force_inline __attribute__((always_inline)) inline
    #elif defined(_MSC_VER)
        #define force_inline __forceinline
    #else
        #define force_inline inline
    #endif
#endif

//
// Helper Macros
//

#define MemoryCopy(d,s,size) memcpy(d,s,size)
#define MemoryMove(d,s,size) memmove(d,s,size)
#define MemorySet(ptr,x,size) memset(ptr,x,size)

#define MemoryCompare(a, b, s) memcmp(a, b, s)
#define MemoryEquals(a, b, s) (memcmp(a, b, s) == 0)

#define MemoryCopyStruct(d,s) do { assert((d)!=NULL && (s)!=NULL); Assert(sizeof(*(d))==sizeof(*(s))); MemoryCopy((d),(s),sizeof(*(d))); } while(0)
#define MemoryCopyArray(d,s) do{ assert((d)!=NULL && (s)!=NULL); Assert(sizeof(d)==sizeof(s)); MemoryCopy((d),(s),sizeof(s)); }while(0)

#define MemoryZero(p,s) do { assert((p)!=NULL); MemorySet((p), 0, (s)); } while(0)
#define MemoryZeroStruct(p) MemoryZero((p), sizeof(*(p)))
#define MemoryZeroArray(a) MemoryZero((a), sizeof(a))

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define IntFromPtr(p) (u64)(((u8*)p) - 0)
#define PtrFromInt(i) (void*)(((u8*)0) + i)
#define Member(S,m) ((S*)0)->m
#define OffsetOf(S,m) IntFromPtr(&Member(S,m))
#define CastFromMember(S,m,p) (S*)(((u8*)p) - OffsetOf(S,m))
#define MemberFromOffset(ptr, off, type) *(type *)((u8 *)(ptr) + off)
#define UnusedVariable(name) (void)name

#define Bytes(n)      (n)
#define Kilobytes(n)  (n << 10)
#define Megabytes(n)  (n << 20)
#define Gigabytes(n)  (((u64)n) << 30)
#define Terabytes(n)  (((u64)n) << 40)

#define Thousand(x) ((x)*1000)
#define Million(x) ((x)*1000000)
#define Billion(x) ((x)*1000000000LL)

#define HasFlag(fl,fi) ((fl)&(fi))
#define SetFlag(fl,fi) ((fl)|=(fi))
#define RemFlag(fl,fi) ((fl)&=~(fi))
#define ToggleFlag(fl,fi) ((fl)^=(fi))
#define SetFlagState(fl,fi,set) do { if (set) SetFlag(fl,fi); else RemFlag(fl,fi); } while (0)

#define Swap(T,a,b) do{ T t__ = a; a = b; b = t__; }while(0)

#define FourCC(a, b, c, d) \
    (((u32)(a) << 0) | ((u32)(b) << 8) | ((u32)(c) << 16) | ((u32)(d) << 24))

#define count_of ArrayCount
#define offset_of OffsetOf
#define cast(T) (T)

//
// Linked-List Macros
//

#define CheckNull(p) ((p)==0)
#define SetNull(p) ((p)=0)

#define QueuePush_NZ(f,l,n,next,zchk,zset) (zchk(f)?\
(((f)=(l)=(n)), zset((n)->next)):\
((l)->next=(n),(l)=(n),zset((n)->next)))
#define QueuePushFront_NZ(f,l,n,next,zchk,zset) (zchk(f) ? (((f) = (l) = (n)), zset((n)->next)) :\
((n)->next = (f)), ((f) = (n)))
#define QueuePop_NZ(f,l,next,zset) ((f)==(l)?\
(zset(f),zset(l)):\
(f)=(f)->next)
#define StackPush_N(f,n,next) ((n)->next=(f),(f)=(n))
#define StackPop_NZ(f,next,zchk) (zchk(f)?0:((f)=(f)->next))

#define DLLInsert_NPZ(f,l,p,n,next,prev,zchk,zset) \
(zchk(f) ? (((f) = (l) = (n)), zset((n)->next), zset((n)->prev)) :\
zchk(p) ? (zset((n)->prev), (n)->next = (f), (zchk(f) ? (0) : ((f)->prev = (n))), (f) = (n)) :\
((zchk((p)->next) ? (0) : (((p)->next->prev) = (n))), (n)->next = (p)->next, (n)->prev = (p), (p)->next = (n),\
((p) == (l) ? (l) = (n) : (0))))
#define DLLPushBack_NPZ(f,l,n,next,prev,zchk,zset) DLLInsert_NPZ(f,l,l,n,next,prev,zchk,zset)
#define DLLRemove_NPZ(f,l,n,next,prev,zchk,zset) (((f)==(n))?\
((f)=(f)->next, (zchk(f) ? (zset(l)) : zset((f)->prev))):\
((l)==(n))?\
((l)=(l)->prev, (zchk(l) ? (zset(f)) : zset((l)->next))):\
((zchk((n)->next) ? (0) : ((n)->next->prev=(n)->prev)),\
(zchk((n)->prev) ? (0) : ((n)->prev->next=(n)->next))))


#define QueuePush(f,l,n)         QueuePush_NZ(f,l,n,next,CheckNull,SetNull)
#define QueuePushFront(f,l,n)    QueuePushFront_NZ(f,l,n,next,CheckNull,SetNull)
#define QueuePop(f,l)            QueuePop_NZ(f,l,next,SetNull)
#define StackPush(f,n)           StackPush_N(f,n,next)
#define StackPop(f)              StackPop_NZ(f,next,CheckNull)
#define DLLPushBack(f,l,n)       DLLPushBack_NPZ(f,l,n,next,prev,CheckNull,SetNull)
#define DLLPushFront(f,l,n)      DLLPushBack_NPZ(l,f,n,prev,next,CheckNull,SetNull)
#define DLLInsert(f,l,p,n)       DLLInsert_NPZ(f,l,p,n,next,prev,CheckNull,SetNull)
#define DLLRemove(f,l,n)         DLLRemove_NPZ(f,l,n,next,prev,CheckNull,SetNull)

#define ListEach(it,list) auto it = (list)->first; it != NULL; it = it->next

//
// Clamps
//

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Clamp(value, lower, upper) (Max(Min(value, upper), lower))
#define ClampTop(a, b) Min(a, b)
#define ClampBot(a, b) Max(a, b)
#define Sign(x) (((x) > 0) - ((x) < 0))
#define Abs(x) (((x) < 0) ? (0u - x) : (0u + x))

#define AlignUpPow2(x, p) (((x) + (p) - 1) & ~((p) - 1))
#define AlignDownPow2(x, p) ((x) & ~((p) - 1))
#define IsPow2(x) (((x) & ((x) - 1)) == 0)

//
// Defer
//

#define Defer(start, end) for(int _i_ = (start, 0); _i_ == 0; _i_ += 1, end)

#if LANG_CPP
    #ifndef CONCAT
    #define CONCAT_HELPER(x, y) x##y
    #define CONCAT(x, y) CONCAT_HELPER(x, y)
    #endif

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
#endif // LANG_CPP

//
// Types
//

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
// Language Helpers
//

typedef struct MemberOffset MemberOffset;
struct MemberOffset
{
    u64 v;
};

#define MemberOff(S, member) (MemberOffset){OffsetOf(S, m)}
#define MemberFromOff(ptr, type, memoff) (*(type *)((u8 *)ptr + memoff.v))

//
// Assert
//

int na__assert(bool cond, const char *expr, const char *file, long int line, char *msg) {
    if (!cond) {
        printf("%s(%ld): %s: ", file, line, "Assertion Failed");
        if (expr) {
            printf( "`%s` ", expr);
        }
        if (msg) {
            printf("- %s", msg);
        }
        printf("\n");
        fflush(stdout);

        #if COMPILER_MSVC
        __debugbreak();
        #endif

        *(volatile int *)0 = 0;
    }

    return 0;
}

#undef assert
#define assert(expr) na__assert(expr, #expr, __FILE__, __LINE__, NULL)

#define Assert(expr) na__assert(expr, #expr, __FILE__, __LINE__, NULL)


#if LANG_C
    #ifdef _Static_assert
        #define StaticAssert(c,label) _Static_assert(c, label)
    #else
        #define StaticAssert(c,label) u8 static_assert__failed_on_line_##__LINE__[(c)?(1):(-1)]
    #endif
#else
    #define StaticAssert(c,label) static_assert(c, #label)
#endif

#define NotImplemented assert(!"Not Implemented")
#define InvalidPath assert(!"Invalid Path")

#endif // BASE_TYPES_H

#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

#if !defined(ARENA_DEFAULT_ALIGNMENT)
#define ARENA_DEFAULT_ALIGNMENT 8
#endif

#if !defined(ARENA_COMMIT_SIZE)
#define ARENA_COMMIT_SIZE Kilobytes(4)
#endif

#if !defined(ARENA_INITIAL_COMMIT_SIZE)
#define ARENA_INITIAL_COMMIT_SIZE Kilobytes(4)
#endif

#if !defined(ARENA_DECOMMIT_THRESHOLD)
#define ARENA_DECOMMIT_THRESHOLD Kilobytes(64)
#endif

struct Arena {
    u8 *data;
    u64 pos;
    u64 size;
    u64 commit_pos;
};

struct M_Temp {
    Arena *arena;
    u64 pos;
};

//
// API
//

function Arena arena_make(u8 *data, u64 size);
function void arena_init(Arena *arena, u8 *data, u64 size);
function Arena *arena_alloc(u64 size);
function Arena *arena_alloc_default();
function void arena_free(Arena *arena);
function void *arena_push_bytes(Arena *arena, u64 size);
function void arena_pop_to(Arena *arena, u64 pos);
function void arena_pop(Arena *arena, u64 size);
function void arena_set_pos(Arena *arena, u64 pos);
function void arena_reset(Arena *arena);
function void arena_align(Arena *arena, u64 pow2_align);
function void *arena_push(Arena *arena, u64 size);
function void *arena_push_zero(Arena *arena, u64 size);
function bool arena_write(Arena *arena, u8 *data, u64 size);

function void *arena_resize_ptr(Arena *arena, u64 new_size, void *old_memory_pointer, u64 old_size);
function void arena_free_ptr(Arena *arena, void *old_memory_pointer, u64 old_size);

function M_Temp arena_begin_temp(Arena *arena);
function void arena_end_temp(M_Temp temp);

#define PushArray(a,T,c)     (T*)arena_push((a), sizeof(T)*(c))
#define PushArrayZero(a,T,c) (T*)arena_push_zero((a), sizeof(T)*(c))
#define PushStruct(a, T)     (T*)arena_push((a), sizeof(T))
#define PushStructZero(a, T) (T*)arena_push_zero((a), sizeof(T))

function M_Temp arena_get_scratch(Arena **conflicts, u64 conflict_count);
#define GetScratch(conflicts, conflict_count) arena_get_scratch(conflicts, conflict_count)
#define ReleaseScratch(temp) arena_end_temp(temp)
function Arena *temp_arena();

// Helpers
typedef i32 Compare_Proc(void *a, void *b);

function void memory_swap(void *i, void *j, u64 size);
function void memory_sort(void *base_, u64 count, u64 size, Compare_Proc cmp);
function i64 memory_binary_search(void *base, u64 count, u64 size, void *key, Compare_Proc cmp);

//
// Allocator
//

#if !defined(ALLOCATOR_DEFAULT_ALIGNMENT)
#define ALLOCATOR_DEFAULT_ALIGNMENT 16
#endif


enum Allocator_Mode
{
    AllocatorMode_Alloc    = 0,
    AllocatorMode_Resize   = 1,
    AllocatorMode_Free     = 2,
    AllocatorMode_FreeAll  = 3,
};

#define ALLOCATOR_PROC(name) void *name(Allocator_Mode mode, u64 requested_size, u64 old_size, void *old_memory_pointer, void *allocator_data, u32 alignment)
typedef ALLOCATOR_PROC(Allocator_Proc);

struct Allocator
{
    Allocator_Proc *proc;
    void *data;
};

//
// API
//

function void *allocator_alloc(Allocator allocator, u64 size);
function void allocator_free(Allocator allocator, void *data, u64 old_size);
function void *allocator_realloc(Allocator allocator, void *data, u64 new_size, u64 old_size);

function void *allocator_alloc_aligned(Allocator allocator, u64 size, u32 alignment);
function void *allocator_realloc_aligned(Allocator allocator, void *data, u64 new_size, u64 old_size, u32 alignment);

#ifndef default_allocator
#define default_allocator() os_allocator()
#endif

function Allocator os_allocator();
function Allocator arena_allocator(Arena *arena);

#endif // BASE_MEMORY_H

#ifndef BASE_STRINGS_H
#define BASE_STRINGS_H

struct String {
    u8 *data;
    i64 count;
};

struct String16 {
    u16 *data;
    i64 count;
};

struct String32 {
    u32 *data;
    i64 count;
};

struct String_Decode {
    u32 codepoint;
    u8 advance; // 1 - 4
};

struct String_Node
{
    String_Node *next;
    String string;
};

struct String_List
{
    String_Node *first;
    String_Node *last;
    u64 node_count;
    u64 total_size;
};

struct String_Join
{
    String pre;
    String sep;
    String post;
};

typedef u32 Match_Flags;
enum
{
    MatchFlag_None             = 0,
    MatchFlag_CaseInsensitive  = 1 << 0,
    MatchFlag_RightSideSloppy  = 1 << 1,
    MatchFlag_SlashInsensitive = 1 << 2,
    MatchFlag_FindLast         = 1 << 3,
};

// Char Functions
function b32 char_is_alpha(u8 c);
function b32 char_is_lower(u8 c);
function b32 char_is_upper(u8 c);
function b32 char_is_digit(u8 c);
function b32 char_is_space(u8 c);
function b32 char_is_whitespace(u8 c);
function b32 char_is_symbol(u8 c);
function u8 char_to_upper(u8 c);
function u8 char_to_lower(u8 c);
function u8 char_to_forward_slash(u8 c);

// Constructors
function String string_make(u8 *data, i64 count);
function String string_range(u8 *at, u8 *end);
function String string_from_cstr(char *cstr);
function char *string_to_cstr(Arena *arena, String str);
function String16 string16_make(u16 *data, i64 count);
function String16 string16_from_cstr(u16 *data);
function String32 string32_make(u32 *data, i64 count);
#define S(x) {(u8 *)(x), sizeof(x)-1}
#define Str8(data, count) string_make((u8 *)data, count)
#define Str16(data, count) string16_make((u16 *)data, count)
#define Str32(data, count) string32_make((u32 *)data, count)

// Print Helper
#define LIT(str) (int)str.count, (char *)str.data

// Substrings
function String string_slice(String str, i64 start_index, i64 end_index);
function String string_substr(String str, i64 offset, i64 count);
function String string_skip(String str, i64 offset);
function String string_chop(String str, i64 offset);
function String string_prefix(String str, i64 count);
function String string_suffix(String str, i64 count);

// Matching
function bool string_equals(String a, String b);
function bool string_match(String a, String b, Match_Flags flags);
function i64 string_find(String str, String search, i64 start_index, Match_Flags flags);
function bool string_starts_with(String str, String prefix);
function bool string_ends_with(String str, String postfix);
function i64 string_index(String str, String search, i64 start_index);
function i64 string_char_index(String str, u8 search, i64 start_index);
function i64 string_last_index(String str, String search);
function bool string_contains(String str, String search);

// Allocation
function String string_copy(Arena *arena, String str);
function String string_printv(Arena *arena, char *fmt, va_list args);
function String string_print(Arena *arena, char *fmt, ...);
#define PushStringCopy(arena, str) string_copy(arena, str)
#define PushStringFV(arena, fmt, args) string_printv(arena, fmt, args)
#define PushStringF(arena, fmt, ...) string_print(arena, fmt, __VA_ARGS__)

#define sprint(fmt, ...) string_print(temp_arena(), fmt, __VA_ARGS__)

function String string_replace(Arena *arena, String str, String find, String replacer, u64 replace_limit);

// Unicode Conversions
function String_Decode string_decode_utf8(u8 *str, u32 capacity);
function u32 string_encode_utf8(u8 *dest, u32 codepoint);
function u8 string_seek_right_utf8(u8 *data, u32 capacity);
function u8 string_seek_left_utf8(u8 *data, u32 capacity);
function String_Decode string_decode_utf16(u16 *str, u32 capacity);
function u32 string_encode_utf16(u16 *dest, u32 codepoint);
function String32 string32_from_string(Arena *arena, String str);
function String string_from_string32(Arena *arena, String32 str);
function String16 string16_from_string(Arena *arena, String str);
function String string_from_string16(Arena *arena, String16 str);

// Conversions
function u64 string_to_u64(String string, u32 radix);
function i64 string_to_i64(String str, u32 base);
function f64 string_to_f64(String str);
function b32 string_to_b32(String str);

// String Lists
function void string_list_push_node(String_List *list, String_Node *n);
function void string_list_push(Arena *arena, String_List *list, String str);
function void string_list_concat(String_List *list, String_List *to_push);
function String_List string_splits(Arena *arena, String string, int split_count, String *splits);
function String_List string_split(Arena *arena, String string, int split_count, String *splits);
function String string_list_joins(Arena *arena, String_List list, String_Join *optional_params);
function String string_list_join(Arena *arena, String_List list, String join);
function String string_list_print(Arena *arena, String_List *list, char *fmt, ...);
function String string_list_to_string(Arena *arena, String_List *list);

// Misc Helpers
function String string_concat2(Arena *arena, String a, String b);
function String string_concat3(Arena *arena, String a, String b, String c);
function String string_concat4(Arena *arena, String a, String b, String c, String d);
#define string_concat(a, b) string_concat2(temp_arena(), a, b)

function String string_chop_last_period(String string);
function String string_skip_last_period(String string);
function String string_chop_last_slash(String string);
function String string_skip_last_slash(String string);

function String string_trim_whitespace(String str);

// Path Helpers
function String path_filename(String path);
function String path_basename(String path);
function String path_extension(String path);
function String path_strip_extension(String path);

function String path_join2(Arena *arena, String a, String b);
function String path_join3(Arena *arena, String a, String b, String c);
function String path_join4(Arena *arena, String a, String b, String c, String d);
#define path_join(a, b) path_join2(temp_arena(), a, b)

function b32 path_is_absolute(String path);

#if DEBUG
    #define dump(x) print("%s = %S\n", #x, to_string(x))
#else
    #define dump(x)
#endif

#endif // BASE_STRINGS_H

#ifndef BASE_FUNCTIONS_H
#define BASE_FUNCTIONS_H

struct Random_State
{
    u64 state;
    u64 selector;
};

//
// API
//

// Math
function b32 is_power_of_two(i64 x);
function u64 next_power_of_two(u64 x);
function u64 previous_power_of_two(u64 x);

function u16 endian_swap16(u16 i);
function u32 endian_swap32(u32 i);
function u64 endian_swap64(u64 i);

function u32 rotate_left_u32(u32 value, i32 amount);
function u32 rotate_right_u32(u32 value, i32 amount);

// Hashing
function u32 murmur32_seed(void const *data, i64 len, u32 seed);
function u64 murmur64_seed(void const *data_, i64 len, u64 seed);
function u32 murmur32(void const *data, i64 len);
function u64 murmur64(void const *data, i64 len);
function u32 fnv32a(void const *data, i64 len);
function u64 fnv64a(void const *data, i64 len);

// Random PCG
function Random_State random_make_default();
function void random_set_seed(Random_State *series, u64 state, u64 selector);
function u32 random_u32(Random_State *series);
function f32 random_f32(Random_State *series);
function f32 random_f32_between(Random_State *series, f32 min, f32 max);
function i32 random_i32_between(Random_State *series, i32 min, i32 max);
function void random_shuffle(Random_State *it, void *base, u64 count, u64 size);

#endif // BASE_FUNCTIONS_H

#ifndef OS_H
#define OS_H

typedef u32 SystemPath;
enum {
    SystemPath_Null,
    //SystemPath_Initial,
    SystemPath_Current,
    SystemPath_Binary,
    SystemPath_AppData,
    SystemPath_COUNT,
};

struct File {
    void *handle;
    bool has_errors;
    u64 offset;
};

typedef u32 File_Attribute_Flags;
enum {
    File_IsDirectory = 0x01,
    File_IsHidden    = 0x02,
    File_IsSystem    = 0x04,
};

typedef u32 File_Access_Flags;
enum {
    FileAccess_Read    = 0x01,
    FileAccess_Write   = 0x02,
    FileAccess_Execute = 0x04,
};

typedef u64 Dense_Time;

struct File_Info {
    String name;
    u64 size;
    Dense_Time created_at;
    Dense_Time updated_at;
    Dense_Time last_accessed_at;
    File_Attribute_Flags flags;
    File_Access_Flags access;
};

typedef u32 File_Mode;
enum {
    FileMode_Read   = 0x1,
    FileMode_Write  = 0x2,
    FileMode_Append = 0x4,
};

struct File_Lister
{
    u8 opaque[1024];
};

struct OS_Library {
    void *handle;
};

struct Date_Time {
    u16 msec; // [0,999]
    u8 sec;   // [0,60]
    u8 min;   // [0,59]
    u8 hour;  // [0,23]
    u8 day;   // [1,31]
    u8 mon;   // [1,12]
    i16 year; // 1 = 1 CE; 2020 = 2020 CE; 0 = 1 BCE; -100 = 101 BCE; etc.
};

typedef u32 Month;
enum {
    Month_Jan = 1,
    Month_Feb,
    Month_Mar,
    Month_Apr,
    Month_May,
    Month_Jun,
    Month_Jul,
    Month_Aug,
    Month_Sep,
    Month_Oct,
    Month_Nov,
    Month_Dec
};

typedef u32 DayOfWeek;
enum {
    DayOfWeek_Sunday,
    DayOfWeek_Monday,
    DayOfWeek_Tuesday,
    DayOfWeek_Wednesday,
    DayOfWeek_Thursday,
    DayOfWeek_Friday,
    DayOfWeek_Saturday
};

// System
function bool os_init();
function void os_exit(i32 code);
function String os_get_system_path(Arena *arena, SystemPath path);
#define os_get_executable_path() os_get_system_path(temp_arena(), SystemPath_Binary)
#define os_get_current_path() os_get_system_path(temp_arena(), SystemPath_Current)

// Timing
function f64 os_time();
function f64 os_time_in_miliseconds();
function u64 os_clock_cycles();
function void os_sleep(f64 seconds);
function void os_set_high_process_priority(bool enable);

// Memory
function u64  os_page_size();
function void *os_reserve(u64 size);
function bool os_commit(void *ptr, u64 size);
function bool os_decommit(void *ptr, u64 size);
function bool os_release(void *ptr, u64 size);
function void *os_alloc(u64 size);
function void os_free(void *ptr);
#define New(T) (T *)os_alloc(sizeof(T))
#define Free(ptr) os_free((void *)ptr)

// Files
function String os_read_entire_file(Arena *arena, String path);
function bool os_write_entire_file(String path, String contents);
function File_Info os_get_file_info(String path);

function File os_file_open(String path, u32 mode_flags);
function void os_file_read(File *file, u64 offset, u64 size, void *dest);
function void os_file_write(File *file, u64 offset, u64 size, void *data);
function void os_file_close(File *file);

function void os_file_append(File *file, String data);
function void os_file_print(File *file, char *fmt, ...);

function bool os_file_rename(String from, String to);
function bool os_delete_file(String path);
function bool os_make_directory(String path);
function bool os_delete_directory(String path);

// File Lister
function File_Lister *os_file_list_begin(Arena *arena, String path);
function bool os_file_list_next(File_Lister *iter, File_Info *info);
function void os_file_list_end(File_Lister *iter);

// Clipboard
function String os_get_clipboard_text();
function bool os_set_clipboard_text(String str);

// Dates
function Date_Time os_get_current_time_in_utc();
function Date_Time os_get_local_time();

// Library
function OS_Library os_library_load(String path);
function void os_library_unload(OS_Library lib);
function void *os_library_get_proc(OS_Library lib, char *proc_name);
function bool os_library_is_loaded(OS_Library lib);

// Shell
function bool os_shell_open(String path);
function bool os_shell_execute(String cmd, String arguments, bool admin = false);

// Dates
function Date_Time os_get_current_time_in_utc();
function Date_Time os_get_local_time();
function Dense_Time dense_time_from_date_time(Date_Time in);
function Date_Time date_time_from_dense_time(Dense_Time in);

// Misc.
function void os_get_entropy(void *data, u64 size);
function f64 os_caret_blink_time();

// Debugging
function void os_open_file_in_debugger(String path, int line);
function void os_attach_to_debugger(b32 pause);


#define M_Reserve os_reserve
#define M_Release os_release
#define M_Commit os_commit
#define M_Decommit os_decommit

//
// Threads
//

struct Thread {
    u32 id;
    void *handle;
};

#define THREAD_PROC(name) u32 name(void *data)
typedef THREAD_PROC(Thread_Proc);

struct Semaphore {
    void *handle;
};

struct Mutex {
    void *handle;
};

// Basic
function u32 thread_get_id();

#if COMPILER_MSVC
    #define read_barrier() _ReadBarrier()
    #define write_barrier() _WriteBarrier()
#else
    #define read_barrier() asm volatile("" ::: "memory")
    #define write_barrier() asm volatile("" ::: "memory")
#endif

// Atomics
inline u32 atomic_compare_exchange_u32(u32 volatile *value, u32 New, u32 Expected);
inline u64 atomic_exchange_u64(u64 volatile *value, u64 New);
inline u64 atomic_add_u64(u64 volatile *value, u64 Addend);

// Threads
function Thread thread_create(Thread_Proc *proc, void *data, u64 copy_size);
function void thread_pause(Thread thread);
function void thread_resume(Thread thread);
function void thread_detach(Thread thread);
function u32 thread_await(Thread thread);

// Data Structures
function Semaphore semaphore_create(u32 max_count);
function void semaphore_signal(Semaphore *sem);
function void semaphore_wait_for(Semaphore *sem, bool infinite);
function void semaphore_destroy(Semaphore *sem);

function Mutex mutex_create(u32 spin_count);
function void mutex_aquire_lock(Mutex *mutex);
function bool mutex_try_aquire_lock(Mutex *mutex);
function void mutex_release_lock(Mutex *mutex);
function void mutex_destroy(Mutex *mutex);

//
// Platform-Specific Headers:
//

#if OS_WINDOWS

    #pragma push_macro("function")
    #pragma push_macro("Free")
    #undef function
    #undef Free
    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #define NOMINMAX
    #include <windows.h>
    #include <Shlobj.h>
    #pragma pop_macro("function")
    #pragma pop_macro("Free")

#elif OS_MACOS
#else
    // TODO(nick): support linux
    #error OS layer not implemented.
#endif

#endif // OS_H

#endif // NA_H

#ifndef NA_IMPL
#define NA_IMPL

//
// Memory
//

#if !defined(M_Reserve)
#error M_Reserve must be defined to use base memory.
#endif
#if !defined(M_Release)
#error M_Release must be defined to use base memory.
#endif
#if !defined(M_Commit)
#error M_Commit must be defined to use base memory.
#endif
#if !defined(M_Decommit)
#error M_Decommit must be defined to use base memory.
#endif

#define arena_has_virtual_backing(arena) (arena->commit_pos < U64_MAX)

function Arena arena_make(u8 *data, u64 size) {
    Arena result = {0};
    result.data = data;
    result.size = size;
    result.pos = 0;
    result.commit_pos = U64_MAX;
    return result;
}

function void arena_init(Arena *arena, u8 *data, u64 size) {
    arena->data = data;
    arena->size = size;
    arena->pos  = 0;
    arena->commit_pos = U64_MAX;
}

function Arena *arena_alloc(u64 size) {
    size = Max(size, ARENA_INITIAL_COMMIT_SIZE);

    Arena *result = NULL;
    u8 *data = cast(u8 *)M_Reserve(size);

    if (data && M_Commit(data, ARENA_INITIAL_COMMIT_SIZE))
    {
        result = cast(Arena *)data;
        result->data = data + AlignUpPow2(sizeof(Arena), 64);
        result->size = size;
        result->pos  = 0;
        result->commit_pos = ARENA_INITIAL_COMMIT_SIZE;
    }

    assert(result != 0);
    return result;
}

function Arena *arena_alloc_default()
{
    return arena_alloc(Gigabytes(1));
}

function void arena_free(Arena *arena) {
    if (arena->data)
    {
        arena->data = NULL;

        if (arena_has_virtual_backing(arena))
        {
            M_Release(arena, arena->size);
        }
    }
}

function void *arena_push_bytes(Arena *arena, u64 size) {
    void *result = NULL;

    if (arena->pos + size <= arena->size)
    {
        void *result_on_success = arena->data + arena->pos;
        u64 p = arena->pos + size;
        u64 commit_p = arena->commit_pos;

        if (arena_has_virtual_backing(arena))
        {
            u64 base_p = p + AlignUpPow2(sizeof(Arena), 64);
            if (base_p > commit_p)
            {
                u64 p_aligned = AlignUpPow2(base_p, ARENA_COMMIT_SIZE);
                u64 next_commit_position = ClampTop(p_aligned, arena->size);
                u64 commit_size = next_commit_position - commit_p;

                if (M_Commit((u8 *)arena + arena->commit_pos, commit_size))
                {
                    commit_p = next_commit_position;
                    arena->commit_pos = next_commit_position;
                }
            }
        }

        if (p <= commit_p)
        {
            result = result_on_success;
            arena->pos = p;
        }
    }

    return result;
}

function void arena_pop_to(Arena *arena, u64 pos) {
    if (arena->pos > pos)
    {
        arena->pos = pos;

        if (arena_has_virtual_backing(arena))
        {
            u64 base_p = pos + AlignUpPow2(sizeof(Arena), 64);
            u64 decommit_pos = AlignUpPow2(base_p, ARENA_COMMIT_SIZE);
            u64 over_committed = arena->commit_pos - decommit_pos;

            if (decommit_pos < arena->commit_pos && over_committed >= ARENA_DECOMMIT_THRESHOLD)
            {
                if (M_Decommit((u8 *)arena + decommit_pos, over_committed))
                {
                    arena->commit_pos -= over_committed;
                }
            }
        }
    }
}

function void arena_pop(Arena *arena, u64 size) {
    arena_pop_to(arena, arena->pos - size);
}

function void arena_set_pos(Arena *arena, u64 pos) {
    if (arena->pos > pos) {
        arena_pop_to(arena, pos);
    } else {
        arena_push_bytes(arena, pos - arena->pos);
    }
}

function void arena_reset(Arena *arena) {
    arena_pop_to(arena, 0);
}

function void arena_align(Arena *arena, u64 pow2_align) {
    u64 p = arena->pos;
    u64 p_aligned = AlignUpPow2(p, pow2_align);
    u64 z = p_aligned - p;
    if (z > 0) {
        arena_push_bytes(arena, z);
    }
}

function void *arena_push(Arena *arena, u64 size) {
    arena_align(arena, ARENA_DEFAULT_ALIGNMENT);
    return arena_push_bytes(arena, size);
}

function void *arena_push_zero(Arena *arena, u64 size) {
    void *result = arena_push(arena, size);
    if (result != NULL)
    {
        MemoryZero(result, size);
    }
    return result;
}

function bool arena_write(Arena *arena, u8 *data, u64 size) {
    bool result = false;

    u8 *buffer = (u8 *)arena_push(arena, size);
    if (buffer != NULL) {
        MemoryCopy(buffer, data, size);
        result = true;
    }

    return result;
}

function void *arena_resize_ptr(Arena *arena, u64 new_size, void *old_memory_pointer, u64 old_size)
{
    void *result = NULL;

    if (old_memory_pointer && old_size > 0)
    {
        u64 old_memory_offset = (u8 *)(old_memory_pointer) - arena->data;
        if (old_memory_offset == arena->pos - old_size)
        {
            arena_set_pos(arena, old_memory_offset + new_size);
            result = old_memory_pointer;
        }
    }

    if (!result)
    {
        result = arena_push_bytes(arena, new_size);

        if (result && old_memory_pointer)
        {
            MemoryCopy(result, old_memory_pointer, Min(new_size, old_size));
        }
    }

    return result;
}

function void arena_free_ptr(Arena *arena, void *old_memory_pointer, u64 old_size)
{
    if (old_memory_pointer && old_size > 0)
    {
        u64 old_memory_offset = (u8 *)(old_memory_pointer) - arena->data;
        if (old_memory_offset == arena->pos - old_size)
        {
            arena_pop(arena, old_size);
        }
    }
}

function M_Temp arena_begin_temp(Arena *arena) {
    M_Temp result = {arena, arena->pos};
    return result;
}

function void arena_end_temp(M_Temp temp) {
    arena_pop_to(temp.arena, temp.pos);
}

thread_local Arena *m__scratch_pool[2] = {0};

function M_Temp arena_get_scratch(Arena **conflicts, u64 conflict_count) {
    if (m__scratch_pool[0] == 0) {
        for (int i = 0; i < count_of(m__scratch_pool); i += 1)
        {
            m__scratch_pool[i] = arena_alloc_default();
        }
    }

    M_Temp result = {0};
    for (u64 i = 0; i < count_of(m__scratch_pool); i += 1)
    {
        b32 is_conflict = false;
        for (Arena **conflict = conflicts; conflict < conflicts+conflict_count; conflict += 1)
        {
            if (*conflict == m__scratch_pool[i]) {
                is_conflict = true;
                break;
            }
        }

        if (!is_conflict) {
            result = arena_begin_temp(m__scratch_pool[i]);
            break;
        }
    }
    return result;
}

function Arena *temp_arena() {
    if (m__scratch_pool[0] == 0) {
        for (int i = 0; i < count_of(m__scratch_pool); i += 1)
        {
            m__scratch_pool[i] = arena_alloc_default();
        }
    }

    return m__scratch_pool[0];
}

function u64 m_align_offset(void *ptr, u64 alignment){
    u64 base_address = (u64)ptr;

    assert(alignment >= 1);
    assert(IsPow2(alignment));

    u64 align_offset = alignment - (base_address & (alignment - 1));
    align_offset &= (alignment - 1);

    return align_offset;
}

function void memory_swap(void *i, void *j, u64 size)
{
    if (size == 4) {
        Swap(u32, *(u32 *)i, *(u32 *)j);
    } else if (size == 8) {
        Swap(u64, *(u64 *)i, *(u64 *)j);
    } else if (size < 8) {
        u8 *a = (u8 *)i;
        u8 *b = (u8 *)j;

        if (a != b) {
            while (size--) {
                Swap(u8, *a, *b);
                a++, b++;
            }
        }
    } else {
        char buffer[256];

        // TODO(bill): Is the recursion ever a problem?
        while (size > sizeof(buffer))
        {
            memory_swap(i, j, sizeof(buffer));
            i = (void *)((u8 *)i + sizeof(buffer));
            j = (void *)((u8 *)j + sizeof(buffer));
            size -= sizeof(buffer);
        }

        MemoryCopy(buffer, i, size);
        MemoryCopy(i, j, size);
        MemoryCopy(j, buffer, size);
    }
}

function void memory_sort(void *base_, u64 count, u64 size, Compare_Proc cmp)
{
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

    u8 *i, *j;
    u8 *base = (u8 *)base_;
    u8 *limit = base + count*size;
    u64 threshold = GB__SORT_INSERT_SORT_THRESHOLD * size;

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

    #undef GB__SORT_PUSH
    #undef GB__SORT_POP
}

function i64 memory_binary_search(void *base, u64 count, u64 size, void *key, Compare_Proc cmp)
{
    u64 start = 0;
    u64 end = count;

    while (start < end) {
        u64 mid = start + (end-start)/2;
        i64 result = cmp(key, cast(u8 *)base + mid*size);

        if (result < 0) {
            end = mid;
        } else if (result > 0) {
            start = mid+1;
        } else {
            return mid;
        }
    }

    return -1;
}

//
// Allocator
//

function void *allocator_alloc(Allocator allocator, u64 size)
{
    if (!allocator.proc) allocator = default_allocator();

    if (allocator.proc) {
        void *result = allocator.proc(AllocatorMode_Alloc, size, 0, NULL, allocator.data, ALLOCATOR_DEFAULT_ALIGNMENT);
        return result;
    }

    return NULL;
}

function void *allocator_alloc_aligned(Allocator allocator, u64 size, u32 alignment)
{
    if (!allocator.proc) allocator = default_allocator();

    if (allocator.proc) {
        void *result = allocator.proc(AllocatorMode_Alloc, size, 0, NULL, allocator.data, alignment);
        return result;
    }

    return NULL;
}

function void allocator_free(Allocator allocator, void *data, u64 old_size)
{
    if (!allocator.proc) allocator = default_allocator();

    if (allocator.proc) {
        if (data != NULL) {
            allocator.proc(AllocatorMode_Free, 0, old_size, data, allocator.data, 0);
        }
    }
}

function void *allocator_realloc(Allocator allocator, void *data, u64 new_size, u64 old_size)
{
    if (!allocator.proc) allocator = default_allocator();

    if (allocator.proc) {
        return allocator.proc(AllocatorMode_Resize, new_size, old_size, data, allocator.data, ALLOCATOR_DEFAULT_ALIGNMENT);
    }

    return NULL;
}

function void *allocator_realloc_aligned(Allocator allocator, void *data, u64 new_size, u64 old_size, u32 alignment)
{
    if (!allocator.proc) allocator = default_allocator();

    if (allocator.proc) {
        return allocator.proc(AllocatorMode_Resize, new_size, old_size, data, allocator.data, alignment);
    }

    return NULL;
}

function ALLOCATOR_PROC(os_allocator_proc)
{
    switch (mode) {
        case AllocatorMode_Alloc: {
            u64 actual_size = requested_size + m_align_offset(0, alignment);
            void *result = os_alloc(actual_size);
            return result;
        }

        case AllocatorMode_Resize: {
            u64 actual_size = requested_size + m_align_offset(0, alignment);

            void *result = os_alloc(actual_size);

            if (result && old_memory_pointer) {
                // @Incomplete: provide os-level realloc function
                MemoryCopy(result, old_memory_pointer, Min(requested_size, old_size));
                os_free(old_memory_pointer);
            }

            return result;
        }
        
        case AllocatorMode_Free: {
            os_free(old_memory_pointer);
            return NULL;
        }

        case AllocatorMode_FreeAll: {
            // @Incomplete
            return NULL;
        }

        default: {
            return NULL;
        }
    }
}

function Allocator os_allocator() {
    return Allocator{os_allocator_proc, 0};
}

function ALLOCATOR_PROC(arena_allocator_proc)
{
    Arena *arena = cast(Arena *)allocator_data; 

    switch (mode) {
        case AllocatorMode_Alloc:
        {
            arena_align(arena, alignment);
            void *result = arena_push_bytes(arena, requested_size);
            MemoryZero(result, requested_size);
            return result;
        } break;

        case AllocatorMode_Resize:
        {
            arena_align(arena, alignment);
            void *result = arena_resize_ptr(arena, requested_size, old_memory_pointer, old_size);
            return result;
        } break;

        case AllocatorMode_Free:
        {
            arena_free_ptr(arena, old_memory_pointer, old_size);
            return NULL;
        } break;

        case AllocatorMode_FreeAll:
        {
            arena_reset(arena);
            return NULL;
        } break;

        default: {
            return NULL;
        } break;
    }
}

Allocator arena_allocator(Arena *arena) {
    assert(arena);
    return Allocator{arena_allocator_proc, arena};
}

//
// Strings
//

#ifndef PrintToBuffer

#include <stdio.h>
#define PrintToBuffer vsnprintf
#define print na__print

void na__print(const char *format, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    fflush(stdout);
}

#endif // PrintToBuffer

#if OS_WINDOWS
    #define PATH_SEP '\\'
#else
    #define PATH_SEP '/'
#endif


//
// Char Functions
//

function b32 char_is_alpha(u8 c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

function b32 char_is_lower(u8 c) {
    return c >= 'a' && c <= 'z';
}

function b32 char_is_upper(u8 c) {
    return c >= 'A' && c <= 'Z';
}

function b32 char_is_digit(u8 c) {
    return c >= '0' && c <= '9';
}

function b32 char_is_space(u8 c) {
    return c == ' ' || c == '\t' || c == '\f' || c == '\v';
}

function b32 char_is_whitespace(u8 c) {
    return char_is_space(c) || c == '\r' || c == '\n';
}

function b32 char_is_symbol(u8 c)
{
    return (c == '~' || c == '!'  || c == '$' || c == '%' || c == '^' ||
            c == '&' || c == '*'  || c == '-' || c == '=' || c == '+' ||
            c == '<' || c == '.'  || c == '>' || c == '/' || c == '?' ||
            c == '|' || c == '\\' || c == '{' || c == '}' || c == '(' ||
            c == ')' || c == '['  || c == ']' || c == '#' || c == ',' ||
            c == ';'  || c == ':' || c == '@');
}

function u8 char_to_upper(u8 c) {
    return c >= 'a' && c <= 'z' ? c - ('a' - 'A') : c;
}

function u8 char_to_lower(u8 c) {
    return c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c;
}

function u8 char_to_forward_slash(u8 c) {
    return c == '\\' ? '/' : c;
}

//
// Constructors
//

function String string_make(u8 *data, i64 count) {
    String result;
    result.data = data;
    result.count = count;
    return result;
}

function String string_range(u8 *at, u8 *end) {
    assert(end >= at);
    return string_make(at, (end - at));
}

function i64 cstr_length(char *cstr) {
    char *at = cstr;

    // @Speed: this can be made wide
    if (at != NULL) {
        while (*at != 0) {
            at ++;
        }
    }

    return at - cstr;
}

function String string_from_cstr(char *cstr) {
    return string_make((u8 *)cstr, cstr_length(cstr));
};

function char *string_to_cstr(Arena *arena, String str) {
    if (!str.count || !str.data) {
        return NULL;
    }

    char *result = PushArray(arena, char, str.count + 1); // size for null character
    MemoryCopy(result, str.data, str.count);
    result[str.count] = 0;
    return result;
}

function String16 string16_make(u16 *data, i64 count) {
    String16 result;
    result.data = data;
    result.count = count;
    return result;
}

function String16 string16_from_cstr(u16 *data) {
    u16 *at = data;
    while (*at != 0) at += 1;
    String16 result = Str16(data, at - data);
    return result;
}

function String32 string32_make(u32 *data, i64 count) {
    String32 result;
    result.data = data;
    result.count = count;
    return result;
}

//
// Substrings
//

function String string_slice(String str, i64 start_index, i64 end_index) {
    start_index = Clamp(start_index, 0, str.count);
    end_index   = Clamp(end_index, 0, str.count);

    if (start_index > end_index)
    {
        i64 tmp = start_index;
        start_index = end_index;
        end_index = tmp;
    }

    return string_make(str.data + start_index, end_index - start_index);
}

function String string_substr(String str, i64 offset, i64 count)
{
    return string_slice(str, offset, offset + count);
}

function String string_skip(String str, i64 offset) {
    return string_slice(str, offset, str.count);
}

function String string_chop(String str, i64 offset) {
    return string_slice(str, 0, str.count - offset);
}

function String string_prefix(String str, i64 count) {
    return string_slice(str, 0, count);
}

function String string_suffix(String str, i64 count) {
    return string_slice(str, str.count - count, str.count);
}

//
// Matching
//

function bool string_equals(String a, String b)
{
    return a.count == b.count && MemoryEquals(a.data, b.data, a.count);
}

function bool string_match(String a, String b, Match_Flags flags)
{
    bool result = false;

    if (a.count == b.count || ((flags & MatchFlag_RightSideSloppy) && a.count >= b.count))
    {
        result = true;

        for (i64 i = 0; i < b.count; i += 1)
        {
            bool match = a.data[i] == b.data[i];
            if (flags & MatchFlag_CaseInsensitive)
            {
                match |= (char_to_lower(a.data[i]) == char_to_lower(b.data[i]));
            }
            if (flags & MatchFlag_SlashInsensitive)
            {
                match |= (char_to_forward_slash(a.data[i]) == char_to_forward_slash(b.data[i]));
            }

            if (!match)
            {
                result = false;
                break;
            }
        }
    }

    return result;
}

function i64 string_find(String str, String search, i64 start_index, Match_Flags flags)
{
    i64 result = str.count;

    for (i64 i = start_index; i < str.count; i += 1)
    {
        if (i + search.count > str.count) break;

        String substr = string_slice(str, i, i+search.count);
        if (string_match(substr, search, flags))
        {
            result = i;
            if (!(flags & MatchFlag_FindLast))
            {
                break;
            }
        }
    }

    return result;
}

function bool string_starts_with(String str, String prefix) {
    return str.count >= prefix.count && MemoryEquals(str.data, prefix.data, prefix.count);
}

function bool string_ends_with(String str, String postfix) {
    return (
        str.count >= postfix.count &&
        MemoryEquals(str.data + (str.count - postfix.count), postfix.data, postfix.count)
    );
}

function i64 string_index(String str, String search, i64 start_index) {
    i64 result = string_find(str, search, start_index, 0);
    return result < str.count ? result : -1;
}

function i64 string_char_index(String str, u8 search, i64 start_index) {
    return string_index(str, string_make(&search, 1), start_index);
}

function i64 string_last_index(String str, String search) {
    i64 result = string_find(str, search, 0, MatchFlag_FindLast);
    return result < str.count ? result : -1;
}

function bool string_contains(String str, String search) {
    return string_find(str, search, 0, 0) < str.count;
}

//
// Allocation
//

function String string_copy(Arena *arena, String str)
{
    String copy = {0};
    u8 *data = PushArray(arena, u8, str.count);

    if (data) {
        copy = string_make(data, str.count);
        MemoryCopy(copy.data, str.data, str.count);
    }

    return copy;
}

function String string_printv(Arena *arena, char *fmt, va_list args)
{
    String result = {0};
    
    // in case we need to try a second time
    va_list args2;
    va_copy(args2, args);

    i64 buffer_size = 1024;
    u8 *buffer = PushArray(arena, u8, buffer_size);

    if (buffer != NULL)
    {
        // NOTE(nick): print_to_buffer returns size excluding the null terminator
        i64 actual_size = PrintToBuffer((char *)buffer, buffer_size, fmt, args);

        if (actual_size > 0)
        {
            if (actual_size < buffer_size)
            {
                // NOTE(nick): happy path
                arena_pop(arena, buffer_size - actual_size);
                result = string_make(buffer, actual_size);
            }
            else
            {
                arena_pop(arena, buffer_size);
                u8 *fixed_buffer = PushArray(arena, u8, actual_size);

                if (fixed_buffer != NULL)
                {
                    i64 final_size = PrintToBuffer((char *)fixed_buffer, actual_size + 1, fmt, args2);
                    result = string_make(fixed_buffer, final_size);
                }
            }
        }
    }

    va_end(args2);

    return result;
}

function String string_print(Arena *arena, char *fmt, ...)
{
    String result = {0};

    va_list args;
    va_start(args, fmt);
    result = string_printv(arena, fmt, args);
    va_end(args);

    return result;
}

function String string_replace(Arena *arena, String str, String find, String replacer, u64 replace_limit)
{
    String result = str;
    if (replace_limit == 0) replace_limit = U64_MAX;

    u64 match_count = 0;
    u64 replace_count = replace_limit;
    if (replace_count > 0 && find.count > 0)
    {
        i64 index = -1;
        while (index < str.count)
        {
            index = string_find(str, find, index + 1, 0);
            if (index < str.count)
            {
                replace_count -= 1;
                if (replace_count == 0) break;

                match_count += 1;
            }
        }
    }

    if (match_count > 0)
    {
        u64 count = str.count - (replacer.count - find.count) * match_count;
        u8 *data = PushArray(arena, u8, count);
        u8 *at = data;

        i64 index = -1;
        while (index < str.count)
        {
            i64 start_index = Max(index, 0);
            index = string_find(str, find, index + 1, 0);

            MemoryCopy(at, str.data + start_index, (index - start_index));
            at += (index - start_index);

            if (index < str.count)
            {
                MemoryCopy(at, replacer.data, replacer.count);
                at += replacer.count;
                index += find.count;

                if (match_count == 0)
                {
                    MemoryCopy(at, str.data + index, str.count - index);
                    at += str.count - index;
                    break;
                }
                match_count -= 1;
            }
        }

        result = Str8(data, at - data);
    }

    return result;
}

//
// Unicode Conversions
//

function String_Decode string_decode_utf8(u8 *str, u32 capacity) {
    String_Decode result = {'?', 1};

    static u8 utf8_class[] = {
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0,
        2, 2, 2, 2, 3, 3, 4, 0,
    };

    u8 byte = str[0];
    u8 count = utf8_class[byte >> 3];

    if (capacity >= count) {
        switch (count) {
            case 1: {
                result.advance = 1;
                result.codepoint = str[0] & 0x7F;
            } break;

            case 2: {
                if (utf8_class[str[1] >> 3] == 0)
                {
                    result.advance = 2;
                    result.codepoint = ((str[0] & 0x1F) << 6) | (str[1] & 0x3F);
                }
            } break;

            case 3: {
                if (utf8_class[str[1] >> 3] == 0 &&
                    utf8_class[str[2] >> 3] == 0)
                {
                    result.advance = 3;
                    result.codepoint = (
                        ((str[0] & 0x0F) << 12) |
                        ((str[1] & 0x3F) << 6) |
                        ((str[2] & 0x3F))
                    );
                }
            } break;

            case 4: {
                if (utf8_class[str[1] >> 3] == 0 &&
                    utf8_class[str[2] >> 3] == 0 &&
                    utf8_class[str[3] >> 3] == 0)
                {
                    result.advance = 4;
                    result.codepoint = (
                        ((str[0] & 0x07) << 18) |
                        ((str[1] & 0x3F) << 12) |
                        ((str[2] & 0x3F) << 6) |
                        ((str[3] & 0x3F))
                    );
                }
            } break;
        }
    }

    return result;
}

function u32 string_encode_utf8(u8 *dest, u32 codepoint) {
    u32 advance = 0;

    if (codepoint <= 0x7F) {
        advance = 1;
        dest[0] = (u8)codepoint;
    }
    else if (codepoint <= 0x7FF) {
        advance = 2;
        dest[0] = 0xC0 | (codepoint >> 6);
        dest[1] = 0x80 | (codepoint & 0x3F);
    }
    else if (codepoint <= 0xFFFF) {
        advance = 3;
        dest[0] = 0xE0 | (codepoint >> 12);
        dest[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        dest[2] = 0x80 | (codepoint & 0x3F);
    }
    else if (codepoint <= 0x10FFFF) {
        advance = 4;
        dest[0] = 0xF0 | (codepoint >> 18);
        dest[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        dest[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        dest[3] = 0x80 | (codepoint & 0x3F);
    }
    else
    {
        dest[0] = '?';
        advance = 1;
    }

    return advance;
}

function u8 string_seek_right_utf8(u8 *data, u32 capacity) {
    String_Decode decode = string_decode_utf8(data, capacity);
    return Min(decode.advance, capacity);
}

function u8 string_seek_left_utf8(u8 *data, u32 capacity) {
    u8 advance = 0;

    if (capacity >= 1)
    {
        u8 at = *(data - 1);

        if (at < 128)
        {
            advance = 1;
        }
        else if (capacity >= 2)
        {
            advance = 2;
            at = *(data - 2);

            // scan for 10xxxxxx
            if (capacity >= 3 && (at & 0xC0) == 0x80)
            {
                advance += 1;
                at = *(data - 3);

                if (capacity >= 4 && (at & 0xC0) == 0x80)
                {
                    advance += 1;
                }
            }
        }
    }

    return advance;
}

function String_Decode string_decode_utf16(u16 *str, u32 capacity) {
    String_Decode result = {'?', 1};

    u16 x = str[0];

    if (x < 0xD800 || 0xDFFF < x) {
        result.codepoint = x;
        result.advance = 1;
    } else if (capacity >= 2) {
        u16 y = str[1];

        if (0xD800 <= x && x < 0xDC00 && 0xDC00 <= y && y < 0xE000) {
            result.codepoint = (((x - 0xD800) << 10) | (y - 0xDC00)) + 0x10000;
            result.advance = 2;
        }
    }

    return result;
}

function u32 string_encode_utf16(u16 *dest, u32 codepoint)
{
    u32 advance = 1;
    if (codepoint == ~((u32)0))
    {
        dest[0] = (u16)'?';
    }
    else if (codepoint < 0x10000)
    {
        dest[0] = (u16)codepoint;
    }
    else
    {
        u32 v = codepoint - 0x10000;
        dest[0] = 0xD800 + (v >> 10);
        dest[1] = 0xDC00 + (v & 0x3FF);
        advance = 2;
    }
    return advance;
}

function String32 string32_from_string(Arena *arena, String str) {
    u32 *data = PushArray(arena, u32, str.count);

    u32 *at = data;
    u8 *p0 = str.data;
    u8 *p1 = str.data + str.count;

    while (p0 < p1) {
        String_Decode decode = string_decode_utf8(p0, cast(u64)(p1 - p0));

        *at = decode.codepoint;
        p0 += decode.advance;
        at += 1;
    }

    i64 alloc_count = str.count;
    i64 string_count = cast(u64)(at - data);
    i64 unused_count = alloc_count - string_count;

    arena_pop(arena, unused_count * sizeof(u32));

    return string32_make(data, string_count);
}

function String string_from_string32(Arena *arena, String32 str) {
    u8 *data = PushArray(arena, u8, str.count * 4);

    u32 *p0 = str.data;
    u32 *p1 = str.data + str.count;
    u8 *at = data;

    while (p0 < p1) {
        u32 advance = string_encode_utf8(at, *p0);

        p0 += 1;
        at += advance;
    }

    i64 alloc_count = str.count * 4;
    i64 string_count = cast(u64)(at - data);
    i64 unused_count = alloc_count - string_count;

    arena_pop(arena, unused_count);

    return Str8(data, string_count);
}

function String16 string16_from_string(Arena *arena, String str) {
    arena_align(arena, sizeof(u16));
    u16 *data = PushArray(arena, u16, str.count * 2 + 1);

    u16 *at = data;
    u8 *p0 = str.data;
    u8 *p1 = str.data + str.count;

    while (p0 < p1) {
        String_Decode decode = string_decode_utf8(p0, (u64)(p1 - p0));
        u32 encode_size = string_encode_utf16(at, decode.codepoint);

        p0 += decode.advance;
        at += encode_size;
    }

    *at = 0;

    i64 alloc_count = str.count * 2 + 1;
    i64 string_count = cast(u64)(at - data);
    i64 unused_count = alloc_count - string_count - 1;

    arena_pop(arena, unused_count * sizeof(u16));

    return Str16(data, string_count);
}

function String string_from_string16(Arena *arena, String16 str) {
    String result = {0};
    result.data = PushArray(arena, u8, str.count * 3 + 1);

    u16 *p0 = str.data;
    u16 *p1 = str.data + str.count;
    u8 *at = result.data;

    while (p0 < p1) {
        String_Decode decode = string_decode_utf16(p0, cast(u64)(p1 - p0));
        u32 encode_size = string_encode_utf8(at, decode.codepoint);

        p0 += decode.advance;
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
// Conversions
//

function u64 string_to_u64(String string, u32 radix)
{
    assert(2 <= radix && radix <= 16);
    local_persist u8 char_to_value[] =
    {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    };
    u64 value = 0;
    for (u64 i = 0; i < string.count; i += 1) {
        value *= radix;
        u8 c = string.data[i];
        value += char_to_value[(c - 0x30)&0x1F];
    }
    return value;
}

function i64 string_to_i64(String str, u32 base) {
    u64 p = 0;
    
    // consume sign
    i64 sign = +1;
    if (p < str.count) {
        u8 c = str.data[p];
        if (c == '-'){
            sign = -1;
            p += 1;
        }
        else if (c == '+'){
            p += 1;
        }
    }
    
    // radix from prefix
    u64 radix = 10;
    if (p < str.count) {
        u8 c0 = str.data[p];
        if (c0 == '0') {
            p += 1;
            radix = 8;
            if (p < str.count) {
                u8 c1 = str.data[p];
                if (c1 == 'x') {
                    p += 1;
                    radix = 16;
                }
                else if (c1 == 'b') {
                    p += 1;
                    radix = 2;
                }
            }
        }
    }
    
    // consume integer "digits"
    String digits_substr = string_skip(str, p);
    u64 n = string_to_u64(digits_substr, radix);
    
    // combine result
    i64 result = sign*n;
    return result;
}

function f64 string_to_f64(String string) {
    char str[64];
    u64 count = string.count;
    if (count > sizeof(str) - 1)
    {
        count = sizeof(str) - 1;
    }
    MemoryCopy(str, string.data, count);
    str[count] = 0;
    return (atof(str));
}

function b32 string_to_b32(String str) {
    return (
        string_match(str, S("true"), MatchFlag_CaseInsensitive) ||
        string_match(str, S("yes"), MatchFlag_CaseInsensitive) ||
        string_equals(str, S("1"))
    );
}

//
// String Lists
//

function void string_list_push_node(String_List *list, String_Node *n)
{
    QueuePush(list->first, list->last, n);
    list->node_count += 1;
    list->total_size += n->string.count;
}

function void string_list_push(Arena *arena, String_List *list, String str)
{
    String_Node *n = PushArrayZero(arena, String_Node, 1);
    n->string = str;
    string_list_push_node(list, n);
}

function void string_concat_list(String_List *list, String_List *to_push)
{
    if (to_push->first)
    {
        list->node_count += to_push->node_count;
        list->total_size += to_push->total_size;
        if (list->last == 0)
        {
            *list = *to_push;
        }
        else
        {
            list->last->next = to_push->first;
            list->last = to_push->last;
        }
    }
    MemoryZero(to_push, sizeof(*to_push));
}

function String_List string_splits(Arena *arena, String string, int split_count, String *splits)
{
    String_List list = {0};
    
    u64 split_start = 0;
    for (u64 i = 0; i < string.count; i += 1)
    {
        b32 was_split = 0;
        for(int split_idx = 0; split_idx < split_count; split_idx += 1)
        {
            b32 match = 0;
            if(i + splits[split_idx].count <= string.count)
            {
                match = 1;
                for(u64 split_i = 0; split_i < splits[split_idx].count && i + split_i < string.count; split_i += 1)
                {
                    if(splits[split_idx].data[split_i] != string.data[i + split_i])
                    {
                        match = 0;
                        break;
                    }
                }
            }
            if(match)
            {
                String split_string = Str8(string.data + split_start, i - split_start);
                string_list_push(arena, &list, split_string);
                split_start = i + splits[split_idx].count;
                i += splits[split_idx].count - 1;
                was_split = 1;
                break;
            }
        }
        
        if(was_split == 0 && i == string.count - 1)
        {
            String split_string = Str8(string.data + split_start, i+1 - split_start);
            string_list_push(arena, &list, split_string);
            break;
        }
    }
    
    return list;
}

function String_List string_split(Arena *arena, String string, String split)
{
    return string_splits(arena, string, 1, &split);
}

function String string_list_joins(Arena *arena, String_List list, String_Join *optional_params)
{
    String_Join join = {0};
    if (optional_params)
    {
        MemoryCopy(&join, optional_params, sizeof(join));
    }
    
    u64 sep_count = 0;
    if (list.node_count > 1)
    {
        sep_count = list.node_count - 1;
    }
    String result = {0};
    result.count = (list.total_size + join.pre.count +
                   sep_count*join.sep.count + join.post.count);
    result.data = PushArray(arena, u8, result.count);
    
    u8 *ptr = result.data;
    MemoryCopy(ptr, join.pre.data, join.pre.count);
    ptr += join.pre.count;
    for (String_Node *node = list.first; node; node = node->next)
    {
        MemoryCopy(ptr, node->string.data, node->string.count);
        ptr += node->string.count;
        if (node != list.last) {
            MemoryCopy(ptr, join.sep.data, join.sep.count);
            ptr += join.sep.count;
        }
    }
    MemoryCopy(ptr, join.pre.data, join.pre.count);
    ptr += join.pre.count;
    
    return result;
}

function String string_list_join(Arena *arena, String_List list, String join)
{
    String_Join params = {0};
    params.sep = join;
    return string_list_joins(arena, list, &params);
}

function String string_list_print(Arena *arena, String_List *list, char *fmt, ...)
{
    String result = {0};

    va_list args;
    va_start(args, fmt);
    result = string_printv(arena, fmt, args);
    va_end(args);

    string_list_push(arena, list, result);
    return result;
}

function String string_list_to_string(Arena *arena, String_List *list)
{
    return string_list_join(arena, *list, S(""));
}

//
// Misc Helpers
//

function String string_concat2(Arena *arena, String a, String b) {
    u64 count = a.count + b.count;
    u8 *data = PushArray(arena, u8, count);

    if (data) {
        MemoryCopy(data + 0      , a.data, a.count);
        MemoryCopy(data + a.count, b.data, b.count);
    }

    return string_make(data, count);
}

function String string_concat3(Arena *arena, String a, String b, String c) {
    u64 count = a.count + b.count + c.count;
    u8 *data = PushArray(arena, u8, count);

    if (data) {
        MemoryCopy(data + 0                , a.data, a.count);
        MemoryCopy(data + a.count          , b.data, b.count);
        MemoryCopy(data + a.count + b.count, c.data, c.count);
    }

    return string_make(data, count);
}

function String string_concat4(Arena *arena, String a, String b, String c, String d) {
    u64 count = a.count + b.count + c.count + d.count;
    u8 *data = PushArray(arena, u8, count);

    if (data) {
        MemoryCopy(data + 0,                           a.data, a.count);
        MemoryCopy(data + a.count,                     b.data, b.count);
        MemoryCopy(data + a.count + b.count,           c.data, c.count);
        MemoryCopy(data + a.count + b.count + c.count, d.data, d.count);
    }

    return string_make(data, count);
}

function String string_chop_last_period(String string)
{
    u64 pos = string_find(string, S("."), 0, MatchFlag_FindLast);
    if (pos < string.count)
    {
        string.count = pos;
    }
    return string;
}

function String string_skip_last_period(String string)
{
    u64 pos = string_find(string, S("."), 0, MatchFlag_FindLast);
    if (pos < string.count)
    {
        string.data += pos+1;
        string.count -= pos+1;
    }
    return string;
}

function String string_chop_last_slash(String string)
{
    u64 pos = string_find(string, S("/"), 0, MatchFlag_SlashInsensitive|MatchFlag_FindLast);
    if (pos < string.count)
    {
        string.count = pos;
    }
    return string;
}

function String string_skip_last_slash(String string)
{
    u64 pos = string_find(string, S("/"), 0, MatchFlag_SlashInsensitive|MatchFlag_FindLast);
    if (pos < string.count)
    {
        string.data += pos+1;
        string.count -= pos+1;
    }
    return string;
}

function String string_trim_whitespace(String str)
{
    String result = str;
    while (result.count > 0 && char_is_whitespace(result.data[0]))
    {
        result.data  += 1;
        result.count -= 1;
    }

    while (result.count > 0 && char_is_whitespace(result.data[result.count - 1]))
    {
        result.count -= 1;
    }
    return result;
}

//
// Path Helpers
//

function String path_filename(String path)
{
    return string_skip_last_slash(path);
}

function String path_basename(String path)
{
    return string_chop_last_slash(path);
}

function String path_extension(String path)
{
    String result = {0};
    u64 slash_pos = string_find(path, S("/"), 0, MatchFlag_SlashInsensitive|MatchFlag_FindLast);
    if (slash_pos < path.count)
    {
        u64 dot_pos = string_find(path, S("."), slash_pos, MatchFlag_FindLast);
        if (dot_pos < path.count)
        {
            result = string_slice(path, dot_pos, path.count);
        }
    }
    return result;
}

function String path_strip_extension(String path)
{
    String result = {0};
    u64 slash_pos = string_find(path, S("/"), 0, MatchFlag_SlashInsensitive|MatchFlag_FindLast);
    if (slash_pos < path.count)
    {
        u64 dot_pos = string_find(path, S("."), slash_pos, MatchFlag_FindLast);
        if (dot_pos < path.count)
        {
            result = string_slice(path, 0, dot_pos);
        }
    }
    return result;
}

function String path_join2(Arena *arena, String a, String b)
{
    return string_print(arena, "%.*s%c%.*s", LIT(a), PATH_SEP, LIT(b));
}

function String path_join3(Arena *arena, String a, String b, String c)
{
    return string_print(arena, "%.*s%c%.*s%c%.*s", LIT(a), PATH_SEP, LIT(b), PATH_SEP, LIT(c));
}

function String path_join4(Arena *arena, String a, String b, String c, String d)
{
    return string_print(arena, "%.*s%c%.*s%c%.*s%c%.*s", LIT(a), PATH_SEP, LIT(b), PATH_SEP, LIT(c), PATH_SEP, LIT(d));
}

function b32 path_is_absolute(String path)
{
    b32 result = false;
    if (path.count > 0 && path.data[0] == '/')
    {
        result = true;
    }
    else
    {
        // Windows drive root:
        if (path.count >= 2)
        {
            result = char_is_alpha(path.data[0]) && path.data[1] == ':';

            if (path.count > 2)
            {
                result |= (path.data[2] == '\\' || path.data[2] == '/');
            }
        }
    }
    return result;
}

//
// String Conversions
//

#if LANG_CPP

function String to_string(bool x)   { if (x) return S("true"); return S("false"); }
function String to_string(char x)   { return sprint("%c", x); }
function String to_string(char *x)  { return string_from_cstr(x); }
function String to_string(i8 x)     { return sprint("%d", x); }
function String to_string(u8 x)     { return sprint("%d", x); }
function String to_string(i16 x)    { return sprint("%d", x); }
function String to_string(u16 x)    { return sprint("%d", x); }
function String to_string(i32 x)    { return sprint("%d", x); }
function String to_string(u32 x)    { return sprint("%d", x); }
function String to_string(i64 x)    { return sprint("%lld", x); }
function String to_string(u64 x)    { return sprint("%llu", x); }
function String to_string(f32 x)    { return sprint("%f", x); }
function String to_string(f64 x)    { return sprint("%f", x); }
function String to_string(void *x)  { return sprint("%p", x); }
function String to_string(String x) { return x; }

#endif // LANG_CPP

//
// Functions
//

function b32 is_power_of_two(i64 x) {
    return (x & (x - 1)) == 0;
}

// NOTE(nick): if x is already a power of two this function just returns x
function u64 next_power_of_two(u64 x) {
    if (x == 0) return 1;

    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;

    return ++x;
}

function u64 previous_power_of_two(u64 x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return x - (x >> 1);
}

function u16 endian_swap16(u16 i) {
    return (i>>8) | (i<<8);
}

function u32 endian_swap32(u32 i) {
    return (i>>24) |(i<<24) | ((i&0x00ff0000u)>>8)  | ((i&0x0000ff00u)<<8);
}

function u64 endian_swap64(u64 i) {
    return (i>>56) | (i<<56) |
           ((i&0x00ff000000000000ull)>>40) | ((i&0x000000000000ff00ull)<<40) |
           ((i&0x0000ff0000000000ull)>>24) | ((i&0x0000000000ff0000ull)<<24) |
           ((i&0x000000ff00000000ull)>>8)  | ((i&0x00000000ff000000ull)<<8);
}

function u32 rotate_left_u32(u32 value, i32 amount) {
#if COMPILER_MSVC
    u32 result = _rotl(value, amount);
#else
    // TODO(casey): Actually port this to other compiler platforms!
    amount &= 31;
    u32 result = ((value << amount) | (value >> (32 - amount)));
#endif

    return result;
}

function u32 rotate_right_u32(u32 value, i32 amount) {
#if COMPILER_MSVC
    u32 result = _rotr(value, amount);
#else
    // TODO(casey): Actually port this to other compiler platforms!
    amount &= 31;
    u32 result = ((value >> amount) | (value << (32 - amount)));
#endif

    return result;
}

//
// Hashing
//

function u32 murmur32_seed(void const *data, i64 len, u32 seed) {
    u32 const c1 = 0xcc9e2d51;
    u32 const c2 = 0x1b873593;
    u32 const r1 = 15;
    u32 const r2 = 13;
    u32 const m  = 5;
    u32 const n  = 0xe6546b64;

    i64 i, nblocks = len / 4;
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

function u64 murmur64_seed(void const *data_, i64 len, u64 seed) {
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

function u32 murmur32(void const *data, i64 len)
{
    return murmur32_seed(data, len, 0x9747b28c);
}

function u64 murmur64(void const *data, i64 len)
{
    return murmur64_seed(data, len, 0x9747b28c);
}

function u32 fnv32a(void const *data, i64 len) {
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

function u64 fnv64a(void const *data, i64 len) {
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
// Random
//

function Random_State random_make_default()
{
    Random_State result = {0};
    // TODO(nick): seed with os_time
    //random_set_seed();
    return result;
}

function void random_set_seed(Random_State *it, u64 state, u64 selector)
{
    it->state = state;
    it->selector = (selector << 1) | 1;
}

function u32 random_u32(Random_State *it)
{
    u64 state = it->state;
    state = state * 6364136223846793005ULL + it->selector;
    it->state = state;

    u32 pre_rotate = (u32)((state ^ (state >> 18)) >> 27);
    u32 result = rotate_right_u32(pre_rotate, (i32)(state >> 59));

    return result;
}

function f32 random_f32(Random_State *series) 
{
    f32 divisor = 1.0f / (f32)U32_MAX;
    return divisor * (f32)random_u32(series);
}

function f32 random_f32_between(Random_State *series, f32 min, f32 max) {
    return min + (max - min) * random_f32(series);
}

function i32 random_i32_between(Random_State *series, i32 min, i32 max) {
    assert(max >= min);
    return min + (i32)(random_u32(series) % (max - min + 1));
}

function void random_shuffle(Random_State *it, void *base, u64 count, u64 size) {
    u8 *at = cast(u8 *)base + (count-1) * size;
    for (i64 i = count; i > 1; i--)
    {
        i64 j = random_u32(it) % i;
        memory_swap(at, (u8 *)base + j*size, size);
        at -= size;
    }
}

//
// Platform-Specific:
//

#if OS_WINDOWS

#pragma comment(lib, "user32")
#pragma comment(lib, "shell32")

#include <tlhelp32.h>

static Arena *win32_global_arena = NULL;
static SRWLOCK win32_mutex = SRWLOCK_INIT;

struct Win32_File_Lister {
    bool is_first_file;
    HANDLE handle;
    WIN32_FIND_DATAW data;
};

void win32_file_error(File *file, const char *message, String file_name = {}) {
    #if DEBUG
    if (file_name.data) {
        print("[file] %s: %.*s\n", message, LIT(file_name));
    } else {
        print("[file] %s\n", message);
    }
    #endif

    file->has_errors = true;
}

function Date_Time win32_date_time_from_system_time(SYSTEMTIME *in) {
    Date_Time result = {};

    result.year = in->wYear;
    result.mon  = cast(u8)in->wMonth;
    result.day  = in->wDay;
    result.hour = in->wHour;
    result.min  = in->wMinute;
    result.sec  = in->wSecond;
    result.msec = in->wMilliseconds;

    return result;
}

function SYSTEMTIME win32_system_time_from_date_time(Date_Time *in) {
    SYSTEMTIME result = {};

    result.wYear = in->year;
    result.wMonth = in->mon;
    result.wDay = in->day;
    result.wHour = in->hour;
    result.wMinute = in->min;
    result.wSecond = in->sec;
    result.wMilliseconds = in->msec;

    return result;
}

function Dense_Time win32_dense_time_from_file_time(FILETIME *file_time) {
    SYSTEMTIME system_time = {};
    FileTimeToSystemTime(file_time, &system_time);
    Date_Time date_time = win32_date_time_from_system_time(&system_time);
    Dense_Time result = dense_time_from_date_time(date_time);
    return result;
}

function u32 win32_flags_from_attributes(DWORD attributes) {
    u32 result = 0;

    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        result |= File_IsDirectory;
    }

    if (attributes & FILE_ATTRIBUTE_HIDDEN) {
        result |= File_IsHidden;
    }

    if (attributes & FILE_ATTRIBUTE_SYSTEM) {
        result |= File_IsSystem;
    }

    return result;
}

function u32 win32_access_from_attributes(DWORD attributes) {
    u32 result = FileAccess_Read | FileAccess_Execute;

    if (!(attributes & FILE_ATTRIBUTE_READONLY)) {
        result |= FileAccess_Write;
    }

    return result;
}

function b32 W32_ProcessIsRunning(String process_name)
{
    b32 result = false;
    
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(snapshot, &entry))
    {
        while (Process32Next(snapshot, &entry))
        {
            if (string_match(string_from_cstr(entry.szExeFile), process_name, MatchFlag_CaseInsensitive))
            {
                result = true;
                break;
            }
        }
    }

    CloseHandle(snapshot);
    return result;
}

//
// System
//

function LONG win32_crash_handler(EXCEPTION_POINTERS *ExceptionInfo)
{
    print("[win32_crash_handler]");

    #if DEBUG
    os_attach_to_debugger(true);
    #endif

    return EXCEPTION_EXECUTE_HANDLER;
}

function bool os_init()
{
    // NOTE(nick): calling these functions initializes their state
    GetScratch(0, 0);
    os_time();
    os_sleep(0);

    static HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!stdout_handle) {
        AttachConsole(ATTACH_PARENT_PROCESS);
    }

    win32_global_arena = arena_alloc(Gigabytes(1));

    SetUnhandledExceptionFilter(win32_crash_handler);

    return true;
}

function void os_exit(i32 code)
{
    ExitProcess(code);
}

function String os_get_system_path(Arena *arena, SystemPath path)
{
    M_Temp scratch = GetScratch(&arena, 1);
    String result = {};

    switch (path)
    {
        case SystemPath_Current:
        {
            DWORD length = GetCurrentDirectoryW(0, NULL);
            if (length > 0)
            {
                u16 *buffer = PushArray(scratch.arena, u16, length);
                DWORD length_without_null = GetCurrentDirectoryW(length, (WCHAR *)buffer);
                if (length_without_null + 1 == length)
                {
                    result = string_from_string16(arena, Str16(buffer, length_without_null));
                }
            }
        } break;

        case SystemPath_Binary:
        {
            local_persist String name = {0};
            local_persist b32 first = true;
            if (first)
            {
                first = false;
                u64 size = Kilobytes(32);
                u16 *buffer = PushArray(scratch.arena, u16, size);

                DWORD length = GetModuleFileNameW(0, (WCHAR *)buffer, size);
                if (length > 0)
                {
                    name = string_from_string16(arena, Str16(buffer, length));
                    name = string_chop_last_slash(name);

                    AcquireSRWLockExclusive(&win32_mutex);
                    u8 *buffer8 = PushArrayZero(win32_global_arena, u8, name.count);
                    ReleaseSRWLockExclusive(&win32_mutex);
                    MemoryCopy(buffer8, name.data, name.count);
                    name.data = buffer8;
                }
            }

            result = name;
        } break;

        case SystemPath_AppData:
        {
            local_persist String name = {0};
            local_persist b32 first = true;
            if (first)
            {
                first = false;
                u64 size = Kilobytes(32);
                u16 *buffer = PushArray(scratch.arena, u16, size);
                if (SUCCEEDED(SHGetFolderPathW(0, CSIDL_APPDATA, 0, 0, (WCHAR *)buffer)))
                {
                    name = string_from_string16(arena, string16_from_cstr(buffer));

                    AcquireSRWLockExclusive(&win32_mutex);
                    u8 *buffer8 = PushArrayZero(win32_global_arena, u8, name.count);
                    ReleaseSRWLockExclusive(&win32_mutex);
                    MemoryCopy(buffer8, name.data, name.count);
                    name.data = buffer8;
                }
            }

            result = name;
        } break;
    }

    ReleaseScratch(scratch);

    return result;
}

//
// Timing
//

function f64 os_time()
{
    static u64 win32_ticks_per_second = 0;
    static u64 win32_counter_offset = 0;

    if (win32_ticks_per_second == 0)
    {
        LARGE_INTEGER perf_frequency = {};
        if (QueryPerformanceFrequency(&perf_frequency)) {
            win32_ticks_per_second = perf_frequency.QuadPart;
        }
        LARGE_INTEGER perf_counter = {};
        if (QueryPerformanceCounter(&perf_counter)) {
            win32_counter_offset = perf_counter.QuadPart;
        }

        assert(win32_ticks_per_second != 0);
    }

    f64 result = 0;

    LARGE_INTEGER perf_counter;
    if (QueryPerformanceCounter(&perf_counter)) {
        perf_counter.QuadPart -= win32_counter_offset;
        result = (f64)(perf_counter.QuadPart) / win32_ticks_per_second;
    }

    return result;
}

function void os_sleep(f64 seconds)
{
    // @Incomplete: only do this if win32_sleep_is_granular
    // Otherwise do some sort of busy wait thing

    static bool win32_sleep_is_granular = false;
    static bool win32_did_init_sleep = false;

    if (!win32_did_init_sleep)
    {
        HMODULE libwinmm = LoadLibraryA("winmm.dll");
        typedef UINT (WINAPI * timeBeginPeriod_t)(UINT);
        timeBeginPeriod_t timeBeginPeriod = (timeBeginPeriod_t)GetProcAddress(libwinmm, "timeBeginPeriod");
        if (timeBeginPeriod) {
            win32_sleep_is_granular = timeBeginPeriod(1) == 0 /* TIMERR_NOERROR */;
        }

        win32_did_init_sleep = true;
    }


    LARGE_INTEGER ft;
    ft.QuadPart = -(10 * (__int64)(seconds * 1000 * 1000));

    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

function void os_set_high_process_priority(bool enable) {
    if (enable) {
        SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    } else {
        SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    }
}

//
// Memory
//

function u64 os_page_size() {
    static bool win32_got_system_info = false;
    static SYSTEM_INFO win32_system_info;

    if (!win32_got_system_info)
    {
        GetSystemInfo(&win32_system_info);
        win32_got_system_info = true;
    }

    return win32_system_info.dwPageSize;
}

function void *os_reserve(u64 size) {
    return VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
}

function bool os_commit(void *ptr, u64 size) {
    u64 page_snapped_size = size;
    page_snapped_size += os_page_size() - 1;
    page_snapped_size -= page_snapped_size % os_page_size();
    return VirtualAlloc(ptr, page_snapped_size, MEM_COMMIT, PAGE_READWRITE) != NULL;
}

function bool os_decommit(void *ptr, u64 size) {
    return VirtualFree(ptr, size, MEM_DECOMMIT) != NULL;
}

function bool os_release(void *ptr, u64 size) {
    // According to the docs, the size should be 0 when using MEM_RELEASE
    return VirtualFree(ptr, 0, MEM_RELEASE) != NULL;
}

function void *os_alloc(u64 size) {
    // Memory allocated by this function is automatically initialized to zero.
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

function void os_free(void *ptr) {
    if (ptr) {
        VirtualFree(ptr, 0, MEM_RELEASE);
        ptr = 0;
    }
}

//
// Files
//

function String os_read_entire_file(Arena *arena, String path)
{
    String result = {};

    M_Temp scratch = GetScratch(0, 0);
    String16 path_w = string16_from_string(scratch.arena, path);
    HANDLE handle = CreateFileW(cast(WCHAR *)path_w.data, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    ReleaseScratch(scratch);

    if (handle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER file_size;
        GetFileSizeEx(handle, &file_size);
        u64 size = cast(u64)file_size.QuadPart;
        if (size > 0)
        {
            u8 *data = cast(u8 *)arena_push(arena, size);
            result.data  = data;
            result.count = size;

            u8 *ptr = data;
            u8 *end = data + size;

            for (;;)
            {
                u64 bytes_remaining = (u64)(end - ptr);
                DWORD to_read = (DWORD)(ClampTop(bytes_remaining, U32_MAX));
                DWORD bytes_read = 0;
                if (!ReadFile(handle, ptr, to_read, &bytes_read, 0))
                {
                    break;
                }

                ptr += bytes_read;
                if (ptr >= end)
                {
                    break;
                }
            }
        }

        CloseHandle(handle);
    }

    return result;
}

function bool os_write_entire_file(String path, String contents)
{
    bool result = false;

    M_Temp scratch = GetScratch(0, 0);
    String16 path_w = string16_from_string(scratch.arena, path);
    HANDLE handle = CreateFileW(cast(WCHAR *)path_w.data, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
    ReleaseScratch(scratch);

    if (handle != INVALID_HANDLE_VALUE)
    {
        u8 *ptr = contents.data;
        u8 *end = contents.data + contents.count;
        for (;;)
        {
            u64 bytes_remaining = (u64)(end - ptr);
            DWORD to_write = (DWORD)(ClampTop(bytes_remaining, U32_MAX));

            DWORD bytes_written = 0;
            if (!WriteFile(handle, ptr, to_write, &bytes_written, 0)) {
                break;
            }

            ptr += bytes_written;
            if (ptr >= end)
            {
                result = true;
                break;
            }
        }

        CloseHandle(handle);
    }

    return result;
}

function File_Info os_get_file_info(String path) {
    File_Info info = {};

    M_Temp scratch = GetScratch(0, 0);
    String16 str = string16_from_string(scratch.arena, path);

    WIN32_FILE_ATTRIBUTE_DATA data = {};
    if (GetFileAttributesExW(cast(WCHAR *)str.data, GetFileExInfoStandard, &data))
    {
        // @See https://docs.microsoft.com/en-us/windows/win32/api/fileapi/ns-fileapi-win32_file_attribute_data

        info.name             = string_skip_last_slash(path);
        info.size             = ((u64)data.nFileSizeHigh << (u64)32) | (u64)data.nFileSizeLow;
        info.created_at       = win32_dense_time_from_file_time(&data.ftCreationTime);
        info.updated_at       = win32_dense_time_from_file_time(&data.ftLastWriteTime);
        info.last_accessed_at = win32_dense_time_from_file_time(&data.ftLastAccessTime);
        info.flags            = win32_flags_from_attributes(data.dwFileAttributes);
        info.access           = win32_access_from_attributes(data.dwFileAttributes);
    }

    ReleaseScratch(scratch);

    return info;
}

function File os_file_open(String path, u32 mode_flags) {
    File result = {};

    DWORD permissions = 0;
    DWORD creation = 0;

    if (mode_flags & FileMode_Read) {
        permissions |= GENERIC_READ;
        creation = OPEN_EXISTING;
    }

    if (mode_flags & FileMode_Append) {
        permissions |= GENERIC_WRITE;
        creation = OPEN_ALWAYS;
    }

    if (mode_flags & FileMode_Write) {
        permissions |= GENERIC_WRITE;
        creation = CREATE_ALWAYS;
    }

    M_Temp scratch = GetScratch(0, 0);

    String16 path_w = string16_from_string(scratch.arena, path);
    HANDLE handle = CreateFileW(cast(WCHAR *)path_w.data, permissions, FILE_SHARE_READ, 0, creation, 0, 0);
    result.handle = (void *)handle;

    ReleaseScratch(scratch);

    if (handle == INVALID_HANDLE_VALUE) {
        win32_file_error(&result, "Failed to open file", path);
    }

    if ((mode_flags & FileMode_Append) && !(mode_flags & FileMode_Write)) {
        LARGE_INTEGER file_size;
        GetFileSizeEx(handle, &file_size);
        u64 size = cast(u64)file_size.QuadPart;

        result.offset = size;
    }

    return result;
}

function void os_file_read(File *file, u64 offset, u64 size, void *dest) {
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

function void os_file_write(File *file, u64 offset, u64 size, void *data) {
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

function void os_file_close(File *file) {
    if (file->handle) {
        HANDLE handle = (HANDLE)file->handle;
        BOOL success = CloseHandle(handle);
        file->handle = NULL;

        if (!success) {
            win32_file_error(file, "Failed to close file");
        }
    }
}

function bool os_file_rename(String from, String to) {
    M_Temp scratch = GetScratch(0, 0);

    String16 from16 = string16_from_string(scratch.arena, from);
    String16 to16   = string16_from_string(scratch.arena, to);

    BOOL result = MoveFileW((WCHAR *)from16.data, (WCHAR *)to16.data);
    ReleaseScratch(scratch);
    return result;
}

function bool os_delete_file(String path) {
    M_Temp scratch = GetScratch(0, 0);
    String16 str = string16_from_string(scratch.arena, path);
    BOOL success = DeleteFileW(cast(WCHAR *)str.data);
    ReleaseScratch(scratch);

    return success;
}

function bool os_make_directory(String path) {
    M_Temp scratch = GetScratch(0, 0);
    String16 str = string16_from_string(scratch.arena, path);
    BOOL success = CreateDirectoryW(cast(WCHAR *)str.data, NULL);
    ReleaseScratch(scratch);

    return success;
}

function bool os_delete_directory(String path) {
    M_Temp scratch = GetScratch(0, 0);
    String16 str = string16_from_string(scratch.arena, path);
    BOOL success = RemoveDirectoryW(cast(WCHAR *)str.data);
    ReleaseScratch(scratch);

    return success;
}

//
// File Lister
//

function File_Lister *os_file_list_begin(Arena *arena, String path) {
    M_Temp scratch = GetScratch(&arena, 1);
    String16 path16 = string16_from_string(scratch.arena, path);

    Win32_File_Lister *it = PushStructZero(arena, Win32_File_Lister);
    it->is_first_file = true;
    it->handle        = 0;

    it->handle = FindFirstFileW((WCHAR*)path16.data, &it->data);
    ReleaseScratch(scratch);

    return (File_Lister *)it;
}

function bool os_file_list_next(Arena *arena, File_Lister *it, File_Info *info) {
    bool should_continue = true;

    Win32_File_Lister *iter = (Win32_File_Lister *)it;

    if (iter->is_first_file) {
        iter->is_first_file = false;
    } else {
        should_continue = FindNextFileW(iter->handle, &iter->data);
    }

    if (iter->handle != INVALID_HANDLE_VALUE) {
        String16 name16 = string16_from_cstr((u16 *)iter->data.cFileName);

        WIN32_FIND_DATAW data = iter->data;

        MemoryZeroStruct(info);
        info->name             = string_from_string16(arena, name16);
        info->size             = ((u64)data.nFileSizeHigh << (u64)32) | (u64)data.nFileSizeLow;
        info->created_at       = win32_dense_time_from_file_time(&data.ftCreationTime);
        info->updated_at       = win32_dense_time_from_file_time(&data.ftLastWriteTime);
        info->last_accessed_at = win32_dense_time_from_file_time(&data.ftLastAccessTime);
        info->flags            = win32_flags_from_attributes(data.dwFileAttributes);
        info->access           = win32_access_from_attributes(data.dwFileAttributes);
    } else {
        should_continue = false;
    }

    return should_continue;
}

function void os_file_list_end(File_Lister *iter) {
    Win32_File_Lister *it = (Win32_File_Lister *)iter;
    if (it->handle) {
        FindClose(it->handle);
        it->handle = 0;
    }
}

//
// Clipboard
//

function String os_get_clipboard_text() {
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

    String16 str16 = string16_from_cstr((u16 *)buffer);
    String result = string_from_string16(temp_arena(), str16);

    GlobalUnlock(handle);
    CloseClipboard();

    return result;
}

bool os_set_clipboard_text(String str) {
    M_Temp scratch = GetScratch(0, 0);

    char *cstr = string_to_cstr(scratch.arena, str);
    int count = MultiByteToWideChar(CP_UTF8, 0, cstr, -1, NULL, 0);

    if (!count) {
        ReleaseScratch(scratch);
        return false;
    }

    HANDLE handle = GlobalAlloc(GMEM_MOVEABLE, count * sizeof(WCHAR));
    if (!handle) {
        print("[clipboard] Failed to allocate global handle for clipboard.");
        ReleaseScratch(scratch);
        return false;
    }

    WCHAR *buffer = (WCHAR *)GlobalLock(handle);
    if (!buffer) {
        print("[clipboard] Failed to lock global handle.");
        GlobalFree(handle);
        ReleaseScratch(scratch);
        return false;
    }

    MultiByteToWideChar(CP_UTF8, 0, cstr, -1, buffer, count);
    GlobalUnlock(handle);

    if (!OpenClipboard(NULL)) {
        print("[clipboard] Failed to open clipboard.");
        GlobalFree(handle);
        ReleaseScratch(scratch);
        return false;
    }

    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, handle);
    CloseClipboard();

    GlobalFree(handle);
    ReleaseScratch(scratch);

    return true;
}

//
// Dates
//

function Date_Time os_get_current_time_in_utc() {
    SYSTEMTIME st;
    GetSystemTime(&st);
    return win32_date_time_from_system_time(&st);
}

function Date_Time os_get_local_time() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    return win32_date_time_from_system_time(&st);
}

//
// Library
//

function OS_Library os_library_load(String path) {
    M_Temp scratch = GetScratch(0, 0);
    String16 path16 = string16_from_string(scratch.arena, path);

    OS_Library result = {};
    result.handle     = LoadLibraryW((WCHAR *)path16.data);

    ReleaseScratch(scratch);
    return result;
}

function void os_library_unload(OS_Library lib) {
    if (lib.handle) {
        FreeLibrary((HMODULE)lib.handle);
        lib.handle = 0;
    }
}

function void *os_library_get_proc(OS_Library lib, char *proc_name) {
    return (void *)GetProcAddress((HMODULE)lib.handle, proc_name);
}

//
// Shell
//

typedef HINSTANCE (WINAPI * ShellExecuteW_t)(
    HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd);

static ShellExecuteW_t _ShellExecuteW = NULL;

function bool os_shell_open(String path) {
    if (_ShellExecuteW == NULL)
    {
        HMODULE libshell32 = LoadLibraryA("shell32.dll");
        _ShellExecuteW = (ShellExecuteW_t) GetProcAddress(libshell32, "ShellExecuteW");

        if (!_ShellExecuteW)
        {
            return false;
        }
    }

    M_Temp scratch = GetScratch(0, 0);
    String16 path_w = string16_from_string(scratch.arena, path);
    // If the function succeeds, it returns a value greater than 32.
    bool success = (INT_PTR)_ShellExecuteW(0, 0, (WCHAR *)path_w.data, 0, 0 , SW_SHOW) > 32;
    ReleaseScratch(scratch);

    return success;
}

function bool os_shell_execute(String cmd, String arguments, bool admin) {
    if (_ShellExecuteW == NULL)
    {
        HMODULE libshell32 = LoadLibraryA("shell32.dll");
        _ShellExecuteW = (ShellExecuteW_t) GetProcAddress(libshell32, "ShellExecuteW");

        if (!_ShellExecuteW)
        {
            return false;
        }
    }

    M_Temp scratch = GetScratch(0, 0);
    String16 cmd_w = string16_from_string(scratch.arena, cmd);
    String16 arguments_w = string16_from_string(scratch.arena, arguments);

    WCHAR *verb = admin ? L"runas" : L"open";

    // If the function succeeds, it returns a value greater than 32.
    bool success = (INT_PTR)_ShellExecuteW(0, verb, (WCHAR *)cmd_w.data, (WCHAR *)arguments_w.data, 0, SW_HIDE) > 32;
    ReleaseScratch(scratch);

    return success;
}

//
// Misc
//

function void os_get_entropy(void *data, u64 size)
{
    static BOOL (*RtlGenRandom)(VOID *RandomBuffer, ULONG RandomBufferLength);
    if (RtlGenRandom == NULL)
    {
        HMODULE advapi_dll = LoadLibraryA("advapi32.dll");
        if (advapi_dll)
        {
            *(FARPROC *)&RtlGenRandom = GetProcAddress(advapi_dll, "SystemFunction036");
        }

        if (!RtlGenRandom)
        {
            return;
        }
    }

    RtlGenRandom(data, size);
}

function f64 os_caret_blink_time()
{
    f32 seconds = GetCaretBlinkTime() / 1000.f;
    return seconds;
}

//
// Debug
//

function void os_open_file_in_debugger(String path, int line)
{
    if (W32_ProcessIsRunning(S("remedybg.exe")))
    {
        STARTUPINFO startup_info = {0};
        PROCESS_INFORMATION process_info = {0};
        startup_info.cb = sizeof(startup_info);
        char cmd_line[4096] = {0};
        snprintf(cmd_line, sizeof(cmd_line), "remedybg.exe open-file %.*s %i", LIT(path), line);
        CreateProcessA(0, cmd_line, 0, 0, 0, DETACHED_PROCESS, 0, 0, &startup_info, &process_info);
    }
}

function void os_attach_to_debugger(b32 pause)
{
    if (W32_ProcessIsRunning(S("remedybg.exe")))
    {
        STARTUPINFO startup_info = {0};
        PROCESS_INFORMATION process_info = {0};
        startup_info.cb = sizeof(startup_info);

        u32 pid = GetCurrentProcessId();
        String break_str = {0};
        if (!pause) break_str = S("--continue");

        char cmd_line[4096] = {0};
        snprintf(cmd_line, sizeof(cmd_line), "remedybg.exe attach-to-process-by-id %d %.*s", pid, LIT(break_str));
        CreateProcessA(0, cmd_line, 0, 0, 0, DETACHED_PROCESS, 0, 0, &startup_info, &process_info);
    }
}

//
// Threads
//

struct Win32_Thread_Params {
    Thread_Proc *proc;
    void *data;
};

//
// Basic
//

function u32 thread_get_id() {
    u8 *ThreadLocalStorage = (u8 *)__readgsqword(0x30);
    u32 result = *(u32 *)(ThreadLocalStorage + 0x48);
    return result;
}

function u32 atomic_compare_exchange_u32(u32 volatile *value, u32 New, u32 Expected) {
    u32 result = _InterlockedCompareExchange((long volatile *)value, New, Expected);
    return (result);
}

function u64 atomic_exchange_u64(u64 volatile *value, u64 New) {
    u64 result = _InterlockedExchange64((__int64 volatile *)value, New);
    return (result);
}

function u64 atomic_add_u64(u64 volatile *value, u64 Addend) {
    // NOTE(casey): Returns the original value _prior_ to adding
    u64 result = _InterlockedExchangeAdd64((__int64 volatile *)value, Addend);
    return (result);
}

//
// Threads
//

DWORD WINAPI win32_thread_proc(LPVOID lpParameter) {
    Win32_Thread_Params *params = (Win32_Thread_Params *)lpParameter;

    // NOTE(nick): initialize scratch memory
    GetScratch(0, 0);

    assert(params->proc);
    u32 result = params->proc(params->data);

    os_free(params);

    return result;
}

function Thread thread_create(Thread_Proc *proc, void *data, u64 copy_size) {
    Win32_Thread_Params *params = (Win32_Thread_Params *)os_alloc(sizeof(Win32_Thread_Params) + copy_size);
    params->proc = proc;
    params->data = data;
    if (copy_size)
    {
        params->data = params + sizeof(Win32_Thread_Params);
        MemoryCopy(params->data, data, copy_size);
    }

    DWORD thread_id;
    HANDLE handle = CreateThread(0, 0, win32_thread_proc, params, 0, &thread_id);

    Thread result = {};
    result.handle = handle;
    result.id = thread_id;
    return result;
}

function void thread_pause(Thread thread) {
    HANDLE handle = (HANDLE)thread.handle;
    SuspendThread(handle);
}

function void thread_resume(Thread thread) {
    HANDLE handle = (HANDLE)thread.handle;
    ResumeThread(handle);
}

function void thread_detach(Thread thread) {
    HANDLE handle = (HANDLE)thread.handle;
    CloseHandle(handle);
}

function u32 thread_await(Thread thread) {
    HANDLE handle = (HANDLE)thread.handle;
    WaitForSingleObject(handle, INFINITE);
    DWORD result;
    GetExitCodeThread(handle, &result);
    return result;
}

//
// Data Structures
//

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

    // TODO(nick): this is only 40 bytes, should we just bake this into the Mutex itself?
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
    if (mutex->handle)
    {
        DeleteCriticalSection(cast(LPCRITICAL_SECTION)mutex->handle);
        os_free(mutex->handle);
        mutex->handle = 0;
    }
}

#elif OS_MACOS
#include <mach/clock.h>
#include <mach/mach_time.h>
#include <mach/mach_host.h>
#include <mach/mach_port.h>
#include <mach-o/dyld.h>

#include <time.h>
#include <unistd.h>

#include <pthread.h>
#include <stdlib.h>

#define PATH_MAX 2048

static pthread_key_t macos_thread_local_key;

//
// System
//

function bool os_init() {
    // NOTE(nick): calling these functions initializes their state
    GetScratch(0, 0);
    os_time();

    pthread_key_create(&macos_thread_local_key, NULL);

    return true;
}

function void os_exit(i32 code)
{
    exit(code);
}

function String os_get_system_path(Arena *arena, SystemPath path)
{
    String result = {};

    switch (path)
    {
        case SystemPath_Current:
        {
            char *buffer = (char *)arena_push(arena, PATH_MAX);
            getcwd(buffer, PATH_MAX);

            result = string_from_cstr(buffer);
            i64 unused_size = PATH_MAX - result.count;
            arena_pop(arena, unused_size);
        } break;

        case SystemPath_Binary:
        {
            u32 length = 0;
            _NSGetExecutablePath(0, &length);

            Arena *arena = temp_arena();
            char *buffer = (char *)arena_push(arena, length);

            if (_NSGetExecutablePath(buffer, &length) >= 0)
            {
                u64 alloc_size = Max(length, PATH_MAX);

                char *normalized = (char *)arena_push(arena, alloc_size);
                if (realpath(buffer, normalized) != NULL)
                {
                    result = Str8(normalized, length);
                    i64 unused_size = alloc_size - result.count;
                    arena_pop(arena, unused_size);
                }
                else
                {
                    result = Str8(buffer, length);
                }

                result = string_chop_last_slash(result);
            }
        } break;

        case SystemPath_AppData:
        {
            char *home_str = getenv("HOME");
            String home = string_from_cstr(home_str);
            result = string_concat2(arena, home, S("/Library/Application Support/"));
        } break;
    }

    return result;
}

//
// Timing
//

function f64 os_time() {
    static f64 macos_perf_frequency = 0;
    static f64 macos_perf_counter = 0;

    if (macos_perf_frequency == 0)
    {
        mach_timebase_info_data_t rate_nsec;
        mach_timebase_info(&rate_nsec);

        macos_perf_frequency = 1000000000LL * rate_nsec.numer / rate_nsec.denom;
        macos_perf_counter = mach_absolute_time();
    }

    f64 now = mach_absolute_time();
    return (now - macos_perf_counter) / macos_perf_frequency;
}

function void os_sleep(f64 seconds) {
    u32 ms = (u32)(seconds * 1000);
    struct timespec req = {(time_t)ms / 1000, (long)((ms % 1000) * 1000000)};
    struct timespec rem = {0, 0};
    nanosleep(&req, &rem);
}

//
// Clipboard
//

#include <objc/objc.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <objc/NSObjCRuntime.h>

struct NSString;
struct NSURL;

typedef NSString * NSPasteboardType;
import NSPasteboardType const NSPasteboardTypeString; // Available MacOS 10.6

#define objc_msgSend_id ((id (*)(id, SEL))objc_msgSend)
#define objc_method(ReturnType, ...) ((ReturnType (*)(__VA_ARGS__))objc_msgSend)


function String os_get_clipboard_text()
{
    // NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    id pasteboard = objc_msgSend_id((id)objc_getClass("NSPasteboard"), sel_registerName("generalPasteboard"));
    // NSString *string = [pasteboard stringForType:NSPasteboardTypeString];
    id string = objc_method(id, id, SEL, id)(pasteboard, sel_registerName("stringForType:"), (id)NSPasteboardTypeString);
    // char *text = [string UTF8String];
    char *text = objc_method(char*, id, SEL)(string, sel_registerName("UTF8String"));

    auto result = string_copy(temp_arena(), string_from_cstr(text));
    return result;
}

function bool os_set_clipboard_text(String text)
{
    M_Temp scratch = GetScratch(0, 0);
    char *str = string_to_cstr(scratch.arena, text);

    // [NSString stringWithUTF8String: str];
    id string = objc_method(id, id, SEL, const char *)((id)objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"), str);
    // NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    id pasteboard = objc_msgSend_id((id)objc_getClass("NSPasteboard"), sel_registerName("generalPasteboard"));
    // [pasteboard clearContents];
    objc_msgSend(pasteboard, sel_registerName("clearContents"));
    // [pasteboard setString:str forType:NSPasteboardTypeString];
    BOOL result = objc_method(BOOL, id, SEL, id, id)(pasteboard, sel_registerName("setString:forType:"), string, (id)NSPasteboardTypeString);

    ReleaseScratch(scratch);
    return result == YES;
}

//
// Library
//

#include <dlfcn.h>

function OS_Library os_library_load(String path) {
    M_Temp scratch = GetScratch(0, 0);

    OS_Library result = {};
    // TODO(bill): Should this be RTLD_LOCAL?
    result.handle = dlopen(string_to_cstr(scratch.arena, path), RTLD_LAZY | RTLD_GLOBAL);

    ReleaseScratch(scratch);
    return result;
}

function void os_library_unload(OS_Library lib) {
    if (lib.handle) {
        dlclose(lib.handle);
        lib.handle = 0;
    }
}

function void *os_library_get_proc(OS_Library lib, char *proc_name) {
    return (void *)dlsym(lib.handle, proc_name);
}

//
// Shell
//

function bool os_shell_open(String path)
{
    M_Temp scratch = GetScratch(0, 0);
    // [NSWorkspace sharedWorkspace]
    id workspace = objc_method(id, id, SEL)(cast(id)objc_getClass("NSWorkspace"), sel_registerName("sharedWorkspace"));

    /*
    NSString* folder = @"/path/to/folder"  
    [[NSWorkspace sharedWorkspace] openFile:folder withApplication:@"Finder"];

    NSArray *fileURLs = [NSArray arrayWithObjects:fileURL1, nil];
    [[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:fileURLs];

    [[NSWorkspace sharedWorkspace] selectFile:fullPathString inFileViewerRootedAtPath:pathString];
    */

    char *str = string_to_cstr(scratch.arena, path);

    // [NSString stringWithUTF8String: str];
    id string = objc_method(id, id, SEL, const char *)((id)objc_getClass("NSString"), sel_registerName("stringWithUTF8String:"), str);
    // NSURL *fileURL = [NSURL fileURLWithPath:@"/path/to/user/"];
    id fileURL = objc_method(id, id, SEL, id)(cast(id)objc_getClass("NSURL"), sel_registerName("fileURLWithPath:"), string);

    BOOL result = objc_method(BOOL, id, SEL, id)(workspace, sel_registerName("openURL:"), fileURL);

    ReleaseScratch(scratch);
    return result == YES;
}

//
// Threading Primitives
//

#include <mach/mach.h>
#include <mach/semaphore.h>
#include <mach/task.h>
#define SYNC_POLICY_PREPOST 0x4

// Make sure this fits into the handle pointer!
StaticAssert(sizeof(semaphore_t) <= sizeof(void *), "check_semaphore_size");

function Semaphore semaphore_create(u32 max_count) {
    Semaphore result = {};

    mach_port_t self = mach_task_self();

    semaphore_t *handle = cast(semaphore_t *)os_alloc(sizeof(semaphore_t)); // @Memory @Cleanup
    result.handle = handle;

    kern_return_t ret = semaphore_create(self, handle, SYNC_POLICY_PREPOST, 1);
    assert(ret == KERN_SUCCESS);

    return result;
}

function void semaphore_signal(Semaphore *sem) {
    auto handle = cast(semaphore_t *)sem->handle;
    kern_return_t ret = semaphore_signal(*handle);
    assert(ret == KERN_SUCCESS);
}

function void semaphore_wait_for(Semaphore *sem, bool infinite) {
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

function void semaphore_destroy(Semaphore *sem) {
    mach_port_t self = mach_task_self();
    auto handle = cast(semaphore_t *)sem->handle;
    semaphore_destroy(self, *handle);
    os_free(handle); // @Memory @Cleanup
    handle = 0;
}

function Mutex mutex_create(u32 spin_count) {
    Mutex result = {};

    result.handle = os_alloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(cast(pthread_mutex_t *)result.handle, NULL);

    // @Incomplete: should spin_count do anything?

    return result;
}

function void mutex_aquire_lock(Mutex *mutex) {
    pthread_mutex_lock(cast(pthread_mutex_t *)mutex->handle);
}

function bool mutex_try_aquire_lock(Mutex *mutex) {
    return pthread_mutex_trylock(cast(pthread_mutex_t *)mutex->handle) != 0;
}

function void mutex_release_lock(Mutex *mutex) {
    pthread_mutex_unlock(cast(pthread_mutex_t *)mutex->handle);
}

function void mutex_destroy(Mutex *mutex) {
    // @Robustness: track if it's been released before deleting?
    pthread_mutex_destroy(cast(pthread_mutex_t *)mutex->handle);
    os_free(mutex->handle);
    mutex->handle = 0;
}

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

#include <sys/mman.h>

struct Unix_File_Lister {
    char *find_path;
    DIR *handle;
};

//
// Timing
//

function void os_set_high_process_priority(bool enable) {
    if (enable) {
        setpriority(PRIO_PROCESS, getpid(), -20);
    } else {
        setpriority(PRIO_PROCESS, getpid(), 0);
    }
}

//
// Memory
//

function u64 os_page_size() {
    int result = sysconf(_SC_PAGE_SIZE);
    return result;
}

function void *os_reserve(u64 size) {
    void *result = 0;

    result = mmap(NULL, size, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    //msync(result, size, MS_SYNC | MS_INVALIDATE);

    return result;
}

function bool os_commit(void *ptr, u64 size) {
    u64 page_size = os_page_size();

    i64 p = (i64)ptr;
    i64 p_aligned = AlignDownPow2(p, page_size);

    if (p != p_aligned) {
        i64 delta = p - p_aligned;
        ptr = (void *)((u8 *)ptr - delta);
        size += delta;
    }

    size = AlignUpPow2(size, page_size);

    // NOTE(nick): ptr must be aligned to a page boundary.
    int result = mprotect(ptr, size, PROT_READ | PROT_WRITE);
    madvise(ptr, size, MADV_WILLNEED);
    return result == 0;
}

function bool os_decommit(void *ptr, u64 size) {
    u64 page_size = os_page_size();

    i64 p = (i64)ptr;
    i64 p_aligned = AlignDownPow2(p, page_size);

    if (p != p_aligned) {
        i64 delta = p - p_aligned;
        ptr = (void *)((u8 *)ptr - delta);
        size += delta;
    }

    size = AlignUpPow2(size, page_size);

    // NOTE(nick): ptr must be aligned to a page boundary.
    int result = mprotect(ptr, size, PROT_NONE);
    //madvise(ptr, size, MADV_DONTNEED); // is this too harsh?
    return result == 0;
}

function bool os_release(void *ptr, u64 size) {
    return munmap(ptr, size) == 0;
}

function void *os_alloc(u64 size) {
    return calloc(size, 1);
}

function void os_free(void *memory) {
    if (memory) {
        free(memory);
        memory = 0;
    }
}

//
// File
//

function bool os_file_rename(String from, String to) {
    M_Temp scratch = GetScratch(0, 0);

    char *from_cstr = string_to_cstr(scratch.arena, from);
    char *to_cstr   = string_to_cstr(scratch.arena, to);

    bool result = rename(from_cstr, to_cstr) == 0;
    ReleaseScratch(scratch);
    return result;
}

u64 unix_date_from_time(time_t time) {
    // @Incomplete
    return cast(u64)time;
}

u32 unix_flags_from_mode(mode_t mode, String name) {
    u32 result = 0;

    if (S_ISDIR(mode)) {
        result |= File_IsDirectory;
    }

    if (string_starts_with(name, S("."))) {
        result |= File_IsHidden;
    }

    // @Incomplete
    #if 0
    if (attributes & FILE_ATTRIBUTE_SYSTEM) {
        result |= File_IsSystem;
    }
    #endif

    return result;
}

u32 unix_access_from_mode(mode_t mode) {
    u32 result = FileAccess_Read | FileAccess_Execute;

    // @Incomplete
    #if 0
    if (!(attributes & FILE_ATTRIBUTE_READONLY)) {
        result |= FileAccess_Write;
    }
    #endif

    return result;
}

function File_Info os_get_file_info(Arena *arena, String path) {
    M_Temp scratch = GetScratch(&arena, 1);

    char *cpath = string_to_cstr(scratch.arena, path);

    struct stat64 stat_info;
    bool file_exists = stat64(cpath, &stat_info) == 0;

    File_Info info = {};

    if (file_exists)
    {
        info.name             = path_filename(path);
        info.last_accessed_at = unix_date_from_time(stat_info.st_atime);
        info.updated_at       = unix_date_from_time(stat_info.st_mtime);
         // NOTE(nick): not really created time, but UNIX doesn't have this concept
        info.created_at       = unix_date_from_time(stat_info.st_ctime);
        info.flags            = unix_flags_from_mode(stat_info.st_mode, info.name);
        info.access           = unix_access_from_mode(stat_info.st_mode);
    }

    return info;
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

function File os_file_open(String path, File_Mode mode_flags) {
    File result = {};

    char mode[4] = {};
    if ((mode_flags & FileMode_Read) && (mode_flags & FileMode_Write)) {
        mode[0] = 'r';
        mode[1] = 'b';
        mode[2] = '+';
    } else if (mode_flags & FileMode_Append) {
        if (mode_flags & FileMode_Write) {
            mode[0] = 'w';
            mode[1] = 'b';
            mode[2] = '+';
        } else {
            mode[0] = 'a';
            mode[1] = 'b';
            if (mode_flags & FileMode_Read) {
                mode[2] = '+';
            }
        }
    } else if (mode_flags & FileMode_Write) {
        mode[0] = 'w';
        mode[1] = 'b';
    } else {
        mode[0] = 'r';
        mode[1] = 'b';
    }

    M_Temp scratch = GetScratch(0, 0);

    FILE *f = fopen(string_to_cstr(scratch.arena, path), mode);
    result.handle = f;

    ReleaseScratch(scratch);

    if (!f) {
        unix_file_error(&result, "Failed to open file", path);
        return result;
    }

    if ((mode_flags & FileMode_Append) && !(mode_flags & FileMode_Write)) {
        fseek(f, 0, SEEK_END);
        i32 size = ftell(f);
        if (size != -1) {
            result.offset = size;
        }
    }

    return result;
}

function void os_file_read(File *file, u64 offset, u64 size, void *dest) {
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

function void os_file_write(File *file, u64 offset, u64 size, void *data) {
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

function void os_file_close(File *file) {
    if (file->handle) {
        FILE *f = (FILE *)file->handle;
        file->handle = NULL;

        int close_result = fclose(f);
        if (close_result != 0) { unix_file_error(file, "Failed to close file"); }
    }
}

function String os_read_entire_file(Arena *arena, String path) {
    File file = os_file_open(path, FileMode_Read);

    u64 size = 0;
    {
        FILE *f = (FILE *)file.handle;
        u64 prev_position = ftell(f);
        fseek(f, 0, SEEK_END);
        u64 size = ftell(f);
        fseek(f, prev_position, SEEK_SET);
    }

    String result = {};
    result.data = cast(u8 *)arena_push(arena, size);
    result.count = size;

    os_file_read(&file, 0, size, result.data);
    os_file_close(&file);

    return result;
}

function bool os_write_entire_file(String path, String contents) {
    File file = os_file_open(path, FileMode_Write);
    if (!file.has_errors)
    {
        os_file_write(&file, 0, contents.count, contents.data);
    }
    os_file_close(&file);
    return !file.has_errors;
}

//
// File Lister
//

function File_Lister *os_file_list_begin(Arena *arena, String path) {
    M_Temp scratch = GetScratch(&arena, 1);

    char *cpath = string_to_cstr(scratch.arena, path);
    DIR *handle = opendir(cpath);

    Unix_File_Lister *it = PushStructZero(arena, Unix_File_Lister);
    it->find_path = cpath;
    it->handle    = handle;

    ReleaseScratch(scratch);

    return (File_Lister *)it;
}

function bool os_file_list_next(Arena *arena, File_Lister *iter, File_Info *info) {
    Unix_File_Lister *it = (Unix_File_Lister *)iter;

    if (!it->handle) {
        return false;
    }

    struct dirent *data = readdir(it->handle);

    if (data != NULL)
    {
        char buffer[PATH_MAX + 1];
        if (realpath(data->d_name, buffer) != NULL)
        {
            *info = os_get_file_info(arena, string_from_cstr(buffer));
        }
    }

    return data != NULL;
}

function void os_file_list_end(File_Lister *iter) {
    Unix_File_Lister *it = (Unix_File_Lister *)iter;
    if (it->handle) {
        closedir(it->handle);
        it->handle = 0;
    }
}

function bool os_make_directory(String path) {
    M_Temp scratch = GetScratch(0, 0);
    int result = mkdir(string_to_cstr(scratch.arena, path), 0700);
    ReleaseScratch(scratch);
    return result == 0;
}

function bool os_delete_file(String path) {
    M_Temp scratch = GetScratch(0, 0);
    int result = unlink(string_to_cstr(scratch.arena, path));
    ReleaseScratch(scratch);
    return result == 0;
}

function bool os_delete_directory(String path) {
    M_Temp scratch = GetScratch(0, 0);
    int result = rmdir(string_to_cstr(scratch.arena, path));
    ReleaseScratch(scratch);
    return result == 0;
}

//
// Threads
//

#include <pthread.h>
#include <sys/resource.h> // setpriority

struct Unix_Thread_Params {
    Thread_Proc *proc;
    void *data;
};

void *unix_thread_proc(void *data) {
    Unix_Thread_Params *params = (Unix_Thread_Params *)data;

    // NOTE(nick): initialize scratch memory
    GetScratch(0, 0);

    assert(params->proc);
    u32 result = params->proc(params->data);

    os_free(params);

    return (void *)result;
}

function Thread thread_create(Thread_Proc *proc, void *data, u64 copy_size) {
    Unix_Thread_Params *params = (Unix_Thread_Params *)os_alloc(sizeof(Unix_Thread_Params) + copy_size);
    params->proc = proc;
    params->data = data;
    if (copy_size)
    {
        params->data = params + sizeof(Unix_Thread_Params);
        MemoryCopy(params->data, data, copy_size);
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, unix_thread_proc, params);

    Thread result = {};
    result.handle = (void *)thread_id;
    return result;
}

function void thread_detach(Thread thread) {
    pthread_t tid = (pthread_t)thread.handle;
    pthread_detach(tid);
}

function u32 thread_await(Thread thread) {
    pthread_t tid = (pthread_t)thread.handle;
    void *result = 0;
    pthread_join(tid, &result);
    return *(u32 *)result;
}

#elif OS_LINUX
    #error OS_LINUX Not implemented
#endif

//
// Shared:
//

function f64 os_time_in_miliseconds()
{
    return os_time() * 1000.0;
}

function u64 os_clock_cycles(void)
{
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

function bool os_library_is_loaded(OS_Library lib)
{
    return lib.handle != 0;
}

function bool os_file_exists(String path)
{
    return os_get_file_info(path).size > 0;
}

function void os_file_append(File *file, String data)
{
    os_file_write(file, file->offset, data.count, data.data);
    file->offset += data.count;
}

function void os_file_print(File *file, char *fmt, ...)
{
    M_Temp scratch = GetScratch(0, 0);

    va_list args;
    va_start(args, fmt);
    String result = string_printv(scratch.arena, fmt, args);
    va_end(args);

    os_file_append(file, result);

    ReleaseScratch(scratch);
}

function Dense_Time dense_time_from_date_time(Date_Time in) {
    u32 year_encoded = (u32)((i32)in.year + 0x8000);
    Dense_Time result = 0;

    result += year_encoded;
    result *= 12;
    result += (in.mon - 1);
    result *= 31;
    result += (in.day - 1);
    result *= 24;
    result += in.hour;
    result *= 60;
    result += in.min;
    result *= 61;
    result += in.sec;
    result *= 1000;
    result += in.msec;

    return result;
}

function Date_Time date_time_from_dense_time(Dense_Time in) {
    Date_Time result = {};

    result.msec = in%1000;
    in /= 1000;
    result.sec = in%61;
    in /= 61;
    result.min = in%60;
    in /= 60;
    result.hour = in%24;
    in /= 24;
    result.day = (in%31) + 1;
    in /= 31;
    result.mon = (in%12) + 1;
    in /= 12;

    i32 year_encoded = (i32)in;
    result.year = (year_encoded - 0x8000);

    return result;
}

#endif // NA_IMPL

#ifndef NA_DATA_H
#define NA_DATA_H

//
// Array macros for partial functionality:
//

#define Array_Alloc(a,arr,T,s) do { \
    (arr)->data = PushArray(a,T,s); \
    (arr)->count = 0; \
    (arr)->capacity = s; \
} while(0)

#define ArrayPush(a) ((a)->count += 1, &(a)->data[(a)->count - 1])

#define ArrayPop(a) ((a)->count -= 1, &(a)->data[(a)->count + 1])

#define ArrayPeek(a) ((a)->count > 0 ? &(a)->data[(a)->count - 1] : NULL)

#define ArrayCopy(d,s) do { \
    assert(sizeof((d)->data[0]) == sizeof((s).data[0])); \
    assert((d)->capacity != 0); \
    (d)->count = Min((s).count, (d)->capacity); \
    MemoryCopy((d)->data, (s).data, (d)->count * sizeof((s).data[0])); \
} while(0)

#define ArrayReset(a) ((a)->count = 0)

#define ArrayBegin(a) ((a)->data ? (a)->data : NULL)
#define ArrayEnd(a) ((a)->data ? ((a)->data + (a)->count) : NULL)
 
#define ArrayEach(it, array) auto it = ArrayBegin(array); it < ArrayEnd(array); it ++

#define ArrayZero(a) MemoryZero((a)->data, (a)->count * sizeof((a)->data[0]));


#define For(T, array) \
    for (T *it = (T *)(array).data; it != NULL && it < (T *)((array).data + ((array).item_size * (array).count)); it ++)

#define For_Index(array) \
    for (i64 index = 0; index < (array).count; index ++)

//
// Array
//

struct Array_T
{
    Arena *arena;
    u64   capacity;
    i64   count;
    u8    *data;
    u64   item_size;
};

#define Array(Type) Array_T

function void array_init_from_arena(Array_T *it, Arena *arena, u64 item_size, u64 initial_capacity);
function void array_init(Array_T *it, u64 item_size);
function void array_free(Array_T *it);
function void array_reset(Array_T *it);

function void array_resize(Array_T *it, u64 next_capacity);
function void array_reserve(Array_T *it, u64 minimum_count);

function void *array_get_t(Array_T *it, i64 index);
function void *array_peek_t(Array_T *it);
function void *array_pop_t(Array_T *it);

function void array_remove_unordered(Array_T *it, i64 index);
function void array_remove_ordered(Array_T *it, i64 index);

function void *array_push_t(Array_T *it);
function void *array_push_items_t(Array_T *it, void *items, u64 count);

#define array_get(it, T, index) (T *)(assert(sizeof(T) == (it)->item_size), array_get_t(it, index))
#define array_peek(it, T) (T *)(assert(sizeof(T) == (it)->item_size), array_peek_t(it))
#define array_push(it, T) (T *)(assert(sizeof(T) == (it)->item_size), array_push_t(it))
#define array_pop(it, T) (T *)(assert(sizeof(T) == (it)->item_size), array_pop_t(it))
#define array_add(it, T, value) do { assert(sizeof(T) == (it)->item_size); T __tmp = value; array_push_items_t(it, &__tmp, 1); } while (0)
#define array_push_items(it, items, count) (T *)(assert(sizeof(T) == (it)->item_size), array_push_items_t(it, items, count))

function void array_init_from_arena(Array_T *it, Arena *arena, u64 item_size, u64 initial_capacity) {
    it->arena = arena;
    it->count = 0;
    it->capacity = Max(initial_capacity, 1);
    it->item_size = item_size;
    // NOTE(nick): push uses MemoryZero, so we don't need to clear the memory here
    arena_reset(arena);
    it->data = PushArray(arena, u8, it->capacity * it->item_size);
}

function void array_init(Array_T *it, u64 item_size) {
    array_init_from_arena(it, arena_alloc(Gigabytes(1)), item_size, 32);
}

function void array_free(Array_T *it) {
    if (it->data) {
        if (it->arena) {
            arena_free(it->arena);
            it->arena = NULL;
        }

        it->data = NULL;
    }

    it->capacity = 0;
    it->count = 0;
    it->item_size = 0;
}

function void array_reset(Array_T *it) {
    it->count = 0;
}

function void array_resize(Array_T *it, u64 next_capacity) {
    assert(it->data);
    assert(it->item_size > 0);
    if (it->capacity == next_capacity) return;

    arena_set_pos(it->arena, next_capacity * it->item_size);
    it->capacity = next_capacity;
}

function void array_reserve(Array_T *it, u64 minimum_count) {
    if (it->capacity < minimum_count) {
        array_resize(it, minimum_count);
    }
}

function void *array_get_t(Array_T *it, i64 index)
{
    assert(it->data);
    assert(it->item_size > 0);
    assert(index >= 0 && index < it->count);

    void *result = it->data + it->item_size * index;
    return result;
}

function void *array_peek_t(Array_T *it) {
    assert(it->data);
    assert(it->item_size > 0);

    return it->count > 0 ? it->data + it->item_size * (it->count - 1) : NULL;
}

function void *array_pop_t(Array_T *it) {
    void *removed = array_peek_t(it);
    if (removed) it->count --;
    return removed;
}

function void array_remove_unordered(Array_T *it, i64 index) {
    assert(it->data);
    assert(it->item_size > 0);
    assert(index >= 0 && index < it->count);
    
    MemoryCopy(it->data + it->item_size * (it->count - 1), it->data + it->item_size * index, it->item_size);
    it->count--;
}

function void array_remove_ordered(Array_T *it, i64 index) {
    assert(it->data);
    assert(it->item_size > 0);
    assert(index >= 0 && index < it->count);

    u64 i = index + 1;
    u64 remaining_count = it->count - i;

    MemoryMove(it->data + it->item_size * index, it->data + it->item_size * i, it->item_size * remaining_count);

    it->count -= 1;
}

function void *array_push_t(Array_T *it) {
    if (it->count >= it->capacity) {
        u64 next_capacity = next_power_of_two(Max(it->capacity + 1, 16));
        array_resize(it, next_capacity);
    }

    void *result = it->data + it->item_size * it->count;
    MemoryZero(result, it->item_size);
    it->count += 1;
    return result;
}

function void *array_push_items_t(Array_T *it, void *items, u64 count) {
    if (it->count + count > it->capacity) {
        u64 next_capacity = next_power_of_two(Max(it->capacity + count, 16));
        array_resize(it, next_capacity);
    }

    void *result = it->data + it->item_size * it->count;
    MemoryCopy(result, items, count * it->item_size);
    it->count += count;
    return result;
}

//
// Table
//

struct H_Hash
{
    u64 value;
};

struct H_Slot
{
    u64 hash;
};

struct Table_KV
{
    Arena *arenas[2];

    u64     capacity;
    i64     count;
    u64     slots_filled;

    H_Slot *slots;
    u8     *keys;
    u64    key_size;
    u8     *data;
    u64    item_size;
};

#define Table(K, V) Table_KV

//
// API
//

// Table
function void table_init_from_arena(Table_KV *it, Arena *arena0, Arena *arena1, u64 key_size, u64 item_size, u64 table_size);
function void table_init(Table_KV *it, u64 key_size, u64 item_size);
function void table_reset(Table_KV *it);
function void table_free(Table_KV *it);
function void table_rehash(Table_KV *it, u64 next_capacity);

function void *table_add(Table_KV *it, H_Hash hash, void *key, void *value);
function void *table_set(Table_KV *it, H_Hash hash, void *key, void *value);

function b32 table_find(Table_KV *it, H_Hash hash, void *key, i64 *found_index);

function void *table_key(Table_KV *it, i64 index);
function void *table_value(Table_KV *it, i64 index);
function void *table_get(Table_KV *it, H_Hash hash, void *key);

function b32 table_remove(Table_KV *it, H_Hash hash, void *key);
function b32 table_delete(Table_KV *it, i64 index);

//
// Hash Functions
//

function b32 hash_equals(H_Hash a, H_Hash b) {
    return a.value == b.value;
}

function b32 hash_is_valid(H_Hash a) {
    return a.value > 0;
}

function H_Hash hash_i64(i64 it) {
    H_Hash result;
    result.value = fnv64a(&it, sizeof(i64));
    return result;
}

//
// Table
//

const int TABLE_NEVER_OCCUPIED_HASH = 0;
const int TABLE_REMOVED_HASH = 1;
const int TABLE_FIRST_VALID_HASH = 2;

const int TABLE_SIZE_MIN = 32;

//
// TODO(nick):
// - do some sort of quadratic probing thing
//

function void table_init_from_arena(Table_KV *it, Arena *arena0, Arena *arena1, u64 key_size, u64 item_size, u64 initial_capacity)
{
    it->arenas[0] = arena0;
    it->arenas[1] = arena1;

    it->capacity = Max(next_power_of_two(initial_capacity), TABLE_SIZE_MIN);
    it->count = 0;
    it->slots_filled = 0;

    it->key_size   = key_size;
    it->item_size  = item_size;

    assert(it->arenas[0]);
    assert(it->arenas[1]);
    assert(is_power_of_two(it->capacity));
    assert(it->item_size > 0);

    arena_reset(arena0);
    arena_reset(arena1);
    it->slots = PushArrayZero(arena0, H_Slot, it->capacity);
    it->keys  = PushArrayZero(arena0, u8,     it->capacity * it->key_size);
    it->data  = PushArrayZero(arena0, u8,     it->capacity * it->item_size);
}

function void table_init(Table_KV *it, u64 key_size, u64 item_size)
{
    table_init_from_arena(it, arena_alloc(Gigabytes(1)), arena_alloc(Gigabytes(1)), key_size, item_size, 32);
}

function void table_reset(Table_KV *it)
{
    it->count = 0;
    it->slots_filled = 0;

    if (it->slots) {
        MemoryZero(it->slots, it->capacity * sizeof(H_Slot));
    }
}

function void table_free(Table_KV *it)
{
    if (it->slots) {
        if (it->arenas[0])
        {
            arena_free(it->arenas[0]);
            it->arenas[0] = NULL;
        }

        if (it->arenas[1])
        {
            arena_free(it->arenas[1]);
            it->arenas[1] = NULL;
        }

        it->slots = NULL;
        it->keys = NULL;
        it->data = NULL;
    }

    it->capacity = 0;
    it->count = 0;
    it->slots_filled = 0;
}

function void table_rehash(Table_KV *it, u64 next_capacity)
{
    assert(is_power_of_two(it->capacity));
    assert(it->item_size > 0);

    u64 old_capacity  = it->capacity;
    H_Slot *old_slots = it->slots;
    u8     *old_keys  = it->keys;
    u8     *old_data  = it->data;

    Swap(Arena *, it->arenas[0], it->arenas[1]);

    // count and slots_filled will be incremented by add.
    it->capacity = Max(next_power_of_two(next_capacity), TABLE_SIZE_MIN);
    it->count = 0;
    it->slots_filled = 0;

    Arena *arena = it->arenas[0];
    u64 next_size = sizeof(H_Slot) * it->capacity + it->capacity * it->key_size + it->capacity * it->item_size;
    arena_set_pos(arena, next_size);

    it->slots = (H_Slot *)arena->data;
    it->keys  = (u8 *)it->slots + sizeof(H_Slot) * it->capacity;
    it->data  = (u8 *)it->keys  + it->capacity * it->key_size;

    for (u64 index = 0; index < old_capacity; index++) {
        H_Slot *entry = &old_slots[index];

        if (entry->hash >= TABLE_FIRST_VALID_HASH) {
            u8 *key   = old_keys + (it->key_size  * index);
            u8 *value = old_data + (it->item_size * index);

            table_add(it, {entry->hash}, key, value);
        }
    }
}

// Sets the key-value pair, replacing it if it already exists.
function void *table_set(Table_KV *it, H_Hash hash, void *key, void *value)
{
    i64 index;
    if (table_find(it, hash, key, &index))
    {
        void *data = table_value(it, index);
        MemoryCopy(data, value, it->item_size);
    }
    else
    {
        table_add(it, hash, key, value);
    }
}

// Adds the given key-value pair to the table, returns a pointer to the inserted item.
function void *table_add(Table_KV *it, H_Hash hash, void *key, void *value)
{
    assert(key);
    assert(value);

    // slots_filled / capacity >= 7 / 10 ...therefore:
    // slots_filled * 10 >= capacity * 7
    // The + 1 is here to handle the weird case where the table size is 1 and you add the first item
    if ((it->slots_filled + 1) * 10 >= it->capacity * 7) {
        u32 next_capacity = it->capacity ? it->capacity * 2 : TABLE_SIZE_MIN;
        table_rehash(it, next_capacity);
    }

    assert(it->slots_filled <= it->capacity);

    if (hash.value < TABLE_FIRST_VALID_HASH) hash.value += TABLE_FIRST_VALID_HASH;

    u64 index = hash.value & (it->capacity - 1);

    while (it->slots[index].hash) {
        index += 1;
        index &= (it->capacity - 1);
    }

    it->slots[index] = {hash.value};
    it->count ++;
    it->slots_filled ++;

    void *result = (it->data) + (it->item_size * index);

    MemoryCopy((it->keys) + (it->key_size * index), key, it->key_size);
    MemoryCopy(result, value, it->item_size);

    return result;
}

function b32 table_find(Table_KV *it, H_Hash hash, void *key, i64 *found_index)
{
    assert(key);

    if (it->slots)
    {
        if (hash.value < TABLE_FIRST_VALID_HASH) hash.value += TABLE_FIRST_VALID_HASH;
        u64 index = hash.value & (it->capacity - 1);

        while (it->slots[index].hash) {
            H_Slot *slot = &it->slots[index];
            u8 *entry_key = it->keys + (it->key_size * index);

            if (slot->hash == hash.value && MemoryEquals(entry_key, key, it->key_size))
            {
                if (found_index)
                {
                    *found_index = index;
                }
                return true;
            }

            index += 1;
            index &= (it->capacity - 1);
        }
    }

    return false;
}

function void *table_key(Table_KV *it, i64 index) {
    void *result = NULL;
    if (index >= 0 && index < it->capacity)
    {
        result = it->keys + (it->key_size * index);
    }
    return result;
}

function void *table_value(Table_KV *it, i64 index) {
    void *result = NULL;
    if (index >= 0 && index < it->capacity)
    {
        result = it->data + (it->item_size * index);
    }
    return result;
}

function void *table_get(Table_KV *it, H_Hash hash, void *key)
{
    i64 index;
    if (table_find(it, hash, key, &index))
    {
        return table_value(it, index);
    }
    return NULL;
}

function b32 table_remove(Table_KV *it, H_Hash hash, void *key)
{ 
    assert(key);
    assert(it->slots); // Must be initialized!

    if (hash.value < TABLE_FIRST_VALID_HASH) hash.value += TABLE_FIRST_VALID_HASH;
    u64 index = hash.value & (it->capacity - 1);

    while (it->slots[index].hash) {
        H_Slot *slot = &it->slots[index];
        u8 *entry_key = it->keys + (it->key_size * index);

        if (slot->hash == hash.value && MemoryEquals(entry_key, key, it->key_size)) {
            // No valid entry will ever hash to TABLE_REMOVED_HASH.
            it->slots[index].hash = TABLE_REMOVED_HASH;
            it->count --;
            return true;
        }

        index += 1;
        index &= (it->capacity - 1);
    }

    return false;
}

function b32 table_delete(Table_KV *it, i64 index)
{
    b32 result = false;
    if (index >= 0 && index < it->capacity)
    {
        H_Slot *slot = &it->slots[index];
        if (slot->hash >= TABLE_FIRST_VALID_HASH)
        {
            it->count --;
            slot->hash = TABLE_REMOVED_HASH;
            result = true;
        }
    }
    return result;
}


#endif // NA_DATA_H
