#define UNICODE
#define _UNICODE
#define SOKOL_TIME_IMPL
#define ix_SHIP 1
#define ix_DO_TEST 0
#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_NLS
#define ix_WIN_INCLUDE_USER
#define ix_WIN_INCLUDE_WINMESSAGES
#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_MB
#define ix_WIN_INCLUDE_NLS
// FILE BEGIN: ./src/textunpack/textunpack_cli.cpp
// FILE BEGIN: ./src/textunpack/textunpack.hpp

// FILE BEGIN: ./src/ix/ix.hpp

#if !defined(ix_SHIP)
#define ix_SHIP 0
#endif

#if !defined(ix_MEASURE_COVERAGE)
#define ix_MEASURE_COVERAGE 0
#endif

#if !defined(ix_CPPCHECK)
#define ix_CPPCHECK 0
#endif

#if !defined(ix_DO_TEST)
#if ix_SHIP
#define ix_DO_TEST 0
#else
#define ix_DO_TEST 1
#endif
#endif

#define ix_PP_TO_STRING_LITERAL(x) #x
#define ix_PP_CONCAT_IMPL(s1, s2) s1##s2
#define ix_PP_CONCAT(s1, s2) ix_PP_CONCAT_IMPL(s1, s2)
#ifdef __COUNTER__
#define ix_PP_ADD_UNIQUE_ID(x) ix_PP_CONCAT(x, __COUNTER__)
#else
#define ix_PP_ADD_UNIQUE_ID(x) ix_PP_CONCAT(x, __LINE__)
#endif

#define ix_OPT_LEVEL(x) ix_OPT_LEVEL_IMPL_##x()
#define ix_OPT_LEVEL_IMPL_DEBUG() 0
#define ix_OPT_LEVEL_IMPL_RELEASE() 0

#if defined(NDEBUG)
#undef ix_OPT_LEVEL_IMPL_RELEASE
#define ix_OPT_LEVEL_IMPL_RELEASE() 1
#else
#undef ix_OPT_LEVEL_IMPL_DEBUG
#define ix_OPT_LEVEL_IMPL_DEBUG() 1
#endif

// ix is a C++ code base!
static_assert(__cplusplus);
#define ix_LANG(x) ix_LANG_IMPL_##x()
#define ix_LANG_IMPL_C() 0
#define ix_LANG_IMPL_CPP() 1

#define ix_C_VERSION(x) ix_C_VERSION_IMPL_##x()
#define ix_C_VERSION_IMPL_PRE98() 0
#define ix_C_VERSION_IMPL_98() 0
#define ix_C_VERSION_IMPL_11() 0
#define ix_C_VERSION_IMPL_17() 0

#define ix_CPP_VERSION(x) ix_CPP_VERSION_IMPL_##x()
#define ix_CPP_VERSION_IMPL_PRE98() 0
#define ix_CPP_VERSION_IMPL_98() 0
#define ix_CPP_VERSION_IMPL_11() 0
#define ix_CPP_VERSION_IMPL_14() 0
#define ix_CPP_VERSION_IMPL_17() 0

#if __cplusplus == 1
#undef ix_CPP_VERSION_IMPL_PRE98
#define ix_CPP_VERSION_IMPL_PRE98() 1
#define ix_CPP_VERSION_YEAR 1
#elif __cplusplus == 199711L
#undef ix_CPP_VERSION_IMPL_98
#define ix_CPP_VERSION_IMPL_98() 1
#define ix_CPP_VERSION_YEAR 1998
#elif __cplusplus == 201103L
#undef ix_CPP_VERSION_IMPL_11
#define ix_CPP_VERSION_IMPL_11() 1
#define ix_CPP_VERSION_YEAR 2011
#elif __cplusplus == 201402L
#undef ix_CPP_VERSION_IMPL_14
#define ix_CPP_VERSION_IMPL_14() 1
#define ix_CPP_VERSION_YEAR 2014
#elif __cplusplus == 201703L
#undef ix_CPP_VERSION_IMPL_17
#define ix_CPP_VERSION_IMPL_17() 1
#define ix_CPP_VERSION_YEAR 2017
#elif __cplusplus == 202002L
#undef ix_CPP_VERSION_IMPL_20
#define ix_CPP_VERSION_IMPL_20() 1
#define ix_CPP_VERSION_YEAR 2020
#endif

#define ix_COMPILER(x) ix_COMPILER_IMPL_##x()
#define ix_COMPILER_IMPL_MSVC() 0
#define ix_COMPILER_IMPL_GCC() 0
#define ix_COMPILER_IMPL_CLANG() 0
#define ix_COMPILER_IMPL_ICC() 0

#if defined(__clang__)
#undef ix_COMPILER_IMPL_CLANG
#define ix_COMPILER_IMPL_CLANG() 1
#elif defined(__GNUC__) && !defined(__clang__)
#undef ix_COMPILER_IMPL_GCC
#define ix_COMPILER_IMPL_GCC() 1
#elif defined(_MSC_VER) && !defined(__clang__)
#undef ix_COMPILER_IMPL_MSVC
#define ix_COMPILER_IMPL_MSVC() 1
#else
#error "Unknown compiler"
#endif

#define ix_PLATFORM(x) ix_PLATFORM_IMPL_##x()

#define ix_PLATFORM_IMPL_WIN() 0
#define ix_PLATFORM_IMPL_LINUX() 0
#define ix_PLATFORM_IMPL_MAC() 0
#define ix_PLATFORM_IMPL_WASM() 0

#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)
#undef ix_PLATFORM_IMPL_WIN
#define ix_PLATFORM_IMPL_WIN() 1
#elif defined(__APPLE__)
#undef ix_PLATFORM_IMPL_MAC
#define ix_PLATFORM_IMPL_MAC() 1
#elif defined(__linux__)
#undef ix_PLATFORM_IMPL_LINUX
#define ix_PLATFORM_IMPL_LINUX() 1
#elif defined(__wasm__)
#undef ix_PLATFORM_IMPL_WASM
#define ix_PLATFORM_IMPL_WASM() 1
#else
#error "Unknown platform"
#endif

#define ix_ARCH(x) ix_ARCH_IMPL_##x()
#define ix_ARCH_IMPL_x64() 0
#define ix_ARCH_IMPL_ARM64() 0
#define ix_ARCH_IMPL_WASM() 0

#if defined(_M_AMD64) || defined(__x86_64__)
#undef ix_ARCH_IMPL_x64
#define ix_ARCH_IMPL_x64() 1
#elif defined(__aarch64__) || defined(_M_ARM64)
#undef ix_ARCH_IMPL_ARM64
#define ix_ARCH_IMPL_ARM64() 1
#elif defined(__wasm__)
#undef ix_ARCH_IMPL_WASM
#define ix_ARCH_IMPL_WASM() 1
#else
#error "Unknown architecture"
#endif

#if ix_COMPILER(CLANG)
#define ix_DISABLE_CLANG_WARNING_BEGIN _Pragma("clang diagnostic push")
#define ix_DISABLE_CLANG_WARNING_END _Pragma("clang diagnostic pop")
#define ix_DISABLE_CLANG_WARNING(w) _Pragma(ix_PP_TO_STRING_LITERAL(clang diagnostic ignored w))
#else
#define ix_DISABLE_CLANG_WARNING_BEGIN
#define ix_DISABLE_CLANG_WARNING_END
#define ix_DISABLE_CLANG_WARNING(w)
#endif

#if ix_COMPILER(GCC)
#define ix_DISABLE_GCC_WARNING_BEGIN _Pragma("GCC diagnostic push")
#define ix_DISABLE_GCC_WARNING_END _Pragma("GCC diagnostic pop")
#define ix_DISABLE_GCC_WARNING(w) _Pragma(ix_PP_TO_STRING_LITERAL(GCC diagnostic ignored w))
#else
#define ix_DISABLE_GCC_WARNING_BEGIN
#define ix_DISABLE_GCC_WARNING_END
#define ix_DISABLE_GCC_WARNING(w)
#endif

#if ix_COMPILER(MSVC)
#define ix_DISABLE_MSVC_WARNING_BEGIN __pragma(warning(push))
#define ix_DISABLE_MSVC_WARNING_END __pragma(warning(pop))
#define ix_DISABLE_MSVC_WARNING(N) __pragma(warning(disable : N))
#else
#define ix_DISABLE_MSVC_WARNING_BEGIN
#define ix_DISABLE_MSVC_WARNING_END
#define ix_DISABLE_MSVC_WARNING(N)
#endif

#define ix_LENGTH_OF(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ix_UNUSED(x) (void)(x)

[[noreturn]] void ix_abort();
[[noreturn]] void ix_abort_with_info(const char *file, int line, const char *msg);

#if ix_MEASURE_COVERAGE
#define ix_UNREACHABLE()
#elif ix_OPT_LEVEL(DEBUG) && (!ix_COMPILER(MSVC))
#define ix_UNREACHABLE() ix_abort_with_info(__FILE__, __LINE__, "Unrechable!")
#elif ix_COMPILER(MSVC)
#define ix_UNREACHABLE() __assume(false)
#else
#define ix_UNREACHABLE() __builtin_unreachable()
#endif

// Only clang has a proper exhaustive check of an enum in switch statements.
#if ix_MEASURE_COVERAGE
#define ix_CASE_EXHAUSTED()
#else
#define ix_CASE_EXHAUSTED() \
    default:                \
        break
#endif

#define ix_NON_EXHASTIVE_SWITCH(expr)         \
    ix_DISABLE_CLANG_WARNING_BEGIN            \
    ix_DISABLE_CLANG_WARNING("-Wswitch-enum") \
    switch (expr)                             \
        ix_DISABLE_CLANG_WARNING_END

#if !ix_COMPILER(MSVC)
#define ix_LIKELY(cond) __builtin_expect((cond), 1)
#define ix_UNLIKELY(cond) __builtin_expect((cond), 0)
#else
#define ix_LIKELY(cond) cond
#define ix_UNLIKELY(cond) cond
#endif

#if ix_MEASURE_COVERAGE
#define ix_FORCE_INLINE inline
#else
#if ix_COMPILER(MSVC)
#define ix_FORCE_INLINE __forceinline
#else
#define ix_FORCE_INLINE [[gnu::always_inline]] __attribute__((always_inline)) inline
#endif
#endif

#if ix_PLATFORM(WIN)
ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wreserved-macro-identifier")
#define _CRT_SECURE_NO_WARNINGS 1    // NOLINT
#define _CRT_NONSTDC_NO_WARNINGS 1   // NOLINT
#define _CRT_NONSTDC_NO_DEPRECATE 1  // NOLINT
#define _CRT_DECLARE_NONSTDC_NAMES 1 // NOLINT
ix_DISABLE_CLANG_WARNING_END
#endif

#if !ix_COMPILER(MSVC)
__attribute__((__format__(printf, 1, 2))) void ix_log_debug(const char *format, ...);
__attribute__((__format__(printf, 1, 2))) void ix_log_warning(const char *format, ...);
__attribute__((__format__(printf, 1, 2))) void ix_log_fatal(const char *format, ...);
#else
void ix_log_debug(const char *format, ...);
void ix_log_warning(const char *format, ...);
void ix_log_fatal(const char *format, ...);
#endif

#if defined(__INT8_TYPE__)
// clang-format off
typedef __INT8_TYPE__   int8_t;
typedef __INT16_TYPE__  int16_t;
typedef __INT32_TYPE__  int32_t;
typedef __INT64_TYPE__  int64_t;
typedef __UINT8_TYPE__  uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
#elif ix_PLATFORM(WASM)
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
#else
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
// clang-format on
#endif

#if ix_PLATFORM(WASM)
typedef unsigned long size_t;
typedef long intptr_t;
typedef unsigned long uintptr_t;
typedef long ptrdiff_t;
#else
typedef uint64_t size_t;
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;
typedef int64_t ptrdiff_t;
#endif

// clang-format off
#define ix_INT8_MIN   (-127 - 1)
#define ix_INT16_MIN  (-32767 - 1)
#define ix_INT32_MIN  (-2147483647 - 1)
#define ix_INT64_MIN  (-9223372036854775807LL - 1)

#define ix_INT8_MAX   127
#define ix_INT16_MAX  32767
#define ix_INT32_MAX  2147483647
#define ix_INT64_MAX  9223372036854775807LL

#define ix_UINT8_MAX  0xFF
#define ix_UINT16_MAX 0xFFFF
#define ix_UINT32_MAX 0xFFFFFFFF
#define ix_UINT64_MAX 0xFFFFFFFFFFFFFFFFULL

#if ix_PLATFORM(WASM)
#define ix_SIZE_MAX ix_UINT32_MAX
#else
#define ix_SIZE_MAX ix_UINT64_MAX
#endif
// clang-format on

#if ix_COMPILER(MSVC)
ix_DISABLE_MSVC_WARNING(26439) // "This kind of function may not throw. Declare it 'noexcept'."
ix_DISABLE_MSVC_WARNING(26495) // "Member variable is not initialized."
#endif

typedef unsigned int UINT;
typedef void *HANDLE;
ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wreserved-identifier")
struct HWND__; // NOLINT
ix_DISABLE_CLANG_WARNING_END
typedef struct HWND__ *HWND;
#if defined(__CYGWIN__)
typedef unsigned int DWORD;
typedef long long int LONG_PTR;
typedef long long unsigned int UINT_PTR;
#else
typedef unsigned long DWORD;
typedef int64_t LONG_PTR;
typedef uint64_t UINT_PTR;
#endif
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;

// ix_nchar = native character type used in syscalls.
#if ix_PLATFORM(WIN)
typedef wchar_t ix_nchar;
#else
typedef char ix_nchar;
#endif

#define ix_MAX_PATH 255

#if ix_PLATFORM(WIN)
#define ix_NSTR(text) L##text
#else
#define ix_NSTR(text) text
#endif

class ix_Result;

template <typename T>
class ix_Option;

template <typename T>
class ix_UniquePointer;
// FILE END: ./src/ix/ix.hpp

class ix_StringView;
class ix_Writer;
class ix_FileHandle;

const char *tup_find_first_entry_path_start(const char *p);
const char *tup_find_next_entry_path_start(const char *p);
size_t tup_calculate_entry_path_length(const char *entry_path_start);

void tup_unpack_entry(
    const ix_StringView &entry_path,
    const char *archive_end,
    ix_Writer *entry_writer,
    const ix_FileHandle *err_handle
);
// FILE END: ./src/textunpack/textunpack.hpp

// FILE BEGIN: ./src/ix/ix_Buffer.hpp

// FILE BEGIN: ./src/ix/ix_printf.hpp


#include <stdarg.h>

#if ix_COMPILER(MSVC)
#include <sal.h>
#define ix_PRINTF_FORMAT(format_idx, args_start)
#define ix_FORMAT_ARG _Printf_format_string_
#else
#define ix_PRINTF_FORMAT(format_idx, var_args_start) __attribute__((__format__(printf, format_idx, var_args_start)))
#define ix_FORMAT_ARG
#endif

ix_PRINTF_FORMAT(3, 0) int ix_vsnprintf(char *buf, size_t buf_length, const char *format, va_list args);
ix_PRINTF_FORMAT(3, 4) int ix_snprintf(char *buf, size_t buf_length, ix_FORMAT_ARG const char *format, ...);

template <size_t N, typename... Args>
ix_PRINTF_FORMAT(2, 3) int ix_asnprintf(char (&buf)[N], ix_FORMAT_ARG const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = ix_vsnprintf(buf, N, format, args);
    va_end(args);
    return ret;
}
// FILE END: ./src/ix/ix_printf.hpp

class ix_FileHandle;

class ix_Buffer
{
    char *m_data = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;

  public:
    ~ix_Buffer();

    ix_Buffer() = default;
    explicit ix_Buffer(size_t initial_capacity);

    static ix_Buffer from_existing_region(char *p, size_t length);

    template <size_t N>
    static ix_Buffer from_existing_array(char (&arr)[N])
    {
        return ix_Buffer::from_existing_region(arr, N);
    }

    static ix_Buffer from_file(const char *filename);

    ix_Buffer(ix_Buffer &&other) noexcept;
    ix_Buffer(const ix_Buffer &other) = delete;
    ix_Buffer &operator=(ix_Buffer &&other) noexcept;
    ix_Buffer &operator=(const ix_Buffer &other) = delete;

    const char *data() const;
    size_t size() const;
    size_t capacity() const;

    bool empty() const;

    void clear();
    void reset();
    void pop_back(size_t length);
    ix_UniquePointer<char[]> detach();

    // Exact
    void *allocate(size_t size);
    void reserve(size_t new_capacity);
    void ensure(size_t extra_capacity);

    // Not exact
    void reserve_aggressively(size_t minimum_capacity);
    void ensure_aggressively(size_t minimum_extra_capacity);

    // Not exact (uses reserve_aggressively and ensure_aggressively)
    void push(const void *data, size_t data_size);
    void push_between(const void *start, const void *end);
    void push_str(const char *str);
    void push_char(char c);
    void push_char_repeat(char c, size_t n);
    ix_PRINTF_FORMAT(2, 0) size_t push_stringfv(ix_FORMAT_ARG const char *format, va_list args);
    ix_PRINTF_FORMAT(2, 3) size_t push_stringf(ix_FORMAT_ARG const char *format, ...);

    // Not exact
    ix_Option<size_t> load_file(const char *filename);
    // `first_read_size = 0` means `first_read_size = m_capacity - m_size`.
    ix_Option<size_t> load_file_handle(const ix_FileHandle &file, size_t first_read_size = 0);

    char *data();
    char *begin();
    char *end();
    const char *begin() const;
    const char *end() const;

    // Danger zone. Use with caution!
    void set_size(size_t size);
    void add_size(size_t size);

  private:
    bool is_resizeable() const;
};
// FILE END: ./src/ix/ix_Buffer.hpp
// FILE BEGIN: ./src/ix/ix_CmdArgsEater.hpp

// FILE BEGIN: ./src/ix/ix_Span.hpp

// FILE BEGIN: ./src/ix/ix_initializer_list.hpp


ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wreserved-macro-identifier")
#define _INITIALIZER_LIST_ // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
#define _INITIALIZER_LIST  // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
ix_DISABLE_CLANG_WARNING_END

#if ix_PLATFORM(WASM)

#include <initializer_list>

#elif ix_COMPILER(GCC)

namespace std
{

template <typename T>
class initializer_list // NOLINT (cert-dcl58-cpp)
{
    const T *m_begin;
    size_t m_length;

  public:
    ix_FORCE_INLINE constexpr initializer_list()
        : m_begin(nullptr),
          m_length(0)
    {
    }

    ix_FORCE_INLINE constexpr initializer_list(const T *begin, size_t length)
        : m_begin(begin),
          m_length(length)
    {
    }

    ix_FORCE_INLINE constexpr const T *begin() const
    {
        return m_begin;
    }

    ix_FORCE_INLINE constexpr const T *end() const
    {
        return m_begin + m_length;
    }

    ix_FORCE_INLINE constexpr size_t size() const
    {
        return m_length;
    }
};

template <typename T>
ix_FORCE_INLINE constexpr const T *begin(initializer_list<T> list) // NOLINT (cert-dcl58-cpp)
{
    return list.begin();
}

template <typename T>
ix_FORCE_INLINE constexpr const T *end(initializer_list<T> list) // NOLINT (cert-dcl58-cpp)
{
    return list.end();
}

} // namespace std

#elif ix_COMPILER(MSVC) || ix_COMPILER(CLANG)

namespace std
{

template <typename T>
class initializer_list // NOLINT (cert-dcl58-cpp)
{
    const T *m_begin;
    const T *m_end;

  public:
    ix_FORCE_INLINE constexpr initializer_list()
        : m_begin(nullptr),
          m_end(nullptr)
    {
    }

#if ix_COMPILER(MSVC)
    ix_FORCE_INLINE constexpr initializer_list(const T *begin, const T *end)
        : m_begin(begin),
          m_end(end)
    {
    }
#else
    ix_FORCE_INLINE constexpr initializer_list(const T *begin, size_t length)
        : m_begin(begin),
          m_end(begin + length)
    {
    }
#endif

    ix_FORCE_INLINE constexpr const T *begin() const
    {
        return m_begin;
    }

    ix_FORCE_INLINE constexpr const T *end() const
    {
        return m_end;
    }

    ix_FORCE_INLINE constexpr size_t size() const
    {
        return static_cast<size_t>(m_end - m_begin);
    }
};

template <typename T>
ix_FORCE_INLINE constexpr const T *begin(initializer_list<T> list) // NOLINT (cert-dcl58-cpp)
{
    return list.begin();
}

template <typename T>
ix_FORCE_INLINE constexpr const T *end(initializer_list<T> list) // NOLINT (cert-dcl58-cpp)
{
    return list.end();
}

} // namespace std

#else
#error "Unknown compiler"
#endif
// FILE END: ./src/ix/ix_initializer_list.hpp

template <typename T>
class ix_Vector;

template <typename T, size_t N>
struct ix_Array;

template <typename T>
class ix_Span
{
    T *m_data = nullptr;
    size_t m_size = 0;

  public:
    ix_FORCE_INLINE ix_Span() = default;

    template <size_t N>
    ix_FORCE_INLINE explicit ix_Span(T (&data)[N])
        : m_data(data),
          m_size(N)
    {
    }

    ix_FORCE_INLINE ix_Span(T *data, size_t len)
        : m_data(data),
          m_size(len)
    {
    }

    ix_DISABLE_GCC_WARNING_BEGIN
    ix_DISABLE_GCC_WARNING("-Winit-list-lifetime")

    // cppcheck-suppress noExplicitConstructor
    ix_FORCE_INLINE ix_Span(std::initializer_list<T> &&list)
        : m_data(const_cast<T *>(list.begin())),
          m_size(list.size())
    {
    }

    ix_DISABLE_GCC_WARNING_END

    ix_FORCE_INLINE T *data() const
    {
        return m_data;
    }

    ix_FORCE_INLINE size_t size() const
    {
        return m_size;
    }

    ix_FORCE_INLINE bool empty() const
    {
        return (m_size == 0);
    }

    ix_FORCE_INLINE const T *begin() const
    {
        return m_data;
    }

    ix_FORCE_INLINE const T *end() const
    {
        return m_data + m_size;
    }

    ix_FORCE_INLINE T *begin()
    {
        return m_data;
    }

    ix_FORCE_INLINE T *end()
    {
        return m_data + m_size;
    }

    ix_FORCE_INLINE T &operator[](size_t i)
    {
        return m_data[i];
    }

    ix_FORCE_INLINE const T &operator[](size_t i) const
    {
        return m_data[i];
    }

    bool contains(const T &value) const
    {
        for (const T &candidate : *this)
        {
            if (candidate == value)
            {
                return true;
            }
        }
        return false;
    }

    bool operator==(const ix_Span &other) const
    {
        if (m_size != other.m_size)
        {
            return false;
        }

        for (size_t i = 0; i < m_size; i++)
        {
            if (m_data[i] != other.m_data[i])
            {
                return false;
            }
        }

        return true;
    }

    ix_FORCE_INLINE bool operator!=(const ix_Span &other) const
    {
        return !(*this == other);
    }
};
// FILE END: ./src/ix/ix_Span.hpp
// FILE BEGIN: ./src/ix/ix_Vector.hpp

// FILE BEGIN: ./src/ix/ix_Vector_util_fwd.hpp


template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, T &&x);

template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, const T &x);

template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, const T *start, const T *end);

template <typename Vec, typename T>
void ix_Vector_erase(Vec &vec, T *pos);

template <typename Vec, typename T>
void ix_Vector_erase(Vec &vec, T *pos, size_t n);

// NOLINTBEGIN(bugprone-macro-parentheses)
#define ix_VECTOR_UTIL_FRIEND_DECLARE(cls)                                  \
    friend void ix_Vector_insert<cls, T>(cls &, T *, T && x);               \
    friend void ix_Vector_insert<cls, T>(cls &, T *, const T &x);           \
    friend void ix_Vector_insert<cls, T>(cls &, T *, const T *, const T *); \
    friend void ix_Vector_erase<cls, T>(cls &, T *);                        \
    friend void ix_Vector_erase<cls, T>(cls &, T *, size_t)
// NOLINTEND(bugprone-macro-parentheses)
// FILE END: ./src/ix/ix_Vector_util_fwd.hpp
// FILE BEGIN: ./src/ix/ix_assert.hpp


#if ix_COMPILER(MSVC)
#define ix_ASSERT_FATAL(expr, msg) \
    do                             \
    {                              \
        if (ix_UNLIKELY(!(expr)))  \
        {                          \
            __debugbreak();        \
            ix_abort();            \
        }                          \
    } while (0)
#else
#define ix_ASSERT_FATAL(expr, msg)                       \
    do                                                   \
    {                                                    \
        if (ix_UNLIKELY(!(expr)))                        \
        {                                                \
            ix_abort_with_info(__FILE__, __LINE__, msg); \
        }                                                \
    } while (0)
#endif

#if ix_OPT_LEVEL(DEBUG)
#define ix_ASSERT(expr) ix_ASSERT_FATAL((expr), "Assertion failed!")
#else
#define ix_ASSERT(expr) (void)(expr)
#endif

// ix_ASSERT_FATAL without message (for benchmarks)
#define ix_ABORT_IF_NOT(expr)     \
    do                            \
    {                             \
        if (ix_UNLIKELY(!(expr))) \
        {                         \
            ix_abort();           \
        }                         \
    } while (0)
// FILE END: ./src/ix/ix_assert.hpp
// FILE BEGIN: ./src/ix/ix_bulk.hpp

// FILE BEGIN: ./src/ix/ix_memory.hpp

// FILE BEGIN: ./src/ix/ix_polyfill.hpp


#if ix_COMPILER(CLANG) || ix_COMPILER(MSVC) || ix_COMPILER(GCC)

ix_FORCE_INLINE constexpr bool ix_is_constant_evaluated()
{
    return __builtin_is_constant_evaluated();
}

#else

#error "Please implement ix_is_constant_evaluated()."

#endif
// FILE END: ./src/ix/ix_polyfill.hpp

#if ix_COMPILER(MSVC)
#define ix_ATTRIBUTE_MALLOC __declspec(restrict)
#else
#define ix_ATTRIBUTE_MALLOC __attribute__((malloc))
#endif

#define ix_MALLOC(T, size) static_cast<T>(ix_malloc(size))
#define ix_REALLOC(T, p, size) static_cast<T>(ix_realloc(p, size))
#define ix_FREE(p) ix_free(p)
#define ix_ALLOC_STRUCT(T) static_cast<T *>(ix_malloc(sizeof(T)))                                           // NOLINT
#define ix_ALLOC_ARRAY(ELEM_T, length) static_cast<ELEM_T *>(ix_malloc(sizeof(ELEM_T) * (length)))          // NOLINT
#define ix_REALLOC_ARRAY(ELEM_T, p, length) static_cast<ELEM_T *>(ix_realloc(p, sizeof(ELEM_T) * (length))) // NOLINT

void ix_free(void *p);
ix_ATTRIBUTE_MALLOC void *ix_malloc(size_t size);
ix_ATTRIBUTE_MALLOC void *ix_realloc(void *p, size_t size);
size_t ix_grow_array_size(size_t size);

void ix_memmove(void *dst, const void *src, size_t n);
void ix_memset(void *dst, char val, size_t n);
int ix_memcmp(const void *a, const void *b, size_t n);
void ix_memcpy(void *dst, const void *src, size_t n);

ix_FORCE_INLINE constexpr uint64_t ix_memread8(const char *src)
{
    uint64_t v = 0;
    if (ix_is_constant_evaluated())
    {
        v = static_cast<uint64_t>(static_cast<uint8_t>(*src));
        v |= static_cast<uint64_t>(static_cast<uint8_t>(*(src + 1))) << 8;
        v |= static_cast<uint64_t>(static_cast<uint8_t>(*(src + 2))) << 16;
        v |= static_cast<uint64_t>(static_cast<uint8_t>(*(src + 3))) << 24;
        v |= static_cast<uint64_t>(static_cast<uint8_t>(*(src + 4))) << 32;
        v |= static_cast<uint64_t>(static_cast<uint8_t>(*(src + 5))) << 40;
        v |= static_cast<uint64_t>(static_cast<uint8_t>(*(src + 6))) << 48;
        v |= static_cast<uint64_t>(static_cast<uint8_t>(*(src + 7))) << 56;
    }
    else
    {
        ix_memcpy(&v, src, 8);
    }

    return v;
}

ix_FORCE_INLINE constexpr uint32_t ix_memread4(const char *src)
{
    uint32_t v = 0;
    if (ix_is_constant_evaluated())
    {
        v = static_cast<uint32_t>(static_cast<uint8_t>(*src));
        v |= static_cast<uint32_t>(static_cast<uint8_t>(*(src + 1))) << 8;
        v |= static_cast<uint32_t>(static_cast<uint8_t>(*(src + 2))) << 16;
        v |= static_cast<uint32_t>(static_cast<uint8_t>(*(src + 3))) << 24;
    }
    else
    {
        ix_memcpy(&v, src, 4);
    }

    return v;
}
// FILE END: ./src/ix/ix_memory.hpp
// FILE BEGIN: ./src/ix/ix_new.hpp


#if (ix_COMPILER(GCC) && ix_OPT_LEVEL(RELEASE))

#include <new>

#elif ix_PLATFORM(WASM)

#include <__config>
inline _LIBCPP_HIDE_FROM_ABI void *operator new(size_t, void *__p) noexcept;

#elif ix_COMPILER(GCC)

ix_DISABLE_GCC_WARNING_BEGIN
ix_DISABLE_GCC_WARNING("-Wredundant-decls")
void *operator new(size_t size, void *p) noexcept;
ix_DISABLE_GCC_WARNING_END

#elif ix_COMPILER(MSVC)

#define __PLACEMENT_NEW_INLINE
inline void *operator new(size_t, void *p) noexcept;

#elif ix_COMPILER(CLANG)

void *operator new(size_t size, void *p) noexcept;

#else

#error "Unknown compiler"

#endif
// FILE END: ./src/ix/ix_new.hpp
// FILE BEGIN: ./src/ix/ix_type_traits.hpp


// Based on Microsoft's STL (https://github.com/microsoft/STL, Apache License v2.0 with LLVM Exception).

ix_DISABLE_MSVC_WARNING_BEGIN
ix_DISABLE_MSVC_WARNING(4180) // Don't put const/volatile to function types.

template <typename T>
T &&declval();

template <class...>
using ix_void_t = void;

template <bool b>
struct ix_bool_constant
{
    static constexpr bool value = b;
};

using ix_true_type = ix_bool_constant<true>;
using ix_false_type = ix_bool_constant<false>;

using ix_nullptr_t = decltype(nullptr);

template <bool Test, class T = void>
struct ix_enable_if
{
};

template <class T>
struct ix_enable_if<true, T>
{
    using type = T;
};

template <bool Test, class T = void>
using ix_enable_if_t = typename ix_enable_if<Test, T>::type;

#if ix_COMPILER(CLANG)

template <class T1, class T2>
constexpr bool ix_is_same_v = __is_same(T1, T2);

#else

template <class, class>
constexpr bool ix_is_same_v = false;

template <class T>
constexpr bool ix_is_same_v<T, T> = true;

#endif

template <class T, class = void>
struct ix_add_reference
{
    using lvalue = T;
    using rvalue = T;
};

template <class T>
struct ix_add_reference<T, ix_void_t<T &>>
{
    using lvalue = T &;
    using rvalue = T &&;
};

template <class T>
using ix_add_lvalue_reference_t = typename ix_add_reference<T>::lvalue;

template <class T>
using ix_add_rvalue_reference_t = typename ix_add_reference<T>::rvalue;

template <class T>
struct ix_remove_reference
{
    using type = T;
};

template <class T>
struct ix_remove_reference<T &>
{
    using type = T;
};

template <class T>
struct ix_remove_reference<T &&>
{
    using type = T;
};

template <class T>
using ix_remove_reference_t = typename ix_remove_reference<T>::type;

template <class T>
struct ix_remove_cv
{
    using type = T;
};

template <class T>
struct ix_remove_cv<const T>
{
    using type = T;
};

template <class T>
struct ix_remove_cv<volatile T>
{
    using type = T;
};

template <class T>
struct ix_remove_cv<const volatile T>
{
    using type = T;
};

template <class T>
using ix_remove_cv_t = typename ix_remove_cv<T>::type;

template <class T>
using ix_remove_cvref_t = typename ix_remove_cv<typename ix_remove_reference<T>::type>::type;

template <class T>
struct ix_remove_all_extents
{
    using type = T;
};

template <class T, size_t n>
struct ix_remove_all_extents<T[n]>
{
    using type = typename ix_remove_all_extents<T>::type;
};

template <class T>
struct ix_remove_all_extents<T[]>
{
    using type = typename ix_remove_all_extents<T>::type;
};

template <class T>
using ix_remove_all_extents_t = typename ix_remove_all_extents<T>::type;

template <typename T>
constexpr bool ix_is_void_v = ix_is_same_v<ix_remove_cv_t<T>, void>;

template <typename T>
constexpr bool ix_is_unbounded_array_v = false;

template <typename T>
constexpr bool ix_is_unbounded_array_v<T[]> = true;

template <class T>
constexpr bool ix_is_integral_v_impl =   //
    ix_is_same_v<T, bool> ||             //
    ix_is_same_v<T, char> ||             //
    ix_is_same_v<T, signed char> ||      //
    ix_is_same_v<T, unsigned char> ||    //
    ix_is_same_v<T, wchar_t> ||          //
#ifdef __cpp_char8_t                     //
    ix_is_same_v<T, char8_t> ||          //
#endif                                   //
    ix_is_same_v<T, char16_t> ||         //
    ix_is_same_v<T, char32_t> ||         //
    ix_is_same_v<T, short> ||            //
    ix_is_same_v<T, unsigned short> ||   //
    ix_is_same_v<T, int> ||              //
    ix_is_same_v<T, unsigned int> ||     //
    ix_is_same_v<T, long> ||             //
    ix_is_same_v<T, unsigned long> ||    //
    ix_is_same_v<T, long long> ||        //
    ix_is_same_v<T, unsigned long long>; //

template <class T>
constexpr bool ix_is_integral_v = ix_is_integral_v_impl<ix_remove_cv_t<T>>;

template <class T>
constexpr bool ix_is_floating_point_v_impl = //
    ix_is_same_v<T, float> ||                //
    ix_is_same_v<T, double> ||               //
    ix_is_same_v<T, long double>;            //

template <class T>
constexpr bool ix_is_floating_point_v = ix_is_floating_point_v_impl<ix_remove_cv_t<T>>;

template <class T>
constexpr bool ix_is_enum_v = __is_enum(T);

template <class T>
constexpr bool ix_is_arithmetic_v = ix_is_integral_v<T> || ix_is_floating_point_v<T>;

template <class>
constexpr bool ix_is_pointer_v = false;

template <class T>
constexpr bool ix_is_pointer_v<T *> = true;

template <class T>
constexpr bool ix_is_pointer_v<T *const> = true;

template <class T>
constexpr bool ix_is_pointer_v<T *volatile> = true;

template <class T>
constexpr bool ix_is_pointer_v<T *const volatile> = true;

template <class>
constexpr bool ix_is_const_v = false;

template <class T>
constexpr bool ix_is_const_v<const T> = true;

template <class>
constexpr bool ix_is_reference_v = false;

template <class T>
constexpr bool ix_is_reference_v<T &> = true;

template <class T>
constexpr bool ix_is_reference_v<T &&> = true;

template <class T>
constexpr bool ix_is_function_v = !ix_is_const_v<const T> && !ix_is_reference_v<T>;

template <class T>
constexpr bool ix_is_null_pointer_v = ix_is_same_v<ix_remove_cv_t<T>, ix_nullptr_t>;

#if ix_COMPILER(CLANG)

template <class T>
constexpr bool ix_is_member_pointer_v = __is_member_pointer(T);

#else

template <class>
struct ix_is_member_object_pointer_impl
{
    static constexpr bool value = false;
};

template <class T1, class T2>
struct ix_is_member_object_pointer_impl<T1 T2::*>
{
    static constexpr bool value = !ix_is_function_v<T1>;
};

template <class T>
constexpr bool ix_is_member_object_pointer_v = ix_is_member_object_pointer_impl<ix_remove_cv_t<T>>::value;

template <class T>
struct ix_is_member_function_pointer_impl
{
    static constexpr bool value = false;
};

template <class T1, class T2>
struct ix_is_member_function_pointer_impl<T1 T2::*>
{
    static constexpr bool value = ix_is_function_v<T1>;
};

template <typename T>
constexpr bool ix_is_member_function_pointer_v = ix_is_member_function_pointer_impl<ix_remove_cv_t<T>>::value;

template <class T>
constexpr bool ix_is_member_pointer_v = ix_is_member_object_pointer_v<T> || ix_is_member_function_pointer_v<T>;

#endif

template <class T>
constexpr bool ix_is_scalar_v =  //
    ix_is_arithmetic_v<T> ||     //
    ix_is_enum_v<T> ||           //
    ix_is_pointer_v<T> ||        //
    ix_is_member_pointer_v<T> || //
    ix_is_null_pointer_v<T>;     //

template <class>
constexpr bool ix_is_lvalue_reference_v = false;

template <class T>
constexpr bool ix_is_lvalue_reference_v<T &> = true;

template <class>
constexpr bool ix_is_rvalue_reference_v = false;

template <class T>
constexpr bool ix_is_rvalue_reference_v<T &&> = true;

template <class T>
constexpr bool ix_is_trivial_v = __is_trivial(T);

template <class T>
constexpr bool ix_is_default_constructible_v = __is_constructible(T);

template <class T>
constexpr bool ix_is_copy_constructible_v = __is_constructible(T, ix_add_lvalue_reference_t<const T>);

template <class T>
constexpr bool ix_is_move_constructible_v = __is_constructible(T, T);

template <class T>
constexpr bool ix_is_trivially_constructible_v = __is_trivially_constructible(T);

template <class T>
constexpr bool ix_is_trivially_copy_constructible_v =
    __is_trivially_constructible(T, ix_add_rvalue_reference_t<const T>);

template <class T>
constexpr bool ix_is_trivially_move_constructible_v = __is_trivially_constructible(T, T);

template <class T>
constexpr bool ix_is_copy_assignable_v =
    __is_assignable(ix_add_lvalue_reference_t<T>, ix_add_lvalue_reference_t<const T>);

template <class T>
constexpr bool ix_is_move_assignable_v = __is_assignable(ix_add_lvalue_reference_t<T>, T);

template <class T>
constexpr bool ix_is_trivially_copy_assignable_v =
    __is_trivially_assignable(ix_add_lvalue_reference_t<T>, ix_add_lvalue_reference_t<const T>);

template <class T>
constexpr bool ix_is_trivially_move_assignable_v = __is_trivially_assignable(ix_add_lvalue_reference_t<T>, T);

#if ix_PLATFORM(WIN) && (ix_COMPILER(CLANG) || ix_COMPILER(MSVC))

ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wlanguage-extension-token")

template <class T>
constexpr bool ix_is_destructible_v = __is_destructible(T);

ix_DISABLE_CLANG_WARNING_END

#else

template <typename T, typename = decltype(declval<T &>().~T())>
constexpr ix_true_type ix_is_destructible_v_impl_helper(int);

template <typename>
constexpr ix_false_type ix_is_destructible_v_impl_helper(...);

template <typename T>
constexpr bool ix_is_destructible_v_impl = decltype(ix_is_destructible_v_impl_helper<T>(0))::value;

template <typename T>
constexpr bool ix_is_destructible_v =
    !(ix_is_void_v<T> || ix_is_unbounded_array_v<T> || ix_is_function_v<T>) &&
    (ix_is_reference_v<T> || ix_is_scalar_v<T> || ix_is_destructible_v_impl<ix_remove_all_extents_t<T>>);

#endif

#if ix_COMPILER(CLANG) || ix_COMPILER(MSVC)

template <class T>
constexpr bool ix_is_trivially_destructible_v = __is_trivially_destructible(T);

#else

template <class T>
constexpr bool ix_is_trivially_destructible_v = ix_is_destructible_v<T> && __has_trivial_destructor(T);

#endif

template <bool Cond, class T, class F>
struct ix_conditional
{
    using type = T;
};

template <class T, class F>
struct ix_conditional<false, T, F>
{
    using type = F;
};

template <bool Cond, class T, class F>
using ix_conditional_t = typename ix_conditional<Cond, T, F>::type;

template <class T, class = void>
struct ix_add_pointer_helper
{
    using type = T;
};

template <class T>
struct ix_add_pointer_helper<T, ix_void_t<ix_remove_reference_t<T> *>>
{
    using type = ix_remove_reference_t<T> *;
};

template <class T>
struct ix_add_pointer
{
    using type = typename ix_add_pointer_helper<T>::type;
};

template <class T>
using ix_add_pointer_t = typename ix_add_pointer_helper<T>::type;

template <class T>
constexpr bool ix_is_class_v = __is_class(T);

template <class T>
constexpr bool ix_is_union_v = __is_union(T);

ix_DISABLE_MSVC_WARNING_END
// FILE END: ./src/ix/ix_type_traits.hpp
// FILE BEGIN: ./src/ix/ix_utility.hpp


template <class T>
ix_FORCE_INLINE constexpr T &&ix_forward(ix_remove_reference_t<T> &arg) noexcept
{
    return static_cast<T &&>(arg);
}

template <class T>
ix_FORCE_INLINE constexpr T &&ix_forward(ix_remove_reference_t<T> &&arg) noexcept
{
    static_assert(!ix_is_lvalue_reference_v<T>, "bad forward call");
    return static_cast<T &&>(arg);
}

template <class T>
ix_FORCE_INLINE constexpr ix_remove_reference_t<T> &&ix_move(T &&arg) noexcept
{
    return static_cast<ix_remove_reference_t<T> &&>(arg);
}

template <class T>
ix_FORCE_INLINE constexpr void ix_swap(T &x, T &y)
{
    T temp = ix_move(x);
    x = ix_move(y);
    y = ix_move(temp);
}

template <class Iter>
ix_FORCE_INLINE constexpr void ix_iter_swap(Iter x, Iter y)
{
    ix_swap(*x, *y);
}
// FILE END: ./src/ix/ix_utility.hpp

template <typename T>
ix_FORCE_INLINE constexpr void ix_bulk_default_construct(T *dst, size_t n)
{
    if constexpr (ix_is_scalar_v<T>)
    {
        ix_memset(dst, 0, n * sizeof(T));
    }
    else if constexpr (!ix_is_trivially_constructible_v<T>)
    {
        for (size_t i = 0; i < n; i++)
        {
            new (dst + i) T();
        }
    }
}

template <typename T>
ix_FORCE_INLINE constexpr void ix_bulk_copy_construct(T *dst, const T *src, size_t n)
{
    if constexpr (ix_is_trivially_copy_constructible_v<T>)
    {
        ix_memcpy(dst, src, sizeof(T) * n);
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            new (dst + i) T(src[i]);
        }
    }
}

template <typename T>
ix_FORCE_INLINE constexpr void ix_bulk_move_construct(T *dst, T *src, size_t n)
{
    if constexpr (ix_is_trivially_move_constructible_v<T>)
    {
        ix_memcpy(dst, src, sizeof(T) * n);
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            new (dst + i) T(ix_move(src[i]));
        }
    }
}

template <typename T>
ix_FORCE_INLINE constexpr void ix_bulk_copy_assign(T *dst, const T *src, size_t n)
{
    if constexpr (ix_is_trivially_move_assignable_v<T>)
    {
        ix_memcpy(dst, src, sizeof(T) * n);
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            dst[i] = src[i];
        }
    }
}

template <typename T>
ix_FORCE_INLINE constexpr void ix_bulk_move_assign(T *dst, T *src, size_t n)
{
    if constexpr (ix_is_trivially_move_assignable_v<T>)
    {
        ix_memcpy(dst, src, sizeof(T) * n);
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            dst[i] = ix_move(src[i]);
        }
    }
}

template <typename T>
ix_FORCE_INLINE constexpr void ix_bulk_destruct(T *p, size_t n)
{
    if constexpr (!ix_is_trivially_destructible_v<T>)
    {
        for (size_t i = 0; i < n; i++)
        {
            p[i].~T();
        }
    }
}
// FILE END: ./src/ix/ix_bulk.hpp
// FILE BEGIN: ./src/ix/ix_compare.hpp

// FILE BEGIN: ./src/ix/ix_string.hpp


bool ix_contains(const char *s, char c);
bool ix_starts_with(const char *a, const char *b);
bool ix_wildcard_match(const char *string, const char *wildcard);
bool ix_wstarts_with(const wchar_t *a, const wchar_t *b);
int ix_wstrcmp(const wchar_t *a, const wchar_t *b);
size_t ix_wstrlen(const wchar_t *s);

char *ix_strstr(char *haystack, const char *needle);
const char *ix_strstr(const char *haystack, const char *needle);

const char *ix_strchr(const char *haystack, char c);

void *ix_memchr(char *haystack, char c, size_t length);
const void *ix_memchr(const char *haystack, char c, size_t length);

char *ix_memnext(char *haystack, char c);
const char *ix_memnext(const char *haystack, char c);

char *ix_memnext2(char *haystack, char c0, char c1);
const char *ix_memnext2(const char *haystack, char c0, char c1);

size_t ix_strlen_runtime(const char *s);
int ix_strcmp_runtime(const char *a, const char *b);

ix_FORCE_INLINE constexpr size_t ix_strlen(const char *s)
{
#if !ix_MEASURE_COVERAGE
    if (!ix_is_constant_evaluated())
    {
        return ix_strlen_runtime(s);
    }
#endif

    const char *start = s;
    while (*s != '\0')
    {
        s += 1;
    }
    return static_cast<size_t>(s - start);
}

ix_FORCE_INLINE constexpr int ix_strcmp(const char *a, const char *b)
{
#if !ix_MEASURE_COVERAGE
    if (!ix_is_constant_evaluated())
    {
        return ix_strcmp_runtime(a, b);
    }
#endif

    while (*a != '\0')
    {
        if (*a != *b)
        {
            break;
        }

        a += 1;
        b += 1;
    }

    return (*a - *b);
}
// FILE END: ./src/ix/ix_string.hpp

ix_DISABLE_GCC_WARNING_BEGIN
ix_DISABLE_GCC_WARNING("-Wfloat-equal")
ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wfloat-equal")

template <typename T>
struct ix_Equal
{
    ix_FORCE_INLINE static constexpr bool equal(const T &x, const T &y)
    {
        return (x == y);
    }
};

template <typename T>
ix_FORCE_INLINE constexpr bool ix_equal(const T &x, const T &y)
{
    return (x == y);
}

template <typename T>
ix_FORCE_INLINE constexpr bool ix_not_equal(const T &x, const T &y)
{
    return (x != y);
}

template <typename T>
ix_FORCE_INLINE constexpr bool ix_is_zero(const T &x)
{
    return (x == T{0});
}

ix_DISABLE_GCC_WARNING_END
ix_DISABLE_CLANG_WARNING_END

struct ix_StringEqual
{
    ix_FORCE_INLINE static constexpr bool equal(const char *x, const char *y)
    {
        return (ix_strcmp(x, y) == 0);
    }
};

template <typename T>
struct ix_Compare
{
    ix_FORCE_INLINE static constexpr int8_t compare(const T x, const T y)
    {
        if (x < y)
        {
            return -1;
        }
        if (x > y)
        {
            return 1;
        }
        return 0;
    }
};

template <>
struct ix_Compare<const char *>
{
    ix_FORCE_INLINE static constexpr int8_t compare(const char *x, const char *y)
    {
        return static_cast<int8_t>(ix_strcmp(x, y));
    }
};

template <typename T>
struct ix_Less
{
    ix_FORCE_INLINE constexpr bool operator()(const T &x, const T &y)
    {
        return x < y;
    }
};

template <typename T>
struct ix_Greater
{
    ix_FORCE_INLINE constexpr bool operator()(const T &x, const T &y)
    {
        return x > y;
    }
};
// FILE END: ./src/ix/ix_compare.hpp

template <typename T>
class ix_Span;

template <typename T>
class ix_Vector
{
    T *m_data;
    size_t m_size;
    size_t m_capacity;

    ix_VECTOR_UTIL_FRIEND_DECLARE(ix_Vector);

  public:
    ix_Vector(const ix_Vector &other) = delete;
    ix_Vector &operator=(const ix_Vector &other) = delete;

    ix_FORCE_INLINE constexpr ix_Vector()
        : m_data(nullptr),
          m_size(0),
          m_capacity(0)
    {
    }

    ix_FORCE_INLINE constexpr explicit ix_Vector(size_t initial_size)
        : m_data(ix_ALLOC_ARRAY(T, initial_size)),
          m_size(initial_size),
          m_capacity(initial_size)
    {
        ix_bulk_default_construct(m_data, initial_size);
    }

    // cppcheck-suppress noExplicitConstructor
    ix_FORCE_INLINE constexpr ix_Vector(const std::initializer_list<T> &xs)
        : m_size(xs.size()),
          m_capacity(xs.size())
    {
        m_data = ix_ALLOC_ARRAY(T, m_size);
        ix_bulk_copy_construct(m_data, xs.begin(), m_size);
    }

    ix_FORCE_INLINE ~ix_Vector()
    {
        ix_bulk_destruct(m_data, m_size);
        ix_FREE(m_data);
    }

    constexpr ix_Vector(ix_Vector &&other) noexcept
        : m_data(other.m_data),
          m_size(other.m_size),
          m_capacity(other.m_capacity)
    {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    constexpr ix_Vector &operator=(ix_Vector &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        ix_bulk_destruct(m_data, m_size);
        ix_FREE(m_data);

        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;

        return *this;
    }

    constexpr bool operator==(const ix_Vector &other) const
    {
        if (m_size != other.m_size)
        {
            return false;
        }

        for (size_t i = 0; i < m_size; i++)
        {
            if (ix_not_equal<T>(m_data[i], other.m_data[i]))
            {
                return false;
            }
        }

        return true;
    }

    constexpr bool operator!=(const ix_Vector &other) const
    {
        return !(*this == other);
    }

    ix_FORCE_INLINE constexpr T *data()
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr const T *data() const
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr size_t size() const
    {
        return m_size;
    }

    ix_FORCE_INLINE constexpr size_t capacity() const
    {
        return m_capacity;
    }

    ix_FORCE_INLINE constexpr ix_Span<T> span()
    {
        return ix_Span<T>(m_data, m_size);
    }

    ix_FORCE_INLINE constexpr ix_Span<T> span(size_t i)
    {
        return ix_Span<T>(m_data, i);
    }

    ix_FORCE_INLINE constexpr bool empty() const
    {
        return (m_size == 0);
    }

    ix_FORCE_INLINE constexpr const T *begin() const
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr T *begin()
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr const T *end() const
    {
        return m_data + m_size;
    }

    ix_FORCE_INLINE constexpr T *end()
    {
        return m_data + m_size;
    }

    ix_FORCE_INLINE constexpr T &operator[](size_t i)
    {
        ix_ASSERT(i < m_size);
        return m_data[i];
    }

    ix_FORCE_INLINE constexpr const T &operator[](size_t i) const
    {
        ix_ASSERT(i < m_size);
        return m_data[i];
    }

    ix_FORCE_INLINE constexpr T &back()
    {
        ix_ASSERT(0 < m_size);
        return m_data[m_size - 1];
    }

    ix_FORCE_INLINE constexpr const T &back() const
    {
        ix_ASSERT(0 < m_size);
        return m_data[m_size - 1];
    }

    ix_FORCE_INLINE constexpr void pop_back()
    {
        ix_ASSERT(0 < m_size);
        m_data[m_size - 1].~T();
        m_size -= 1;
    }

    ix_FORCE_INLINE constexpr void clear()
    {
        ix_bulk_destruct(m_data, m_size);
        m_size = 0;
    }

    ix_FORCE_INLINE constexpr void reserve(size_t n)
    {
        if (n <= m_capacity)
        {
            return;
        }

        if constexpr (ix_is_trivially_move_constructible_v<T> && ix_is_trivially_destructible_v<T>)
        {
            m_data = ix_REALLOC_ARRAY(T, m_data, n);
            m_capacity = n;
        }
        else
        {
            T *new_data = ix_ALLOC_ARRAY(T, n);

            ix_bulk_move_construct(new_data, m_data, m_size);
            ix_bulk_destruct(m_data, m_size);

            ix_FREE(m_data);
            m_data = new_data;
            m_capacity = n;
        }
    }

    ix_FORCE_INLINE constexpr void resize(size_t n)
    {
        if (n < m_size)
        {
            ix_bulk_destruct(m_data + n, m_size - n);
            m_size = n;
            return;
        }

        if (m_capacity < n)
        {
            reserve(n);
        }

        ix_bulk_default_construct(m_data + m_size, n - m_size);
        m_size = n;
    }

    ix_FORCE_INLINE constexpr void resize(size_t n, const T &x)
    {
        if (n < m_size)
        {
            ix_bulk_destruct(m_data + n, m_size - n);
            m_size = n;
            return;
        }

        if (m_capacity < n)
        {
            reserve(n);
        }

        T *p = m_data + m_size;
        const T *p_end = m_data + n;
        while (p < p_end)
        {
            new (p) T(x);
            p += 1;
        }

        m_size = n;
    }

    ix_FORCE_INLINE constexpr void set_size(size_t size)
    {
        m_size = size;
    }

    void swap_erase(size_t index)
    {
        const size_t last_index = m_size - 1;

        if (index != last_index)
        {
            (*this)[index] = ix_move((*this)[last_index]);
        }

        pop_back();
    }

    ix_FORCE_INLINE constexpr void push_back(const T &x)
    {
        emplace_back(x);
    }

    ix_FORCE_INLINE constexpr void push_back(T &&x)
    {
        emplace_back(ix_move(x));
    }

    template <typename... Args>
    ix_FORCE_INLINE constexpr T &emplace_back(Args &&...args)
    {
        if (ix_UNLIKELY(m_size == m_capacity))
        {
            size_t new_capacity = ix_grow_array_size(m_size);
            reserve(new_capacity);
        }

        T *p = m_data + m_size;
        new (p) T(ix_forward<Args>(args)...);
        m_size += 1;

        return *p;
    }
};
// FILE END: ./src/ix/ix_Vector.hpp

// ix_CmdArgsEater is not efficient in terms of both space and time. However, this should not be a
// problem because N is almost always small. If you process thousands of arguments and hundreds of options,
// ix_CmdArgsEater might not suffice.
class ix_CmdArgsEater
{
    ix_Vector<const char *> m_args;

  public:
    ix_CmdArgsEater(int argc, const char *const *argv);
    ix_CmdArgsEater(std::initializer_list<const char *> args); // cppcheck-suppress noExplicitConstructor; NOLINT

    size_t size() const;
    int argc() const;
    const char *const *argv() const;
    const char *const &operator[](size_t index) const;
    bool eat_boolean(const char *name);
    bool eat_boolean(ix_Span<const char *> names);
    const char *eat_kv(const char *key);
    const char *eat_kv(ix_Span<const char *> keys);
    bool eat_kv_n(const char *key, size_t n, const char **out);
};
// FILE END: ./src/ix/ix_CmdArgsEater.hpp
// FILE BEGIN: ./src/ix/ix_Mutex.hpp


// Loosely based on bx::Thread (by Branimir Karadzic, BSD 2-clause license).
class ix_Mutex
{
    alignas(void *) uint8_t m_detail[64];

  public:
    ix_Mutex();
    ix_Mutex(const ix_Mutex &) = delete;
    ix_Mutex(ix_Mutex &&) = delete;
    ix_Mutex &operator=(const ix_Mutex &) = delete;
    ix_Mutex &operator=(ix_Mutex &&) = delete;
    ~ix_Mutex();

    void lock();
    void unlock();

    template <typename F>
    void with_lock(const F &f)
    {
        lock();
        f();
        unlock();
    }

    template <typename T>
    T *native_handle()
    {
        return reinterpret_cast<T *>(m_detail);
    }
};
// FILE END: ./src/ix/ix_Mutex.hpp
// FILE BEGIN: ./src/ix/ix_StringView.hpp

// FILE BEGIN: ./src/ix/ix_hash.hpp


// Based on wyhash (https://github.com/wangyi-fudan/wyhash, The Unlicense)
size_t ix_hash(const void *p, size_t length);
size_t ix_hash_between(const void *start, const void *end);
constexpr size_t ix_hash(const char *p, size_t length);
ix_FORCE_INLINE constexpr size_t ix_hash_str(const char *s);
ix_FORCE_INLINE constexpr size_t ix_hash64(uint64_t x);

#if ix_PLATFORM(WASM)

static_assert(ix_COMPILER(CLANG));

ix_FORCE_INLINE constexpr uint32_t ix_wyr32(const char *p)
{
    uint32_t v = ix_memread4(p);

#if (ix_COMPILER(GCC) || ix_COMPILER(ICC) || ix_COMPILER(CLANG))
    if constexpr (!ix_is_constant_evaluated() && (ix_COMPILER(GCC) || ix_COMPILER(ICC) || ix_COMPILER(CLANG)))
    {
        return __builtin_bswap32(v);
    }
#endif

    return (
        ((v >> 24) & 0xff) |     //
        ((v >> 8) & 0xff00) |    //
        ((v << 8) & 0xff0000) |  //
        ((v << 24) & 0xff000000) //
    );
}

ix_FORCE_INLINE constexpr uint32_t ix_wyr24(const char *p, uint32_t k)
{
    return ((static_cast<uint32_t>(p[0])) << 16) |     //
           ((static_cast<uint32_t>(p[k >> 1])) << 8) | //
           static_cast<uint32_t>(p[k - 1]);            //
}

ix_FORCE_INLINE constexpr void ix_wymix32(uint32_t *A, uint32_t *B)
{
    uint64_t c = *A ^ 0x53c5ca59U;
    c *= *B ^ 0x74743c1bU;
    *A = static_cast<uint32_t>(c);
    *B = static_cast<uint32_t>(c >> 32);
}

constexpr size_t ix_hash(const char *p, size_t length)
{
    uint32_t seed = 0xb380fdf4; // Random number
    uint64_t i = static_cast<uint64_t>(length);
    uint32_t see1 = static_cast<uint32_t>(length);
    // seed ^= static_cast<uint32_t>(length >> 32);
    ix_wymix32(&seed, &see1);
    for (; i > 8; i -= 8, p += 8)
    {
        seed ^= ix_wyr32(p);
        see1 ^= ix_wyr32(p + 4);
        ix_wymix32(&seed, &see1);
    }
    if (i >= 4)
    {
        seed ^= ix_wyr32(p);
        see1 ^= ix_wyr32(p + i - 4);
    }
    else if (i != 0U)
    {
        seed ^= ix_wyr24(p, static_cast<uint32_t>(i));
    }
    ix_wymix32(&seed, &see1);
    ix_wymix32(&seed, &see1);
    return seed ^ see1;
}

ix_FORCE_INLINE constexpr size_t ix_hash64(uint64_t x)
{
    uint32_t a = static_cast<uint32_t>(x);
    uint32_t b = static_cast<uint32_t>(x >> 32);
    ix_wymix32(&a, &b);
    return a ^ b;
}

#else

#if ix_COMPILER(MSVC) && ix_ARCH(x64)

extern "C" unsigned __int64 _umul128(
    unsigned __int64 Multiplier,
    unsigned __int64 Multiplicand,
    unsigned __int64 *HighProduct
);
extern "C" unsigned long _byteswap_ulong(unsigned long val);
extern "C" unsigned __int64 _byteswap_uint64(unsigned __int64 val);

#endif

// Can't use pointers because MSVC will crush when you take the pointer of a value inside a constexpr function.
ix_FORCE_INLINE constexpr void ix_wymum(uint64_t &a, uint64_t &b)
{
#if __SIZEOF_INT128__
    if constexpr (__SIZEOF_INT128__)
    {
        __uint128_t r = a;
        r *= b;
        a = static_cast<uint64_t>(r);
        b = static_cast<uint64_t>(r >> 64);
        return;
    }
#endif

#if ix_COMPILER(MSVC) && ix_ARCH(x64)
    if constexpr (!ix_is_constant_evaluated() && ix_COMPILER(MSVC) && ix_ARCH(x64))
    {
        // It's ok to take the pointer of 'b' because this path is not constexpr.
        a = _umul128(a, b, &b);
        return;
    }
#endif

    const uint64_t ha = a >> 32;
    const uint64_t hb = b >> 32;
    const uint64_t la = static_cast<uint32_t>(a);
    const uint64_t lb = static_cast<uint32_t>(b);
    const uint64_t rh = ha * hb;
    const uint64_t rm0 = ha * lb;
    const uint64_t rm1 = hb * la;
    const uint64_t rl = la * lb;
    const uint64_t t = rl + (rm0 << 32);
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t c = static_cast<uint64_t>(t < rl);
    lo = t + (rm1 << 32);
    c += static_cast<uint64_t>(lo < t);
    hi = rh + (rm0 >> 32) + (rm1 >> 32) + c;
    a = lo;
    b = hi;
}

ix_FORCE_INLINE constexpr uint64_t ix_wymix(uint64_t a, uint64_t b)
{
    ix_wymum(a, b);
    return a ^ b;
}

ix_FORCE_INLINE constexpr uint64_t ix_wyr8(const char *p)
{
    const uint64_t v = ix_memread8(p);

#if (ix_COMPILER(GCC) || ix_COMPILER(ICC) || ix_COMPILER(CLANG))
    if constexpr (!ix_is_constant_evaluated() && (ix_COMPILER(GCC) || ix_COMPILER(ICC) || ix_COMPILER(CLANG)))
    {
        return __builtin_bswap64(v);
    }
#endif

#if ix_COMPILER(MSVC)
    if constexpr (!ix_is_constant_evaluated() && ix_COMPILER(MSVC))
    {
        return _byteswap_uint64(v);
    }
#endif

    return (
        ((v >> 56) & 0xff) |             //
        ((v >> 40) & 0xff00) |           //
        ((v >> 24) & 0xff0000) |         //
        ((v >> 8) & 0xff000000) |        //
        ((v << 8) & 0xff00000000) |      //
        ((v << 24) & 0xff0000000000) |   //
        ((v << 40) & 0xff000000000000) | //
        ((v << 56) & 0xff00000000000000) //
    );
}

ix_FORCE_INLINE constexpr uint64_t ix_wyr4(const char *p)
{
    const uint32_t v = ix_memread4(p);

#if (ix_COMPILER(GCC) || ix_COMPILER(ICC) || ix_COMPILER(CLANG))
    if constexpr (!ix_is_constant_evaluated() && (ix_COMPILER(GCC) || ix_COMPILER(ICC) || ix_COMPILER(CLANG)))
    {
        return __builtin_bswap32(v);
    }
#endif

#if ix_COMPILER(MSVC)
    if constexpr (!ix_is_constant_evaluated() && ix_COMPILER(MSVC))
    {
        return _byteswap_ulong(v);
    }
#endif

    return (
        ((v >> 24) & 0xff) |     //
        ((v >> 8) & 0xff00) |    //
        ((v << 8) & 0xff0000) |  //
        ((v << 24) & 0xff000000) //
    );
}

ix_FORCE_INLINE constexpr uint64_t ix_wyr3(const char *p, uint64_t k)
{
    return (static_cast<uint64_t>(p[0]) << 16) |     //
           (static_cast<uint64_t>(p[k >> 1]) << 8) | //
           (static_cast<uint64_t>(p[k - 1]));
}

constexpr size_t ix_hash(const char *p, uint64_t length)
{
    constexpr uint64_t secret[4] =
        {0xa0761d6478bd642fULL, 0xe7037ed1a0b428dbULL, 0x8ebc6af09c88c6e3ULL, 0x589965cc75374cc3ULL};
    uint64_t seed = 0xfc5473a997896bdaULL; // Random number

    seed ^= ix_wymix(seed ^ secret[0], secret[1]);
    uint64_t a = 0;
    uint64_t b = 0;
    if (ix_LIKELY(length <= 16))
    {
        if (ix_LIKELY(length >= 4))
        {
            a = (ix_wyr4(p) << 32) | ix_wyr4(p + ((length >> 3) << 2));
            b = (ix_wyr4(p + length - 4) << 32) | ix_wyr4(p + length - 4 - ((length >> 3) << 2));
        }
        else if (ix_LIKELY(length > 0))
        {
            a = ix_wyr3(p, length);
            b = 0;
        }
        else
        {
            a = b = 0;
        }
    }
    else
    {
        size_t i = length;
        if (ix_UNLIKELY(i >= 48))
        {
            uint64_t see1 = seed;
            uint64_t see2 = seed;
            do
            {
                seed = ix_wymix(ix_wyr8(p) ^ secret[1], ix_wyr8(p + 8) ^ seed);
                see1 = ix_wymix(ix_wyr8(p + 16) ^ secret[2], ix_wyr8(p + 24) ^ see1);
                see2 = ix_wymix(ix_wyr8(p + 32) ^ secret[3], ix_wyr8(p + 40) ^ see2);
                p += 48;
                i -= 48;
            } while (ix_LIKELY(i >= 48));
            seed ^= see1 ^ see2;
        }
        while (ix_UNLIKELY(i > 16))
        {
            seed = ix_wymix(ix_wyr8(p) ^ secret[1], ix_wyr8(p + 8) ^ seed);
            i -= 16;
            p += 16;
        }
        a = ix_wyr8(p + i - 16);
        b = ix_wyr8(p + i - 8);
    }
    a ^= secret[1];
    b ^= seed;
    ix_wymum(a, b);
    return ix_wymix(a ^ secret[0] ^ length, b ^ secret[1]);
}

ix_FORCE_INLINE constexpr size_t ix_hash64(uint64_t x)
{
    constexpr uint64_t s = 0x1a83813678375e8dULL; // Random number.
    return ix_wymix(x, s);
}

#endif

ix_FORCE_INLINE constexpr size_t ix_hash_str(const char *s)
{
    return ix_hash(s, ix_strlen(s));
}

template <typename T>
struct ix_Hash;

#define ix_DEFINE_INTEGER_HASH(T)                         \
    template <>                                           \
    struct ix_Hash<T>                                     \
    {                                                     \
        ix_FORCE_INLINE static constexpr size_t hash(T x) \
        {                                                 \
            return ix_hash64(static_cast<uint64_t>(x));   \
        }                                                 \
    }

ix_DEFINE_INTEGER_HASH(int8_t);
ix_DEFINE_INTEGER_HASH(int16_t);
ix_DEFINE_INTEGER_HASH(int32_t);
ix_DEFINE_INTEGER_HASH(int64_t);
ix_DEFINE_INTEGER_HASH(uint8_t);
ix_DEFINE_INTEGER_HASH(uint16_t);
ix_DEFINE_INTEGER_HASH(uint32_t);
ix_DEFINE_INTEGER_HASH(uint64_t);

#if ix_PLATFORM(WASM)
ix_DEFINE_INTEGER_HASH(unsigned long);
#endif

template <>
struct ix_Hash<float>
{
    ix_FORCE_INLINE static constexpr size_t hash(float x)
    {
        union Temp
        {
            float f;
            uint32_t i;
        } temp = {};
        temp.f = x; // cppcheck-suppress redundantInitialization
        return ix_hash64(static_cast<uint64_t>(temp.i));
    }
};

template <>
struct ix_Hash<double>
{
    ix_FORCE_INLINE static constexpr size_t hash(double x)
    {
        union Temp
        {
            double d;
            size_t i;
        } temp = {};
        temp.d = x; // cppcheck-suppress redundantInitialization
        return ix_hash64(temp.i);
    }
};

template <typename T>
struct ix_Hash<T *>
{
    ix_FORCE_INLINE static constexpr size_t hash(T *x)
    {
        return ix_hash64(reinterpret_cast<size_t>(x));
    }
};

struct ix_StringHash
{
    ix_FORCE_INLINE static constexpr size_t hash(const char *x)
    {
        return ix_hash_str(x);
    }
};
// FILE END: ./src/ix/ix_hash.hpp

class ix_Writer;

template <typename T>
struct ix_Print;

class ix_StringView
{
    const char *m_data;
    size_t m_length;

  public:
    ix_StringView() = default;

    ix_FORCE_INLINE constexpr ix_StringView(const char *data, size_t length)
        : m_data(data),
          m_length(length)
    {
    }

    ix_FORCE_INLINE constexpr explicit ix_StringView(decltype(nullptr))
        : m_data(nullptr),
          m_length(0)
    {
    }

    ix_FORCE_INLINE constexpr explicit ix_StringView(const char *text)
        : m_data(text),
          m_length(ix_strlen(text))
    {
    }

    ix_FORCE_INLINE constexpr ix_StringView(const char *data, const char *end)
        : m_data(data),
          m_length(static_cast<size_t>(end - m_data))
    {
    }

    ix_FORCE_INLINE constexpr const char *data() const
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr size_t length() const
    {
        return m_length;
    }

    ix_FORCE_INLINE constexpr size_t hash() const
    {
        return ix_hash(m_data, m_length);
    }

    ix_FORCE_INLINE constexpr bool operator==(const ix_StringView &other) const
    {
        if (m_length != other.length())
        {
            return false;
        }

        for (size_t i = 0; i < m_length; i++)
        {
            if (m_data[i] != other.m_data[i])
            {
                return false;
            }
        }

        return true;
    }

    ix_FORCE_INLINE constexpr bool operator!=(const ix_StringView &other) const
    {
        return !(*this == other);
    }

    constexpr bool operator==(const char *other) const
    {
        if (*other == '\0' && m_length != 0)
        {
            return false;
        }

        if (*other != '\0' && m_length == 0)
        {
            return false;
        }

        for (size_t i = 0; i < m_length; i++)
        {
            if (other[i] == '\0')
            {
                return false;
            }

            if (m_data[i] != other[i])
            {
                return false;
            }
        }

        return true;
    }

    ix_FORCE_INLINE constexpr bool operator!=(const char *other) const
    {
        return !(*this == other);
    }
};

ix_FORCE_INLINE constexpr bool operator==(const char *lhs, const ix_StringView &rhs)
{
    return (rhs == lhs);
}

ix_FORCE_INLINE constexpr bool operator!=(const char *lhs, const ix_StringView &rhs)
{
    return (rhs != lhs);
}

template <>
struct ix_Hash<ix_StringView>
{
    ix_FORCE_INLINE static constexpr size_t hash(const ix_StringView &x)
    {
        return x.hash();
    }
};

template <>
struct ix_Print<ix_StringView>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, const ix_StringView &sv);
};
// FILE END: ./src/ix/ix_StringView.hpp
// FILE BEGIN: ./src/ix/ix_SystemManager.hpp

// FILE BEGIN: ./src/ix/ix_HashMapSingleArray.hpp

// FILE BEGIN: ./src/ix/ix_Pair.hpp


template <typename T1, typename T2>
struct ix_Pair
{
    T1 first;
    T2 second;
};

template <typename T1, typename T2>
ix_FORCE_INLINE constexpr bool operator==(const ix_Pair<T1, T2> &x, const ix_Pair<T1, T2> &y)
{
    return (x.first == y.first) && (x.second == y.second);
}

template <typename T1, typename T2>
struct ix_Hash<ix_Pair<T1, T2>>
{
    ix_FORCE_INLINE constexpr static size_t hash(const ix_Pair<T1, T2> &pair)
    {
        return ix_wymix(ix_Hash<T1>::hash(pair.first), ix_Hash<T2>::hash(pair.second));
    }
};

template <typename Key, typename Value>
struct ix_KVPair
{
    Key key;
    Value value;
};
// FILE END: ./src/ix/ix_Pair.hpp
// FILE BEGIN: ./src/ix/ix_bit.hpp


template <typename T>
int ix_popcount(T x);


#if ix_PLATFORM(WASM)
template <>
ix_FORCE_INLINE int ix_popcount(size_t x)
{
    return ix_popcount<uint32_t>(x);
}
#endif

template <typename T>
int ix_count_trailing_zeros(T x);


#if ix_PLATFORM(WASM)
template <>
ix_FORCE_INLINE int ix_count_trailing_zeros(size_t x)
{
    return ix_count_trailing_zeros<uint32_t>(x);
}
#endif

template <typename T>
int ix_count_leading_zeros(T x);


template <typename T>
T ix_ceil_2_power(T x);


#if ix_PLATFORM(WASM)
template <>
ix_FORCE_INLINE size_t ix_ceil_2_power(size_t x)
{
    return ix_ceil_2_power<uint32_t>(x);
}
#endif

uint32_t ix_bit_next_bit_set(uint32_t x);

template <typename T>
ix_FORCE_INLINE constexpr bool ix_is_power_of_2(T x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}
// FILE END: ./src/ix/ix_bit.hpp
// FILE BEGIN: ./src/ix/ix_hash_container_detail.hpp


namespace ix_detail::hash_container
{

constexpr uint32_t DISP_LENGTH = 8;
constexpr uint32_t HIGHER_HASH_LENGTH = 32 - DISP_LENGTH;
constexpr uint32_t HIGHER_HASH_MASK = (1UL << HIGHER_HASH_LENGTH) - 1;
constexpr size_t INITIAL_LENGTH = 8;
constexpr size_t MAX_LOAD_FACTOR_NUMERATOR = 8;
constexpr size_t MAX_LOAD_FACTOR_DENOMINATOR = 10;

struct Bucket
{
    uint32_t disp_and_higher_hash;
    uint32_t index;
};

uint32_t increment_disp(uint32_t disp_and_higher_hash);
void emplace_bucket(Bucket *bucket, Bucket bucket_to_emplace, Bucket *buckets_begin, Bucket *buckets_end);

} // namespace ix_detail::hash_container
// FILE END: ./src/ix/ix_hash_container_detail.hpp
// FILE BEGIN: ./src/ix/ix_min_max.hpp


template <typename T>
ix_FORCE_INLINE constexpr T ix_max(T x, T y)
{
    return (x < y) ? y : x;
}

template <typename T>
ix_FORCE_INLINE constexpr T ix_min(T x, T y)
{
    return (x < y) ? x : y;
}
// FILE END: ./src/ix/ix_min_max.hpp
// FILE BEGIN: ./src/ix/ix_wrap.hpp

// FILE BEGIN: ./src/ix/ix_limits.hpp


template <class T>
struct ix_numeric_limits
{
    static constexpr T min();
    static constexpr T max();
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = false;
};

template <>
struct ix_numeric_limits<int8_t>
{
    static constexpr int8_t min()
    {
        return (-127 - 1);
    }

    static constexpr int8_t max()
    {
        return 127;
    }

    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
};

template <>
struct ix_numeric_limits<int16_t>
{
    static constexpr int16_t min()
    {
        return (-32767 - 1);
    }

    static constexpr int16_t max()
    {
        return 32767;
    }

    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
};

template <>
struct ix_numeric_limits<int32_t>
{
    static constexpr int32_t min()
    {
        return (-2147483647 - 1);
    }

    static constexpr int32_t max()
    {
        return 2147483647;
    }

    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
};

template <>
struct ix_numeric_limits<int64_t>
{
    static constexpr int64_t min()
    {
        return (-9223372036854775807LL - 1);
    }

    static constexpr int64_t max()
    {
        return 9223372036854775807LL;
    }

    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
};

template <>
struct ix_numeric_limits<uint8_t>
{
    static constexpr uint8_t min()
    {
        return 0;
    }

    static constexpr uint8_t max()
    {
        return 0xFF;
    }

    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
};

template <>
struct ix_numeric_limits<uint16_t>
{
    static constexpr uint16_t min()
    {
        return 0;
    }

    static constexpr uint16_t max()
    {
        return 0xFFFF;
    }

    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
};

template <>
struct ix_numeric_limits<uint32_t>
{
    static constexpr uint32_t min()
    {
        return 0;
    }

    static constexpr uint32_t max()
    {
        return 0xFFFFFFFF;
    }

    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
};

#if ix_PLATFORM(WASM)
template <>
struct ix_numeric_limits<size_t>
{
    static constexpr size_t min()
    {
        return 0;
    }

    static constexpr size_t max()
    {
        return 0xFFFFFFFF;
    }

    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
};
#endif

template <>
struct ix_numeric_limits<uint64_t>
{
    static constexpr uint64_t min()
    {
        return 0;
    }

    static constexpr uint64_t max()
    {
        return 0xFFFFFFFFFFFFFFFF;
    }

    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
};

template <>
struct ix_numeric_limits<float>
{
    static constexpr float min()
    {
        return 1.17549435e-38F;
    }

    static constexpr float max()
    {
        return 3.40282347e+38F;
    }

    static constexpr float epsilon()
    {
        return 1.19209290e-7F;
    }

    static constexpr bool is_signed = true;
    static constexpr bool is_integer = false;
};

template <>
struct ix_numeric_limits<double>
{
    static constexpr double min()
    {
        return 2.2250738585072014e-308;
    }

    static constexpr double max()
    {
        return 1.7976931348623157e+308;
    }

    static constexpr double epsilon()
    {
        return 2.2204460492503131e-16;
    }

    static constexpr bool is_signed = true;
    static constexpr bool is_integer = false;
};
// FILE END: ./src/ix/ix_limits.hpp

template <typename T>
constexpr bool ix_can_add(T x, T y)
{
    if constexpr (!ix_numeric_limits<T>::is_integer || !ix_numeric_limits<T>::is_signed)
    {
        return true;
    }
    else
    {
        const bool no_overflow_max = (y <= 0) || (x <= ix_numeric_limits<T>::max() - y);
        const bool no_overflow_min = (y >= 0) || (x >= ix_numeric_limits<T>::min() - y);
        return (no_overflow_max && no_overflow_min);
    }
}

template <typename T>
constexpr bool ix_can_sub(T x, T y)
{
    if constexpr (!ix_numeric_limits<T>::is_integer || !ix_numeric_limits<T>::is_signed)
    {
        return true;
    }
    else
    {
        const bool no_overflow_max = (y >= 0) || (x <= ix_numeric_limits<T>::max() + y);
        const bool no_overflow_min = (y <= 0) || (x >= ix_numeric_limits<T>::min() + y);
        return (no_overflow_max && no_overflow_min);
    }
}

template <typename T>
ix_FORCE_INLINE constexpr T ix_wrap_increment(T x, T begin, T end)
{
    const T maybe_next = x + 1;
    // Any way to ensure that `cmov` is used?
    return ix_UNLIKELY(maybe_next == end) ? begin : maybe_next;
}
// FILE END: ./src/ix/ix_wrap.hpp

template <typename Key, typename Value, typename Hash = ix_Hash<Key>, typename Equal = ix_Equal<Key>>
class ix_HashMapSingleArray;

template <typename Value>
using ix_StringHashMapSingleArray = ix_HashMapSingleArray<const char *, Value, ix_StringHash, ix_StringEqual>;

template <typename Key, typename Value, typename Hash, typename Equal>
class ix_HashMapSingleArray
{
    using Bucket = ix_detail::hash_container::Bucket;
    using KVPair = ix_KVPair<Key, Value>;

    ix_Vector<KVPair> m_kv_pairs;
    ix_Vector<Bucket> m_buckets;

  public:
    ix_HashMapSingleArray() = default;

    // cppcheck-suppress noExplicitConstructor
    ix_HashMapSingleArray(const std::initializer_list<KVPair> &kv_pairs)
    {
        reserve(kv_pairs.size());
        for (const KVPair &kv_pair : kv_pairs)
        {
            emplace(kv_pair.key, kv_pair.value);
        }
    }

    ix_FORCE_INLINE size_t size() const
    {
        return m_kv_pairs.size();
    }

    ix_FORCE_INLINE size_t bucket_count() const
    {
        return m_buckets.size();
    }

    ix_FORCE_INLINE size_t capacity() const
    {
        using ix_detail::hash_container::MAX_LOAD_FACTOR_DENOMINATOR;
        using ix_detail::hash_container::MAX_LOAD_FACTOR_NUMERATOR;

        return m_buckets.size() * MAX_LOAD_FACTOR_NUMERATOR / MAX_LOAD_FACTOR_DENOMINATOR;
    }

    ix_FORCE_INLINE bool empty() const
    {
        return m_kv_pairs.empty();
    }

    void reserve(size_t size)
    {
        using ix_detail::hash_container::INITIAL_LENGTH;
        using ix_detail::hash_container::MAX_LOAD_FACTOR_DENOMINATOR;
        using ix_detail::hash_container::MAX_LOAD_FACTOR_NUMERATOR;

        m_kv_pairs.reserve(size);
        const size_t n_minimum = (size * MAX_LOAD_FACTOR_DENOMINATOR / MAX_LOAD_FACTOR_NUMERATOR) + 1;
        const size_t n = ix_max(INITIAL_LENGTH, ix_ceil_2_power(n_minimum));
        rehash(n);
    }

    void clear()
    {
        if (m_kv_pairs.empty())
        {
            return;
        }

        m_kv_pairs.clear();
        ix_memset(m_buckets.data(), 0, m_buckets.size() * sizeof(Bucket));
    }

    ix_FORCE_INLINE KVPair *begin()
    {
        return m_kv_pairs.begin();
    }

    ix_FORCE_INLINE const KVPair *begin() const
    {
        return m_kv_pairs.begin();
    }

    ix_FORCE_INLINE KVPair *end()
    {
        return m_kv_pairs.end();
    }

    ix_FORCE_INLINE const KVPair *end() const
    {
        return m_kv_pairs.end();
    }

    ix_FORCE_INLINE Value &insert(const Key &key, const Value &value)
    {
        return emplace(key, value);
    }

    ix_FORCE_INLINE Value &insert(const Key &key, Value &&value)
    {
        return emplace(key, ix_move(value));
    }

    ix_FORCE_INLINE Value &insert(Key &&key, const Value &value)
    {
        return emplace(ix_move(key), value);
    }

    ix_FORCE_INLINE Value &insert(Key &&key, Value &&value)
    {
        return emplace(ix_move(key), ix_move(value));
    }

    template <typename... Args>
    ix_FORCE_INLINE Value &emplace(const Key &key, Args &&...args)
    {
        return emplace(ix_move(Key(key)), ix_forward<Args>(args)...);
    }

    template <typename... Args>
    Value &emplace(Key &&key, Args &&...args)
    {
        if (ix_UNLIKELY(m_kv_pairs.size() >= capacity()))
        {
            rehash();
        }

        const Bucket *bucket = find_or_emplace_bucket(key);
        const size_t index = bucket->index;
        const bool already_exists = (index != m_kv_pairs.size());
        if (already_exists)
        {
            m_kv_pairs[index].value = Value(ix_forward<Args>(args)...);
            return m_kv_pairs[index].value;
        }

        return m_kv_pairs.emplace_back(KVPair{ix_move(key), Value(ix_forward<Args>(args)...)}).value;
    }

    const Value &operator[](const Key &key) const
    {
        const Bucket *bucket = find_bucket(key);
        ix_ASSERT(bucket != nullptr);
        return m_kv_pairs[bucket->index].value;
    }

    ix_FORCE_INLINE bool contains(const Key &key) const
    {
        const Bucket *bucket = find_bucket(key);
        return (bucket != nullptr);
    }

    Value *find(const Key &key)
    {
        const Bucket *bucket = find_bucket(key);
        return (bucket == nullptr) ? nullptr : &m_kv_pairs[bucket->index].value;
    }

    const Value *find(const Key &key) const
    {
        const Bucket *bucket = find_bucket(key);
        return (bucket == nullptr) ? nullptr : &m_kv_pairs[bucket->index].value;
    }

    Value &get_or(const Key &key, Value &value) // cppcheck-suppress constParameterReference
    {
        const Bucket *bucket = find_bucket(key);
        return (bucket == nullptr) ? value : m_kv_pairs[bucket->index].value;
    }

    const Value &get_or(const Key &key, const Value &value) const
    {
        const Bucket *bucket = find_bucket(key);
        return (bucket == nullptr) ? value : m_kv_pairs[bucket->index].value;
    }

    void erase(const Key &key)
    {
        using ix_detail::hash_container::HIGHER_HASH_LENGTH;
        using ix_detail::hash_container::HIGHER_HASH_MASK;

        Bucket *bucket_to_erase = find_bucket(key);
        if (bucket_to_erase == nullptr)
        {
            return;
        }

        const uint32_t erasing_kv_pair_index = bucket_to_erase->index;
        Bucket *buckets_begin = m_buckets.begin();
        Bucket *buckets_end = m_buckets.end();
        Bucket *bucket = ix_wrap_increment(bucket_to_erase, buckets_begin, buckets_end);
        Bucket *prev_bucket = bucket_to_erase;
        while ((HIGHER_HASH_MASK + (1 << HIGHER_HASH_LENGTH)) < bucket->disp_and_higher_hash)
        {
            prev_bucket->index = bucket->index;
            prev_bucket->disp_and_higher_hash = bucket->disp_and_higher_hash - (1 << HIGHER_HASH_LENGTH);
            prev_bucket = bucket;
            bucket = ix_wrap_increment(bucket, buckets_begin, buckets_end);
        }
        prev_bucket->disp_and_higher_hash = 0;

        if (ix_LIKELY(erasing_kv_pair_index != m_kv_pairs.size() - 1))
        {
            const uint32_t last_kv_pair_index = static_cast<uint32_t>(m_kv_pairs.size() - 1);
            KVPair &&last_kv_pair = ix_move(m_kv_pairs[last_kv_pair_index]);
            Bucket *last_kv_pair_bucket = find_bucket(last_kv_pair.key);
            last_kv_pair_bucket->index = erasing_kv_pair_index;
            m_kv_pairs[erasing_kv_pair_index].key = ix_move(last_kv_pair.key);
            m_kv_pairs[erasing_kv_pair_index].value = ix_move(last_kv_pair.value);
        }

        m_kv_pairs.pop_back();
    }

  private:
    void rehash(size_t num_new_buckets = 0)
    {
        using ix_detail::hash_container::DISP_LENGTH;
        using ix_detail::hash_container::HIGHER_HASH_LENGTH;
        using ix_detail::hash_container::HIGHER_HASH_MASK;
        using ix_detail::hash_container::increment_disp;
        using ix_detail::hash_container::INITIAL_LENGTH;

        if (ix_LIKELY(num_new_buckets == 0))
        {
            num_new_buckets = ix_max(INITIAL_LENGTH, 2 * m_buckets.size());
        }

        m_buckets.resize(num_new_buckets);
        ix_memset(m_buckets.data(), 0, num_new_buckets * sizeof(Bucket));

        const uint64_t buckets_mask = static_cast<uint32_t>(num_new_buckets - 1);
        Bucket *buckets_begin = m_buckets.begin();
        Bucket *buckets_end = m_buckets.end();
        for (size_t i = 0; i < m_kv_pairs.size(); i++)
        {
            const size_t hash = Hash::hash(m_kv_pairs[i].key);
            const uint32_t higher_hash = static_cast<uint32_t>(hash >> (sizeof(size_t) * 8 - 32 + DISP_LENGTH));
            uint32_t disp_and_higher_hash = (1 << HIGHER_HASH_LENGTH) | higher_hash;
            Bucket *new_bucket_pos = &m_buckets[hash & buckets_mask];

            while (disp_and_higher_hash <= new_bucket_pos->disp_and_higher_hash)
            {
                disp_and_higher_hash = increment_disp(disp_and_higher_hash);
                new_bucket_pos = ix_wrap_increment(new_bucket_pos, buckets_begin, buckets_end);
            }

            Bucket new_bucket{disp_and_higher_hash, static_cast<uint32_t>(i)};
            emplace_bucket(new_bucket_pos, new_bucket, buckets_begin, buckets_end);
        }
    }

    ix_FORCE_INLINE const Bucket *find_bucket(const Key &key) const
    {
        return const_cast<ix_HashMapSingleArray *>(this)->find_bucket_kernel<false>(key);
    }

    ix_FORCE_INLINE Bucket *find_bucket(const Key &key)
    {
        return find_bucket_kernel<false>(key);
    }

    ix_FORCE_INLINE Bucket *find_or_emplace_bucket(const Key &key)
    {
        return find_bucket_kernel<true>(key);
    }

    template <bool emplace_bucket_if_not_found>
    Bucket *find_bucket_kernel(const Key &key)
    {
        using ix_detail::hash_container::DISP_LENGTH;
        using ix_detail::hash_container::HIGHER_HASH_LENGTH;
        using ix_detail::hash_container::increment_disp;

        if constexpr (!emplace_bucket_if_not_found)
        {
            if (ix_UNLIKELY(m_kv_pairs.empty()))
            {
                return nullptr;
            }
        }

        const size_t hash = Hash::hash(key);
        const uint32_t higher_hash = static_cast<uint32_t>(hash >> (sizeof(size_t) * 8 - 32 + DISP_LENGTH));
        uint32_t disp_and_higher_hash = (1 << HIGHER_HASH_LENGTH) | higher_hash;
        const uint64_t buckets_mask = static_cast<uint32_t>(m_buckets.size() - 1);
        Bucket *bucket = &m_buckets[hash & buckets_mask];
        Bucket *buckets_begin = m_buckets.begin();
        Bucket *buckets_end = m_buckets.end();

        while (true)
        {
            if (disp_and_higher_hash == bucket->disp_and_higher_hash)
            {
                if (Equal::equal(key, m_kv_pairs[bucket->index].key))
                {
                    return bucket;
                }
            }
            else if (disp_and_higher_hash > bucket->disp_and_higher_hash)
            {
                break;
            }
            disp_and_higher_hash = increment_disp(disp_and_higher_hash);
            bucket = ix_wrap_increment(bucket, buckets_begin, buckets_end);
        }

        if constexpr (emplace_bucket_if_not_found)
        {
            Bucket new_bucket{disp_and_higher_hash, static_cast<uint32_t>(m_kv_pairs.size())};
            emplace_bucket(bucket, new_bucket, buckets_begin, buckets_end);
            return bucket;
        }
        else
        {
            return nullptr;
        }
    }
};
// FILE END: ./src/ix/ix_HashMapSingleArray.hpp
// FILE BEGIN: ./src/ix/ix_Result.hpp


class ix_StringView;
class ix_Writer;

template <typename T>
struct ix_Print;

enum [[nodiscard]] ix_ResultKind : uint8_t;

ix_StringView ix_result_kind_to_string(ix_ResultKind kind);
ix_StringView ix_result_kind_to_description(ix_ResultKind kind);

enum ix_ResultKind : uint8_t
{
    ix_OK,
    ix_ERROR_SOMETHING_WENT_WRONG,

    ix_ERROR_ALREADY_INITIALIZED,
    ix_ERROR_BUFFER_TOO_SMALL,
    ix_ERROR_DIR_ACCESS_DENIED,
    ix_ERROR_DIR_ALREADY_EXISTS,
    ix_ERROR_DIR_NOT_FOUND,
    ix_ERROR_FILE_ACCESS_DENIED,
    ix_ERROR_FILE_ALREADY_EXISTS,
    ix_ERROR_FILE_NOT_FOUND,
    ix_ERROR_FILE_READ_FAILED,
    ix_ERROR_FILE_WRITE_FAILED,
    ix_ERROR_INVALID_INPUT,
    ix_ERROR_INVALID_PARAMETER,
    ix_ERROR_INVALID_PATH,
    ix_ERROR_INVALID_UTF16,
    ix_ERROR_INVALID_UTF8,
    ix_ERROR_NOT_FOUND,
    ix_ERROR_NOT_INITIALIZED,
    ix_ERROR_PATH_TOO_LONG,
    ix_ERROR_PROCESS_INVOCATION_FAILED,
};

class [[nodiscard]] ix_Result
{
    ix_ResultKind m_kind;

  public:
    ix_Result() = default;
    ix_Result(ix_ResultKind kind); // cppcheck-suppress noExplicitConstructor; NOLINT
    ix_ResultKind kind() const;
    ix_StringView string() const;
    ix_StringView description() const;
    bool is_ok() const;
    bool is_error() const;
    void assert_ok() const;
};

template <>
struct ix_Print<ix_Result>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, ix_Result result);
};

template <>
struct ix_Print<ix_ResultKind>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, ix_ResultKind kind);
};
// FILE END: ./src/ix/ix_Result.hpp

template <typename T>
class ix_HollowValue;

class ix_SystemManager
{
    ix_StringHashMapSingleArray<ix_Result (*)()> m_system_name_to_deinitializer;

  public:
    static bool is_initialized();
    static ix_SystemManager &initialize();
    static ix_SystemManager &get();
    static void deinitialize();

    ix_SystemManager(const ix_SystemManager &) = delete;
    ix_SystemManager(ix_SystemManager &&) = default;
    ix_SystemManager &operator=(const ix_SystemManager &) = delete;
    ix_SystemManager &operator=(ix_SystemManager &&) = default;

    bool is_initialized(const char *name) const;
    void assert_initialized(const char *name) const;

    // We cannot create `init_system(const char *name)' because that makes it impossible to strip unused init
    // functions from the final executable.
    ix_Result initialize_stdio();
    ix_Result initialize_time();
    ix_Result initialize_logger();
    ix_Result initialize_dummy();
    ix_Result initialize_dummy_nullptr();

  private:
    friend class ix_HollowValue<ix_SystemManager>;

    ix_SystemManager();
    ~ix_SystemManager();
};
// FILE END: ./src/ix/ix_SystemManager.hpp
// FILE BEGIN: ./src/ix/ix_ThreadPool.hpp

// FILE BEGIN: ./src/ix/ix_Function.hpp


template <size_t, typename>
class ix_FunctionN;

template <size_t N, typename ReturnType, typename... Args>
class ix_FunctionN<N, ReturnType(Args...)>
{
    ReturnType (*m_invoker)(const ix_FunctionN *, Args...);
    // Buffer of 16 bytes (global data + per function data) is almost always enough.
    alignas(void *) uint8_t m_buffer[N];

  public:
    constexpr ix_FunctionN() = default;

    template <typename Lambda, typename = ix_enable_if_t<!ix_is_same_v<ix_remove_cvref_t<Lambda>, ix_FunctionN>>>
    ix_FORCE_INLINE constexpr ix_FunctionN(Lambda &&f) // cppcheck-suppress noExplicitConstructor; NOLINT
        : m_invoker(+[](const ix_FunctionN *function, Args... args) -> ReturnType {
              const Lambda *f_restored = reinterpret_cast<const Lambda *>(function->m_buffer);
              return f_restored->operator()(ix_forward<Args>(args)...);
          })
    {
        static_assert(!ix_is_same_v<ix_remove_all_extents_t<Lambda>, ix_FunctionN<N, ReturnType(Args...)>>);
        static_assert(sizeof(Lambda) <= N);
        static_assert((8 % alignof(Lambda)) == 0);
        // static_assert(ix_is_lambda_v<F>); // Not available...
        static_assert(ix_is_trivially_copy_constructible_v<Lambda>);
        static_assert(ix_is_trivially_destructible_v<Lambda>);
        ix_memcpy(m_buffer, &f, sizeof(Lambda));
    }

    // cppcheck-suppress noExplicitConstructor; NOLINTNEXTLINE
    ix_FORCE_INLINE constexpr ix_FunctionN(ReturnType (*f)(Args...))
        : ix_FunctionN([f](Args... args) -> ReturnType { return f(args...); })
    {
    }

    ix_FORCE_INLINE constexpr ReturnType operator()(Args... args) const
    {
        return m_invoker(this, ix_forward<Args>(args)...);
    }

    ix_FORCE_INLINE constexpr void nullify()
    {
        m_invoker = nullptr;
    }

    ix_FORCE_INLINE constexpr bool is_null() const
    {
        return (m_invoker == nullptr);
    }

    ix_FORCE_INLINE constexpr bool is_valid() const
    {
        return (m_invoker != nullptr);
    }
};
// FILE END: ./src/ix/ix_Function.hpp
// FILE BEGIN: ./src/ix/ix_Thread.hpp


void ix_yield_this_thread();

// Loosely based on bx::Thread (by Branimir Karadzic, BSD 2-clause license).
class ix_Thread
{
    friend struct ix_ThreadDetail;

    ix_FunctionN<16, void()> m_user_entry;
    uint8_t m_detail[8];

  public:
    ~ix_Thread();
    ix_Thread() = default;
    ix_Thread(const ix_Thread &) = delete;
    ix_Thread &operator=(const ix_Thread &) = delete;
    ix_Thread(ix_Thread &&other);
    ix_Thread &operator=(ix_Thread &&);

    bool is_joinable() const;

    bool start(const ix_FunctionN<16, void()> &user_entry);
    void join();
};
// FILE END: ./src/ix/ix_Thread.hpp
// FILE BEGIN: ./src/ix/ix_random.hpp


class ix_Xoshiro256PlusPlus;

namespace ix_global
{
extern ix_Xoshiro256PlusPlus rng;
} // namespace ix_global

// From https://prng.di.unimi.it/xoshiro256plusplus.c
// (by David Blackman and Sebastiano Vigna, public domain)
class ix_Xoshiro256PlusPlus
{
    uint64_t m_state[4];

  public:
    constexpr ix_Xoshiro256PlusPlus(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
        : m_state{a, b, c, d}
    {
    }

    explicit ix_Xoshiro256PlusPlus(uint64_t x);
    void set_random_seed();
    void set_seed(uint64_t seed);

    template <typename T>
    T rand();

    template <typename T>
    T rand_non_negative();

    template <typename T>
    T rand_range(T min, T max);

    void fill_alphanumeric(char *buf, size_t length);

    template <typename T>
    void fill_unique(T *buf, size_t length);

    template <typename T>
    void fill_unique_range(T *buf, size_t length, T min, T max);
};

void ix_rand_set_seed(uint64_t seed);
void ix_rand_set_random_seed();

template <typename T>
ix_FORCE_INLINE T ix_rand()
{
    return ix_global::rng.rand<T>();
}

template <typename T>
ix_FORCE_INLINE T ix_rand_non_negative()
{
    return ix_global::rng.rand_non_negative<T>();
}
template <typename T>
ix_FORCE_INLINE T ix_rand_range(T min, T max)
{
    return ix_global::rng.rand_range(min, max);
}

ix_FORCE_INLINE void ix_rand_fill_alphanumeric(char *buf, size_t length)
{
    ix_global::rng.fill_alphanumeric(buf, length);
}

template <typename T>
ix_FORCE_INLINE void ix_rand_fill_unique(T *buf, size_t length)
{
    return ix_global::rng.fill_unique(buf, length);
}

template <typename T>
void ix_rand_fill_unique_range(T *buf, size_t length, T min, T max)
{
    return ix_global::rng.fill_unique_range(buf, length, min, max);
}

#if ix_PLATFORM(WASM)
template <>
ix_FORCE_INLINE size_t ix_Xoshiro256PlusPlus::rand()
{
    return rand<uint32_t>();
}

template <>
ix_FORCE_INLINE size_t ix_Xoshiro256PlusPlus::rand_non_negative()
{
    return rand<uint32_t>();
}

template <>
ix_FORCE_INLINE size_t ix_Xoshiro256PlusPlus::rand_range(size_t min, size_t max)
{
    return rand_range<uint32_t>(min, max);
}
#endif

// clang-format off




// clang-format on
// FILE END: ./src/ix/ix_random.hpp

// TODO: Benchmark (with realistic workloads)
// TODO: Remove busy-wait
// TODO: Implement the caching scheme described in the paper

class ix_Job;
class ix_Worker;

class ix_JobStealingDeque
{
    // cf.
    // - "Dynamic Circular Work-Stealing Deque" by David Chase and Yossi Lev
    // - "Correct and Efficient Work-Stealing for Weak Memory Models" by Nhat Minh Lê et. al.
    // - "Job System 2.0: Lock-Free Work Stealing" by Stefan Reinalter

    struct JobArray
    {
        uint64_t capacity;
        ix_Job **data;
    };

    ix_DISABLE_MSVC_WARNING_BEGIN
    ix_DISABLE_MSVC_WARNING(4324) // structure was padded due to alignment specifier
    alignas(64) uint64_t m_initial_capacity;
    alignas(64) uint64_t m_top;
    alignas(64) uint64_t m_bottom;
    alignas(64) JobArray *m_job_array;
    alignas(64) ix_Vector<void *> m_garbages;
    ix_DISABLE_MSVC_WARNING_END

  public:
    ix_JobStealingDeque(const ix_JobStealingDeque &) = delete;
    ix_JobStealingDeque &operator=(const ix_JobStealingDeque &) = delete;
    ix_JobStealingDeque(ix_JobStealingDeque &&other);
    ix_JobStealingDeque &operator=(ix_JobStealingDeque &&other);

    explicit ix_JobStealingDeque(size_t initial_capacity);
    ~ix_JobStealingDeque();
    void free_memory();
    size_t size() const;
    void push_bottom(ix_Job *job);
    ix_Job *pop_bottom();
    ix_Job *steal_top();
};

struct ix_ThreadPoolOption
{
    size_t num_worker_threads;
    size_t initial_job_queue_capacity;
    size_t job_pool_size;
    bool spwan_workes_lazily;

    ix_ThreadPoolOption();
};

class ix_ThreadPool
{
    friend class ix_Worker;

    ix_ThreadPoolOption m_option;
    ix_Vector<ix_Thread> m_extra_worker_threads;
    ix_Vector<ix_Worker> m_workers;

  public:
    explicit ix_ThreadPool(const ix_ThreadPoolOption &option = ix_ThreadPoolOption());
    ~ix_ThreadPool();

    ix_ThreadPool(const ix_ThreadPool &) = delete;
    ix_ThreadPool(ix_ThreadPool &&) = delete;
    ix_ThreadPool &operator=(const ix_ThreadPool &) = delete;
    ix_ThreadPool &operator=(ix_ThreadPool &&) = delete;

    size_t size() const;
    void wait(ix_Job *job);
    void shutdown();
    static ix_Worker *get_current_worker();
    static ix_Job *get_current_job();

    [[nodiscard]] ix_Job *add_empty_job();
    [[nodiscard]] ix_Job *add_job(const ix_FunctionN<16, void()> &job_entry);
    ix_Job *add_job(ix_Job *parent, const ix_FunctionN<16, void()> &job_entry);

  private:
    void spawn_worker();
};

class ix_Job
{
    friend class ix_ThreadPool;
    friend class ix_Worker;
    friend class ix_JobStealingDeque;

    ix_FunctionN<16, void()> entry;
    int64_t ttl = 0;
    ix_Job *parent;
};

class ix_Worker
{
    friend class ix_ThreadPool;
    friend class ix_Vector<ix_Worker>;

    uint64_t m_next_job_index = 0;
    uint64_t m_index;
    bool m_should_die = false;
    ix_ThreadPool *m_thread_pool;
    ix_Vector<ix_Job> m_job_pool;
    ix_Vector<ix_Job *> m_job_stack;
    ix_Xoshiro256PlusPlus m_rng;
    ix_JobStealingDeque m_jobs;

  public:
    size_t index() const;
    void wait(ix_Job *job);
    ix_Job *get_current_job() const;

    [[nodiscard]] ix_Job *add_empty_job();
    [[nodiscard]] ix_Job *add_job(const ix_FunctionN<16, void()> &job_entry);
    ix_Job *add_job(ix_Job *parent, const ix_FunctionN<16, void()> &job_entry);

  private:
    ix_Worker(ix_ThreadPool *thread_pool, size_t index, const ix_ThreadPoolOption &option);
    ix_Job *get_executable_job();
    ix_Job *allocate_job();
    ix_Job *allocate_job(const ix_FunctionN<16, void()> &job_entry, ix_Job *parent);
    void execute_job_maybe();
    static void finalize_job(ix_Job *job);
};
// FILE END: ./src/ix/ix_ThreadPool.hpp
// FILE BEGIN: ./src/ix/ix_UniquePointer.hpp

// FILE BEGIN: ./src/ix/ix_new_delete.hpp


template <typename T, typename... Args>
ix_FORCE_INLINE T *ix_new(Args &&...args)
{
    T *p = ix_ALLOC_STRUCT(T);
    return new (p) T(ix_forward<Args>(args)...);
}

template <typename T>
ix_FORCE_INLINE void ix_delete(T *p)
{
    p->~T();
    ix_FREE(p);
}
// FILE END: ./src/ix/ix_new_delete.hpp

// Use `free()` to release the pointer, assuming that it is allocated by `malloc()`, not `new T(...)`.
template <typename T>
class ix_UniquePointer;

template <typename T, typename... Args>
ix_FORCE_INLINE ix_UniquePointer<T> ix_make_unique(Args &&...args)
{
    return ix_UniquePointer<T>(ix_new<T>(ix_forward<Args>(args)...));
}

template <typename T>
ix_FORCE_INLINE ix_UniquePointer<T[]> ix_make_unique_array(size_t n)
{
    return ix_UniquePointer<T[]>(ix_ALLOC_ARRAY(T, n));
}

template <typename T>
class ix_UniquePointer
{
    T *m_ptr = nullptr;

  public:
    ix_UniquePointer() = default;
    ix_UniquePointer(const ix_UniquePointer &) = delete;
    ix_UniquePointer &operator=(const ix_UniquePointer &) = delete;

    ix_FORCE_INLINE explicit ix_UniquePointer(T *&&ptr)
        : m_ptr(ptr)
    {
    }

    ix_FORCE_INLINE ~ix_UniquePointer()
    {
        if (m_ptr == nullptr)
        {
            return;
        }

        ix_delete(m_ptr);
    }

    ix_UniquePointer(ix_UniquePointer &&other) noexcept
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    // cppcheck-suppress operatorEqVarError
    ix_UniquePointer &operator=(ix_UniquePointer &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        if (m_ptr != nullptr)
        {
            m_ptr->~T();
            ix_FREE(m_ptr);
        }

        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }

    ix_FORCE_INLINE void swap(T *&&p)
    {
        if (m_ptr != nullptr)
        {
            m_ptr->~T();
            ix_FREE(m_ptr);
        }

        m_ptr = p;
    }

    ix_FORCE_INLINE T *detach()
    {
        T *p = m_ptr;
        m_ptr = nullptr;
        return p;
    }

    ix_FORCE_INLINE T *get()
    {
        return m_ptr;
    }

    ix_FORCE_INLINE const T *get() const
    {
        return m_ptr;
    }

    ix_FORCE_INLINE T &operator*()
    {
        return *m_ptr; // NOLINT(clang-analyzer-core.uninitialized.UndefReturn)
    }

    ix_FORCE_INLINE const T &operator*() const
    {
        return *m_ptr; // NOLINT(clang-analyzer-core.uninitialized.UndefReturn)
    }

    ix_FORCE_INLINE T *operator->()
    {
        return m_ptr;
    }

    ix_FORCE_INLINE const T *operator->() const
    {
        return m_ptr;
    }
};

template <typename T>
class ix_UniquePointer<T[]>
{
    static_assert(ix_is_trivially_destructible_v<T>);
    T *m_ptr = nullptr;

  public:
    ix_UniquePointer() = default;
    ix_UniquePointer(const ix_UniquePointer &) = delete;
    ix_UniquePointer &operator=(const ix_UniquePointer &) = delete;

    ix_FORCE_INLINE explicit ix_UniquePointer(T *&&ptr)
        : m_ptr(ptr)
    {
    }

    ix_FORCE_INLINE ~ix_UniquePointer()
    {
        ix_FREE(m_ptr);
    }

    ix_UniquePointer(ix_UniquePointer &&other) noexcept
        : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }

    // cppcheck-suppress operatorEqVarError
    ix_UniquePointer &operator=(ix_UniquePointer &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        ix_FREE(m_ptr);
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }

    ix_FORCE_INLINE void realloc(size_t size)
    {
        m_ptr = ix_REALLOC_ARRAY(T, m_ptr, size);
    }

    ix_FORCE_INLINE void swap(T *&&p)
    {
        ix_FREE(m_ptr);
        m_ptr = p;
    }

    ix_FORCE_INLINE T *detach()
    {
        T *p = m_ptr;
        m_ptr = nullptr;
        return p;
    }

    ix_FORCE_INLINE T *get()
    {
        return m_ptr;
    }

    ix_FORCE_INLINE const T *get() const
    {
        return m_ptr;
    }

    ix_FORCE_INLINE T &operator[](size_t i)
    {
        return m_ptr[i]; // NOLINT(clang-analyzer-core.uninitialized.UndefReturn)
    }

    ix_FORCE_INLINE const T &operator[](size_t i) const
    {
        return m_ptr[i]; // NOLINT(clang-analyzer-core.uninitialized.UndefReturn)
    }
};
// FILE END: ./src/ix/ix_UniquePointer.hpp
// FILE BEGIN: ./src/ix/ix_Writer.hpp

// FILE BEGIN: ./src/ix/ix_Option.hpp


class ix_StringView;

template <typename T>
class [[nodiscard]] ix_Option
{
    ix_Result m_result;
    T m_value;

  public:
    constexpr ix_Option() = default;
    constexpr ix_Option(const ix_Option &) = default;
    constexpr ix_Option(ix_Option &&) = default;
    constexpr ix_Option &operator=(const ix_Option &) = default;
    constexpr ix_Option &operator=(ix_Option &&) = default;

    // cppcheck-suppress noExplicitConstructor; NOLINTNEXTLINE
    ix_FORCE_INLINE constexpr ix_Option(ix_ResultKind kind)
        : m_result(kind)
    {
        ix_ASSERT(kind != ix_OK);
    }

    // cppcheck-suppress noExplicitConstructor; NOLINTNEXTLINE
    ix_FORCE_INLINE constexpr ix_Option(const T &value)
        : m_result(ix_OK),
          m_value(value)
    {
    }

    // cppcheck-suppress noExplicitConstructor; NOLINTNEXTLINE
    ix_FORCE_INLINE constexpr ix_Option(T &&value)
        : m_result(ix_OK),
          m_value(ix_move(value))
    {
    }

    ix_FORCE_INLINE constexpr ix_ResultKind result_kind() const
    {
        return m_result.kind();
    }

    ix_FORCE_INLINE ix_StringView result_string() const
    {
        return m_result.string();
    }

    ix_FORCE_INLINE ix_StringView result_description() const
    {
        return m_result.description();
    }

    ix_FORCE_INLINE constexpr bool is_ok() const
    {
        return m_result.is_ok();
    }

    ix_FORCE_INLINE constexpr bool is_error() const
    {
        return m_result.is_error();
    }

    ix_FORCE_INLINE constexpr bool is(const T &x) const
    {
        return m_result.is_ok() && (m_value == x);
    }

    ix_FORCE_INLINE constexpr void assert_ok() const
    {
        ix_ASSERT(m_result.is_ok());
    }

    ix_FORCE_INLINE constexpr T &unwrap() &
    {
        ix_ASSERT(m_result.is_ok());
        return m_value;
    }

    ix_FORCE_INLINE constexpr const T &unwrap() const &
    {
        ix_ASSERT(m_result.is_ok());
        return m_value;
    }

    ix_FORCE_INLINE constexpr T &&unwrap() &&
    {
        ix_ASSERT(m_result.is_ok());
        return ix_move(m_value);
    }

    ix_FORCE_INLINE constexpr T unwrap_or(const T &v) const &
    {
        return m_result.is_ok() ? m_value : v;
    }

    ix_FORCE_INLINE constexpr T &&unwrap_or(T &&v) &&
    {
        return m_result.is_ok() ? ix_move(m_value) : ix_move(v);
    }
};

// FILE END: ./src/ix/ix_Option.hpp

#include <stdarg.h>

template <typename T>
class ix_UniquePointer;

template <typename T>
struct ix_Print;

class ix_Writer
{
    ix_Buffer m_buffer;
    const ix_FileHandle *m_file;

  public:
    explicit ix_Writer(size_t buffer_initial_size, const ix_FileHandle *file = nullptr);

    ~ix_Writer();
    ix_Writer(ix_Writer &&);
    ix_Writer(const ix_Writer &) = delete;
    ix_Writer &operator=(ix_Writer &&);
    ix_Writer &operator=(const ix_Writer &) = delete;

    ix_FORCE_INLINE static ix_Writer from_existing_region(char *p, size_t length, const ix_FileHandle *file = nullptr)
    {
        return ix_Writer(ix_Buffer::from_existing_region(p, length), file);
    }

    template <size_t N>
    ix_FORCE_INLINE static ix_Writer from_existing_array(char (&arr)[N], const ix_FileHandle *file = nullptr)
    {
        return ix_Writer::from_existing_region(arr, N, file);
    }

    const ix_FileHandle *file_handle() const;
    const char *data() const;
    size_t buffer_size() const;
    size_t buffer_capacity() const;
    void reserve_buffer_capacity(size_t new_capacity);

    template <typename T>
    ix_FORCE_INLINE size_t print(const T &v)
    {
        static_assert(ix_Print<T>::is_implemented);
        return ix_Print<T>::print(*this, v);
    }

    void write(const void *data, size_t data_size);
    void write_between(const void *begin, const void *end);
    void write_char(char c);
    void write_char_repeat(char c, size_t n);
    void write_string(const char *str);
    void end_string();
    ix_PRINTF_FORMAT(2, 0) size_t write_stringfv(ix_FORMAT_ARG const char *format, va_list args);
    ix_PRINTF_FORMAT(2, 3) size_t write_stringf(ix_FORMAT_ARG const char *format, ...);
    void flush();
    void clear();

    ix_UniquePointer<char[]> detach();
    void substitute(const ix_FileHandle *file);

  private:
    ix_Writer(ix_Buffer &&buffer, const ix_FileHandle *file);
};
// FILE END: ./src/ix/ix_Writer.hpp
// FILE BEGIN: ./src/ix/ix_atomic.hpp


enum ix_MemoryOrder : uint8_t
{
    ix_MEMORY_ORDER_RELAXED,
    ix_MEMORY_ORDER_CONSUME,
    ix_MEMORY_ORDER_ACQUIRE,
    ix_MEMORY_ORDER_RELEASE,
    ix_MEMORY_ORDER_ACQ_REL,
    ix_MEMORY_ORDER_SEQ_CST,
};

void ix_atomic_memory_barrier(ix_MemoryOrder order);

// RETURN x;
template <typename T>
T ix_atomic_load(const T volatile *x, ix_MemoryOrder order);

// x <- value;
template <typename T>
void ix_atomic_store(T volatile *x, T value, ix_MemoryOrder order);

// old <- *x;
// *x <- old + value;
// RETURN old;
template <typename T>
T ix_atomic_fetch_add(T volatile *x, T value, ix_MemoryOrder order);

// old <- *x;
// *x <- old - value;
// RETURN old;
template <typename T>
T ix_atomic_fetch_sub(T volatile *x, T value, ix_MemoryOrder order);

// IF (*x == *expected) THEN
//   *x <- desired;
//   RETURN true;
// ELSE
//   *expected <- *x;
//   return false;
// ENDIF
template <typename T>
bool ix_atomic_cas_weak(T volatile *x, T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure);

template <typename T>
bool ix_atomic_cas_strong(T volatile *x, T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure);

template <typename T>
class ix_Atomic
{
    T x;

  public:
    ix_Atomic() = default;

    ix_FORCE_INLINE T load(ix_MemoryOrder order) const
    {
        return ix_atomic_load(&x, order);
    }

    ix_FORCE_INLINE void store(T new_value, ix_MemoryOrder order)
    {
        ix_atomic_store(&x, new_value, order);
    }

    ix_FORCE_INLINE T fetch_add(T operand, ix_MemoryOrder order)
    {
        return ix_atomic_fetch_add(&x, operand, order);
    }

    ix_FORCE_INLINE T fetch_sub(T operand, ix_MemoryOrder order)
    {
        return ix_atomic_fetch_add(&x, operand, order);
    }

    ix_FORCE_INLINE bool cas_weak(T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure)
    {
        return ix_atomic_cas_weak(&x, expected, desired, success, failure);
    }

    ix_FORCE_INLINE bool cas_weak(T *expected, T desired, ix_MemoryOrder order)
    {
        return ix_atomic_cas_weak(&x, expected, desired, order, order);
    }

    ix_FORCE_INLINE bool cas_strong(T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure)
    {
        return ix_atomic_cas_strong(&x, expected, desired, success, failure);
    }

    ix_FORCE_INLINE bool cas_strong(T *expected, T desired, ix_MemoryOrder order)
    {
        return ix_atomic_cas_strong(&x, expected, desired, order, order);
    }
};

template <typename T>
ix_FORCE_INLINE T *ix_atomic_load(T *volatile const *x, ix_MemoryOrder order)
{
    if constexpr (sizeof(T *) == 8)
    {
        const uint64_t x_u64 = ix_atomic_load(reinterpret_cast<const uint64_t volatile *>(x), order);
        return reinterpret_cast<T *>(x_u64);
    }
    else if constexpr (sizeof(T *) == 4)
    {
        const uint32_t x_u32 = ix_atomic_load(reinterpret_cast<const uint32_t volatile *>(x), order);
        return reinterpret_cast<T *>(x_u32);
    }
    else
    {
        ix_UNREACHABLE();
    }
}

template <typename T>
ix_FORCE_INLINE void ix_atomic_store(T *volatile *x, T *value, ix_MemoryOrder order)
{
    if constexpr (sizeof(T *) == 8)
    {
        uint64_t value_8;
        ix_memcpy(&value_8, &value, sizeof(T *));
        ix_atomic_store(reinterpret_cast<uint64_t volatile *>(x), value_8, order);
        return;
    }
    else if constexpr (sizeof(T *) == 4)
    {
        uint32_t value_4;
        ix_memcpy(&value_4, &value, sizeof(T *));
        ix_atomic_store(reinterpret_cast<uint32_t volatile *>(x), value_4, order);
    }
    else
    {
        ix_UNREACHABLE();
    }
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_weak(
    T *volatile *x,
    T **expected,
    T *desired,
    ix_MemoryOrder success,
    ix_MemoryOrder failure
)
{
    if constexpr (sizeof(T *) == 8)
    {
        return ix_atomic_cas_weak<uint64_t>(
            reinterpret_cast<uint64_t volatile *>(x),
            reinterpret_cast<uint64_t *>(expected),
            reinterpret_cast<uint64_t>(desired),
            success,
            failure
        );
    }
    else if constexpr (sizeof(T *) == 4)
    {
        return ix_atomic_cas_weak<uint32_t>(
            reinterpret_cast<uint32_t volatile *>(x),
            reinterpret_cast<uint32_t *>(expected),
            reinterpret_cast<uint32_t>(desired),
            success,
            failure
        );
    }
    else
    {
        ix_UNREACHABLE();
    }
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_strong(
    T *volatile *x,
    T **expected,
    T *desired,
    ix_MemoryOrder success,
    ix_MemoryOrder failure
)
{
    if constexpr (sizeof(T *) == 8)
    {
        return ix_atomic_cas_strong<uint64_t>(
            reinterpret_cast<uint64_t volatile *>(x),
            reinterpret_cast<uint64_t *>(expected),
            reinterpret_cast<uint64_t>(desired),
            success,
            failure
        );
    }
    else if constexpr (sizeof(T *) == 4)
    {
        return ix_atomic_cas_strong<uint32_t>(
            reinterpret_cast<uint32_t volatile *>(x),
            reinterpret_cast<uint32_t *>(expected),
            reinterpret_cast<uint32_t>(desired),
            success,
            failure
        );
    }
    else
    {
        ix_UNREACHABLE();
    }
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_weak(T volatile *x, T *expected, T desired, ix_MemoryOrder order)
{
    return ix_atomic_cas_weak(x, expected, desired, order, order);
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_weak(T *volatile *x, T **expected, T *desired, ix_MemoryOrder order)
{
    return ix_atomic_cas_weak(x, expected, desired, order, order);
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_strong(T volatile *x, T *expected, T desired, ix_MemoryOrder order)
{
    return ix_atomic_cas_strong(x, expected, desired, order, order);
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_strong(T *volatile *x, T **expected, T *desired, ix_MemoryOrder order)
{
    return ix_atomic_cas_strong(x, expected, desired, order, order);
}

// clang-format off
#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif
// clang-format on
// FILE END: ./src/ix/ix_atomic.hpp
// FILE BEGIN: ./src/ix/ix_defer.hpp


template <typename F>
class ix_Deferred
{
    F m_deferred_function;

  public:
    ix_Deferred(const ix_Deferred &) = delete;
    ix_Deferred(ix_Deferred &&) = delete;
    ix_Deferred &operator=(const ix_Deferred &) = delete;
    ix_Deferred &operator=(ix_Deferred &&) = delete;

    ix_FORCE_INLINE explicit ix_Deferred(F &&f)
        : m_deferred_function(ix_move(f))
    {
    }

    ix_FORCE_INLINE ~ix_Deferred()
    {
        m_deferred_function();
    }
};

template <typename F>
ix_FORCE_INLINE ix_Deferred<F> ix_defer(F &&f)
{
    return ix_Deferred<F>(ix_move(f));
}

// NOLINTNEXTLINE(bugprone-macro-parentheses)
#define ix_DEFER(lambda_expr) auto ix_PP_ADD_UNIQUE_ID(defer_) = ix_defer(lambda_expr)
// FILE END: ./src/ix/ix_defer.hpp
// FILE BEGIN: ./src/ix/ix_environment.hpp


size_t ix_hardware_concurrency();
size_t ix_process_id();
size_t ix_thread_id();

bool ix_is_valgrind_active();
// FILE END: ./src/ix/ix_environment.hpp
// FILE BEGIN: ./src/ix/ix_file.hpp


class ix_FileHandle;

enum ix_FileOpenOption : uint8_t
{
    ix_READ_ONLY,
    ix_WRITE_ONLY,
};

ix_Result ix_initialize_stdio();
bool ix_is_file(const char *path);
bool ix_is_directory(const char *path);
ix_Result ix_remove_file(const char *path);
ix_Result ix_create_directory(const char *path);
ix_Result ix_remove_directory(const char *path);
ix_Result ix_ensure_directories(const char *path);
ix_Option<ix_FileHandle> ix_open_file(const char *path, ix_FileOpenOption option);
ix_Option<ix_FileHandle> ix_create_directories_and_file(const char *path);
ix_Result ix_write_to_file(const char *filename, const char *data, size_t data_length);
ix_Result ix_write_string_to_file(const char *filename, const char *str);
ix_Option<ix_UniquePointer<char[]>> ix_load_file(
    const char *filename,
    size_t *data_length_out = nullptr,
    size_t padding_length = 1
);

class ix_FileHandle
{
    friend ix_Result ix_initialize_stdio();
    friend ix_Option<ix_FileHandle> ix_open_file(const char *path, ix_FileOpenOption option);

    union
    {
        int fd;
        HANDLE handle;
    } v;

  public:
    ix_FileHandle();
    ~ix_FileHandle();
    ix_FileHandle(const ix_FileHandle &other) = delete;
    ix_FileHandle &operator=(const ix_FileHandle &) = delete;
    ix_FileHandle(ix_FileHandle &&other);
    ix_FileHandle &operator=(ix_FileHandle &&other);

    void close();
    bool is_valid() const;
    HANDLE get_native_handle() const;
    int get_native_fd() const;
    void write(const void *data, size_t data_length) const;
    void write_string(const char *str) const;
    void write_char(char c) const;
    ix_PRINTF_FORMAT(2, 0) size_t write_stringfv(ix_FORMAT_ARG const char *format, va_list args) const;
    ix_PRINTF_FORMAT(2, 3) size_t write_stringf(ix_FORMAT_ARG const char *format, ...) const;
    size_t read(void *buffer, size_t buffer_length) const;
    ix_Option<ix_UniquePointer<char[]>> read_all(
        size_t *data_length_out = nullptr,
        size_t padding_length = 1,
        size_t first_read_size = 1024
    ) const;
    size_t size() const;
    void substitute(ix_FileHandle &&another_file);
    ix_FileHandle copy() const;

    static ix_FileHandle null(ix_FileOpenOption option = ix_WRITE_ONLY);
    static const ix_FileHandle &of_stdin();
    static const ix_FileHandle &of_stdout();
    static const ix_FileHandle &of_stderr();

  private:
    void invalidate();
};
// FILE END: ./src/ix/ix_file.hpp
// FILE BEGIN: ./src/ix/ix_filepath.hpp


bool ix_is_path_separator(char c);
char *ix_next_path_separator(const char *p);
size_t ix_dirname_length(const char *filename);
size_t ix_dirname_length(const char *filename, size_t filename_length);
size_t ix_basename_length(const char *filename);
size_t ix_basename_length(const char *filename, size_t filename_length);
// FILE END: ./src/ix/ix_filepath.hpp
// FILE BEGIN: ./src/ix/ix_scanf.hpp


void ix_skip_to_next_word(const char **c);
const char *ix_read_string(const char **c);
int ix_read_int(const char **c);
unsigned int ix_read_uint(const char **c);
float ix_read_float(const char **c);
void ix_next_line(const char **c);
void ix_skip_empty_lines(const char **c);
void ix_next_non_empty_line(const char **c);

template <typename T>
ix_Option<T> ix_string_convert(const char *s);

// FILE END: ./src/ix/ix_scanf.hpp


static constexpr const char *HELP_TEXT = R"(textunpck version 1.0.0

USAGE:
  textunpack --help
  textunpack [OPTIONS] [--unpack] ARCHIVE_FILE
  textunpack [OPTIONS] [--unpack] --stdin BASE_DIRECTORY
  textunpack --pack FILE...

OPTIONS:
  --list, -l                List the paths of entries in the archive.
  --stdin, -s               Read from stdin instead of FILE.
  --dry-run, -d             Process the archive file normally, but do not unpack them.
  --disable-warning, -W     Disable warning.
  --num-threads <N>, -n <N>  Secify the number of threads to use.

)";

static constexpr const char *ERROR_TEXT_INVALID_INVOCATION = "Invalid invocation: no cmd args provided!\n";
static constexpr const char *ERROR_TEXT_STDIN_LOAD_FAILED = "Failed to read from stdin.\n";
static constexpr const char *ERROR_TEXT_FILE_OPEN_FAILED = "Failed to open %s: %s\n";
static constexpr const char *ERROR_TEXT_FILE_READ_FAILED = "File load failed: %s\n";
static constexpr const char *ERROR_TEXT_NO_FILE = "No input file.\n";
static constexpr const char *ERROR_TEXT_TOO_MANY_FILES = "Too many input files. You must specify one input file.\n";
static constexpr const char *ERROR_TEXT_NO_DIRECTORY = "No base directory. You must specify one base directory.\n";
static constexpr const char *ERROR_TEXT_TOO_MANY_DIRECTORY =
    "Too many base directories. You must specify one base directory.\n";
static constexpr const char *ERROR_TEXT_INVALID_NUM_THREADS = "Invalid number of threads: %s\n";
static constexpr const char *ERROR_TEXT_NOT_A_NUMBER = "Not a number: %s\n";

struct G
{
    const ix_FileHandle *stdout_handle;
    const ix_FileHandle *stderr_handle;
    ix_Mutex stderr_handle_mutex;
    ix_CmdArgsEater *args;
    const char *input;
    const char *input_end;
    const char *input_filename;
    size_t input_filename_length;
    size_t input_dirname_length;
    const char *first_entry_path_start;

    int num_failed_entries;
    ix_Vector<ix_Writer> file_writers;
};

static int textunpack_unpack(G &g, size_t num_threads)
{
    ix_ThreadPoolOption thread_pool_option;
    thread_pool_option.num_worker_threads = num_threads;
    thread_pool_option.initial_job_queue_capacity = 128;
    thread_pool_option.job_pool_size = 128;
    thread_pool_option.spwan_workes_lazily = true;
    ix_ThreadPool pool(thread_pool_option);
    ix_Job *root = pool.add_empty_job();

    const size_t writer_buffer_size = 32 * 1024;
    const size_t writer_buffer_total_size = thread_pool_option.num_worker_threads * writer_buffer_size;
    ix_UniquePointer<char[]> writer_buffer = ix_make_unique_array<char>(writer_buffer_total_size);

    for (size_t i = 0; i < thread_pool_option.num_worker_threads; i++)
    {
        char *p = writer_buffer.get() + (i * writer_buffer_size);
        g.file_writers.emplace_back(ix_Writer::from_existing_region(p, writer_buffer_size));
    }

    g.num_failed_entries = 0;
    const char *entry_path_start = g.first_entry_path_start;
    while (entry_path_start != nullptr)
    {
        pool.add_job(root, [&g, entry_path_start]() {
            const size_t worker_index = ix_ThreadPool::get_current_worker()->index();
            char buf[ix_MAX_PATH + 1];
            ix_memcpy(buf, g.input_filename, g.input_dirname_length);
            ix_Writer &file_writer = g.file_writers[worker_index];
            const size_t entry_path_length = tup_calculate_entry_path_length(entry_path_start);
            if (g.input_dirname_length + entry_path_length > ix_MAX_PATH)
            {
                ix_atomic_fetch_add(&g.num_failed_entries, 1, ix_MEMORY_ORDER_SEQ_CST);
                g.stderr_handle_mutex.lock();
                g.stderr_handle->write_stringf(
                    "Path too long: %.*s%.*s\n",
                    static_cast<int>(g.input_dirname_length),
                    g.input_filename,
                    static_cast<int>(entry_path_length),
                    entry_path_start
                );
                g.stderr_handle_mutex.unlock();
                return;
            }
            ix_memcpy(buf + g.input_dirname_length, entry_path_start, entry_path_length);
            buf[g.input_dirname_length + entry_path_length] = '\0';
            const ix_Option<ix_FileHandle> maybe_file = ix_create_directories_and_file(buf);
            if (maybe_file.is_error())
            {
                ix_atomic_fetch_add(&g.num_failed_entries, 1, ix_MEMORY_ORDER_SEQ_CST);
                g.stderr_handle_mutex.lock();
                g.stderr_handle->write_stringf("Failed to open: %s\n", buf);
                g.stderr_handle_mutex.unlock();
                return;
            }
            file_writer.substitute(&maybe_file.unwrap());
            tup_unpack_entry(
                ix_StringView(entry_path_start, entry_path_length),
                g.input_end,
                &file_writer,
                g.stderr_handle
            );
            file_writer.flush();
        });

        entry_path_start = tup_find_next_entry_path_start(entry_path_start);
    }

    pool.wait(root);

    return g.num_failed_entries;
}

static int textunpack_dry_run(G &g)
{
    const char *entry_path_start = g.first_entry_path_start;
    while (entry_path_start != nullptr)
    {
        const ix_StringView entry_path(entry_path_start, tup_calculate_entry_path_length(entry_path_start));
        tup_unpack_entry(entry_path, g.input_end, nullptr, g.stderr_handle);
        entry_path_start = tup_find_next_entry_path_start(entry_path_start);
    }
    return 0;
}

static int textunpack_list_entries(G &g)
{
    ix_Writer stdout_writer(4096, g.stdout_handle);

    const char *entry_path_start = g.first_entry_path_start;
    while (entry_path_start != nullptr)
    {
        const size_t entry_path_length = tup_calculate_entry_path_length(entry_path_start);
        stdout_writer.write_stringf(
            "%.*s%.*s\n",
            static_cast<int>(g.input_dirname_length),
            g.input_filename,
            static_cast<int>(entry_path_length),
            entry_path_start
        );
        const ix_StringView entry_path(entry_path_start, entry_path_length);
        tup_unpack_entry(entry_path, g.input_end, nullptr, g.stderr_handle);
        entry_path_start = tup_find_next_entry_path_start(entry_path_start);
    }

    return 0;
}

static int textunpack_pack(G &g, const char *output_filename, const char *const *filenames, size_t num_files)
{
    ix_Buffer buffer(4096);
    for (size_t i = 0; i < num_files; i++)
    {
        const char *filename = filenames[i];
        buffer.push_stringf("#+EXPORT_BEGIN %s\n", filename);
        ix_Option<ix_FileHandle> file_opt = ix_open_file(filename, ix_READ_ONLY);
        if (file_opt.is_error())
        {
            g.stderr_handle
                ->write_stringf(ERROR_TEXT_FILE_OPEN_FAILED, filename, file_opt.result_description().data());
            return 1;
        }
        ix_Option<size_t> length_opt = buffer.load_file_handle(file_opt.unwrap());
        if (length_opt.is_error())
        {
            g.stderr_handle->write_stringf(ERROR_TEXT_FILE_READ_FAILED, filename);
            return 1;
        }
        buffer.push_stringf("\n#+EXPORT_END %s\n", filename);
    }

    ix_FileHandle output_file;
    if (output_filename == nullptr)
    {
        g.stdout_handle->write(buffer.data(), buffer.size());
    }
    else
    {
        const ix_Option<ix_FileHandle> output_file_opt = ix_open_file(output_filename, ix_WRITE_ONLY);
        if (output_file_opt.is_error())
        {
            g.stderr_handle->write_stringf(
                ERROR_TEXT_FILE_OPEN_FAILED,
                output_filename,
                output_file_opt.result_description().data()
            );
            return 1;
        }
        output_file_opt.unwrap().write(buffer.data(), buffer.size());
    }

    return 0;
}

static int textunpack_main(
    const ix_FileHandle &stdin_handle,
    const ix_FileHandle &stdout_handle,
    const ix_FileHandle &stderr_handle,
    ix_CmdArgsEater &&args
)
{
    if (args.size() == 0)
    {
        stderr_handle.write_string(ERROR_TEXT_INVALID_INVOCATION);
        return 1;
    }


    const bool show_help = args.eat_boolean({"--help", "-h"}) || (args.size() == 1);
    if (show_help)
    {
        stdout_handle.write_string(HELP_TEXT);
        return 0;
    }

    G g;

    ix_FileHandle null = ix_FileHandle::null();
    g.stdout_handle = &stdout_handle;
    g.stderr_handle = args.eat_boolean({"--disable-warning", "-W"}) ? &null : &stderr_handle;

    const bool pack = args.eat_boolean({"--pack", "p"});

    if (pack)
    {
        const char *output_filename = args.eat_kv({"--output", "-o"});
        if (args.size() == 1)
        {
            stderr_handle.write_string(ERROR_TEXT_NO_FILE);
            return 1;
        }
        return textunpack_pack(g, output_filename, args.argv() + 1, args.size() - 1);
    }

    const bool dry_run = args.eat_boolean({"--dry-run", "-d"});
    const bool read_from_stdin = args.eat_boolean({"--stdin", "-s"});
    const bool list_entries = args.eat_boolean({"--list", "-l"});
    const bool unpack = args.eat_boolean("--unpack") || (!list_entries && !dry_run && !pack); // default

    size_t num_threads = ix_hardware_concurrency();
    if (unpack)
    {
        const char *num_threads_str = args.eat_kv({"--num-threads", "-n"});
        if (num_threads_str != nullptr)
        {
            const ix_Option<uint64_t> result = ix_string_convert<uint64_t>(num_threads_str);
            if (result.is_error())
            {
                stderr_handle.write_stringf(ERROR_TEXT_NOT_A_NUMBER, num_threads_str);
                return 1;
            }
            num_threads = result.unwrap();
        }

        if (1024 < num_threads)
        {
            stderr_handle.write_stringf(ERROR_TEXT_INVALID_NUM_THREADS, num_threads_str);
            return 1;
        }
    }

    if (args.size() == 1)
    {
        stderr_handle.write_string(read_from_stdin ? ERROR_TEXT_NO_DIRECTORY : ERROR_TEXT_NO_FILE);
        return 1;
    }

    if (args.size() >= 3)
    {
        stderr_handle.write_string(read_from_stdin ? ERROR_TEXT_TOO_MANY_DIRECTORY : ERROR_TEXT_TOO_MANY_FILES);
        return 1;
    }

    ix_ASSERT(args.size() == 2);
    g.input_filename = args[1];

    ix_Buffer input_buffer(4096);
    if (read_from_stdin)
    {
        const ix_Option<size_t> maybe_size = input_buffer.load_file_handle(stdin_handle);
        if (maybe_size.is_error())
        {
            stderr_handle.write_string(ERROR_TEXT_STDIN_LOAD_FAILED);
            return 1;
        }
    }
    else
    {
        const ix_Option<ix_FileHandle> maybe_file = ix_open_file(g.input_filename, ix_READ_ONLY);
        if (maybe_file.is_error())
        {
            stderr_handle
                .write_stringf(ERROR_TEXT_FILE_OPEN_FAILED, g.input_filename, maybe_file.result_description().data());
            return 1;
        }

        const ix_Option<size_t> maybe_bytes_read = input_buffer.load_file_handle(maybe_file.unwrap());
        if (maybe_bytes_read.is_error())
        {
            stderr_handle.write_stringf(ERROR_TEXT_FILE_READ_FAILED, g.input_filename);
            return 1;
        }
    }

    input_buffer.push_char('\0');
    g.input = input_buffer.data();
    g.input_end = input_buffer.end();
    g.first_entry_path_start = tup_find_first_entry_path_start(g.input);

    g.input_filename_length = ix_strlen(g.input_filename);
    g.input_dirname_length =
        read_from_stdin ? g.input_filename_length : ix_dirname_length(g.input_filename, g.input_filename_length);

    if (dry_run)
    {
        return textunpack_dry_run(g);
    }

    if (list_entries)
    {
        return textunpack_list_entries(g);
    }

    if (unpack)
    {
        return textunpack_unpack(g, num_threads);
    }

    ix_UNREACHABLE();
}


int main(int argc, const char *argv[])
{
    auto &sm = ix_SystemManager::initialize();
    ix_DEFER([]() { ix_SystemManager::deinitialize(); });
    sm.initialize_stdio().assert_ok();
    sm.initialize_time().assert_ok();
    sm.initialize_logger().assert_ok();

    const int ret = textunpack_main(
        ix_FileHandle::of_stdin(),
        ix_FileHandle::of_stdout(),
        ix_FileHandle::of_stderr(),
        ix_CmdArgsEater(argc, argv)
    );
    return ret;
}
// FILE END: ./src/textunpack/textunpack_cli.cpp
// FILE BEGIN: ./src/ix/ix.cpp

#include <stdio.h>
#include <stdlib.h>

static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int64_t) == 8);
static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint64_t) == 8);

#if ix_PLATFORM(WASM)
static_assert(sizeof(size_t) == 4);
static_assert(sizeof(ptrdiff_t) == 4);
#else
static_assert(sizeof(size_t) == 8);
static_assert(sizeof(ptrdiff_t) == 8);
#endif

[[noreturn]] void ix_abort()
{
    abort();
}

[[noreturn]] void ix_abort_with_info(const char *file, int line, const char *msg)
{
    ix_UNUSED(fprintf(stderr, "\n%s:%d: %s\n", file, line, msg));
    ix_abort();
}
// FILE END: ./src/ix/ix.cpp
// FILE BEGIN: ./src/textunpack/textunpack.cpp


ix_FORCE_INLINE static bool is_entry_path_terminater(char c)
{
    return (c == '\n') || (c == '\0');
}

size_t tup_calculate_entry_path_length(const char *entry_path_start)
{
    const char *p = entry_path_start;
    while (!is_entry_path_terminater(*p))
    {
        p += 1;
    }

    return static_cast<size_t>(p - entry_path_start);
}

static const char *find_next_export(const char *p)
{
    while (true)
    {
        const char *candidate = ix_strstr(p, "#+EXPORT_");
        if (candidate == nullptr)
        {
            return nullptr;
        }

        if (*(candidate - 1) == '\n')
        {
            return candidate;
        }

        p = candidate + ix_strlen("#+EXPORT_");
    }
}

static void write_entry_fragment(ix_Writer *writer, const char *start, const char *end)
{
    const char *write_start = start;
    const char *p = start;
    while (p < end)
    {
        p = static_cast<const char *>(ix_memchr(p, '\'', static_cast<size_t>(end - p)));
        if (ix_LIKELY(p == nullptr))
        {
            p = end;
            break;
        }

        p += 1;
        const char *next_write_start = p;
        while (*p == '\'')
        {
            p += 1;
        }

        if (!ix_starts_with(p, "#+EXPORT_"))
        {
            continue;
        }

        p += ix_strlen("#+EXPORT_");

        const bool export_begin = ix_starts_with(p, "BEGIN ");
        const bool export_end = ix_starts_with(p, "END ");

        if (!export_begin && !export_end)
        {
            continue;
        }

        size_t entry_path_length = 0;
        if (export_begin)
        {
            p += ix_strlen("BEGIN ");
            entry_path_length = tup_calculate_entry_path_length(p);
        }

        if (export_end)
        {
            p += ix_strlen("END ");
            entry_path_length = tup_calculate_entry_path_length(p);
        }

        if (entry_path_length == 0)
        {
            continue;
        }

        writer->write_between(write_start, next_write_start - ix_strlen("'"));
        write_start = next_write_start;
    }

    writer->write_between(write_start, p);
}

// TODO: Return ix_Buffer
void tup_unpack_entry(
    const ix_StringView &entry_path,
    const char *archive_end,
    ix_Writer *entry_writer,
    const ix_FileHandle *err_handle
)
{
    const char *write_start = entry_path.data() + entry_path.length() + ix_strlen("\n");
    while (true)
    {
        const char *next_export = find_next_export(write_start);

    CHECK_NEXT_EXPORT:
        const bool entry_is_not_terminated = (next_export == nullptr);
        if (ix_UNLIKELY(entry_is_not_terminated))
        {
            if (entry_writer != nullptr)
            {
                entry_writer->write_between(write_start, archive_end);
            }
            if (err_handle != nullptr)
            {
                err_handle->write_stringf(
                    "Entry \"%.*s\" is not terminated properly.\n",
                    static_cast<int>(entry_path.length()),
                    entry_path.data()
                );
            }
            return;
        }

        const char *p = next_export + ix_strlen("#+EXPORT_");
        const bool export_begin = ix_starts_with(p, "BEGIN ") && //
                                  !is_entry_path_terminater(*(p + ix_strlen("BEGIN ")));
        const bool export_end = ix_starts_with(p, "END ") && //
                                !is_entry_path_terminater(*(p + ix_strlen("END ")));

        if (!export_begin && !export_end)
        {
            next_export = find_next_export(p);
            goto CHECK_NEXT_EXPORT;
        }

        bool entry_end = false;
        if (export_end)
        {
            const char *ending_path_start = next_export + ix_strlen("#+EXPORT_END ");
            const size_t ending_path_length = tup_calculate_entry_path_length(ending_path_start);
            const ix_StringView ending_path(ending_path_start, ending_path_length);
            entry_end = (ending_path == entry_path);
        }

        if (entry_writer != nullptr)
        {
            if (write_start < next_export)
            {
                const size_t offset = entry_end ? ix_strlen("\n") : 0;
                write_entry_fragment(entry_writer, write_start, next_export - offset);
            }
        }

        if (entry_end)
        {
            return;
        }

        write_start = next_export + (export_begin ? ix_strlen("#+EXPORT_BEGIN ") : ix_strlen("#+EXPORT_END "));
        while (!is_entry_path_terminater(*write_start))
        {
            write_start += 1;
        }
        if (*write_start == '\n')
        {
            write_start += ix_strlen("\n");
        }
    }
}

const char *tup_find_next_entry_path_start(const char *p)
{
    while (true)
    {
        const char *next_export_begin = ix_strstr(p, "\n#+EXPORT_BEGIN ");
        if (next_export_begin == nullptr)
        {
            return nullptr;
        }

        const char *entry_path_start = next_export_begin + ix_strlen("\n#+EXPORT_BEGIN ");
        const bool entry_path_empty = is_entry_path_terminater(*entry_path_start);
        if (entry_path_empty)
        {
            p = entry_path_start;
            continue;
        }

        return entry_path_start;
    }
}

const char *tup_find_first_entry_path_start(const char *p)
{
    if (ix_starts_with(p, "#+EXPORT_BEGIN"))
    {
        const char *entry_path_start = p + ix_strlen("#+EXPORT_BEGIN ");
        const bool entry_path_exists = !is_entry_path_terminater(*entry_path_start);
        if (entry_path_exists)
        {
            return entry_path_start;
        }
    }

    return tup_find_next_entry_path_start(p);
}

// FILE END: ./src/textunpack/textunpack.cpp
// FILE BEGIN: ./src/ix/ix_printf.cpp

#include <stdio.h>

ix_PRINTF_FORMAT(3, 0) int ix_vsnprintf(char *buf, size_t buf_length, const char *format, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args); // Copy must occur here.

    const int formatted_length_signed = vsnprintf(buf, buf_length, format, args);
    ix_ASSERT(formatted_length_signed >= 0);

    if (buf == nullptr)
    {
        va_end(args_copy);
        return formatted_length_signed;
    }

    const size_t required_length = static_cast<size_t>(formatted_length_signed) + 1;
    ix_ASSERT_FATAL(required_length <= buf_length, "ix_vsnprintf: Format string is too long");

    const int wrote_length = vsnprintf(buf, required_length, format, args_copy);
    ix_ASSERT(wrote_length == formatted_length_signed);
    ix_ASSERT(buf[wrote_length] == '\0');

    va_end(args_copy);

    return wrote_length;
}

ix_PRINTF_FORMAT(3, 4) int ix_snprintf(char *buf, size_t buf_length, ix_FORMAT_ARG const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int v = ix_vsnprintf(buf, buf_length, format, args);
    va_end(args);
    return v;
}

// FILE END: ./src/ix/ix_printf.cpp
// FILE BEGIN: ./src/ix/ix_Buffer.cpp


#include <fcntl.h>

static constexpr size_t BUFFER_SHAMT = ((sizeof(size_t) * 8) - 1);

bool ix_Buffer::is_resizeable() const
{
    return (m_capacity >> BUFFER_SHAMT) == 0;
}

ix_Buffer::ix_Buffer(size_t initial_capacity)
    : m_capacity(initial_capacity)
{
    if (initial_capacity != 0)
    {
        m_data = ix_MALLOC(char *, initial_capacity);
    }
}

ix_Buffer::~ix_Buffer()
{
    if (is_resizeable())
    {
        ix_FREE(m_data);
    }
}

ix_Buffer::ix_Buffer(ix_Buffer &&other) noexcept
    : m_data(other.m_data),
      m_size(other.m_size),
      m_capacity(other.m_capacity)
{
    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
}

ix_Buffer ix_Buffer::from_existing_region(char *p, size_t length)
{
    constexpr size_t MAX_SIZE = ((size_t{1} << BUFFER_SHAMT) - 1);
    ix_ASSERT_FATAL(length < MAX_SIZE, "ix_Buffer: region too large");
    const size_t fake_capacity = ix_SIZE_MAX - length;
    ix_ASSERT((fake_capacity >> BUFFER_SHAMT) == 1);
    ix_Buffer buffer;
    buffer.m_data = p;
    buffer.m_size = 0;
    buffer.m_capacity = fake_capacity;
    return buffer;
}

ix_Buffer &ix_Buffer::operator=(ix_Buffer &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    if (m_data != nullptr)
    {
        ix_FREE(m_data);
    }

    m_data = other.m_data;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;

    return *this;
}

const char *ix_Buffer::data() const
{
    return m_data;
}

char *ix_Buffer::data()
{
    return m_data;
}

size_t ix_Buffer::size() const
{
    return m_size;
}

size_t ix_Buffer::capacity() const
{
    const bool is_resizable = is_resizeable();
    const size_t fake_capacity = m_capacity;
    return is_resizable ? fake_capacity : (ix_SIZE_MAX - fake_capacity);
}


void ix_Buffer::reserve(size_t new_capacity)
{
    if (new_capacity <= m_capacity)
    {
        ix_ASSERT(new_capacity <= capacity());
        return;
    }

    m_data = ix_REALLOC(char *, m_data, new_capacity);
    m_capacity = new_capacity;
}

void ix_Buffer::reserve_aggressively(size_t minimum_capacity)
{
    if (minimum_capacity <= m_capacity)
    {
        ix_ASSERT(minimum_capacity <= capacity());
        return;
    }

    const size_t new_capacity = ix_max(ix_grow_array_size(m_capacity), minimum_capacity);
    m_data = ix_REALLOC(char *, m_data, new_capacity);
    m_capacity = new_capacity;
}


void ix_Buffer::ensure(size_t extra_capacity)
{
    reserve(m_size + extra_capacity);
}

void ix_Buffer::ensure_aggressively(size_t minimum_extra_capacity)
{
    reserve_aggressively(m_size + minimum_extra_capacity);
}

void *ix_Buffer::allocate(size_t size)
{
    ensure_aggressively(size);
    void *ans = static_cast<void *>(m_data + m_size);
    m_size += size;
    return ans;
}


void ix_Buffer::clear()
{
    m_size = 0;
}

void ix_Buffer::reset()
{
    ix_FREE(m_data);
    m_data = nullptr;
    m_capacity = 0;
    m_size = 0;
}


void ix_Buffer::push(const void *new_data, size_t new_data_size)
{
    if (new_data_size == 0)
    {
        return;
    }

    void *ans = allocate(new_data_size);
    ix_memcpy(ans, new_data, new_data_size);
}


void ix_Buffer::push_between(const void *start, const void *end)
{
    ix_ASSERT(start <= end);
    const ptrdiff_t len_i = static_cast<const uint8_t *>(end) - static_cast<const uint8_t *>(start);
    const size_t len = static_cast<size_t>(len_i);
    push(start, len);
}


void ix_Buffer::push_str(const char *str)
{
    push(str, ix_strlen(str));
}


void ix_Buffer::push_char(char c)
{
    ensure_aggressively(1);
    m_data[m_size] = c;
    m_size += 1;
}


void ix_Buffer::push_char_repeat(char c, size_t n)
{
    ensure_aggressively(n);
    ix_memset(m_data + m_size, c, n);
    m_size += n;
}


ix_PRINTF_FORMAT(2, 0) size_t ix_Buffer::push_stringfv(ix_FORMAT_ARG const char *format, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    const int formatted_length_signed = ix_vsnprintf(nullptr, 0, format, args);
    ix_ASSERT(formatted_length_signed >= 0);

    const size_t required_length = static_cast<size_t>(formatted_length_signed) + 1;

    ensure_aggressively(required_length);

    const int num_bytes_written_signed = ix_vsnprintf(end(), required_length, format, args_copy);
    ix_ASSERT(num_bytes_written_signed >= 0);
    va_end(args_copy);

    const size_t num_bytes_written = static_cast<size_t>(num_bytes_written_signed);
    ix_ASSERT(num_bytes_written == required_length - 1);
    m_size += num_bytes_written;
    ix_ASSERT(m_size <= m_capacity);

    return num_bytes_written;
}

ix_PRINTF_FORMAT(2, 3) size_t ix_Buffer::push_stringf(ix_FORMAT_ARG const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const size_t ret = push_stringfv(format, args);
    va_end(args);
    return ret;
}


void ix_Buffer::pop_back(size_t length)
{
    ix_ASSERT(length <= m_size);
    m_size -= length;
}


void ix_Buffer::add_size(size_t size)
{
    m_size += size;
    ix_ASSERT(m_size <= m_capacity);
}


ix_UniquePointer<char[]> ix_Buffer::detach()
{
    char *ans = m_data;
    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
    return ix_UniquePointer<char[]>(ix_move(ans));
}


void ix_Buffer::set_size(size_t size)
{
    ix_ASSERT(size < m_capacity);
    m_size = size;
}


const char *ix_Buffer::begin() const
{
    return m_data;
}

char *ix_Buffer::begin()
{
    return m_data;
}

const char *ix_Buffer::end() const
{
    return m_data + m_size;
}

char *ix_Buffer::end()
{
    return m_data + m_size;
}


bool ix_Buffer::empty() const
{
    return (m_size == 0);
}


ix_Option<size_t> ix_Buffer::load_file(const char *filename)
{
    ix_ASSERT(filename != nullptr);

    ix_Option<ix_FileHandle> maybe_file = ix_open_file(filename, ix_READ_ONLY);
    if (maybe_file.is_error())
    {
        return maybe_file.result_kind();
    }
    ix_FileHandle &file = maybe_file.unwrap();

    const size_t file_size = file.size();
    char *p = static_cast<char *>(allocate(file_size + 1));
    file.read(p, file_size);
    if (file_size == ix_SIZE_MAX)
    {
        return ix_ERROR_FILE_READ_FAILED;
    }
    p[file_size] = '\0';

    file.close();

    return file_size;
}


ix_Option<size_t> ix_Buffer::load_file_handle(const ix_FileHandle &file, size_t first_read_size)
{
    const size_t file_size = file.size();
    if ((file_size != ix_SIZE_MAX) && (file_size != 0))
    {
        first_read_size = file_size;
    }
    else if (first_read_size == 0)
    {
        first_read_size = ix_max(m_capacity - m_size, size_t{1024});
    }

    const size_t initial_size = m_size;
    size_t read_size = first_read_size;
    while (true)
    {
        ensure(read_size);
        const size_t bytes_read = file.read(m_data + m_size, read_size);
        if (bytes_read == ix_SIZE_MAX)
        {
            return ix_ERROR_FILE_READ_FAILED;
        }

        const bool consumed = (bytes_read == 0);
        if (consumed)
        {
            break;
        }

        m_size += bytes_read;

        if (bytes_read == read_size)
        {
            read_size *= 2;
        }
    }

    return (m_size - initial_size);
}


ix_Buffer ix_Buffer::from_file(const char *filename)
{
    ix_Buffer buffer;
    buffer.load_file(filename).unwrap(); // TODO: remove unwrap
    return buffer;
}

// FILE END: ./src/ix/ix_Buffer.cpp
// FILE BEGIN: ./src/ix/ix_memory.cpp


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr size_t ix_MEMORY_LIMIT = (1024 * 1024 * 256);

ix_ATTRIBUTE_MALLOC void *ix_malloc(size_t size)
{
    ix_ASSERT_FATAL(size <= ix_MEMORY_LIMIT, "ix_malloc(): Too much memory is requested");
    void *ans = malloc(size);
    ix_ASSERT(ans != nullptr);
    return ans;
}


ix_ATTRIBUTE_MALLOC void *ix_realloc(void *p, size_t size)
{
    ix_ASSERT_FATAL(size <= ix_MEMORY_LIMIT, "ix_realloc(): Too much memory is requested");
    void *ans = realloc(p, size);
    ix_ASSERT(ans != nullptr);
    return ans;
}


void ix_free(void *p)
{
    free(p);
}



size_t ix_grow_array_size(size_t size)
{
    return (size < 8) ? 8 : (2 * size);
}


void ix_memmove(void *dst, const void *src, size_t n)
{
    memmove(dst, src, n);
}

void ix_memset(void *dst, char val, size_t n)
{
    memset(dst, val, n);
}

int ix_memcmp(const void *a, const void *b, size_t n)
{
    return memcmp(a, b, n);
}

void ix_memcpy(void *dst, const void *src, size_t n)
{
    memcpy(dst, src, n);
}
// FILE END: ./src/ix/ix_memory.cpp
// FILE BEGIN: ./src/ix/ix_string.cpp


#include <stdlib.h>
#include <string.h>


bool ix_starts_with(const char *a, const char *b)
{
    if (ix_UNLIKELY(*b == '\0'))
    {
        return true;
    }

    while (*a == *b)
    {
        a += 1;
        b += 1;
        if (*b == '\0')
        {
            return true;
        }
    }

    return false;
}


bool ix_wstarts_with(const wchar_t *a, const wchar_t *b)
{
    if (ix_UNLIKELY(*b == L'\0'))
    {
        return true;
    }

    while (*a == *b)
    {
        a += 1;
        b += 1;
        if (*b == L'\0')
        {
            return true;
        }
    }

    return false;
}


bool ix_contains(const char *s, char c)
{
    return (strchr(s, c) != nullptr);
}


char *ix_strstr(char *haystack, const char *needle)
{
    return strstr(haystack, needle);
}

const char *ix_strstr(const char *haystack, const char *needle)
{
    return strstr(haystack, needle);
}


const char *ix_strchr(const char *haystack, char c)
{
    return strchr(haystack, c);
}


const void *ix_memchr(const char *haystack, char c, size_t length)
{
    return memchr(haystack, c, length);
}

void *ix_memchr(char *haystack, char c, size_t length)
{
    return memchr(haystack, c, length);
}


char *ix_memnext(char *haystack, char c)
{
    while (*haystack != c)
    {
        haystack += 1;
    }
    return haystack;
}

const char *ix_memnext(const char *haystack, char c)
{
    while (*haystack != c)
    {
        haystack += 1;
    }
    return haystack;
}


char *ix_memnext2(char *haystack, char c0, char c1)
{
    while ((*haystack != c0) && (*haystack != c1))
    {
        haystack += 1;
    }
    return haystack;
}

const char *ix_memnext2(const char *haystack, char c0, char c1)
{
    while ((*haystack != c0) && (*haystack != c1))
    {
        haystack += 1;
    }
    return haystack;
}


size_t ix_strlen_runtime(const char *s)
{
    return strlen(s);
}

int ix_strcmp_runtime(const char *a, const char *b)
{
    return strcmp(a, b);
}

size_t ix_wstrlen(const wchar_t *s)
{
    const wchar_t *start = s;
    while (*s != '\0')
    {
        s += 1;
    }
    return static_cast<size_t>(s - start);
}


int ix_wstrcmp(const wchar_t *a, const wchar_t *b)
{
    while (*a != L'\0')
    {
        if (*a != *b)
        {
            break;
        }

        a += 1;
        b += 1;
    }

    return (*a - *b);
}


bool ix_wildcard_match(const char *string, const char *wildcard)
{
    char w = *wildcard;
    char s = *string;

    while ((s != '\0') && (w != '*'))
    {
        if ((s != w) && (w != '?'))
        {
            return false;
        }
        wildcard += 1;
        string += 1;
        w = *wildcard;
        s = *string;
    }

    const char *backtrack_wildcard = nullptr;
    const char *backtrack_string = nullptr;
    while (s != '\0')
    {
        if (w == '*')
        {
            wildcard += 1;
            w = *wildcard;
            if (w == '\0')
            {
                return true;
            }
            backtrack_wildcard = wildcard;
            backtrack_string = string + 1;
        }
        else if ((w == s) || (w == '?'))
        {
            wildcard += 1;
            string += 1;
            w = *wildcard;
            s = *string;
        }
        else
        {
            wildcard = backtrack_wildcard;
            string = backtrack_string;
            w = *wildcard;
            s = *string;
            backtrack_string += 1;
        }
    }

    while (w == '*')
    {
        wildcard += 1;
        w = *wildcard;
    }

    return (w == '\0');
}

// FILE END: ./src/ix/ix_string.cpp
// FILE BEGIN: ./src/ix/ix_CmdArgsEater.cpp
// FILE BEGIN: ./src/ix/ix_Vector_util.hpp


template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, T &&x)
{
    if (vec.m_size + 1 > vec.m_capacity)
    {
        const auto pos_index = pos - vec.m_data;
        size_t new_capacity = ix_grow_array_size(vec.m_capacity);
        vec.reserve(new_capacity);
        pos = vec.m_data + pos_index;
    }

    T *new_element = vec.m_data + vec.m_size;

    if (new_element == pos)
    {
        new (new_element) T(ix_move(x));
        vec.m_size += 1;
        return;
    }

    T *last_element = new_element - 1;
    new (new_element) T(ix_move(*last_element));

    if constexpr (ix_is_trivially_move_assignable_v<T>)
    {
        size_t n = static_cast<size_t>(last_element - pos);
        ix_memmove(pos + 1, pos, sizeof(T) * n);
    }
    else
    {
        T *p = last_element;
        while (pos < p)
        {
            *p = ix_move(*(p - 1));
            p -= 1;
        }
    }

    *pos = ix_move(x);

    vec.m_size += 1;
}

template <typename Vec, typename T>
ix_FORCE_INLINE void ix_Vector_insert(Vec &vec, T *pos, const T &x)
{
    ix_Vector_insert(vec, pos, ix_move(T(x)));
}

template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, const T *start, const T *end)
{
    size_t n = static_cast<size_t>(end - start);
    size_t required_capacity = vec.m_size + n;
    if (required_capacity >= vec.m_capacity)
    {
        const auto pos_index = pos - vec.m_data;
        size_t new_capacity = ix_max(ix_grow_array_size(vec.m_capacity), required_capacity);
        vec.reserve(new_capacity);
        pos = vec.m_data + pos_index;
    }

    T *new_element_start = vec.m_data + vec.m_size;
    if (new_element_start == pos)
    {
        ix_bulk_copy_construct(new_element_start, start, n);
        vec.m_size += n;
        return;
    }

    const size_t m = static_cast<size_t>(new_element_start - pos);
    if (m >= n)
    {
        //             pos                  new_element_start
        //              |                          |
        //              |<----------- m ---------->|
        // |------------|--------------------------|
        //              ^                          |
        //              |                          |
        //       |=============|                   |
        //       |<---- n ---->|                   |
        //                                         |
        //                                         |<---- n ---->|
        // |------------|=============|--------------------------|
        //              |<---- n ---->|<----------- m ---------->|

        ix_bulk_copy_construct(new_element_start, new_element_start - n, n);

        if constexpr (ix_is_trivially_copy_assignable_v<T>)
        {
            ix_memmove(pos + n, pos, sizeof(T) * (m - n));
        }
        else
        {
            T *p = new_element_start - 1;
            T *p_last = pos + n;
            while (p_last <= p)
            {
                *p = *(p - n);
                p -= 1;
            }
        }

        ix_bulk_copy_assign(pos, start, n);
    }
    else
    {
        //             pos           new_element_start
        //              |                   |
        //              |<------- m ------->|
        // |------------|-------------------|
        //              ^                   |
        //              |                   |
        //   |==========================|   |
        //   |<----------- n ---------->|   |
        //                                  |
        //                                  |<----------- n ---------->|
        // |------------|==========================|-------------------|
        //              |<----------- n ---------->|<------- m ------->|

        const size_t num_from_external = n - m;
        ix_bulk_copy_construct(new_element_start, start + m, num_from_external);
        ix_bulk_copy_construct(new_element_start + num_from_external, pos, m);
        ix_bulk_copy_assign(pos, start, m);
    }

    vec.m_size += n;
}

template <typename Vec, typename T>
void ix_Vector_erase(Vec &vec, T *pos)
{
    if constexpr (ix_is_trivially_move_assignable_v<T>)
    {
        const size_t n = vec.m_size - static_cast<size_t>(pos - vec.m_data) - 1;
        ix_memmove(pos, pos + 1, sizeof(T) * n);
        (vec.m_data + vec.m_size - 1)->~T();
    }
    else
    {
        T *p = pos;
        const T *p_end = vec.m_data + vec.m_size;
        while (p < p_end - 1)
        {
            *p = ix_move(*(p + 1));
            p += 1;
        }
        p->~T();
    }

    vec.m_size -= 1;
}

template <typename Vec, typename T>
void ix_Vector_erase(Vec &vec, T *pos, size_t n)
{
    ix_ASSERT(vec.m_data <= pos);
    ix_ASSERT(n <= vec.m_size);

    if (n == 0)
    {
        return;
    }

    if constexpr (ix_is_trivially_move_assignable_v<T>)
    {
        const size_t num_elements_to_move = vec.m_size - static_cast<size_t>(pos - vec.m_data) - n;
        ix_memmove(pos, pos + n, sizeof(T) * num_elements_to_move);
        ix_bulk_destruct(vec.m_data + vec.m_size - 1 - n, n);
    }
    else
    {
        T *p = pos;
        const T *p_end = vec.m_data + vec.m_size;
        while (p < p_end - n)
        {
            *p = ix_move(*(p + n));
            p += 1;
        }
        p->~T();
    }

    vec.m_size -= n;
}
// FILE END: ./src/ix/ix_Vector_util.hpp

ix_CmdArgsEater::ix_CmdArgsEater(int argc, const char *const *argv)
{
    const size_t num_args = static_cast<size_t>(argc);
    m_args.reserve(num_args);
    for (size_t i = 0; i < num_args; i++)
    {
        m_args.emplace_back(argv[i]);
    }
}

// cppcheck-suppress noExplicitConstructor; NOLINT
ix_CmdArgsEater::ix_CmdArgsEater(std::initializer_list<const char *> args)
    : ix_CmdArgsEater(static_cast<int>(args.size()), const_cast<const char **>(args.begin()))
{
}

size_t ix_CmdArgsEater::size() const
{
    return m_args.size();
}

int ix_CmdArgsEater::argc() const
{
    return static_cast<int>(m_args.size());
}

const char *const *ix_CmdArgsEater::argv() const
{
    return &m_args[0];
}

const char *const &ix_CmdArgsEater::operator[](size_t index) const
{
    return m_args[index];
}

bool ix_CmdArgsEater::eat_boolean(const char *name)
{
    const size_t num_args = m_args.size();
    for (size_t i = 1; i < num_args; i++)
    {
        const char **arg = &m_args[i];
        if (ix_strcmp(name, *arg) == 0)
        {
            ix_Vector_erase(m_args, arg);
            return true;
        }
    }

    return false;
}

bool ix_CmdArgsEater::eat_boolean(ix_Span<const char *> names)
{
    bool exists = false;

    const size_t num_names = names.size();
    for (size_t i = 0; i < num_names; i++)
    {
        exists |= eat_boolean(names[i]);
    }

    return exists;
}

const char *ix_CmdArgsEater::eat_kv(const char *key)
{
    const size_t num_args = m_args.size();
    for (size_t i = 1; i < num_args - 1; i++)
    {
        const char **arg = &m_args[i];
        if (ix_strcmp(key, *arg) == 0)
        {
            const char *value = *(arg + 1);
            ix_Vector_erase(m_args, arg, 2);
            return value;
        }
    }

    return nullptr;
}

const char *ix_CmdArgsEater::eat_kv(ix_Span<const char *> keys)
{
    const char *value = nullptr;

    const size_t num_keys = keys.size();
    for (size_t i = 0; i < num_keys; i++)
    {
        const char *this_value = eat_kv(keys[i]);
        if (this_value != nullptr)
        {
            value = this_value;
        }
    }

    return value;
}

bool ix_CmdArgsEater::eat_kv_n(const char *key, size_t n, const char **out)
{
    const size_t num_args = m_args.size();

    if (m_args.size() <= n)
    {
        return false;
    }

    for (size_t i = 1; i < num_args - n; i++)
    {
        const char **arg = &m_args[i];
        if (ix_strcmp(key, *arg) == 0)
        {
            for (size_t j = 0; j < n; j++)
            {
                out[j] = *(arg + j + 1);
            }
            ix_Vector_erase(m_args, arg, n + 1);
            return true;
        }
    }

    return false;
}

// FILE END: ./src/ix/ix_CmdArgsEater.cpp
// FILE BEGIN: ./src/ix/ix_Mutex.cpp

#if ix_PLATFORM(WIN)
// FILE BEGIN: ./src/ix/ix_Windows.hpp


#if ix_PLATFORM(WIN)

#if !defined(NOGDICAPMASKS) && !defined(ix_WIN_INCLUDE_GDICAPMASKS)
#define NOGDICAPMASKS // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#endif
#if !defined(NOVIRTUALKEYCODES) && !defined(ix_WIN_INCLUDE_VIRTUALKEYCODES)
#define NOVIRTUALKEYCODES // VK_*
#endif
#if !defined(NOWINMESSAGES) && !defined(ix_WIN_INCLUDE_WINMESSAGES)
#define NOWINMESSAGES // WM_*, EM_*, LB_*, CB_*
#endif
#if !defined(NOWINSTYLES) && !defined(ix_WIN_INCLUDE_WINSTYLES)
#define NOWINSTYLES // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#endif
#if !defined(NOSYSMETRICS) && !defined(ix_WIN_INCLUDE_SYSMETRICS)
#define NOSYSMETRICS // SM_*
#endif
#if !defined(NOMENUS) && !defined(ix_WIN_INCLUDE_MENUS)
#define NOMENUS // MF_*
#endif
#if !defined(NOICONS) && !defined(ix_WIN_INCLUDE_ICONS)
#define NOICONS // IDI_*
#endif
#if !defined(NOKEYSTATES) && !defined(ix_WIN_INCLUDE_KEYSTATES)
#define NOKEYSTATES // MK_*
#endif
#if !defined(NOSYSCOMMANDS) && !defined(ix_WIN_INCLUDE_SYSCOMMANDS)
#define NOSYSCOMMANDS // SC_*
#endif
#if !defined(NORASTEROPS) && !defined(ix_WIN_INCLUDE_RASTEROPS)
#define NORASTEROPS // Binary and Tertiary raster ops
#endif
#if !defined(NOSHOWWINDOW) && !defined(ix_WIN_INCLUDE_SHOWWINDOW)
#define NOSHOWWINDOW // SW_*
#endif
#if !defined(OEMRESOURCE)
#define OEMRESOURCE // OEM Resource values
#endif
#if !defined(NOATOM) && !defined(ix_WIN_INCLUDE_ATOM)
#define NOATOM // Atom Manager routines
#endif
#if !defined(NOCLIPBOARD) && !defined(ix_WIN_INCLUDE_CLIPBOARD)
#define NOCLIPBOARD // Clipboard routines
#endif
#if !defined(NOCOLOR) && !defined(ix_WIN_INCLUDE_COLOR)
#define NOCOLOR // Screen colors
#endif
#if !defined(NOCTLMGR) && !defined(ix_WIN_INCLUDE_CTLMGR)
#define NOCTLMGR // Control and Dialog routines
#endif
#if !defined(NODRAWTEXT) && !defined(ix_WIN_INCLUDE_DRAWTEXT)
#define NODRAWTEXT // DrawText() and DT_*
#endif
#if !defined(NOGDI) && !defined(ix_WIN_INCLUDE_GDI)
#define NOGDI // All GDI defines and routines
#endif
#if !defined(NOKERNEL) && !defined(ix_WIN_INCLUDE_KERNEL)
#define NOKERNEL // All KERNEL defines and routines
#endif
#if !defined(NOUSER) && !defined(ix_WIN_INCLUDE_USER)
#define NOUSER // All USER defines and routines
#endif
#if !defined(NONLS) && !defined(ix_WIN_INCLUDE_NLS)
#define NONLS // All NLS defines and routines
#endif
#if !defined(NOMB) && !defined(ix_WIN_INCLUDE_MB)
#define NOMB // MB_* and MessageBox()
#endif
#if !defined(NOMEMMGR) && !defined(ix_WIN_INCLUDE_MEMMGR)
#define NOMEMMGR // GMEM_*, LMEM_*, GHND, LHND, associated routines
#endif
#if !defined(NOMETAFILE) && !defined(ix_WIN_INCLUDE_METAFILE)
#define NOMETAFILE // typedef METAFILEPICT
#endif
#if !defined(NOMINMAX) && !defined(ix_WIN_INCLUDE_MINMAX)
#define NOMINMAX // Macros min(a,b) and max(a,b)
#endif
#if !defined(NOMSG) && !defined(ix_WIN_INCLUDE_MSG)
#define NOMSG // typedef MSG and associated routines
#endif
#if !defined(NOOPENFILE) && !defined(ix_WIN_INCLUDE_OPENFILE)
#define NOOPENFILE // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#endif
#if !defined(NOSCROLL) && !defined(ix_WIN_INCLUDE_SCROLL)
#define NOSCROLL // SB_* and scrolling routines
#endif
#if !defined(NOSERVICE) && !defined(ix_WIN_INCLUDE_SERVICE)
#define NOSERVICE // All Service Controller routines, SERVICE_ equates, etc.
#endif
#if !defined(NOSOUND) && !defined(ix_WIN_INCLUDE_SOUND)
#define NOSOUND // Sound driver routines
#endif
#if !defined(NOTEXTMETRIC) && !defined(ix_WIN_INCLUDE_TEXTMETRIC)
#define NOTEXTMETRIC // typedef TEXTMETRIC and associated routines
#endif
#if !defined(NOWH) && !defined(ix_WIN_INCLUDE_WH)
#define NOWH // SetWindowsHook and WH_*
#endif
#if !defined(NOWINOFFSETS) && !defined(ix_WIN_INCLUDE_WINOFFSETS)
#define NOWINOFFSETS // GWL_*, GCL_*, associated routines
#endif
#if !defined(NOCOMM) && !defined(ix_WIN_INCLUDE_COMM)
#define NOCOMM // COMM driver routines
#endif
#if !defined(NOKANJI) && !defined(ix_WIN_INCLUDE_KANJI)
#define NOKANJI // Kanji support stuff.
#endif
#if !defined(NOHELP) && !defined(ix_WIN_INCLUDE_HELP)
#define NOHELP // Help engine interface.
#endif
#if !defined(NOPROFILER) && !defined(ix_WIN_INCLUDE_PROFILER)
#define NOPROFILER // Profiler interface.
#endif
#if !defined(NODEFERWINDOWPOS) && !defined(ix_WIN_INCLUDE_DEFERWINDOWPOS)
#define NODEFERWINDOWPOS // DeferWindowPos routines
#endif
#if !defined(NOMCX) && !defined(ix_WIN_INCLUDE_MCX)
#define NOMCX // Modem Configuration Extensions
#endif

ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wreserved-macro-identifier")

#if ix_ARCH(x64)
#define _AMD64_ 1 // NOLINT
#elif ix_ARCH(ARM64)
#define _ARM64_ 1 // NOLINT
#else
#error "Unknown architecture"
#endif

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

ix_DISABLE_CLANG_WARNING_END

#include <windef.h>


#endif
// FILE END: ./src/ix/ix_Windows.hpp
#include <synchapi.h>
#else
#include <pthread.h>
#endif

// Loosely based on bx::Thread (by Branimir Karadzic, BSD 2-clause license).
#if ix_PLATFORM(WIN)
using pthread_mutex_t = CRITICAL_SECTION;
using pthread_mutexattr_t = unsigned;

static inline int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    EnterCriticalSection(mutex);
    return 0;
}

static inline int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    LeaveCriticalSection(mutex);
    return 0;
}

static inline int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t * /*attr*/)
{
    InitializeCriticalSection(mutex);
    return 0;
}

static inline int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    DeleteCriticalSection(mutex);
    return 0;
}
#endif

ix_Mutex::ix_Mutex()
{
    static_assert(sizeof(pthread_mutex_t) <= sizeof(m_detail));
    pthread_mutex_t *handle = reinterpret_cast<pthread_mutex_t *>(m_detail);
    pthread_mutex_init(handle, nullptr);
}

ix_Mutex::~ix_Mutex()
{
    pthread_mutex_t *handle = reinterpret_cast<pthread_mutex_t *>(m_detail);
    pthread_mutex_destroy(handle);
}

void ix_Mutex::lock()
{
    pthread_mutex_t *handle = reinterpret_cast<pthread_mutex_t *>(m_detail);
    pthread_mutex_lock(handle);
}

void ix_Mutex::unlock()
{
    pthread_mutex_t *handle = reinterpret_cast<pthread_mutex_t *>(m_detail);
    pthread_mutex_unlock(handle);
}

// FILE END: ./src/ix/ix_Mutex.cpp
// FILE BEGIN: ./src/ix/ix_hash.cpp

size_t ix_hash_between(const void *start, const void *end)
{
    const ptrdiff_t length = static_cast<const uint8_t *>(end) - static_cast<const uint8_t *>(start);
    return ix_hash(start, static_cast<size_t>(length));
}

size_t ix_hash(const void *p, size_t length)
{
    return ix_hash(static_cast<const char *>(p), length);
}

// FILE END: ./src/ix/ix_hash.cpp
// FILE BEGIN: ./src/ix/ix_StringView.cpp

size_t ix_Print<ix_StringView>::print(ix_Writer &writer, const ix_StringView &sv)
{
    const size_t length = sv.length();
    writer.write(sv.data(), length);
    return length;
}

// FILE END: ./src/ix/ix_StringView.cpp
// FILE BEGIN: ./src/ix/ix_bit.cpp


#if ix_COMPILER(MSVC)
#include <intrin.h>
#endif

template <typename T>
int ix_popcount(T x)
{
    if constexpr (ix_is_same_v<T, uint32_t>)
    {
#if ix_COMPILER(MSVC)
        return static_cast<int>(__popcnt(x));
#else
        return __builtin_popcount(x);
#endif
    }
    else if constexpr (ix_is_same_v<T, uint64_t>)
    {
#if ix_COMPILER(MSVC)
        return static_cast<int>(__popcnt64(x));
#else
        return __builtin_popcountll(x);
#endif
    }
    ix_UNREACHABLE();
}



template <typename T>
int ix_count_trailing_zeros(T x)
{
    if constexpr (ix_is_same_v<T, uint32_t>)
    {
#if ix_COMPILER(MSVC)
        unsigned long ctz = 0;
        _BitScanForward(&ctz, x);
        return ctz;
#else
        return __builtin_ctz(x);
#endif
    }
    else if constexpr (ix_is_same_v<T, uint64_t>)
    {
#if ix_COMPILER(MSVC)
        unsigned long ctz = 0;
        _BitScanForward64(&ctz, x);
        return ctz;
#else
        return __builtin_ctzll(x);
#endif
    }

    ix_UNREACHABLE();
}



template <typename T>
int ix_count_leading_zeros(T x)
{
    if constexpr (ix_is_same_v<T, uint32_t>)
    {
#if ix_COMPILER(MSVC)
        unsigned long ctz = 0;
        _BitScanReverse(&ctz, x);
        return 31 - ctz;
#else
        return __builtin_clz(x);
#endif
    }
    else if constexpr (ix_is_same_v<T, uint64_t>)
    {
#if ix_COMPILER(MSVC)
        unsigned long ctz = 0;
        _BitScanReverse64(&ctz, x);
        return 63 - ctz;
#else
        return __builtin_clzll(x);
#endif
    }

    ix_UNREACHABLE();
}



// By Sean Eron Anderson (public domain)
// From http://graphics.stanford.edu/~seander/bithacks.html
uint32_t ix_bit_next_bit_set(uint32_t x)
{
    const uint32_t t = x | (x - 1); // t gets x's least significant 0 bits set to 1

    // Next set to 1 the most significant bit to change,
    // set to 0 the least significant ones, and add the necessary 1 bits.
    const uint32_t nt = ~t;
#if ix_COMPILER(MSVC)
    const uint32_t mnt = ~nt + 1;
#else
    const uint32_t mnt = -nt;
#endif
    return (t + 1) | (((nt & mnt) - 1) >> (ix_count_trailing_zeros(x) + 1));
}


template <typename T>
T ix_ceil_2_power(T x)
{
    if constexpr (ix_is_same_v<T, uint32_t>)
    {
        if (ix_popcount(x) <= 1)
        {
            return x;
        }
        const int i = ix_count_leading_zeros(x);
        return 1 << (32 - i);
    }
    else if constexpr (ix_is_same_v<T, uint64_t>)
    {
        if (ix_popcount(x) <= 1)
        {
            return x;
        }
        const int i = ix_count_leading_zeros(x);
        return 1ULL << (64 - i);
    }

    ix_UNREACHABLE();
}


// FILE END: ./src/ix/ix_bit.cpp
// FILE BEGIN: ./src/ix/ix_hash_container_detail.cpp

namespace ix_detail::hash_container
{

uint32_t increment_disp(uint32_t disp_and_higher_hash)
{
    constexpr uint32_t disp_inc = 1ULL << HIGHER_HASH_LENGTH;
    const uint32_t maybe_next = disp_and_higher_hash + disp_inc;
    return ix_LIKELY(disp_and_higher_hash < maybe_next) ? maybe_next : disp_and_higher_hash;
}

void emplace_bucket(Bucket *bucket, Bucket bucket_to_emplace, Bucket *buckets_begin, Bucket *buckets_end)
{
    while (bucket->disp_and_higher_hash != 0)
    {
        const Bucket temp = *bucket;
        *bucket = bucket_to_emplace;
        bucket = ix_wrap_increment(bucket, buckets_begin, buckets_end);
        bucket_to_emplace.index = temp.index;
        bucket_to_emplace.disp_and_higher_hash = increment_disp(temp.disp_and_higher_hash);
    }

    *bucket = bucket_to_emplace;
}

} // namespace ix_detail::hash_container
// FILE END: ./src/ix/ix_hash_container_detail.cpp
// FILE BEGIN: ./src/ix/ix_Result.cpp

ix_StringView ix_result_kind_to_string(ix_ResultKind kind)
{
#define IMPLEMENT_CASE(e) \
    case e:               \
        return ix_StringView(ix_PP_TO_STRING_LITERAL(e))

    switch (kind)
    {
        IMPLEMENT_CASE(ix_OK);
        IMPLEMENT_CASE(ix_ERROR_SOMETHING_WENT_WRONG);
        IMPLEMENT_CASE(ix_ERROR_ALREADY_INITIALIZED);
        IMPLEMENT_CASE(ix_ERROR_BUFFER_TOO_SMALL);
        IMPLEMENT_CASE(ix_ERROR_DIR_ACCESS_DENIED);
        IMPLEMENT_CASE(ix_ERROR_DIR_ALREADY_EXISTS);
        IMPLEMENT_CASE(ix_ERROR_DIR_NOT_FOUND);
        IMPLEMENT_CASE(ix_ERROR_FILE_ACCESS_DENIED);
        IMPLEMENT_CASE(ix_ERROR_FILE_ALREADY_EXISTS);
        IMPLEMENT_CASE(ix_ERROR_FILE_NOT_FOUND);
        IMPLEMENT_CASE(ix_ERROR_FILE_READ_FAILED);
        IMPLEMENT_CASE(ix_ERROR_FILE_WRITE_FAILED);
        IMPLEMENT_CASE(ix_ERROR_INVALID_INPUT);
        IMPLEMENT_CASE(ix_ERROR_INVALID_PARAMETER);
        IMPLEMENT_CASE(ix_ERROR_INVALID_PATH);
        IMPLEMENT_CASE(ix_ERROR_INVALID_UTF16);
        IMPLEMENT_CASE(ix_ERROR_INVALID_UTF8);
        IMPLEMENT_CASE(ix_ERROR_NOT_FOUND);
        IMPLEMENT_CASE(ix_ERROR_NOT_INITIALIZED);
        IMPLEMENT_CASE(ix_ERROR_PATH_TOO_LONG);
        IMPLEMENT_CASE(ix_ERROR_PROCESS_INVOCATION_FAILED);
        ix_CASE_EXHAUSTED();
    }

    ix_UNREACHABLE();

#undef IMPLEMENT_CASE
}

ix_StringView ix_result_kind_to_description(ix_ResultKind kind)
{
#define IMPLEMENT_CASE(e, str) \
    case e:                    \
        return ix_StringView(str)

    switch (kind)
    {
        IMPLEMENT_CASE(ix_OK, "OK");
        IMPLEMENT_CASE(ix_ERROR_SOMETHING_WENT_WRONG, "Something went wrong");
        IMPLEMENT_CASE(ix_ERROR_ALREADY_INITIALIZED, "Not initialized");
        IMPLEMENT_CASE(ix_ERROR_BUFFER_TOO_SMALL, "Buffer too small");
        IMPLEMENT_CASE(ix_ERROR_DIR_ACCESS_DENIED, "Directory access denied");
        IMPLEMENT_CASE(ix_ERROR_DIR_ALREADY_EXISTS, "Directory already exists");
        IMPLEMENT_CASE(ix_ERROR_DIR_NOT_FOUND, "Directory not found");
        IMPLEMENT_CASE(ix_ERROR_FILE_ACCESS_DENIED, "File access denied");
        IMPLEMENT_CASE(ix_ERROR_FILE_ALREADY_EXISTS, "File already exists");
        IMPLEMENT_CASE(ix_ERROR_FILE_NOT_FOUND, "File not found");
        IMPLEMENT_CASE(ix_ERROR_FILE_READ_FAILED, "File read failed");
        IMPLEMENT_CASE(ix_ERROR_FILE_WRITE_FAILED, "File write failed");
        IMPLEMENT_CASE(ix_ERROR_INVALID_INPUT, "Invlid input");
        IMPLEMENT_CASE(ix_ERROR_INVALID_PARAMETER, "Invalid parameter");
        IMPLEMENT_CASE(ix_ERROR_INVALID_PATH, "Invalid path");
        IMPLEMENT_CASE(ix_ERROR_INVALID_UTF16, "Invalid UTF-16 string");
        IMPLEMENT_CASE(ix_ERROR_INVALID_UTF8, "Invalid UTF-8 string");
        IMPLEMENT_CASE(ix_ERROR_NOT_FOUND, "Not found");
        IMPLEMENT_CASE(ix_ERROR_NOT_INITIALIZED, "Not initialized");
        IMPLEMENT_CASE(ix_ERROR_PATH_TOO_LONG, "Path too long");
        IMPLEMENT_CASE(ix_ERROR_PROCESS_INVOCATION_FAILED, "Failed to invoke a process");
        ix_CASE_EXHAUSTED();
    }

    ix_UNREACHABLE();

#undef IMPLEMENT_CASE
}

ix_Result::ix_Result(ix_ResultKind kind)
    : m_kind(kind)
{
}

ix_ResultKind ix_Result::kind() const
{
    return m_kind;
}

bool ix_Result::is_ok() const
{
    return (m_kind == ix_OK);
}

bool ix_Result::is_error() const
{
    return (m_kind != ix_OK);
}

void ix_Result::assert_ok() const
{
    ix_ASSERT(m_kind == ix_OK);
}

ix_StringView ix_Result::string() const
{
    return ix_result_kind_to_string(m_kind);
}

ix_StringView ix_Result::description() const
{
    return ix_result_kind_to_description(m_kind);
}

size_t ix_Print<ix_Result>::print(ix_Writer &writer, ix_Result result)
{
    const ix_StringView sv = result.string();
    const size_t length = sv.length();
    writer.write(sv.data(), length);
    return length;
}

size_t ix_Print<ix_ResultKind>::print(ix_Writer &writer, ix_ResultKind kind)
{
    const ix_StringView sv = ix_result_kind_to_string(kind);
    const size_t length = sv.length();
    writer.write(sv.data(), length);
    return length;
}

// FILE END: ./src/ix/ix_Result.cpp
// FILE BEGIN: ./src/ix/ix_SystemManager.cpp
// FILE BEGIN: ./src/ix/ix_HollowValue.hpp


template <typename T>
class ix_HollowValue
{
    alignas(T) uint8_t m_data[sizeof(T)];

  public:
    constexpr ix_HollowValue() = default;

    ~ix_HollowValue() = default;
    ix_HollowValue(const ix_HollowValue &) = delete;
    ix_HollowValue(ix_HollowValue &&) = delete;
    ix_HollowValue &operator=(const ix_HollowValue &) = delete;
    ix_HollowValue &operator=(ix_HollowValue &&) = delete;

    template <typename... Args>
    ix_FORCE_INLINE T &construct(Args &&...args)
    {
        T *p = reinterpret_cast<T *>(m_data);
        new (p) T(ix_forward<Args>(args)...);
        return *p;
    }

    ix_FORCE_INLINE void destruct()
    {
        T *p = reinterpret_cast<T *>(m_data);
        p->~T();
    }

    ix_FORCE_INLINE auto defer_destruct()
    {
        return ix_defer([this]() { destruct(); });
    }

    ix_FORCE_INLINE T &get()
    {
        return *reinterpret_cast<T *>(m_data);
    }
};
// FILE END: ./src/ix/ix_HollowValue.hpp
// FILE BEGIN: ./src/ix/ix_Logger.hpp


class ix_Logger;

ix_Result ix_global_logger_init();
ix_Result ix_global_logger_deinit();
ix_Logger &ix_get_global_logger();

enum ix_LoggerSeverity : uint8_t
{
    ix_LOGGER_DEBUG,
    ix_LOGGER_WARNING,
    ix_LOGGER_FATAL,
    ix_LOGGER_INFINITE,
};

class ix_Logger
{
    ix_Writer m_writer;
    ix_Mutex m_mutex;
    ix_LoggerSeverity m_min_severity;
    const char *m_header;

    uint64_t m_start_tick;
    uint64_t m_previous_tick;

  public:
    ix_Logger(ix_LoggerSeverity min_severity, const ix_FileHandle *file, const char *header);
    void set_min_severity(ix_LoggerSeverity new_min_severity);
    ix_LoggerSeverity get_min_severity() const;

    ix_PRINTF_FORMAT(3, 0) void log(ix_LoggerSeverity severity, ix_FORMAT_ARG const char *format, va_list args);
    ix_PRINTF_FORMAT(2, 3) void log_fatal(ix_FORMAT_ARG const char *format, ...);
    ix_PRINTF_FORMAT(2, 3) void log_warning(ix_FORMAT_ARG const char *format, ...);
    ix_PRINTF_FORMAT(2, 3) void log_debug(ix_FORMAT_ARG const char *format, ...);

    template <typename F>
    void log_custom(const F &f)
    {
        m_mutex.lock();
        f(m_writer);
        m_mutex.unlock();
    }

  private:
    void print_header(uint64_t now, ix_LoggerSeverity severity);
    ix_PRINTF_FORMAT(3, 0) void print_main(ix_LoggerSeverity severity, const char *format, va_list args);
};
// FILE END: ./src/ix/ix_Logger.hpp
// FILE BEGIN: ./src/ix/ix_time.hpp


ix_Result ix_time_init();
uint64_t ix_time_now();
uint64_t ix_time_lap(uint64_t *p);

double ix_time_diff_to_sec(uint64_t diff);
double ix_time_diff_to_ms(uint64_t diff);
double ix_time_diff_to_us(uint64_t diff);
double ix_time_diff_to_ns(uint64_t diff);
// FILE END: ./src/ix/ix_time.hpp

static bool g_system_manager_initialized;
static ix_HollowValue<ix_SystemManager> g_system_manager;

bool ix_SystemManager::is_initialized()
{
    return g_system_manager_initialized;
}

ix_SystemManager &ix_SystemManager::initialize()
{
    ix_ASSERT_FATAL(!g_system_manager_initialized, "ix_SystemManager: Already initialized");
    g_system_manager_initialized = true;
    ix_SystemManager &sm = g_system_manager.construct();
    return sm;
}

ix_SystemManager &ix_SystemManager::get()
{
    return g_system_manager.get();
}

void ix_SystemManager::deinitialize()
{
    g_system_manager.destruct();
}

ix_SystemManager::ix_SystemManager()
{
    m_system_name_to_deinitializer.reserve(8);
}

ix_SystemManager::~ix_SystemManager()
{
    for (const auto &[name, deinitializer] : m_system_name_to_deinitializer)
    {
        if (deinitializer != nullptr)
        {
            const ix_Result res = deinitializer();
            res.assert_ok();
        }
    }
}

#define RETURN_ERROR_IF_INITIALIZED(name)        \
    do                                           \
    {                                            \
        if (is_initialized(name))                \
        {                                        \
            return ix_ERROR_ALREADY_INITIALIZED; \
        }                                        \
    } while (0)

#define RETURN_ERROR_IF_NOT_INITIALIZED(name) \
    do                                        \
    {                                         \
        if (!is_initialized(name))            \
        {                                     \
            return ix_ERROR_NOT_INITIALIZED;  \
        }                                     \
    } while (0)

ix_Result ix_SystemManager::initialize_stdio()
{
    RETURN_ERROR_IF_INITIALIZED("stdio");
    m_system_name_to_deinitializer.emplace("stdio", nullptr);
    return ix_initialize_stdio();
}

ix_Result ix_SystemManager::initialize_time()
{
    RETURN_ERROR_IF_INITIALIZED("time");
    m_system_name_to_deinitializer.emplace("time", nullptr);
    return ix_time_init();
}

ix_Result ix_SystemManager::initialize_logger()
{
    RETURN_ERROR_IF_INITIALIZED("logger");
    RETURN_ERROR_IF_NOT_INITIALIZED("stdio");
    RETURN_ERROR_IF_NOT_INITIALIZED("time");
    m_system_name_to_deinitializer.emplace("logger", []() { return ix_global_logger_deinit(); });
    return ix_global_logger_init();
}

ix_Result ix_SystemManager::initialize_dummy()
{
    RETURN_ERROR_IF_INITIALIZED("dummy");
    m_system_name_to_deinitializer.emplace("dummy", []() -> ix_Result { return ix_OK; });
    return ix_OK;
}

ix_Result ix_SystemManager::initialize_dummy_nullptr()
{
    RETURN_ERROR_IF_INITIALIZED("dummy_nullptr");
    m_system_name_to_deinitializer.emplace("dummy_nullptr", nullptr);
    return ix_OK;
}

bool ix_SystemManager::is_initialized(const char *name) const
{
    const auto *deinitializer = m_system_name_to_deinitializer.find(name);
    return (deinitializer != nullptr);
}

void ix_SystemManager::assert_initialized(const char *name) const
{
    const auto *deinitializer = m_system_name_to_deinitializer.find(name);
    const bool initialzed = deinitializer != nullptr;
    if (initialzed)
    {
        return;
    }

    char buf[64];
    ix_asnprintf(buf, "ix_SystemManager: %s must be initialized", name);
    ix_ASSERT_FATAL(false, buf);
}

// FILE END: ./src/ix/ix_SystemManager.cpp
// FILE BEGIN: ./src/ix/ix_Thread.cpp

#if ix_PLATFORM(WIN)

#include <WinBase.h>

#else
#include <pthread.h>
#include <unistd.h>
#endif

static_assert(sizeof(ix_Thread) == 32);

void ix_yield_this_thread()
{
#if ix_PLATFORM(WIN)
    SwitchToThread();
#else
    sched_yield();
#endif
}

// Loosely based on bx::Thread (by Branimir Karadzic, BSD 2-clause license).
struct ix_ThreadDetail
{
#if ix_PLATFORM(WIN)
    HANDLE m_handle;
    static DWORD system_entry(void *arg)
    {
        static_assert(sizeof(ix_ThreadDetail) <= sizeof(ix_Thread::m_detail));
        ix_Thread *thread = static_cast<ix_Thread *>(arg);
        thread->m_user_entry();
        return 0;
    }
#else
    pthread_t m_handle;
    static void *system_entry(void *arg)
    {
        static_assert(sizeof(ix_ThreadDetail) <= sizeof(ix_Thread::m_detail));
        ix_Thread *thread = static_cast<ix_Thread *>(arg);
        thread->m_user_entry();
        return nullptr;
    }
#endif
};

ix_Thread::ix_Thread(ix_Thread &&other)
    : m_user_entry(ix_move(other.m_user_entry))
{
    ix_memcpy(m_detail, other.m_detail, sizeof(m_detail));
    ix_memset(other.m_detail, 0, sizeof(m_detail));
    other.m_user_entry.nullify();
}

ix_Thread &ix_Thread::operator=(ix_Thread &&other)
{
    if (this == &other)
    {
        return *this;
    }

    join();
    m_user_entry = ix_move(other.m_user_entry);
    ix_memcpy(m_detail, other.m_detail, sizeof(m_detail));

    ix_memset(other.m_detail, 0, sizeof(m_detail));
    other.m_user_entry.nullify();

    return *this;
}

ix_Thread::~ix_Thread()
{
    join();
}

bool ix_Thread::is_joinable() const
{
    return m_user_entry.is_valid();
}

bool ix_Thread::start(const ix_FunctionN<16, void()> &user_entry)
{
    m_user_entry = user_entry;
    ix_ThreadDetail *detail = reinterpret_cast<ix_ThreadDetail *>(m_detail);

#if ix_PLATFORM(WIN)
    detail->m_handle = CreateThread(
        nullptr,                       // lpThreadAttributes
        0,                             // dwStackSize
        ix_ThreadDetail::system_entry, // lpStartAddress
        this,                          // lpParameter
        0,                             // dwCreateFlags
        nullptr                        // lpThreadId
    );
    ix_ASSERT_FATAL(detail->m_handle != nullptr, "ix_Thread::start(): CreateThread() failed");
#else
    int result = pthread_create(&detail->m_handle, nullptr, &ix_ThreadDetail::system_entry, this);
    ix_ASSERT_FATAL(result == 0, "ix_Thread::start(): pthread_create() failed");
#endif

    return true;
}

void ix_Thread::join()
{
    if (!is_joinable())
    {
        return;
    }

    ix_ThreadDetail *detail = reinterpret_cast<ix_ThreadDetail *>(m_detail);

#if ix_PLATFORM(WIN)
    const DWORD val = WaitForSingleObject(detail->m_handle, INFINITE);
    ix_ASSERT_FATAL(val == WAIT_OBJECT_0, "ix_Thread::join(): WaitForSingleObject() failed");

    const BOOL ret = CloseHandle(detail->m_handle);
    ix_ASSERT_FATAL(ret != 0, "ix_Thread::join(): CloseHandle() failed");

    detail->m_handle = INVALID_HANDLE_VALUE;
#else
    pthread_join(detail->m_handle, nullptr);
#endif

    m_user_entry.nullify();
}

// FILE END: ./src/ix/ix_Thread.cpp
// FILE BEGIN: ./src/ix/ix_random.cpp
// FILE BEGIN: ./src/ix/ix_HashSet.hpp


template <typename Key, typename Hash = ix_Hash<Key>, typename Equal = ix_Equal<Key>>
class ix_HashSet;

using ix_StringHashSet = ix_HashSet<const char *, ix_StringHash, ix_StringEqual>;

template <typename Key, typename Hash, typename Equal>
class ix_HashSet
{
    using Bucket = ix_detail::hash_container::Bucket;

    ix_Vector<Key> m_keys;
    ix_Vector<Bucket> m_buckets;

  public:
    ix_HashSet() = default;

    ix_HashSet(const Key *key_start, const Key *key_end)
    {
        size_t n = static_cast<size_t>(key_end - key_start);
        reserve(n);
        while (key_start < key_end)
        {
            emplace(*key_start);
            key_start += 1;
        }
    }

    // cppcheck-suppress noExplicitConstructor
    ix_HashSet(const std::initializer_list<Key> &keys)
    {
        reserve(keys.size());
        for (const Key &key : keys)
        {
            emplace(key);
        }
    }

    ix_FORCE_INLINE size_t size() const
    {
        return m_keys.size();
    }

    ix_FORCE_INLINE size_t bucket_count() const
    {
        return m_buckets.size();
    }

    ix_FORCE_INLINE size_t capacity() const
    {
        using ix_detail::hash_container::MAX_LOAD_FACTOR_DENOMINATOR;
        using ix_detail::hash_container::MAX_LOAD_FACTOR_NUMERATOR;

        return m_buckets.size() * MAX_LOAD_FACTOR_NUMERATOR / MAX_LOAD_FACTOR_DENOMINATOR;
    }

    ix_FORCE_INLINE bool empty() const
    {
        return m_keys.empty();
    }

    void reserve(size_t size)
    {
        using ix_detail::hash_container::INITIAL_LENGTH;
        using ix_detail::hash_container::MAX_LOAD_FACTOR_DENOMINATOR;
        using ix_detail::hash_container::MAX_LOAD_FACTOR_NUMERATOR;

        m_keys.reserve(size);
        const size_t n_minimum = (size * MAX_LOAD_FACTOR_DENOMINATOR / MAX_LOAD_FACTOR_NUMERATOR) + 1;
        const size_t n = ix_max(INITIAL_LENGTH, ix_ceil_2_power(n_minimum));
        rehash(n);
    }

    void clear()
    {
        using ix_detail::hash_container::INITIAL_LENGTH;

        m_keys.clear();
        m_buckets.clear();

        if (m_buckets.capacity() >= INITIAL_LENGTH)
        {
            m_buckets.resize(m_buckets.capacity());
            ix_memset(m_buckets.data(), 0, m_buckets.size() * sizeof(Bucket));
        }
    }

    ix_FORCE_INLINE Key *begin()
    {
        return m_keys.begin();
    }

    ix_FORCE_INLINE const Key *begin() const
    {
        return m_keys.begin();
    }

    ix_FORCE_INLINE Key *end()
    {
        return m_keys.end();
    }

    ix_FORCE_INLINE const Key *end() const
    {
        return m_keys.end();
    }

    ix_FORCE_INLINE void insert(const Key &key)
    {
        emplace(key);
    }

    ix_FORCE_INLINE void insert(Key &&key)
    {
        emplace(ix_move(key));
    }

    template <typename... Args>
    void emplace(Args &&...args)
    {
        if (ix_UNLIKELY(m_keys.size() >= capacity()))
        {
            rehash();
        }

        const size_t new_kv_pair_index = m_keys.size();
        m_keys.emplace_back(ix_forward<Args>(args)...);
        const Bucket *bucket = find_or_emplace_bucket(m_keys[new_kv_pair_index]);

        const size_t index = bucket->index;
        if (index != new_kv_pair_index)
        {
            m_keys.pop_back();
        }
    }

    ix_FORCE_INLINE bool contains(const Key &key) const
    {
        const Bucket *bucket = find_bucket(key);
        return (bucket != nullptr);
    }

    Key *find(const Key &key)
    {
        const Bucket *bucket = find_bucket(key);
        if (bucket == nullptr)
        {
            return end();
        }

        return &m_keys[bucket->index];
    }

    ix_FORCE_INLINE const Key *find(const Key &key) const
    {
        return const_cast<ix_HashSet *>(this)->find(key);
    }

    void erase(const Key &key)
    {
        using ix_detail::hash_container::HIGHER_HASH_LENGTH;
        using ix_detail::hash_container::HIGHER_HASH_MASK;

        Bucket *bucket_to_erase = find_bucket(key);
        if (bucket_to_erase == nullptr)
        {
            return;
        }

        const uint32_t erasing_key_index = bucket_to_erase->index;
        Bucket *buckets_begin = m_buckets.begin();
        Bucket *buckets_end = m_buckets.end();
        Bucket *bucket = ix_wrap_increment(bucket_to_erase, buckets_begin, buckets_end);
        Bucket *prev_bucket = bucket_to_erase;
        while ((HIGHER_HASH_MASK + (1 << HIGHER_HASH_LENGTH)) < bucket->disp_and_higher_hash)
        {
            prev_bucket->index = bucket->index;
            prev_bucket->disp_and_higher_hash = bucket->disp_and_higher_hash - (1 << HIGHER_HASH_LENGTH);
            prev_bucket = bucket;
            bucket = ix_wrap_increment(bucket, buckets_begin, buckets_end);
        }
        prev_bucket->disp_and_higher_hash = 0;

        if (ix_LIKELY(erasing_key_index != m_keys.size() - 1))
        {
            const uint32_t last_key_index = static_cast<uint32_t>(m_keys.size() - 1);
            Key &&last_key = ix_move(m_keys[last_key_index]);
            Bucket *last_key_bucket = find_bucket(last_key);
            last_key_bucket->index = erasing_key_index;
            m_keys[erasing_key_index] = ix_move(last_key);
        }

        m_keys.pop_back();
    }

    bool operator==(const ix_HashSet &other) const
    {
        if (size() != other.size())
        {
            return false;
        }

        for (const Key &key : m_keys)
        {
            if (!other.contains(key))
            {
                return false;
            }
        }

        return true;
    }

    ix_FORCE_INLINE bool operator!=(const ix_HashSet &other) const
    {
        return !(*this == other);
    }

    ix_FORCE_INLINE const Key &back() const
    {
        return m_keys.back();
    }

  private:
    void rehash(size_t num_new_buckets = 0)
    {
        using ix_detail::hash_container::DISP_LENGTH;
        using ix_detail::hash_container::HIGHER_HASH_LENGTH;
        using ix_detail::hash_container::HIGHER_HASH_MASK;
        using ix_detail::hash_container::increment_disp;
        using ix_detail::hash_container::INITIAL_LENGTH;

        if (ix_LIKELY(num_new_buckets == 0))
        {
            num_new_buckets = ix_max(INITIAL_LENGTH, 2 * m_buckets.size());
        }

        m_buckets = ix_Vector<Bucket>(num_new_buckets);
        ix_memset(m_buckets.data(), 0, m_buckets.size() * sizeof(Bucket));

        const uint64_t buckets_mask = static_cast<uint32_t>(m_buckets.size() - 1);
        Bucket *buckets_begin = m_buckets.begin();
        Bucket *buckets_end = m_buckets.end();
        for (size_t i = 0; i < m_keys.size(); i++)
        {
            const size_t hash = Hash::hash(m_keys[i]);
            const uint32_t higher_hash = static_cast<uint32_t>(hash >> (sizeof(size_t) * 8 - 32 + DISP_LENGTH));
            uint32_t disp_and_higher_hash = (1 << HIGHER_HASH_LENGTH) | higher_hash;
            Bucket *new_bucket_pos = &m_buckets[hash & buckets_mask];

            while (disp_and_higher_hash <= new_bucket_pos->disp_and_higher_hash)
            {
                disp_and_higher_hash = increment_disp(disp_and_higher_hash);
                new_bucket_pos = ix_wrap_increment(new_bucket_pos, buckets_begin, buckets_end);
            }

            Bucket new_bucket{disp_and_higher_hash, static_cast<uint32_t>(i)};
            emplace_bucket(new_bucket_pos, new_bucket, buckets_begin, buckets_end);
        }
    }

    ix_FORCE_INLINE const Bucket *find_bucket(const Key &key) const
    {
        return const_cast<ix_HashSet *>(this)->find_bucket_kernel<false>(key);
    }

    ix_FORCE_INLINE Bucket *find_bucket(const Key &key)
    {
        return find_bucket_kernel<false>(key);
    }

    ix_FORCE_INLINE Bucket *find_or_emplace_bucket(const Key &key)
    {
        return find_bucket_kernel<true>(key);
    }

    template <bool emplace_bucket_if_not_found>
    Bucket *find_bucket_kernel(const Key &key)
    {
        using ix_detail::hash_container::DISP_LENGTH;
        using ix_detail::hash_container::HIGHER_HASH_LENGTH;
        using ix_detail::hash_container::increment_disp;

        if constexpr (!emplace_bucket_if_not_found)
        {
            if (ix_UNLIKELY(m_keys.empty()))
            {
                return nullptr;
            }
        }

        const size_t hash = Hash::hash(key);
        const uint32_t higher_hash = static_cast<uint32_t>(hash >> (sizeof(size_t) * 8 - 32 + DISP_LENGTH));
        uint32_t disp_and_higher_hash = (1 << HIGHER_HASH_LENGTH) | higher_hash;
        const uint64_t buckets_mask = static_cast<uint32_t>(m_buckets.size() - 1);
        Bucket *bucket = &m_buckets[hash & buckets_mask];
        Bucket *buckets_begin = m_buckets.begin();
        Bucket *buckets_end = m_buckets.end();

        while (true)
        {
            if (disp_and_higher_hash == bucket->disp_and_higher_hash)
            {
                if (Equal::equal(key, m_keys[bucket->index]))
                {
                    return bucket;
                }
            }
            else if (disp_and_higher_hash > bucket->disp_and_higher_hash)
            {
                break;
            }
            disp_and_higher_hash = increment_disp(disp_and_higher_hash);
            bucket = ix_wrap_increment(bucket, buckets_begin, buckets_end);
        }

        if constexpr (emplace_bucket_if_not_found)
        {
            Bucket new_bucket{disp_and_higher_hash, static_cast<uint32_t>(m_keys.size() - 1)};
            emplace_bucket(bucket, new_bucket, buckets_begin, buckets_end);
            return bucket;
        }
        else
        {
            return nullptr;
        }
    }
};
// FILE END: ./src/ix/ix_HashSet.hpp
// FILE BEGIN: ./src/ix/ix_combinatorics.hpp


template <typename T>
class ix_Span;

template <typename T>
void ix_shuffle(ix_Span<T> span);

template <typename T>
bool ix_is_unique(const T *arr, size_t length);

template <typename T>
bool ix_is_sorted(ix_Span<T> span);

// cf. https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
template <typename T>
void ix_shuffle(T *arr, size_t length)
{
    if (length == 0)
    {
        return;
    }
    for (size_t i = length - 1; i != 0; i--)
    {
        const size_t j = ix_rand<size_t>() % i;
        ix_swap(arr[i], arr[j]);
    }
}

template <typename Container>
ix_FORCE_INLINE void ix_shuffle(Container &container)
{
    ix_shuffle(container.data(), container.size());
}

template <typename T>
bool ix_is_unique(const T *arr, size_t length)
{
    if (length == 0)
    {
        return true;
    }

    ix_HashSet<T> values;
    values.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
        if (values.contains(arr[i]))
        {
            return false;
        }
        values.insert(arr[i]);
    }
    return true;
}

template <typename T, typename Less>
bool ix_is_sorted(ix_Span<T> span, Less less)
{
    const size_t length = span.size();
    if (length <= 1)
    {
        return true;
    }

    for (size_t i = 1; i < length; i++)
    {
        if (less(span[i - 1], span[i]))
        {
            continue;
        }
        return false;
    }

    return true;
}

template <typename T>
ix_FORCE_INLINE bool ix_is_sorted(ix_Span<T> span)
{
    return ix_is_sorted(span, ix_Less<T>());
}
// FILE END: ./src/ix/ix_combinatorics.hpp


#include <time.h>

namespace ix_global
{
ix_Xoshiro256PlusPlus rng(ix_hash64(91), ix_hash64(96), ix_hash64(60), ix_hash64(70));
} // namespace ix_global

ix_Xoshiro256PlusPlus::ix_Xoshiro256PlusPlus(uint64_t x)
    : m_state()
{
    set_seed(x);
}


void ix_Xoshiro256PlusPlus::set_random_seed()
{
    const uint64_t t =
        0xDeadBeefDeadBeefULL ^ static_cast<uint64_t>(time(nullptr)) ^ ix_process_id() ^ ix_thread_id();
    set_seed(t);
}

void ix_Xoshiro256PlusPlus::set_seed(uint64_t seed)
{
    // linear congruential generator
    constexpr uint64_t m = 1ULL << 31;
    constexpr uint64_t a = 1664525;
    constexpr uint64_t c = 1013904223;
    uint64_t x = seed;
    for (uint64_t i = 0; i < ix_LENGTH_OF(m_state); i++)
    {
        x = (a * x + c) % m;
        const uint64_t s1 = x;
        x = (a * x + c) % m;
        const uint64_t s2 = x;

        m_state[i] = (s1 << 32UL) | s2;
    }
}


void ix_rand_set_seed(uint64_t seed)
{
    ix_global::rng.set_seed(seed);
}

void ix_rand_set_random_seed()
{
    ix_global::rng.set_random_seed();
}

// From https://prng.di.unimi.it/xoshiro256starstar.c
// (by David Blackman and Sebastiano Vigna, public domain)
static uint64_t rotl(const uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

template <typename T>
T ix_Xoshiro256PlusPlus::rand()
{
    if constexpr (ix_is_same_v<T, uint64_t>)
    {
        const uint64_t result = rotl(m_state[0] + m_state[3], 23) + m_state[0];

        const uint64_t t = m_state[1] << 17;

        m_state[2] ^= m_state[0];
        m_state[3] ^= m_state[1];
        m_state[1] ^= m_state[2];
        m_state[0] ^= m_state[3];

        m_state[2] ^= t;

        m_state[3] = rotl(m_state[3], 45);

        return result;
    }
    else if constexpr (ix_is_same_v<T, float>)
    {
        // cf. https://stackoverflow.com/questions/52147419
        const uint32_t v = rand<uint32_t>();
        constexpr uint32_t mask1 = 0x3F800000UL;
        constexpr uint32_t mask2 = 0x3FFFFFFFUL;
        const uint32_t masked = (v | mask1) & mask2;
        float one_to_two;
        ix_memcpy(&one_to_two, &masked, sizeof(float));
        return one_to_two - 1.0F;
    }
    else if constexpr (ix_is_same_v<T, double>)
    {
        // cf. https://stackoverflow.com/questions/52147419
        const uint64_t v = rand<uint64_t>();
        constexpr uint64_t mask1 = 0x3FF0000000000000ULL;
        constexpr uint64_t mask2 = 0x3FFFFFFFFFFFFFFFULL;
        const uint64_t masked = (v | mask1) & mask2;
        double one_to_two;
        ix_memcpy(&one_to_two, &masked, sizeof(double));
        return one_to_two - 1.0;
    }
    else
    {
        T x;
        uint64_t v = rand<uint64_t>();
        ix_memcpy(&x, &v, sizeof(T));
        return x;
    }
}


template <typename T>
T ix_Xoshiro256PlusPlus::rand_non_negative()
{
    if constexpr (ix_numeric_limits<T>::is_signed)
    {
        while (true)
        {
            T x = rand<T>();
            x = (x < 0) ? -x : x;
            if (ix_LIKELY(x >= 0))
            {
                return x;
            }
            ix_ASSERT(x == ix_numeric_limits<T>::min());
        }
    }
    else
    {
        return rand<T>();
    }
}


template <typename T>
T ix_Xoshiro256PlusPlus::rand_range(T min, T max)
{
    ix_ASSERT(min <= max);
    if constexpr (ix_numeric_limits<T>::is_integer && !ix_numeric_limits<T>::is_signed)
    {
        const T x = rand_non_negative<T>();
        const bool whole_range = (min == ix_numeric_limits<T>::min()) && (max == ix_numeric_limits<T>::max());
        if (whole_range)
        {
            return x;
        }
        const T range = static_cast<T>(max - min) + 1;
        return (x % range) + min;
    }
    else if constexpr (ix_numeric_limits<T>::is_integer)
    {
        const bool range_representable = ix_can_sub<T>(max, min) && ix_can_add<T>(max - min, 1);
        if (ix_LIKELY(range_representable))
        {
            const T range = static_cast<T>(max - min) + 1;
            return static_cast<T>(rand_non_negative<T>() % range) + min;
        }

        // `max - min + 1` is not representable in T.
        // This means that there is at least 1/2 chance to get the right value by `rand<T>`.
        T x = rand<T>();
        while (x < min || max < x)
        {
            x = rand<T>();
        }
        return x;
    }
    else
    {
        const T range = max - min;
        const T x = rand<T>() * range;
        return x + min;
    }
}


void ix_Xoshiro256PlusPlus::fill_alphanumeric(char *buf, size_t length)
{
    constexpr const char alphanumric[] = "abndefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length; i++)
    {
        const uint8_t idx = rand<uint8_t>() % (ix_LENGTH_OF(alphanumric) - 1);
        buf[i] = alphanumric[idx];
    }
    buf[length] = '\0';
}


template <typename T>
void ix_Xoshiro256PlusPlus::fill_unique(T *buf, size_t length)
{
    ix_HashSet<T> values;
    values.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
        T candidate = rand<T>();
        while (ix_UNLIKELY(values.contains(candidate)))
        {
            candidate = rand<T>();
        }
        values.insert(candidate);
        buf[i] = candidate;
    }
}


template <typename T>
void ix_Xoshiro256PlusPlus::fill_unique_range(T *buf, size_t length, T min, T max)
{
    ix_ASSERT(min < max);
    if constexpr (ix_numeric_limits<T>::is_integer)
    {
        const size_t num_options = static_cast<size_t>(max) - static_cast<size_t>(min) + size_t{1};
        ix_ASSERT(length <= num_options);

        const bool use_direct_pick = (num_options <= 2 * length);
        if (use_direct_pick)
        {
            ix_Vector<T> options;
            options.reserve(num_options);
            T x = min;
            for (size_t i = 0; i < num_options; i++)
            {
                options.emplace_back(x);
                x += 1;
            }

            for (size_t i = 0; i < length; i++)
            {
                const size_t index = rand<size_t>() % (num_options - i);
                buf[i] = options[index];
                options.swap_erase(index);
            }
            return;
        }
    }

    ix_HashSet<T> values;
    values.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
        T candidate = rand_range(min, max);
        while (ix_UNLIKELY(values.contains(candidate)))
        {
            candidate = rand_range(min, max);
        }
        values.insert(candidate);
        buf[i] = candidate;
    }
}


// clang-format off




// clang-format on
// FILE END: ./src/ix/ix_random.cpp
// FILE BEGIN: ./src/ix/ix_ThreadPool.cpp

thread_local static ix_Worker *tls_worker;
static constexpr uint64_t MAX_NUM_NATIVE_THREADS = 1024;

ix_JobStealingDeque::ix_JobStealingDeque(size_t initial_capacity)
    : m_initial_capacity(initial_capacity),
      m_top(MAX_NUM_NATIVE_THREADS), // Prevent overflow in `m_bottom - 1`.
      m_bottom(MAX_NUM_NATIVE_THREADS),
      m_job_array(nullptr)
{
    ix_ASSERT(initial_capacity <= ix_SIZE_MAX);
    ix_ASSERT(ix_is_power_of_2(initial_capacity));
}

ix_JobStealingDeque::ix_JobStealingDeque(ix_JobStealingDeque &&other)
    : m_initial_capacity(other.m_initial_capacity),
      m_top(other.m_top),
      m_bottom(other.m_bottom),
      m_job_array(other.m_job_array),
      m_garbages(ix_move(other.m_garbages))
{
    other.m_top = MAX_NUM_NATIVE_THREADS;
    other.m_bottom = MAX_NUM_NATIVE_THREADS;
    other.m_job_array = nullptr;
}

ix_JobStealingDeque &ix_JobStealingDeque::operator=(ix_JobStealingDeque &&other)
{
    if (this == &other)
    {
        return *this;
    }

    free_memory();

    m_initial_capacity = other.m_initial_capacity;
    m_top = other.m_top;
    m_bottom = other.m_bottom;
    m_job_array = other.m_job_array;
    m_garbages = ix_move(other.m_garbages);

    other.m_top = MAX_NUM_NATIVE_THREADS;
    other.m_bottom = MAX_NUM_NATIVE_THREADS;
    other.m_job_array = nullptr;

    return *this;
}

void ix_JobStealingDeque::free_memory()
{
    for (void *garbarge : m_garbages)
    {
        ix_FREE(garbarge);
    }

    if (m_job_array != nullptr)
    {
        ix_FREE(m_job_array->data);
        ix_FREE(m_job_array);
    }
}

ix_JobStealingDeque::~ix_JobStealingDeque()
{
    free_memory();
}

size_t ix_JobStealingDeque::size() const
{
    const uint64_t b = m_bottom;
    const uint64_t t = m_top;
    ix_ASSERT((b < t) || (b - t < ix_SIZE_MAX));
    return (b < t) ? 0 : static_cast<size_t>(b - t);
}

void ix_JobStealingDeque::push_bottom(ix_Job *job)
{
    const uint64_t b = ix_atomic_load(&m_bottom, ix_MEMORY_ORDER_RELAXED);
    const uint64_t t = ix_atomic_load(&m_top, ix_MEMORY_ORDER_ACQUIRE);
    JobArray *job_array = ix_atomic_load(&m_job_array, ix_MEMORY_ORDER_RELAXED);

    uint64_t capacity = (job_array == nullptr) ? 0 : job_array->capacity;
    const bool do_resize = (capacity < b - t + 1);
    if (ix_UNLIKELY(do_resize))
    {
        const uint64_t old_capacity = capacity;
        const uint64_t new_capacity = (old_capacity == 0) ? m_initial_capacity : (2 * old_capacity);
        ix_Job **new_data = ix_ALLOC_ARRAY(ix_Job *, static_cast<size_t>(new_capacity));

        if (job_array != nullptr)
        {
            ix_Job **old_data = job_array->data;
            const uint64_t old_mask = old_capacity - 1;
            const uint64_t new_mask = new_capacity - 1;
            for (uint64_t i = t; i < b; i++)
            {
                new_data[i & new_mask] = old_data[i & old_mask];
            }

            m_garbages.emplace_back(old_data);
            m_garbages.emplace_back(job_array);
        }

        JobArray *new_job_array = ix_ALLOC_STRUCT(JobArray);

        new_job_array->capacity = new_capacity;
        new_job_array->data = new_data;
        ix_atomic_store(&m_job_array, new_job_array, ix_MEMORY_ORDER_RELEASE);

        capacity = new_capacity;
        job_array = new_job_array;
    }

    ix_atomic_store(&job_array->data[b & (capacity - 1)], job, ix_MEMORY_ORDER_RELAXED);
    ix_atomic_memory_barrier(ix_MEMORY_ORDER_RELEASE);
    ix_atomic_store(&m_bottom, b + 1, ix_MEMORY_ORDER_RELAXED);
}

ix_Job *ix_JobStealingDeque::pop_bottom()
{
    const uint64_t b = ix_atomic_load(&m_bottom, ix_MEMORY_ORDER_RELAXED) - 1;
    JobArray *job_array = ix_atomic_load(&m_job_array, ix_MEMORY_ORDER_RELAXED);

    ix_atomic_store(&m_bottom, b, ix_MEMORY_ORDER_RELAXED);

    ix_atomic_memory_barrier(ix_MEMORY_ORDER_SEQ_CST);

    uint64_t t = ix_atomic_load(&m_top, ix_MEMORY_ORDER_RELAXED);

    const bool no_job_available = (b < t);
    if (no_job_available)
    {
        ix_atomic_store(&m_bottom, b + 1, ix_MEMORY_ORDER_RELAXED);
        return nullptr;
    }

    ix_Job *job = ix_atomic_load(&job_array->data[b & (job_array->capacity - 1)], ix_MEMORY_ORDER_RELAXED);

    const bool only_one_job_is_available = (t == b);
    if (!only_one_job_is_available)
    {
        return job;
    }

    const bool lose = !ix_atomic_cas_strong(&m_top, &t, t + 1, ix_MEMORY_ORDER_SEQ_CST, ix_MEMORY_ORDER_RELAXED);
    if (lose)
    {
        job = nullptr;
    }

    ix_atomic_store(&m_bottom, b + 1, ix_MEMORY_ORDER_RELAXED);

    return job;
}

ix_Job *ix_JobStealingDeque::steal_top()
{
    uint64_t t = ix_atomic_load(&m_top, ix_MEMORY_ORDER_ACQUIRE);
    ix_atomic_memory_barrier(ix_MEMORY_ORDER_SEQ_CST);
    const uint64_t b = ix_atomic_load(&m_bottom, ix_MEMORY_ORDER_ACQUIRE);

    const bool no_job_is_available = (b <= t);
    if (no_job_is_available)
    {
        return nullptr;
    }

    JobArray *job_array = ix_atomic_load(&m_job_array, ix_MEMORY_ORDER_CONSUME);
    ix_Job *job = ix_atomic_load(&job_array->data[t & (job_array->capacity - 1)], ix_MEMORY_ORDER_RELAXED);
    const bool lose = !ix_atomic_cas_strong(&m_top, &t, t + 1, ix_MEMORY_ORDER_SEQ_CST, ix_MEMORY_ORDER_RELAXED);
    if (lose)
    {
        return nullptr;
    }

    ix_ASSERT(job->ttl > 0);

    return job;
}

ix_ThreadPoolOption::ix_ThreadPoolOption()
    : num_worker_threads(ix_hardware_concurrency()),
      initial_job_queue_capacity(ix_OPT_LEVEL(DEBUG) ? 1 : 1024),
      job_pool_size(ix_OPT_LEVEL(DEBUG) ? 4096 : (1024 * 32)),
      spwan_workes_lazily(false)
{
    if (num_worker_threads == 0)
    {
        // 3 is an arbitrary value since we have no way to determine the actual number of cores.
        num_worker_threads = 3;
    }
}

size_t ix_Worker::index() const
{
    return static_cast<size_t>(m_index);
}

ix_Worker::ix_Worker(ix_ThreadPool *thread_pool, size_t index, const ix_ThreadPoolOption &option)
    : m_index(index),
      m_thread_pool(thread_pool),
      m_job_pool(option.job_pool_size),
      m_rng(index),
      m_jobs(option.initial_job_queue_capacity)
{
    m_job_stack.reserve(32);
}

void ix_Worker::execute_job_maybe()
{
    ix_Job *job = get_executable_job();

    if (job == nullptr)
    {
        ix_yield_this_thread();
        return;
    }

    m_job_stack.emplace_back(job);
    job->entry();
    m_job_stack.pop_back();
    finalize_job(job);
}

void ix_Worker::finalize_job(ix_Job *job)
{
    // Finalizer for a finshed job
    ix_Job *j = job;
    while (true)
    {
        const int64_t original_ttl = ix_atomic_fetch_sub(&j->ttl, int64_t{1}, ix_MEMORY_ORDER_RELAXED);
        const bool done = (original_ttl == 1);
        const bool has_parent = (j->parent != nullptr);
        if (!done || !has_parent) // !(done && has_parent)
        {
            break;
        }
        j = j->parent;
    }
}

ix_Job *ix_Worker::get_executable_job()
{
    ix_Job *job = m_jobs.pop_bottom();
    if (job != nullptr)
    {
        ix_ASSERT(job->ttl != 0);
        return job;
    }

    const size_t random_index = m_rng.rand_range<size_t>(0, m_thread_pool->size() - 1);
    const size_t num_workers = m_thread_pool->size();

    for (size_t i = 0; i < num_workers; i++)
    {
        const size_t target_index = (random_index + i) % num_workers;
        if (target_index == m_index)
        {
            continue;
        }

        ix_JobStealingDeque &target_queue = m_thread_pool->m_workers[target_index].m_jobs;
        job = target_queue.steal_top();
        if (job == nullptr)
        {
            continue;
        }

        size_t num_jobs_to_steal = 2 * target_queue.size() / num_workers;
        num_jobs_to_steal = (num_jobs_to_steal == 0) ? 1 : num_jobs_to_steal;
        for (size_t j = 0; j < num_jobs_to_steal; j++)
        {
            ix_Job *stolen_job = target_queue.steal_top();
            if (stolen_job == nullptr)
            {
                break;
            }
            m_jobs.push_bottom(stolen_job);
        }

        ix_ASSERT(job->ttl != 0);
        return job;
    }

    return nullptr;
}

ix_Job *ix_Worker::allocate_job(const ix_FunctionN<16, void()> &job_entry, ix_Job *parent)
{
    static_assert(ix_is_trivially_copy_assignable_v<ix_FunctionN<16, void()>>);
    static_assert(ix_is_trivially_destructible_v<ix_FunctionN<16, void()>>);

    ix_Job *job = allocate_job();
    ix_memcpy(&job->entry, &job_entry, sizeof(job->entry));
    job->parent = parent;
    job->ttl = (parent == nullptr) ? 2 : 1;
    if (parent != nullptr)
    {
        ix_atomic_fetch_add(&parent->ttl, int64_t{1}, ix_MEMORY_ORDER_RELAXED);
    }
    return job;
}

ix_Job *ix_Worker::allocate_job()
{
    ix_Job *first_candidate = &m_job_pool[m_next_job_index % m_job_pool.size()];
    ix_Job *job_pool_begin = m_job_pool.begin();
    ix_Job *job_pool_end = m_job_pool.end();

    ix_Job *job;
    while (true)
    {
        job = first_candidate;
        if (job->ttl == 0)
        {
            break;
        }

        do
        {
            job = ix_wrap_increment(job, job_pool_begin, job_pool_end);
        } while ((job->ttl != 0) && (job != first_candidate));

        const bool job_pool_full = (job == first_candidate);
        if (!job_pool_full)
        {
            ix_ASSERT(job->ttl == 0);
            break;
        }

        execute_job_maybe();
    }

    ix_ASSERT(job->ttl == 0);
    m_next_job_index += 1;

    return job;
}

[[nodiscard]] ix_Job *ix_Worker::add_empty_job()
{
    ix_Job *job = allocate_job();
    job->parent = nullptr;
    job->ttl = 1;
    // Do not push `job` to `m_jobs` because it is no-op.
    // `job->ttl` gets zero when `wait` is called on the returned job.
    return job;
}

[[nodiscard]] ix_Job *ix_Worker::add_job(const ix_FunctionN<16, void()> &job_entry)
{
    return add_job(nullptr, job_entry);
}

ix_Job *ix_Worker::add_job(ix_Job *parent, const ix_FunctionN<16, void()> &job_entry)
{
    ix_Job *job = allocate_job(job_entry, parent);
    m_jobs.push_bottom(job);
    return job;
}

void ix_Worker::wait(ix_Job *job)
{
    // You can't wait for a job with parent because an `ix_Job` value may be reused when its 'ttl' gets zero.
    // IDEA: Relax this constraint.
    ix_ASSERT_FATAL(
        (job->parent == nullptr) && (job->ttl != 0),
        "ix_Worker::wait(): The specified job is not waitable"
    );

    while (job->ttl != 1)
    {
        execute_job_maybe();
    }
    // `finalize_job` gets called twice for the `job`, but this is OK because:
    // - `job` doesn't have a parent, and such jobs have 'ttl' set to 2 on creation.
    // - 'wait' is relatively cold.
    finalize_job(job);
    ix_ASSERT(job->ttl == 0);
}

ix_Job *ix_Worker::get_current_job() const
{
    ix_Job *job = m_job_stack[m_job_stack.size() - 1];
    ix_ASSERT(job->ttl > 0);
    return job;
}

ix_ThreadPool::ix_ThreadPool(const ix_ThreadPoolOption &option)
    : m_option(option)
{
    const size_t num_worker_threads = option.num_worker_threads;

    m_workers.reserve(num_worker_threads);
    m_extra_worker_threads.reserve(num_worker_threads - 1);

    if (m_option.spwan_workes_lazily)
    {
        return;
    }

    for (size_t i = 0; i < num_worker_threads; i++)
    {
        spawn_worker();
    }
}

void ix_ThreadPool::spawn_worker()
{
    const size_t i = m_workers.size();

    m_workers.emplace_back(this, i, m_option);
    ix_Worker *new_worker = &m_workers[i];
    new_worker->m_thread_pool = this;
    new_worker->m_index = i;

    if (i == 0)
    {
        tls_worker = new_worker;
        return;
    }

    ix_Thread &t = m_extra_worker_threads.emplace_back();
    t.start([worker = new_worker]() {
        // ix_log_debug("Worker %zu started.", worker->index());
        tls_worker = worker;
        while (!worker->m_should_die)
        {
            worker->execute_job_maybe();
        }
        // ix_log_debug("Worker %zu died", worker->index());
    });
}

ix_ThreadPool::~ix_ThreadPool()
{
    shutdown();
}

size_t ix_ThreadPool::size() const
{
    return m_workers.size();
}

ix_Worker *ix_ThreadPool::get_current_worker()
{
    return tls_worker;
}

ix_Job *ix_ThreadPool::get_current_job()
{
    return tls_worker->get_current_job();
}

ix_Job *ix_ThreadPool::add_empty_job()
{
    if (m_workers.empty())
    {
        spawn_worker();
    }
    return m_workers[0].add_empty_job();
}

ix_Job *ix_ThreadPool::add_job(const ix_FunctionN<16, void()> &job_entry)
{
    return add_job(nullptr, job_entry);
}

ix_Job *ix_ThreadPool::add_job(ix_Job *parent, const ix_FunctionN<16, void()> &job_entry)
{
    if (m_workers.size() < m_option.num_worker_threads)
    {
        spawn_worker();
    }
    return m_workers[0].add_job(parent, job_entry);
}

void ix_ThreadPool::wait(ix_Job *job)
{
    m_workers[0].wait(job);
}

void ix_ThreadPool::shutdown()
{
    const size_t num_workers = m_workers.size();
    const size_t num_extra_worker_threads = m_extra_worker_threads.size();
    ix_ASSERT(
        ((num_workers == 0) && (num_extra_worker_threads == 0)) ||
        ((num_workers != 0) && (num_extra_worker_threads + 1 == num_workers))
    );

    for (size_t i = 1; i < num_workers; i++)
    {
        m_workers[i].m_should_die = true;
    }

    while (true)
    {
    JOINABILITY_CHECK:
        for (size_t i = 0; i < num_extra_worker_threads; i++)
        {
            if (!m_extra_worker_threads[i].is_joinable())
            {
                m_workers[0].execute_job_maybe();
                goto JOINABILITY_CHECK;
            }
        }
        break;
    }

    if (num_workers != 0)
    {
        for (size_t i = 0; i < num_extra_worker_threads; i++)
        {
            m_extra_worker_threads[i].join();
        }
    }

    m_workers.clear();
    m_extra_worker_threads.clear();
}

// FILE END: ./src/ix/ix_ThreadPool.cpp
// FILE BEGIN: ./src/ix/ix_Writer.cpp


#include <stdarg.h>
#include <stdio.h>

ix_Writer::ix_Writer(size_t buffer_initial_size, const ix_FileHandle *file)
    : m_buffer(buffer_initial_size),
      m_file(file)
{
}

ix_Writer::ix_Writer(ix_Buffer &&buffer, const ix_FileHandle *file)
    : m_buffer(ix_move(buffer)),
      m_file(file)
{
}

ix_Writer::~ix_Writer()
{
    flush();
}

ix_Writer::ix_Writer(ix_Writer &&other)
    : m_buffer(ix_move(other.m_buffer)),
      m_file(other.m_file)
{
    other.m_file = nullptr;
}

ix_Writer &ix_Writer::operator=(ix_Writer &&other)
{
    if (this == &other)
    {
        return *this;
    }

    flush();
    m_file = other.m_file;
    m_buffer = ix_move(other.m_buffer);

    other.m_file = nullptr;

    return *this;
}

size_t ix_Writer::buffer_size() const
{
    return m_buffer.size();
}

size_t ix_Writer::buffer_capacity() const
{
    return m_buffer.capacity();
}

void ix_Writer::reserve_buffer_capacity(size_t new_capacity)
{
    if (new_capacity < m_buffer.capacity())
    {
        return;
    }

    m_buffer.reserve(new_capacity);
}


void ix_Writer::flush()
{
    if (m_file == nullptr)
    {
        return;
    }

    m_file->write(m_buffer.data(), m_buffer.size());
    m_buffer.clear();
}

void ix_Writer::clear()
{
    flush();
    if (m_file == nullptr)
    {
        m_buffer.clear();
    }
}

const char *ix_Writer::data() const
{
    return m_buffer.data();
}

void ix_Writer::write(const void *data, size_t data_size)
{
    if (data_size == 0)
    {
        return;
    }

    if (m_file == nullptr)
    {
        m_buffer.push(data, data_size);
        return;
    }

    ix_ASSERT(m_file->is_valid());

    // If buf has enough sapce for data, we do not need to call write().
    const size_t capacity = m_buffer.capacity();
    if (m_buffer.size() + data_size <= capacity)
    {
        m_buffer.push(data, data_size);
        return;
    }

    // buf does not have enough space for data.
    // We need to write() buf and possibly part of data.
    m_file->write(m_buffer.data(), m_buffer.size());
    m_buffer.clear();

    if (data_size > capacity)
    {
        m_file->write(data, data_size);
        return;
    }

    m_buffer.push(data, data_size);
}


void ix_Writer::write_between(const void *begin, const void *end)
{
    const char *x = static_cast<const char *>(begin);
    const char *y = static_cast<const char *>(end);
    write(begin, static_cast<size_t>(y - x));
}


void ix_Writer::write_char(char c)
{
    write(&c, 1);
}


void ix_Writer::write_char_repeat(char c, size_t n)
{
    if (n == 0)
    {
        return;
    }

    if (m_file == nullptr)
    {
        m_buffer.push_char_repeat(c, n);
        return;
    }

    // If buf has enough sapce, we do not need to call write().
    if (m_buffer.size() + n <= m_buffer.capacity())
    {
        m_buffer.push_char_repeat(c, n);
        return;
    }

    // We need to write() buf and possibly part of data.
    m_file->write(m_buffer.data(), m_buffer.size());
    m_buffer.clear();

    if (n <= m_buffer.capacity())
    {
        m_buffer.push_char_repeat(c, n);
        return;
    }

    // WARN: very slow
    for (size_t i = 0; i < n; i++)
    {
        m_file->write(&c, 1);
    }
}


void ix_Writer::write_string(const char *str)
{
    write(str, ix_strlen(str));
}


void ix_Writer::end_string()
{
    if (m_file != nullptr)
    {
        return;
    }

    write_char('\0');
}


ix_PRINTF_FORMAT(2, 0) size_t ix_Writer::write_stringfv(ix_FORMAT_ARG const char *format, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    const int formatted_length_signed = ix_vsnprintf(nullptr, 0, format, args);
    ix_ASSERT(formatted_length_signed >= 0);

    const size_t required_length = static_cast<size_t>(formatted_length_signed) + 1;

    if ((m_file == nullptr) || (required_length <= m_buffer.capacity() - m_buffer.size()))
    {
        m_buffer.ensure_aggressively(required_length);
        const int num_bytes_written_signed = ix_vsnprintf(m_buffer.end(), required_length, format, args_copy);
        ix_ASSERT(num_bytes_written_signed >= 0);
        va_end(args_copy);

        const size_t num_bytes_written = static_cast<size_t>(num_bytes_written_signed);
        ix_ASSERT(num_bytes_written == required_length - 1);
        m_buffer.add_size(num_bytes_written);

        return num_bytes_written;
    }

    char buffer[ix_OPT_LEVEL(DEBUG) ? 32 : 1024];
    char *formatted = buffer;
    if (required_length > sizeof(buffer))
    {
        formatted = ix_MALLOC(char *, required_length);
    }

    const int wrote_length_signed = ix_vsnprintf(formatted, required_length, format, args_copy);
    va_end(args_copy);
    ix_ASSERT(wrote_length_signed >= 0);

    const size_t bytes_written = static_cast<size_t>(wrote_length_signed);
    write(formatted, bytes_written);

    const bool data_allocated = (formatted != buffer);
    if (data_allocated)
    {
        ix_FREE(formatted);
    }

    return bytes_written;
}

ix_PRINTF_FORMAT(2, 3) size_t ix_Writer::write_stringf(ix_FORMAT_ARG const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const size_t ret = write_stringfv(format, args);
    va_end(args);
    return ret;
}


ix_UniquePointer<char[]> ix_Writer::detach()
{
    return m_buffer.detach();
}

const ix_FileHandle *ix_Writer::file_handle() const
{
    return m_file;
}


void ix_Writer::substitute(const ix_FileHandle *file)
{
    clear();
    m_file = file;
}

// FILE END: ./src/ix/ix_Writer.cpp
// FILE BEGIN: ./src/ix/ix_atomic.cpp

#include <atomic>

static_assert(sizeof(std::atomic<int8_t>) == sizeof(int8_t));
static_assert(sizeof(std::atomic<int16_t>) == sizeof(int16_t));
static_assert(sizeof(std::atomic<int32_t>) == sizeof(int32_t));
static_assert(sizeof(std::atomic<int64_t>) == sizeof(int64_t));

static_assert(alignof(std::atomic<int8_t>) == alignof(int8_t));
static_assert(alignof(std::atomic<int16_t>) == alignof(int16_t));
static_assert(alignof(std::atomic<int32_t>) == alignof(int32_t));
static_assert(alignof(std::atomic<int64_t>) == alignof(int64_t));

static_assert(static_cast<int>(ix_MEMORY_ORDER_RELAXED) == static_cast<int>(std::memory_order_relaxed));
static_assert(static_cast<int>(ix_MEMORY_ORDER_CONSUME) == static_cast<int>(std::memory_order_consume));
static_assert(static_cast<int>(ix_MEMORY_ORDER_ACQUIRE) == static_cast<int>(std::memory_order_acquire));
static_assert(static_cast<int>(ix_MEMORY_ORDER_RELEASE) == static_cast<int>(std::memory_order_release));
static_assert(static_cast<int>(ix_MEMORY_ORDER_ACQ_REL) == static_cast<int>(std::memory_order_acq_rel));
static_assert(static_cast<int>(ix_MEMORY_ORDER_SEQ_CST) == static_cast<int>(std::memory_order_seq_cst));

void ix_atomic_memory_barrier(ix_MemoryOrder order)
{
    std::atomic_thread_fence(static_cast<std::memory_order>(order));
}

template <typename T>
T ix_atomic_load(const T volatile *x, ix_MemoryOrder order)
{
    return reinterpret_cast<const std::atomic<T> volatile *>(x)->load(static_cast<std::memory_order>(order));
}

#define implement_1(T, x, method, arg, order) \
    (reinterpret_cast<std::atomic<T> volatile *>(x)->method(arg, static_cast<std::memory_order>(order)))

template <typename T>
void ix_atomic_store(T volatile *x, T value, ix_MemoryOrder order)
{
    implement_1(T, x, store, value, order);
}

template <typename T>
T ix_atomic_fetch_add(T volatile *x, T value, ix_MemoryOrder order)
{
    return implement_1(T, x, fetch_add, value, order);
}

template <typename T>
T ix_atomic_fetch_sub(T volatile *x, T value, ix_MemoryOrder order)
{
    return implement_1(T, x, fetch_sub, value, order);
}

#define implement_2(T, x, method, arg_0, arg_1, order_0, order_1) \
    (reinterpret_cast<std::atomic<T> volatile *>(x)->method(      \
        *(arg_0),                                                 \
        arg_1,                                                    \
        static_cast<std::memory_order>(order_0),                  \
        static_cast<std::memory_order>(order_1)                   \
    ))

template <typename T>
bool ix_atomic_cas_weak(T volatile *x, T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure)
{
    return implement_2(T, x, compare_exchange_weak, expected, desired, success, failure);
}

template <typename T>
bool ix_atomic_cas_strong(T volatile *x, T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure)
{
    return implement_2(T, x, compare_exchange_strong, expected, desired, success, failure);
}


// clang-format off
#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif

#if ix_PLATFORM(WASM)
#endif
// clang-format on
// FILE END: ./src/ix/ix_atomic.cpp
// FILE BEGIN: ./src/ix/ix_environment.cpp

#if ix_PLATFORM(WIN)

#include <WinBase.h>
#include <process.h>

#elif ix_PLATFORM(LINUX) || ix_PLATFORM(MAC) || ix_PLATFORM(WASM)
#include <pthread.h>
#include <unistd.h>
#endif

#if __has_include(<has_include.hpp>)
#include <valgrind/valgrind.h>
#else
#define RUNNING_ON_VALGRIND false
#endif


size_t ix_hardware_concurrency()
{
#if ix_PLATFORM(WIN)
    SYSTEM_INFO info;
    GetNativeSystemInfo(&info);
    return info.dwNumberOfProcessors;
#elif ix_PLATFORM(LINUX) || ix_PLATFORM(MAC)
    return static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
#elif ix_PLATFORM(WASM)
    return 0;
#else
#error
#endif
}


size_t ix_thread_id()
{
#if ix_PLATFORM(WIN)
    return GetCurrentThreadId();
#else
    return static_cast<size_t>(gettid());
#endif
}


size_t ix_process_id()
{
    return static_cast<size_t>(getpid());
}


bool ix_is_valgrind_active()
{
    return RUNNING_ON_VALGRIND;
}

// FILE END: ./src/ix/ix_environment.cpp
// FILE BEGIN: ./src/ix/ix_file.cpp

// FILE BEGIN: ./src/ix/ix_util_win.hpp


#if ix_PLATFORM(WIN)

template <typename T>
class ix_Vector;

ix_Option<size_t> ix_win_utf8_to_wchar(
    const char *utf8_buf,
    size_t utf8_buf_len,
    wchar_t *wchar_buf,
    size_t wchar_buf_len
);

ix_Option<size_t> ix_win_wchar_to_utf8(
    const wchar_t *wchar_buf,
    size_t wchar_buf_len,
    char *utf8_buf,
    size_t utf8_buf_len
);

ix_Result ix_win_utf8_to_wchar_dynamic(const char *utf8_buf, size_t utf8_buf_len, ix_Vector<wchar_t> &vector);
ix_Result ix_win_wchar_to_utf8_dynamic(const wchar_t *wchar_buf, size_t wchar_buf_len, ix_Vector<char> &vector);
ix_Option<size_t> utf8_path_to_wchar(const char *path_utf8, wchar_t path_wchar[ix_MAX_PATH + 1]);
ix_Option<size_t> wchar_path_to_utf8(const wchar_t *path_wchar, size_t path_length, char path_utf8[ix_MAX_PATH + 1]);

ix_Option<ix_UniquePointer<const char *[]>> ix_win_wargs_to_args(
    int argc,
    const wchar_t *const *wargv,
    ix_Vector<char> &buf
);

const char *ix_win_msg_to_string(unsigned int uMsg);

#if ix_MEASURE_COVERAGE
void ix_win_abort_with_info(const char *file, int line, const char *msg);
#else
[[noreturn]] void ix_win_abort_with_info(const char *file, int line, const char *msg);
#endif

#if ix_OPT_LEVEL(DEBUG)
#define ix_WIN_ASSERT(expr)                                                            \
    do                                                                                 \
    {                                                                                  \
        if (ix_UNLIKELY(!(expr)))                                                      \
        {                                                                              \
            ix_win_abort_with_info(__FILE__, __LINE__, ix_PP_TO_STRING_LITERAL(expr)); \
        }                                                                              \
    } while (0)
#define ix_WIN_ASSERT_NONZERO(expr) ix_WIN_ASSERT((expr) != 0)
#define ix_WIN_CHECK_HR(hr) ix_WIN_ASSERT((hr) == S_OK)
#else
#define ix_WIN_ASSERT(expr) ix_UNUSED(expr)
#define ix_WIN_ASSERT_NONZERO(expr) ix_UNUSED(expr)
#define ix_WIN_CHECK_HR(hr) ix_UNUSED(hr)
#endif

#endif
// FILE END: ./src/ix/ix_util_win.hpp


#include <stdio.h>
#include <stdlib.h>

#if ix_PLATFORM(WIN)


#include <WinBase.h>
#include <fcntl.h>
#include <io.h>

#if !defined(STDIN_FILENO)
#define STDIN_FILENO 0
#endif
#if !defined(STDOUT_FILENO)
#define STDOUT_FILENO 1
#endif
#if !defined(STDERR_FILENO)
#define STDERR_FILENO 2
#endif

#else

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#endif

#if ix_PLATFORM(WASM)
#include <emscripten.h>
#endif


static ix_HollowValue<ix_FileHandle> g_filehandle_stdin;
static ix_HollowValue<ix_FileHandle> g_filehandle_stdout;
static ix_HollowValue<ix_FileHandle> g_filehandle_stderr;

ix_Result ix_initialize_stdio()
{
#if ix_PLATFORM(WIN)
    const int stdin_oldmode = setmode(STDIN_FILENO, O_BINARY);

    if (stdin_oldmode == -1)
    {
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

    const int stdout_oldmode = setmode(STDOUT_FILENO, O_BINARY);

    if (stdout_oldmode == -1)
    {
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

    const int stderr_oldmode = setmode(STDERR_FILENO, O_BINARY);

    if (stderr_oldmode == -1)
    {
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

#endif

    g_filehandle_stdin.construct();
    g_filehandle_stdout.construct();
    g_filehandle_stderr.construct();

#if ix_PLATFORM(WIN)
    g_filehandle_stdin.get().v.handle = GetStdHandle(STD_INPUT_HANDLE);
    g_filehandle_stdout.get().v.handle = GetStdHandle(STD_OUTPUT_HANDLE);
    g_filehandle_stderr.get().v.handle = GetStdHandle(STD_ERROR_HANDLE);
#else
    g_filehandle_stdin.get().v.fd = STDIN_FILENO;
    g_filehandle_stdout.get().v.fd = STDOUT_FILENO;
    g_filehandle_stderr.get().v.fd = STDERR_FILENO;
#endif

    return ix_OK;
}

ix_Result ix_remove_file(const char *path)
{
#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    const BOOL ret = DeleteFile(path_wchar);
    if (ret != 0)
    {
        return ix_OK;
    }

    const DWORD err = GetLastError();
    ix_ASSERT(err != 0);
    switch (err)
    {
    case ERROR_FILE_NOT_FOUND:
        return ix_ERROR_FILE_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_FILE_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#else
    const int ret = unlink(path);
    if (ret == 0)
    {
        return ix_OK;
    }
    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_FILE_NOT_FOUND;
    case EACCES:
        return ix_ERROR_FILE_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#endif
}

bool ix_is_file(const char *path)
{
#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return false;
    }

    const DWORD ret = GetFileAttributes(path_wchar);
    return (ret != INVALID_FILE_ATTRIBUTES) && ((ret & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
    struct stat sb;
    int ret = stat(path, &sb);
    return (ret == 0) && ((sb.st_mode & S_IFMT) == S_IFREG);
#endif
}


bool ix_is_directory(const char *path)
{
#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return false;
    }

    const DWORD ret = GetFileAttributes(path_wchar);
    return (ret != INVALID_FILE_ATTRIBUTES) && ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
    struct stat sb;
    int ret = stat(path, &sb);
    return (ret == 0) && ((sb.st_mode & S_IFMT) == S_IFDIR);
#endif
}


ix_Result ix_write_to_file(const char *filename, const char *data, size_t data_length)
{
    const ix_Option<ix_FileHandle> maybe_file = ix_open_file(filename, ix_WRITE_ONLY);
    if (maybe_file.is_error())
    {
        return maybe_file.result_kind();
    }
    const ix_FileHandle &file = maybe_file.unwrap();
    file.write(data, data_length);
    return ix_OK;
}

ix_Result ix_write_string_to_file(const char *filename, const char *str)
{
    return ix_write_to_file(filename, str, ix_strlen(str));
}

ix_Option<ix_UniquePointer<char[]>> ix_load_file(const char *filename, size_t *data_length_out, size_t padding_length)
{
    ix_ASSERT(filename != nullptr);

    ix_Option<ix_FileHandle> file_opt = ix_open_file(filename, ix_READ_ONLY);
    if (file_opt.is_error())
    {
        return file_opt.result_kind();
    }
    const ix_FileHandle &file = file_opt.unwrap();
    ix_ASSERT(file.is_valid());

    return file.read_all(data_length_out, padding_length);
}


ix_Result ix_remove_directory(const char *path)
{
#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    const BOOL ret = RemoveDirectory(path_wchar);
    if (ret != 0)
    {
        return ix_OK;
    }
    const DWORD err = GetLastError();
    ix_ASSERT(err != 0);
    switch (err)
    {
    case ERROR_FILE_NOT_FOUND:
        return ix_ERROR_DIR_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#else
    const int ret = rmdir(path);
    if (ret == 0)
    {
        return ix_OK;
    }
    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_DIR_NOT_FOUND;
    case EACCES:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#endif
}

ix_Result ix_create_directory(const char *path)
{
    if (*path == '\0')
    {
        return ix_ERROR_DIR_ALREADY_EXISTS;
    }

#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    const BOOL ret = CreateDirectory(path_wchar, nullptr);
    if (ret != 0)
    {
        return ix_OK;
    }

    const DWORD err = GetLastError();
    ix_ASSERT(err != 0);
    switch (err)
    {
    case ERROR_ALREADY_EXISTS:
        return ix_ERROR_DIR_ALREADY_EXISTS;
    case ERROR_PATH_NOT_FOUND:
        return ix_ERROR_DIR_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#else
    const int ret = mkdir(path, 0777);
    if (ret == 0)
    {
        return ix_OK;
    }

    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_DIR_NOT_FOUND;
    case EACCES:
        return ix_ERROR_DIR_ACCESS_DENIED;
    case EEXIST:
        return ix_ERROR_DIR_ALREADY_EXISTS;
#if ix_PLATFORM(WASM)
    case EINVAL:
        return ix_is_directory(path) ? ix_ERROR_DIR_ALREADY_EXISTS : ix_ERROR_SOMETHING_WENT_WRONG;
#endif
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#endif
}


#if ix_PLATFORM(WIN)
ix_FORCE_INLINE static bool is_path_separator(wchar_t c)
{
    return (c == L'\\') || (c == '/');
}

static wchar_t *next_path_separator(wchar_t *p)
{
    wchar_t c = *p;
    while (!is_path_separator(c) && (c != L'\0'))
    {
        p += 1;
        c = *p;
    }
    return p;
}
#endif

ix_Result ix_ensure_directories(const char *path)
{
#if ix_PLATFORM(WIN)
    if (*path == '\0')
    {
        return ix_OK;
    }

    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    const DWORD ret = GetFileAttributes(path_wchar);
    const bool exists = (ret != INVALID_FILE_ATTRIBUTES) && ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0);
    if (exists)
    {
        const bool dir = ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0);
        return dir ? ix_OK : ix_ERROR_FILE_ALREADY_EXISTS;
    }

    wchar_t *path_wchar_end = next_path_separator(path_wchar + 1);
    while (*path_wchar_end != L'\0')
    {
        const wchar_t temp = *path_wchar_end;
        *path_wchar_end = L'\0';
        const BOOL ret_bool = CreateDirectory(path_wchar, nullptr);
        *path_wchar_end = temp;
        const bool fail = (ret_bool == 0);
        if (fail)
        {
            const DWORD err = GetLastError();
            switch (err)
            {
            case ERROR_ALREADY_EXISTS:
                break;
            case ERROR_PATH_NOT_FOUND:
                return ix_ERROR_DIR_NOT_FOUND;
            case ERROR_ACCESS_DENIED:
                return ix_ERROR_DIR_ACCESS_DENIED;
            default:
                return ix_ERROR_SOMETHING_WENT_WRONG;
            }
        }
        path_wchar_end = next_path_separator(path_wchar_end + 1);
    }

    const bool ends_with_separator = is_path_separator(*(path_wchar_end - 1));
    if (ends_with_separator)
    {
        return ix_OK;
    }

    const BOOL ret_bool = CreateDirectory(path_wchar, nullptr);
    const bool success = (ret_bool != 0);
    if (success)
    {
        return ix_OK;
    }

    const DWORD err = GetLastError();
    switch (err)
    {
    case ERROR_PATH_NOT_FOUND:
        return ix_ERROR_DIR_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

#else
    if (*path == '\0')
    {
        return ix_OK;
    }

    if (ix_is_directory(path))
    {
        return ix_OK;
    }

    char buf[ix_MAX_PATH + 1];
    strncpy(buf, path, sizeof(buf));
    const bool path_too_long = (buf[sizeof(buf) - 1] != '\0');
    if (path_too_long)
    {
        return ix_ERROR_PATH_TOO_LONG;
    }

    char *p = ix_next_path_separator(buf + 1);
    while (*p != '\0')
    {
        const char temp = *p;
        *p = '\0';
        const int ret = mkdir(buf, 0777);
        const bool fail = (ret != 0);
        if (fail)
        {
            bool exists = (errno == EEXIST);
#if ix_PLATFORM(WASM)
            // At least on WASM, `mkdir(".")` does not set errno to EEXIST.
            exists |= ix_is_directory(buf);
#endif
            if (!exists)
            {
                switch (errno)
                {
                case ENOENT:
                    return ix_ERROR_DIR_NOT_FOUND;
                case EACCES:
                    return ix_ERROR_DIR_ACCESS_DENIED;
                default:
                    return ix_ERROR_SOMETHING_WENT_WRONG;
                }
            }
        }
        *p = temp;
        p = ix_next_path_separator(p + 1);
    }

    const bool ends_with_separator = ix_is_path_separator(*(p - 1));
    if (ends_with_separator)
    {
        return ix_OK;
    }

    const int ret = mkdir(path, 0777);
    const bool success = (ret == 0);
    if (success)
    {
        return ix_OK;
    }

    ix_ASSERT(errno != EEXIST);

    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_DIR_NOT_FOUND;
    case EACCES:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#endif
}


ix_Option<ix_FileHandle> ix_create_directories_and_file(const char *path)
{
    ix_Option<ix_FileHandle> maybe_file = ix_open_file(path, ix_WRITE_ONLY);
    if (maybe_file.is_ok())
    {
        return maybe_file;
    }

    const size_t dirname_length = ix_dirname_length(path);
    const bool path_too_long = (dirname_length >= ix_MAX_PATH);
    if (path_too_long)
    {
        return ix_FileHandle();
    }

    char buf[ix_MAX_PATH + 1];
    ix_memcpy(buf, path, dirname_length);
    buf[dirname_length] = '\0';
    const ix_Result result = ix_ensure_directories(buf);
    if (result.is_error())
    {
        return ix_FileHandle();
    }

    return ix_open_file(path, ix_WRITE_ONLY);
}


ix_FileHandle ix_FileHandle::null(ix_FileOpenOption option)
{
#if ix_PLATFORM(WIN)
    return ix_open_file("NUL", option).unwrap();
#else
    return ix_open_file("/dev/null", option).unwrap();
#endif
}


ix_FileHandle::ix_FileHandle()
{
    invalidate();
}

ix_Option<ix_FileHandle> ix_open_file(const char *path, ix_FileOpenOption option)
{
#if ix_PLATFORM(WIN)

    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    HANDLE handle = nullptr;

    switch (option)
    {
    case ix_READ_ONLY: {
        handle = CreateFile(
            path_wchar,
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
        break;
    }

    case ix_WRITE_ONLY: {
        handle = CreateFile(path_wchar, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        break;
    }
        ix_CASE_EXHAUSTED();
    }

    if (handle != INVALID_HANDLE_VALUE)
    {
        ix_FileHandle file_handle;
        file_handle.v.handle = handle;
        return file_handle;
    }

    const DWORD err = GetLastError();
    ix_ASSERT(err != 0);
    switch (err)
    {
    case ERROR_FILE_NOT_FOUND:
        return ix_ERROR_FILE_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_FILE_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

#else

    int fd;

    switch (option)
    {
    case ix_READ_ONLY: {
        fd = open(path, O_RDONLY);
        break;
    }
    case ix_WRITE_ONLY: {
        fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        break;
    }
        ix_CASE_EXHAUSTED();
    }

    if (fd != -1)
    {
        ix_FileHandle file_handle;
        file_handle.v.fd = fd;
        return file_handle;
    }

    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_FILE_NOT_FOUND;
    case EACCES:
        return ix_ERROR_FILE_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

#endif
}

ix_FileHandle::ix_FileHandle(ix_FileHandle &&other)
    : v(other.v)
{
    other.invalidate();
}

ix_FileHandle &ix_FileHandle::operator=(ix_FileHandle &&other)
{
    if (is_valid())
    {
        close();
    }

    v = other.v;
    other.invalidate();

    return *this;
}

ix_FileHandle::~ix_FileHandle()
{
    if (is_valid())
    {
        close();
    }
}

void ix_FileHandle::invalidate()
{
#if ix_PLATFORM(WIN)
    v.handle = INVALID_HANDLE_VALUE;
#else
    v.fd = -1;
#endif
}

void ix_FileHandle::close()
{
#if ix_PLATFORM(WIN)
    const BOOL ret = CloseHandle(v.handle);
    ix_ASSERT_FATAL(ret != 0, "CloseHandle() returned 0");
#else
    int ret = ::close(v.fd);
    ix_ASSERT_FATAL(ret == 0, "close() did not return 0");
#endif
    invalidate();
}


bool ix_FileHandle::is_valid() const
{
#if ix_PLATFORM(WIN)
    return (v.handle != INVALID_HANDLE_VALUE);
#else
    return (v.fd != -1);
#endif
}


HANDLE ix_FileHandle::get_native_handle() const
{
    return v.handle;
}

int ix_FileHandle::get_native_fd() const
{
    return v.fd;
}

void ix_FileHandle::write(const void *data, size_t data_length) const
{
#if ix_PLATFORM(WIN)
    while (data_length > 0)
    {
        DWORD bytes_written;
        // For some reason, this makes the write much faster (see benchmark_io).
        const DWORD write_amount = ix_min(static_cast<DWORD>(data_length), 1024UL * 1024UL);
        const BOOL ret = WriteFile(v.handle, data, write_amount, &bytes_written, nullptr);
        const bool fail = (ret == 0);
        if (fail)
        {
            return;
        }
        data = static_cast<const uint8_t *>(data) + bytes_written;
        data_length -= bytes_written;
    }
#else
    while (data_length > 0)
    {
        const ssize_t bytes_written = ::write(v.fd, data, static_cast<unsigned int>(data_length));
        const bool fail = (bytes_written == -1);
        if (fail)
        {
            return;
        }
        data = static_cast<const uint8_t *>(data) + bytes_written;
        const size_t bytes_written_unsigned = static_cast<size_t>(bytes_written);
        data_length -= bytes_written_unsigned;
    }
#endif
}

void ix_FileHandle::write_string(const char *str) const
{
    write(str, ix_strlen(str));
}

void ix_FileHandle::write_char(char c) const
{
    write(&c, 1);
}


ix_PRINTF_FORMAT(2, 0) size_t ix_FileHandle::write_stringfv(ix_FORMAT_ARG const char *format, va_list args) const
{
    va_list args_copy;
    va_copy(args_copy, args); // Copy must occur here.

    const int required_length_s = ix_vsnprintf(nullptr, 0, format, args) + 1;
    ix_ASSERT(required_length_s > 0);
    const size_t required_length = static_cast<size_t>(required_length_s);

    char buffer[ix_OPT_LEVEL(DEBUG) ? 32 : 1024];
    char *formatted = buffer;
    if (required_length > ix_LENGTH_OF(buffer))
    {
        formatted = ix_MALLOC(char *, required_length);
    }

    const int formatted_length_signed = ix_vsnprintf(formatted, required_length, format, args_copy);
    va_end(args_copy);
    ix_ASSERT(formatted_length_signed >= 0);

    const size_t formatted_length = static_cast<size_t>(formatted_length_signed);
    write(formatted, formatted_length);

    const bool data_allocated = (formatted != buffer);
    if (data_allocated)
    {
        ix_FREE(formatted);
    }

    return formatted_length;
}

ix_PRINTF_FORMAT(2, 3) size_t ix_FileHandle::write_stringf(ix_FORMAT_ARG const char *format, ...) const
{
    va_list args;
    va_start(args, format);
    const size_t ret = write_stringfv(format, args);
    va_end(args);
    return ret;
}


size_t ix_FileHandle::read(void *buffer, size_t buffer_length) const
{
#if ix_PLATFORM(WIN)
    size_t total_bytes_read = 0;
    while (buffer_length > 0)
    {
        DWORD bytes_read;
        const BOOL ret = ReadFile(v.handle, buffer, static_cast<DWORD>(buffer_length), &bytes_read, nullptr);
        const bool fail = (ret == 0);
        if (fail)
        {
            const DWORD err = GetLastError();
            return (err == ERROR_BROKEN_PIPE) ? total_bytes_read : ix_SIZE_MAX;
        }
        const bool eof = (bytes_read == 0);
        if (eof)
        {
            break;
        }
        buffer = static_cast<char *>(buffer) + bytes_read;
        const size_t bytes_read_unsigned = static_cast<size_t>(bytes_read);
        buffer_length -= bytes_read_unsigned;
        total_bytes_read += bytes_read_unsigned;
    }

    return total_bytes_read;
#else
    if (v.fd == -1)
    {
        return ix_SIZE_MAX;
    }

    size_t total_bytes_read = 0;
    while (buffer_length > 0)
    {
        const ssize_t bytes_read = ::read(v.fd, buffer, buffer_length);
        const bool fail = (bytes_read == -1);
        if (fail)
        {
            return ix_SIZE_MAX;
        }
        const bool eof = (bytes_read == 0);
        if (eof)
        {
            break;
        }
        buffer = static_cast<char *>(buffer) + bytes_read;
        const size_t bytes_read_unsigned = static_cast<size_t>(bytes_read);
        buffer_length -= bytes_read_unsigned;
        total_bytes_read += bytes_read_unsigned;
    }

    return total_bytes_read;
#endif
}


ix_Option<ix_UniquePointer<char[]>> ix_FileHandle::read_all(
    size_t *data_length_out,
    size_t padding_length,
    size_t first_read_size
) const
{
    char *data = nullptr;
    size_t data_size;

    const size_t file_size = size();
    const bool file_size_valid = (file_size != ix_SIZE_MAX);
    if (file_size_valid)
    {
        data = ix_MALLOC(char *, file_size + padding_length);
        data_size = file_size;
        const size_t bytes_written = read(data, file_size);
        if (bytes_written != file_size)
        {
            ix_FREE(data);
            return ix_UniquePointer<char[]>(nullptr);
        }
    }
    else
    {
        data_size = 0;
        size_t read_size = first_read_size;
        ix_ASSERT(first_read_size > 0);
        while (true)
        {
            data = ix_REALLOC(char *, data, data_size + read_size + padding_length);

            const size_t bytes_read = read(data + data_size, read_size);
            const bool failed = (bytes_read == ix_SIZE_MAX);
            if (failed)
            {
                ix_FREE(data);
                return ix_UniquePointer<char[]>(nullptr);
            }

            data_size += bytes_read;

            const bool consumed = (bytes_read == 0);
            if (consumed)
            {
                break;
            }

            if (bytes_read == read_size)
            {
                read_size *= 2;
            }
        }
    }

    for (size_t i = 0; i < padding_length; i++)
    {
        data[data_size + i] = '\0';
    }

    if (data_length_out != nullptr)
    {
        *data_length_out = data_size;
    }

    return ix_UniquePointer<char[]>(ix_move(data));
}


size_t ix_FileHandle::size() const
{
#if ix_PLATFORM(WIN)
    LARGE_INTEGER file_size;
    const BOOL ret = GetFileSizeEx(v.handle, &file_size);
    if (ret == 0)
    {
        return ix_SIZE_MAX;
    }
    return static_cast<size_t>(file_size.QuadPart);
#else
    const off_t curr = lseek(v.fd, 0, SEEK_CUR);
    if (curr == off_t{-1})
    {
        return ix_SIZE_MAX;
    }

    const off_t file_size = lseek(v.fd, 0, SEEK_END);
    if (file_size == off_t{-1})
    {
        return ix_SIZE_MAX;
    }

    const off_t ret = lseek(v.fd, curr, SEEK_SET);
    if (ret == off_t{-1})
    {
        return ix_SIZE_MAX;
    }

    return static_cast<size_t>(file_size);
#endif
}


void ix_FileHandle::substitute(ix_FileHandle &&another_file)
{
    if (is_valid())
    {
        close();
    }
    v.handle = another_file.v.handle;
    another_file.invalidate();
}


ix_FileHandle ix_FileHandle::copy() const
{
#if ix_PLATFORM(WIN)
    ix_FileHandle copy;
    HANDLE process = GetCurrentProcess();
    const BOOL ret = DuplicateHandle(
        process,        // Source process
        v.handle,       // Source handle
        process,        // Target process
        &copy.v.handle, // New handle
        0,              // Desired access (0 means same access)
        FALSE,          // Not inheritable
        DUPLICATE_SAME_ACCESS
    );
    ix_ASSERT_FATAL(ret, "File handle duplication failed");
    return copy;
#else
    ix_FileHandle copy;
    copy.v.fd = dup(v.fd);
    ix_ASSERT_FATAL(copy.v.fd != -1, "File handle duplication failed");
    return copy;
#endif
}


const ix_FileHandle &ix_FileHandle::of_stdin()
{
    ix_ASSERT_FATAL(g_filehandle_stdin.get().is_valid(), "stdio system is not initialized!");
    return g_filehandle_stdin.get();
}

const ix_FileHandle &ix_FileHandle::of_stdout()
{
    ix_ASSERT_FATAL(g_filehandle_stdout.get().is_valid(), "stdio system is not initialized!");
    return g_filehandle_stdout.get();
}

const ix_FileHandle &ix_FileHandle::of_stderr()
{
    ix_ASSERT_FATAL(g_filehandle_stderr.get().is_valid(), "stdio system is not initialized!");
    return g_filehandle_stderr.get();
}

// FILE END: ./src/ix/ix_file.cpp
// FILE BEGIN: ./src/ix/ix_filepath.cpp


bool ix_is_path_separator(char c)
{
#if ix_PLATFORM(WIN)
    return (c == '\\') || (c == '/');
#else
    return (c == '/');
#endif
}

char *ix_next_path_separator(const char *p)
{
    char c = *p;
    while (!ix_is_path_separator(c) && (c != '\0'))
    {
        p += 1;
        c = *p;
    }
    return const_cast<char *>(p);
}


size_t ix_dirname_length(const char *filename, size_t filename_length)
{
    const char *p = filename + filename_length;

    while (true)
    {
        if (ix_is_path_separator(*p))
        {
            return static_cast<size_t>(p - filename) + 1;
        }

        if (p == filename)
        {
            return 0;
        }

        p -= 1;
    }
}

size_t ix_basename_length(const char *filename, size_t filename_length)
{
    return filename_length - ix_dirname_length(filename);
}

size_t ix_dirname_length(const char *filename)
{
    return ix_dirname_length(filename, ix_strlen(filename));
}

size_t ix_basename_length(const char *filename)
{
    return ix_basename_length(filename, ix_strlen(filename));
}

// FILE END: ./src/ix/ix_filepath.cpp
// FILE BEGIN: ./src/ix/ix_scanf.cpp


#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

static bool ix_is_white(char c)
{
    return (c == ' ') || (c == '\n');
}

void ix_skip_to_next_word(const char **c)
{
    while ((**c != '\0') && !ix_is_white(**c))
    {
        *c += 1;
    }

    while ((**c != '\0') && ix_is_white(**c))
    {
        *c += 1;
    }
}


static char buf_ix_read_string[256];

const char *ix_read_string(const char **c)
{
    size_t size_written = 0;
    const size_t buf_len = ix_LENGTH_OF(buf_ix_read_string);
    while ((**c != '\0') && !ix_is_white(**c) && (size_written < buf_len - 1))
    {
        buf_ix_read_string[size_written] = **c;
        size_written += 1;
        *c += 1;
    }

    ix_ASSERT(size_written < buf_len);
    buf_ix_read_string[size_written] = '\0';

    if (size_written == buf_len - 1 && **c != '\0')
    {
        return nullptr;
    }

    while (ix_is_white(**c))
    {
        *c += 1;
    }

    return buf_ix_read_string;
}


float ix_read_float(const char **c)
{
    const float f = strtof(*c, nullptr);
    ix_skip_to_next_word(c);
    return f;
}


int ix_read_int(const char **c)
{
    const int i = static_cast<int>(strtol(*c, nullptr, 10));
    ix_skip_to_next_word(c);
    return i;
}


unsigned int ix_read_uint(const char **c)
{
    const unsigned int i = static_cast<unsigned int>(strtoul(*c, nullptr, 10));
    ix_skip_to_next_word(c);
    return i;
}


void ix_next_line(const char **c)
{
    if (**c == '\0')
    {
        return;
    }

    while (**c != '\n')
    {
        *c += 1;
    }

    *c += 1;
}


void ix_skip_empty_lines(const char **c)
{
    while (**c == '\n')
    {
        ix_next_line(c);
    }
}


void ix_next_non_empty_line(const char **c)
{
    ix_next_line(c);
    while (**c == '\n')
    {
        ix_next_line(c);
    }
}


template <typename T>
ix_Option<T> ix_string_convert(const char *s)
{
    const bool starts_with_space = (*s == ' ');
    if (starts_with_space)
    {
        return ix_ERROR_INVALID_PARAMETER;
    }

    if constexpr (ix_is_same_v<T, float>)
    {
        char *end;
        errno = 0;
        const float v = strtof(s, &end);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT(
                ix_equal(v, HUGE_VALF) || ix_equal(v, -HUGE_VALF) || //
                ix_equal(v, FLT_MIN) || ix_equal(v, -FLT_MIN) || ix_equal(v, 0.0F)
            );
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, double>)
    {
        char *end;
        errno = 0;
        const double v = strtod(s, &end);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT(
                ix_equal(v, HUGE_VAL) || ix_equal(v, -HUGE_VAL) || //
                ix_equal(v, DBL_MIN) || ix_equal(v, -DBL_MIN) || ix_equal(v, 0.0)
            );
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, long>) // cppcheck-suppress multiCondition
    {
        char *end;
        errno = 0;
        const long v = strtol(s, &end, 10);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT((v == LONG_MIN) || (v == LONG_MAX));
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, long long>) // cppcheck-suppress multiCondition
    {
        char *end;
        errno = 0;
        const long long v = strtoll(s, &end, 10);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT((v == LLONG_MIN) || (v == LLONG_MAX));
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, unsigned long>) // cppcheck-suppress multiCondition
    {
        const bool has_minus_sign = (*s == '-');
        if (has_minus_sign)
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        char *end;
        errno = 0;
        const unsigned long v = strtoul(s, &end, 10);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT(v == ULONG_MAX);
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, unsigned long long>) // cppcheck-suppress multiCondition
    {
        const bool has_minus_sign = (*s == '-');
        if (has_minus_sign)
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        char *end;
        errno = 0;
        const unsigned long long v = strtoull(s, &end, 10);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT(v == ULLONG_MAX);
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    ix_UNREACHABLE();
}


// FILE END: ./src/ix/ix_scanf.cpp
// FILE BEGIN: ./src/ix/ix_Logger.cpp

static ix_HollowValue<ix_Logger> g_logger;

ix_Result ix_global_logger_init()
{
    g_logger.construct(ix_LOGGER_DEBUG, &ix_FileHandle::of_stderr(), nullptr);
    return ix_OK;
}

ix_Result ix_global_logger_deinit()
{
    g_logger.destruct();
    return ix_OK;
}

ix_Logger &ix_get_global_logger()
{
    return g_logger.get();
}

ix_PRINTF_FORMAT(1, 2) void ix_log_fatal(ix_FORMAT_ARG const char *format, ...)
{
    va_list args;
    va_start(args, format);
    g_logger.get().log(ix_LOGGER_FATAL, format, args);
    va_end(args);
}

ix_PRINTF_FORMAT(1, 2) void ix_log_warning(ix_FORMAT_ARG const char *format, ...)
{
    va_list args;
    va_start(args, format);
    g_logger.get().log(ix_LOGGER_WARNING, format, args);
    va_end(args);
}

ix_PRINTF_FORMAT(1, 2) void ix_log_debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    g_logger.get().log(ix_LOGGER_DEBUG, format, args);
    va_end(args);
}

static constexpr const char *severity_strings[] = {
    "DEBUG",   //
    "WARNING", //
    "FATAL",   //
};

ix_Logger::ix_Logger(ix_LoggerSeverity min_severity, const ix_FileHandle *file, const char *header)
    : m_writer(0, file),
      m_min_severity(min_severity),
      m_header(header)
{
    if (file == nullptr)
    {
        m_writer.reserve_buffer_capacity(4096);
    }

    m_start_tick = ix_time_now();
    m_previous_tick = m_start_tick;
}

void ix_Logger::set_min_severity(ix_LoggerSeverity new_min_severity)
{
    m_min_severity = new_min_severity;
}

ix_LoggerSeverity ix_Logger::get_min_severity() const
{
    return m_min_severity;
}

ix_PRINTF_FORMAT(3, 0) void ix_Logger::log(ix_LoggerSeverity severity, ix_FORMAT_ARG const char *format, va_list args)
{
    if (severity < m_min_severity)
    {
        return;
    }

    print_main(severity, format, args);
}

ix_PRINTF_FORMAT(2, 3) void ix_Logger::log_fatal(ix_FORMAT_ARG const char *format, ...)
{
    if (ix_LOGGER_FATAL < m_min_severity)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    print_main(ix_LOGGER_FATAL, format, args);
    va_end(args);
}

ix_PRINTF_FORMAT(2, 3) void ix_Logger::log_warning(ix_FORMAT_ARG const char *format, ...)
{
    if (ix_LOGGER_WARNING < m_min_severity)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    print_main(ix_LOGGER_WARNING, format, args);
    va_end(args);
}

ix_PRINTF_FORMAT(2, 3) void ix_Logger::log_debug(ix_FORMAT_ARG const char *format, ...)
{
    if (ix_LOGGER_DEBUG < m_min_severity)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    print_main(ix_LOGGER_DEBUG, format, args);
    va_end(args);
}

void ix_Logger::print_header(uint64_t now, ix_LoggerSeverity severity)
{
    const double ms_from_start = ix_time_diff_to_ms(now - m_start_tick);
    const double ms_from_previous = ix_time_diff_to_ms(now - m_previous_tick);

    const char *severity_string = severity_strings[static_cast<size_t>(severity)];
    if (m_header != nullptr)
    {
        m_writer.write_stringf(
            "[%09.4f ms] [%09.4f ms] [%s::%s] ",
            ms_from_start,
            ms_from_previous,
            m_header,
            severity_string
        );
    }
    else
    {
        m_writer.write_stringf(
            "[%09.4f ms] [%09.4f ms] [%s] ", //
            ms_from_start,
            ms_from_previous,
            severity_string
        );
    }
}

ix_PRINTF_FORMAT(3, 0) void ix_Logger::print_main(ix_LoggerSeverity severity, const char *format, va_list args)
{
    const uint64_t now = ix_time_now();

    m_mutex.lock();
    print_header(now, severity);
    m_writer.write_stringfv(format, args);
    m_writer.write_char('\n');
    m_mutex.unlock();

    m_previous_tick = now;
}

// FILE END: ./src/ix/ix_Logger.cpp
// FILE BEGIN: ./src/ix/ix_time.cpp


// FILE BEGIN: ./external/sokol_time/sokol_time.h
#if defined(SOKOL_IMPL) && !defined(SOKOL_TIME_IMPL)
#define SOKOL_TIME_IMPL
#endif
#ifndef SOKOL_TIME_INCLUDED
/*
    sokol_time.h    -- simple cross-platform time measurement

    Project URL: https://github.com/floooh/sokol

    Do this:
        #define SOKOL_IMPL or
        #define SOKOL_TIME_IMPL
    before you include this file in *one* C or C++ file to create the
    implementation.

    Optionally provide the following defines with your own implementations:
    SOKOL_ASSERT(c)     - your own assert macro (default: assert(c))
    SOKOL_TIME_API_DECL - public function declaration prefix (default: extern)
    SOKOL_API_DECL      - same as SOKOL_TIME_API_DECL
    SOKOL_API_IMPL      - public function implementation prefix (default: -)

    If sokol_time.h is compiled as a DLL, define the following before
    including the declaration or implementation:

    SOKOL_DLL

    On Windows, SOKOL_DLL will define SOKOL_TIME_API_DECL as __declspec(dllexport)
    or __declspec(dllimport) as needed.

    void stm_setup();
        Call once before any other functions to initialize sokol_time
        (this calls for instance QueryPerformanceFrequency on Windows)

    uint64_t stm_now();
        Get current point in time in unspecified 'ticks'. The value that
        is returned has no relation to the 'wall-clock' time and is
        not in a specific time unit, it is only useful to compute
        time differences.

    uint64_t stm_diff(uint64_t new, uint64_t old);
        Computes the time difference between new and old. This will always
        return a positive, non-zero value.

    uint64_t stm_since(uint64_t start);
        Takes the current time, and returns the elapsed time since start
        (this is a shortcut for "stm_diff(stm_now(), start)")

    uint64_t stm_laptime(uint64_t* last_time);
        This is useful for measuring frame time and other recurring
        events. It takes the current time, returns the time difference
        to the value in last_time, and stores the current time in
        last_time for the next call. If the value in last_time is 0,
        the return value will be zero (this usually happens on the
        very first call).

    uint64_t stm_round_to_common_refresh_rate(uint64_t duration)
        This oddly named function takes a measured frame time and
        returns the closest "nearby" common display refresh rate frame duration
        in ticks. If the input duration isn't close to any common display
        refresh rate, the input duration will be returned unchanged as a fallback.
        The main purpose of this function is to remove jitter/inaccuracies from
        measured frame times, and instead use the display refresh rate as
        frame duration.
        NOTE: for more robust frame timing, consider using the
        sokol_app.h function sapp_frame_duration()

    Use the following functions to convert a duration in ticks into
    useful time units:

    double stm_sec(uint64_t ticks);
    double stm_ms(uint64_t ticks);
    double stm_us(uint64_t ticks);
    double stm_ns(uint64_t ticks);
        Converts a tick value into seconds, milliseconds, microseconds
        or nanoseconds. Note that not all platforms will have nanosecond
        or even microsecond precision.

    Uses the following time measurement functions under the hood:

    Windows:        QueryPerformanceFrequency() / QueryPerformanceCounter()
    MacOS/iOS:      mach_absolute_time()
    emscripten:     emscripten_get_now()
    Linux+others:   clock_gettime(CLOCK_MONOTONIC)

    zlib/libpng license

    Copyright (c) 2018 Andre Weissflog

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.

        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.

        3. This notice may not be removed or altered from any source
        distribution.
*/
#define SOKOL_TIME_INCLUDED (1)
#include <stdint.h>

#if defined(SOKOL_API_DECL) && !defined(SOKOL_TIME_API_DECL)
#define SOKOL_TIME_API_DECL SOKOL_API_DECL
#endif
#ifndef SOKOL_TIME_API_DECL
#if defined(_WIN32) && defined(SOKOL_DLL) && defined(SOKOL_TIME_IMPL)
#define SOKOL_TIME_API_DECL __declspec(dllexport)
#elif defined(_WIN32) && defined(SOKOL_DLL)
#define SOKOL_TIME_API_DECL __declspec(dllimport)
#else
#define SOKOL_TIME_API_DECL extern
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

SOKOL_TIME_API_DECL void stm_setup(void);
SOKOL_TIME_API_DECL uint64_t stm_now(void);
SOKOL_TIME_API_DECL uint64_t stm_diff(uint64_t new_ticks, uint64_t old_ticks);
SOKOL_TIME_API_DECL uint64_t stm_since(uint64_t start_ticks);
SOKOL_TIME_API_DECL uint64_t stm_laptime(uint64_t* last_time);
SOKOL_TIME_API_DECL uint64_t stm_round_to_common_refresh_rate(uint64_t frame_ticks);
SOKOL_TIME_API_DECL double stm_sec(uint64_t ticks);
SOKOL_TIME_API_DECL double stm_ms(uint64_t ticks);
SOKOL_TIME_API_DECL double stm_us(uint64_t ticks);
SOKOL_TIME_API_DECL double stm_ns(uint64_t ticks);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // SOKOL_TIME_INCLUDED

/*-- IMPLEMENTATION ----------------------------------------------------------*/
#ifdef SOKOL_TIME_IMPL
#define SOKOL_TIME_IMPL_INCLUDED (1)
#include <string.h> /* memset */

#ifndef SOKOL_API_IMPL
    #define SOKOL_API_IMPL
#endif
#ifndef SOKOL_ASSERT
    #include <assert.h>
    #define SOKOL_ASSERT(c) assert(c)
#endif
#ifndef _SOKOL_PRIVATE
    #if defined(__GNUC__) || defined(__clang__)
        #define _SOKOL_PRIVATE __attribute__((unused)) static
    #else
        #define _SOKOL_PRIVATE static
    #endif
#endif

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
typedef struct {
    uint32_t initialized;
    LARGE_INTEGER freq;
    LARGE_INTEGER start;
} _stm_state_t;
#elif defined(__APPLE__) && defined(__MACH__)
#include <mach/mach_time.h>
typedef struct {
    uint32_t initialized;
    mach_timebase_info_data_t timebase;
    uint64_t start;
} _stm_state_t;
#elif defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
typedef struct {
    uint32_t initialized;
    double start;
} _stm_state_t;
#else /* anything else, this will need more care for non-Linux platforms */
#ifdef ESP8266
// On the ESP8266, clock_gettime ignores the first argument and CLOCK_MONOTONIC isn't defined
#define CLOCK_MONOTONIC 0
#endif
#include <time.h>
typedef struct {
    uint32_t initialized;
    uint64_t start;
} _stm_state_t;
#endif
static _stm_state_t _stm;

/* prevent 64-bit overflow when computing relative timestamp
    see https://gist.github.com/jspohr/3dc4f00033d79ec5bdaf67bc46c813e3
*/
#if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__))
_SOKOL_PRIVATE int64_t _stm_int64_muldiv(int64_t value, int64_t numer, int64_t denom) {
    int64_t q = value / denom;
    int64_t r = value % denom;
    return q * numer + r * numer / denom;
}
#endif

SOKOL_API_IMPL void stm_setup(void) {
    memset(&_stm, 0, sizeof(_stm));
    _stm.initialized = 0xABCDABCD;
    #if defined(_WIN32)
        QueryPerformanceFrequency(&_stm.freq);
        QueryPerformanceCounter(&_stm.start);
    #elif defined(__APPLE__) && defined(__MACH__)
        mach_timebase_info(&_stm.timebase);
        _stm.start = mach_absolute_time();
    #elif defined(__EMSCRIPTEN__)
        _stm.start = emscripten_get_now();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        _stm.start = (uint64_t)ts.tv_sec*1000000000 + (uint64_t)ts.tv_nsec;
    #endif
}

SOKOL_API_IMPL uint64_t stm_now(void) {
    SOKOL_ASSERT(_stm.initialized == 0xABCDABCD);
    uint64_t now;
    #if defined(_WIN32)
        LARGE_INTEGER qpc_t;
        QueryPerformanceCounter(&qpc_t);
        now = (uint64_t) _stm_int64_muldiv(qpc_t.QuadPart - _stm.start.QuadPart, 1000000000, _stm.freq.QuadPart);
    #elif defined(__APPLE__) && defined(__MACH__)
        const uint64_t mach_now = mach_absolute_time() - _stm.start;
        now = (uint64_t) _stm_int64_muldiv((int64_t)mach_now, (int64_t)_stm.timebase.numer, (int64_t)_stm.timebase.denom);
    #elif defined(__EMSCRIPTEN__)
        double js_now = emscripten_get_now() - _stm.start;
        now = (uint64_t) (js_now * 1000000.0);
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        now = ((uint64_t)ts.tv_sec*1000000000 + (uint64_t)ts.tv_nsec) - _stm.start;
    #endif
    return now;
}

SOKOL_API_IMPL uint64_t stm_diff(uint64_t new_ticks, uint64_t old_ticks) {
    if (new_ticks > old_ticks) {
        return new_ticks - old_ticks;
    }
    else {
        return 1;
    }
}

SOKOL_API_IMPL uint64_t stm_since(uint64_t start_ticks) {
    return stm_diff(stm_now(), start_ticks);
}

SOKOL_API_IMPL uint64_t stm_laptime(uint64_t* last_time) {
    SOKOL_ASSERT(last_time);
    uint64_t dt = 0;
    uint64_t now = stm_now();
    if (0 != *last_time) {
        dt = stm_diff(now, *last_time);
    }
    *last_time = now;
    return dt;
}

// first number is frame duration in ns, second number is tolerance in ns,
// the resulting min/max values must not overlap!
static const uint64_t _stm_refresh_rates[][2] = {
    { 16666667, 1000000 },  //  60 Hz: 16.6667 +- 1ms
    { 13888889,  250000 },  //  72 Hz: 13.8889 +- 0.25ms
    { 13333333,  250000 },  //  75 Hz: 13.3333 +- 0.25ms
    { 11764706,  250000 },  //  85 Hz: 11.7647 +- 0.25
    { 11111111,  250000 },  //  90 Hz: 11.1111 +- 0.25ms
    { 10000000,  500000 },  // 100 Hz: 10.0000 +- 0.5ms
    {  8333333,  500000 },  // 120 Hz:  8.3333 +- 0.5ms
    {  6944445,  500000 },  // 144 Hz:  6.9445 +- 0.5ms
    {  4166667, 1000000 },  // 240 Hz:  4.1666 +- 1ms
    {        0,       0 },  // keep the last element always at zero
};

SOKOL_API_IMPL uint64_t stm_round_to_common_refresh_rate(uint64_t ticks) {
    uint64_t ns;
    int i = 0;
    while (0 != (ns = _stm_refresh_rates[i][0])) {
        uint64_t tol = _stm_refresh_rates[i][1];
        if ((ticks > (ns - tol)) && (ticks < (ns + tol))) {
            return ns;
        }
        i++;
    }
    // fallthough: didn't fit into any buckets
    return ticks;
}

SOKOL_API_IMPL double stm_sec(uint64_t ticks) {
    return (double)ticks / 1000000000.0;
}

SOKOL_API_IMPL double stm_ms(uint64_t ticks) {
    return (double)ticks / 1000000.0;
}

SOKOL_API_IMPL double stm_us(uint64_t ticks) {
    return (double)ticks / 1000.0;
}

SOKOL_API_IMPL double stm_ns(uint64_t ticks) {
    return (double)ticks;
}
#endif /* SOKOL_TIME_IMPL */
// FILE END: ./external/sokol_time/sokol_time.h

ix_Result ix_time_init()
{
    stm_setup();
    return ix_OK;
}

uint64_t ix_time_now()
{
    return stm_now();
}


double ix_time_diff_to_sec(uint64_t diff)
{
    return stm_sec(diff);
}

double ix_time_diff_to_ms(uint64_t diff)
{
    return stm_ms(diff);
}

double ix_time_diff_to_us(uint64_t diff)
{
    return stm_us(diff);
}

double ix_time_diff_to_ns(uint64_t diff)
{
    return stm_ns(diff);
}


uint64_t ix_time_lap(uint64_t *p)
{
    const uint64_t now = ix_time_now();
    const uint64_t lap = now - *p;
    *p = now;
    return lap;
}

// FILE END: ./src/ix/ix_time.cpp
// FILE BEGIN: ./src/ix/ix_util_win.cpp

#if ix_PLATFORM(WIN)
// FILE BEGIN: ./src/ix/ix_StackVector.hpp


template <typename T>
class ix_Span;

template <typename T, size_t N>
class ix_StackVector
{
    alignas(T) uint8_t m_initial_data[sizeof(T) * N];
    T *m_data = reinterpret_cast<T *>(m_initial_data);
    size_t m_size = 0;
    size_t m_capacity = N;

    ix_VECTOR_UTIL_FRIEND_DECLARE(ix_StackVector);

  public:
    constexpr ix_StackVector() = default;
    ix_StackVector(const ix_StackVector &other) = delete;
    ix_StackVector &operator=(const ix_StackVector &other) = delete;

    ix_FORCE_INLINE ~ix_StackVector()
    {
        ix_bulk_destruct(m_data, m_size);

        if (!is_data_initial())
        {
            ix_FREE(m_data);
        }
    }

    constexpr ix_StackVector(ix_StackVector &&other) noexcept
        : m_size(other.m_size),
          m_capacity(other.m_capacity)
    {
        if (other.is_data_initial())
        {
            ix_bulk_move_construct(m_data, other.m_data, other.m_size);
            ix_bulk_destruct(other.m_data, other.m_size);
        }
        else
        {
            m_data = other.m_data;
        }

        other.m_data = reinterpret_cast<T *>(other.m_initial_data);
        other.m_size = 0;
        other.m_capacity = N;
    }

    // cppcheck-suppress operatorEqVarError
    ix_StackVector &operator=(ix_StackVector &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        ix_bulk_destruct(m_data, m_size);
        if (!is_data_initial())
        {
            ix_FREE(m_data);
        }

        if (other.is_data_initial())
        {
            m_data = reinterpret_cast<T *>(m_initial_data);
            ix_bulk_move_construct(m_data, other.m_data, other.m_size);
            ix_bulk_destruct(other.m_data, other.m_size);
        }
        else
        {
            m_data = other.m_data;
        }

        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = reinterpret_cast<T *>(other.m_initial_data);
        other.m_size = 0;
        other.m_capacity = N;

        return *this;
    }

    explicit constexpr ix_StackVector(size_t initial_size)
        : m_size(initial_size),
          m_capacity(ix_max(N, initial_size))
    {
        if (N < initial_size)
        {
            m_data = ix_ALLOC_ARRAY(T, initial_size);
        }

        ix_bulk_default_construct(m_data, initial_size);
    }

    // cppcheck-suppress noExplicitConstructor
    ix_FORCE_INLINE constexpr ix_StackVector(const std::initializer_list<T> &xs)
        : m_size(xs.size()),
          m_capacity(xs.size())
    {
        if (N < xs.size())
        {
            m_data = ix_ALLOC_ARRAY(T, xs.size());
        }

        ix_bulk_copy_construct(m_data, xs.begin(), m_size);
    }

    constexpr bool operator==(const ix_StackVector &other) const
    {
        if (m_size != other.m_size)
        {
            return false;
        }

        for (size_t i = 0; i < m_size; i++)
        {
            if (operator[](i) != other[i])
            {
                return false;
            }
        }

        return true;
    }

    constexpr bool operator!=(const ix_StackVector &other) const
    {
        return !(*this == other);
    }

    ix_FORCE_INLINE constexpr T *data() const
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr size_t size() const
    {
        return m_size;
    }

    ix_FORCE_INLINE constexpr size_t capacity() const
    {
        return m_capacity;
    }

    ix_FORCE_INLINE constexpr ix_Span<T> span()
    {
        return ix_Span<T>(m_data, m_size);
    }

    ix_FORCE_INLINE constexpr ix_Span<T> span(size_t i)
    {
        return ix_Span<T>(m_data, i);
    }

    ix_FORCE_INLINE constexpr bool empty() const
    {
        return (m_size == 0);
    }

    ix_FORCE_INLINE constexpr T &operator[](size_t i)
    {
        ix_ASSERT(i < m_size);
        return m_data[i];
    }

    ix_FORCE_INLINE constexpr const T &operator[](size_t i) const
    {
        ix_ASSERT(i < m_size);
        return m_data[i];
    }

    ix_FORCE_INLINE constexpr T *begin()
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr T *end()
    {
        return m_data + m_size;
    }

    ix_FORCE_INLINE constexpr const T *begin() const
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr const T *end() const
    {
        return m_data + m_size;
    }

    constexpr void reserve(size_t new_capacity)
    {
        if (new_capacity <= m_capacity)
        {
            return;
        }

        m_capacity = new_capacity;

        if (ix_UNLIKELY(is_data_initial()))
        {
            m_data = ix_ALLOC_ARRAY(T, new_capacity);
            ix_bulk_move_construct(m_data, reinterpret_cast<T *>(m_initial_data), m_size);
            ix_bulk_destruct(m_initial_data, m_size);
        }
        else
        {
            T *new_data = ix_ALLOC_ARRAY(T, new_capacity);
            ix_bulk_move_construct(new_data, m_data, m_size);
            ix_bulk_destruct(m_data, m_size);
            ix_FREE(m_data);
            m_data = new_data;
        }
    }

    ix_FORCE_INLINE constexpr void resize(size_t n)
    {
        if (n < m_size)
        {
            ix_bulk_destruct(m_data + n, m_size - n);
            m_size = n;
            return;
        }

        if (m_capacity < n)
        {
            reserve(n);
        }

        ix_bulk_default_construct(m_data + m_size, n - m_size);
        m_size = n;
    }

    ix_FORCE_INLINE constexpr void resize(size_t n, const T &x)
    {
        if (n < m_size)
        {
            ix_bulk_destruct(m_data + n, m_size - n);
            m_size = n;
            return;
        }

        if (m_capacity < n)
        {
            reserve(n);
        }

        T *p = m_data + m_size;
        const T *p_end = m_data + n;
        while (p < p_end)
        {
            new (p) T(x);
            p += 1;
        }

        m_size = n;
    }

    ix_FORCE_INLINE constexpr void set_size(size_t size)
    {
        m_size = size;
    }

    void swap_erase(size_t index)
    {
        const size_t last_index = m_size - 1;

        if (index != last_index)
        {
            (*this)[index] = ix_move((*this)[last_index]);
        }

        pop_back();
    }

    ix_FORCE_INLINE constexpr void push_back(const T &x)
    {
        emplace_back(x);
    }

    ix_FORCE_INLINE constexpr void push_back(T &&x)
    {
        emplace_back(ix_move(x));
    }

    template <typename... Args>
    ix_FORCE_INLINE constexpr T &emplace_back(Args &&...args)
    {
        if (ix_UNLIKELY(m_size == m_capacity))
        {
            size_t new_capacity = ix_grow_array_size(m_size);
            reserve(new_capacity);
        }

        T *p = m_data + m_size;
        new (p) T(ix_forward<Args>(args)...);
        m_size += 1;
        return *p;
    }

    ix_FORCE_INLINE constexpr T &back()
    {
        ix_ASSERT(0 < m_size);
        return m_data[m_size - 1];
    }

    ix_FORCE_INLINE constexpr const T &back() const
    {
        ix_ASSERT(0 < m_size);
        return m_data[m_size - 1];
    }

    ix_FORCE_INLINE constexpr void pop_back()
    {
        m_data[m_size - 1].~T();
        m_size -= 1;
    }

    ix_FORCE_INLINE constexpr void clear()
    {
        ix_bulk_destruct(m_data, m_size);
        m_size = 0;
    }

  private:
    ix_FORCE_INLINE constexpr bool is_data_initial() const
    {
        return (reinterpret_cast<uint8_t *>(m_data) == m_initial_data);
    }
};
// FILE END: ./src/ix/ix_StackVector.hpp



#include <WinBase.h>
#include <WinUser.h>
#include <stringapiset.h>

ix_Option<size_t> ix_win_utf8_to_wchar(
    const char *utf8_buf,
    size_t utf8_buf_len,
    wchar_t *wchar_buf,
    size_t wchar_buf_len
)
{
    if (utf8_buf_len == 0)
    {
        return 0;
    }

    if (utf8_buf == nullptr)
    {
        return ix_ERROR_INVALID_PARAMETER;
    }

    if ((wchar_buf_len == 0) && (wchar_buf != nullptr))
    {
        return ix_ERROR_BUFFER_TOO_SMALL;
    }

    const int ret = MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        utf8_buf,
        static_cast<int>(utf8_buf_len),
        wchar_buf,
        static_cast<int>(wchar_buf_len)
    );

    if (ret != 0)
    {
        return static_cast<size_t>(ret);
    }

    const DWORD err = GetLastError();
    switch (err)
    {
    case ERROR_INSUFFICIENT_BUFFER:
        return ix_ERROR_BUFFER_TOO_SMALL;
    case ERROR_NO_UNICODE_TRANSLATION:
        return ix_ERROR_INVALID_UTF8;
    case ERROR_INVALID_PARAMETER:
        return ix_ERROR_INVALID_PARAMETER;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
}


ix_Option<size_t> ix_win_wchar_to_utf8(
    const wchar_t *wchar_buf,
    size_t wchar_buf_len,
    char *utf8_buf,
    size_t utf8_buf_len
)
{
    if (wchar_buf_len == 0)
    {
        return 0;
    }

    if (wchar_buf == nullptr)
    {
        return ix_ERROR_INVALID_PARAMETER;
    }

    if ((utf8_buf_len == 0) && (utf8_buf != nullptr))
    {
        return ix_ERROR_BUFFER_TOO_SMALL;
    }

    const int ret = WideCharToMultiByte(
        CP_UTF8,
        0,
        wchar_buf,
        static_cast<int>(wchar_buf_len),
        utf8_buf,
        static_cast<int>(utf8_buf_len),
        nullptr,
        nullptr
    );

    if (ret != 0)
    {
        return static_cast<size_t>(ret);
    }

    const DWORD err = GetLastError();
    switch (err)
    {
    case ERROR_INSUFFICIENT_BUFFER:
        return ix_ERROR_BUFFER_TOO_SMALL;
    case ERROR_NO_UNICODE_TRANSLATION:
        return ix_ERROR_INVALID_UTF16;
    case ERROR_INVALID_PARAMETER:
        return ix_ERROR_INVALID_PARAMETER;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
}


ix_Option<size_t> utf8_path_to_wchar(const char *path_utf8, wchar_t path_wchar[ix_MAX_PATH + 1])
{
    const size_t path_utf8_length = ix_strlen(path_utf8);
    ix_ASSERT(path_utf8[path_utf8_length] == '\0');
    const ix_Option<size_t> opt = ix_win_utf8_to_wchar(path_utf8, path_utf8_length + 1, path_wchar, ix_MAX_PATH + 1);
    if (opt.result_kind() == ix_ERROR_BUFFER_TOO_SMALL)
    {
        return ix_ERROR_PATH_TOO_LONG;
    }
    return opt;
}

ix_Option<size_t> wchar_path_to_utf8(const wchar_t *path_wchar, size_t path_length, char path_utf8[ix_MAX_PATH + 1])
{
    ix_ASSERT(path_wchar[path_length] == '\0');
    const ix_Option<size_t> opt = ix_win_wchar_to_utf8(path_wchar, path_length + 1, path_utf8, ix_MAX_PATH + 1);
    if (opt.result_kind() == ix_ERROR_BUFFER_TOO_SMALL)
    {
        return ix_ERROR_PATH_TOO_LONG;
    }
    return opt;
}

ix_Result ix_win_utf8_to_wchar_dynamic(const char *utf8_buf, size_t utf8_buf_len, ix_Vector<wchar_t> &vector)
{
    const ix_Option<size_t> required_buf_length_opt = ix_win_utf8_to_wchar(utf8_buf, utf8_buf_len, nullptr, 0);
    if (required_buf_length_opt.is_error())
    {
        return required_buf_length_opt.result_kind();
    }

    const size_t required_buf_length = required_buf_length_opt.unwrap();
    vector.resize(required_buf_length);

    ix_win_utf8_to_wchar(utf8_buf, utf8_buf_len, vector.data(), vector.capacity()).unwrap();
    return ix_OK;
}


ix_Result ix_win_wchar_to_utf8_dynamic(const wchar_t *wchar_buf, size_t wchar_buf_len, ix_Vector<char> &vector)
{
    const ix_Option<size_t> required_buf_length_opt = ix_win_wchar_to_utf8(wchar_buf, wchar_buf_len, nullptr, 0);
    if (required_buf_length_opt.is_error())
    {
        return required_buf_length_opt.result_kind();
    }

    const size_t required_buf_length = required_buf_length_opt.unwrap();
    vector.resize(required_buf_length);

    ix_win_wchar_to_utf8(wchar_buf, wchar_buf_len, vector.data(), vector.capacity()).unwrap();
    return ix_OK;
}


ix_Option<ix_UniquePointer<const char *[]>> ix_win_wargs_to_args(
    int argc,
    const wchar_t *const *wargv,
    ix_Vector<char> &buf
)
{
    ix_ASSERT(argc >= 0);
    const size_t uargc = static_cast<size_t>(argc);
    ix_StackVector<size_t, 10> arg_lengths(uargc);
    ix_StackVector<size_t, 10> warg_lengths(uargc);
    size_t total_length = 0;
    for (size_t i = 0; i < uargc; i++)
    {
        const wchar_t *warg = wargv[i];
        const size_t wlength = ix_wstrlen(warg) + 1;
        const ix_Option<size_t> length_opt = ix_win_wchar_to_utf8(warg, wlength, nullptr, 0);
        if (length_opt.is_error())
        {
            return length_opt.result_kind();
        }
        const size_t length = length_opt.unwrap();
        total_length += length;
        arg_lengths[i] = length;
        warg_lengths[i] = wlength;
    }

    const size_t old_buf_size = buf.size();
    buf.reserve(old_buf_size + total_length);
    char *p = buf.data() + old_buf_size;
    size_t offset = 0;

    ix_UniquePointer<const char *[]> argv = ix_make_unique_array<const char *>(uargc);
    for (size_t i = 0; i < uargc; i++)
    {
        const wchar_t *warg = wargv[i];
        const size_t warg_length = warg_lengths[i];
        const size_t arg_length = arg_lengths[i];
        argv[i] = p + offset;
        const ix_Option<size_t> arg_length_opt = ix_win_wchar_to_utf8(warg, warg_length, p + offset, arg_length);
        if (arg_length_opt.is_error())
        {
            return arg_length_opt.result_kind();
        }
        ix_ASSERT(arg_length_opt.unwrap() == arg_length);
        offset += arg_length;
    }

    ix_ASSERT(offset == total_length);
    return argv;
}


const char *ix_win_msg_to_string(UINT uMsg)
{
    // clang-format off
    switch (uMsg)
    {
    case 0x0000: return "WM_NULL";
    case 0x0001: return "WM_CREATE";
    case 0x0002: return "WM_DESTROY";
    case 0x0003: return "WM_MOVE";
    case 0x0005: return "WM_SIZE";
    case 0x0006: return "WM_ACTIVATE";
    case 0x0007: return "WM_SETFOCUS";
    case 0x0008: return "WM_KILLFOCUS";
    case 0x000A: return "WM_ENABLE";
    case 0x000B: return "WM_SETREDRAW";
    case 0x000C: return "WM_SETTEXT";
    case 0x000D: return "WM_GETTEXT";
    case 0x000E: return "WM_GETTEXTLENGTH";
    case 0x000F: return "WM_PAINT";
    case 0x0010: return "WM_CLOSE";
    case 0x0011: return "WM_QUERYENDSESSION";
    case 0x0012: return "WM_QUIT";
    case 0x0013: return "WM_QUERYOPEN";
    case 0x0014: return "WM_ERASEBKGND";
    case 0x0015: return "WM_SYSCOLORCHANGE";
    case 0x0016: return "WM_ENDSESSION";
    case 0x0018: return "WM_SHOWWINDOW";
    case 0x001A: return "WM_SETTINGCHANGE (WM_WININICHANGE)";
    // case WM_WININICHANGE: return "WM_SETTINGCHANGE";
    case 0x001B: return "WM_DEVMODECHANGE";
    case 0x001C: return "WM_ACTIVATEAPP";
    case 0x001D: return "WM_FONTCHANGE";
    case 0x001E: return "WM_TIMECHANGE";
    case 0x001F: return "WM_CANCELMODE";
    case 0x0020: return "WM_SETCURSOR";
    case 0x0021: return "WM_MOUSEACTIVATE";
    case 0x0022: return "WM_CHILDACTIVATE";
    case 0x0023: return "WM_QUEUESYNC";
    case 0x0024: return "WM_GETMINMAXINFO";
    case 0x0026: return "WM_PAINTICON";
    case 0x0027: return "WM_ICONERASEBKGND";
    case 0x0028: return "WM_NEXTDLGCTL";
    case 0x002A: return "WM_SPOOLERSTATUS";
    case 0x002B: return "WM_DRAWITEM";
    case 0x002C: return "WM_MEASUREITEM";
    case 0x002D: return "WM_DELETEITEM";
    case 0x002E: return "WM_VKEYTOITEM";
    case 0x002F: return "WM_CHARTOITEM";
    case 0x0030: return "WM_SETFONT";
    case 0x0031: return "WM_GETFONT";
    case 0x0032: return "WM_SETHOTKEY";
    case 0x0033: return "WM_GETHOTKEY";
    case 0x0037: return "WM_QUERYDRAGICON";
    case 0x0039: return "WM_COMPAREITEM";
    case 0x003D: return "WM_GETOBJECT";
    case 0x0041: return "WM_COMPACTING";
    case 0x0044: return "WM_COMMNOTIFY";
    case 0x0046: return "WM_WINDOWPOSCHANGING";
    case 0x0047: return "WM_WINDOWPOSCHANGED";
    case 0x0048: return "WM_POWER";
    case 0x004A: return "WM_COPYDATA";
    case 0x004B: return "WM_CANCELJOURNAL";
    case 0x004E: return "WM_NOTIFY";
    case 0x0050: return "WM_INPUTLANGCHANGEREQUEST";
    case 0x0051: return "WM_INPUTLANGCHANGE";
    case 0x0052: return "WM_TCARD";
    case 0x0053: return "WM_HELP";
    case 0x0054: return "WM_USERCHANGED";
    case 0x0055: return "WM_NOTIFYFORMAT";
    case 0x007B: return "WM_CONTEXTMENU";
    case 0x007C: return "WM_STYLECHANGING";
    case 0x007D: return "WM_STYLECHANGED";
    case 0x007E: return "WM_DISPLAYCHANGE";
    case 0x007F: return "WM_GETICON";
    case 0x0080: return "WM_SETICON";
    case 0x0081: return "WM_NCCREATE";
    case 0x0082: return "WM_NCDESTROY";
    case 0x0083: return "WM_NCCALCSIZE";
    case 0x0084: return "WM_NCHITTEST";
    case 0x0085: return "WM_NCPAINT";
    case 0x0086: return "WM_NCACTIVATE";
    case 0x0087: return "WM_GETDLGCODE";
    case 0x0088: return "WM_SYNCPAINT";
    case 0x00A0: return "WM_NCMOUSEMOVE";
    case 0x00A1: return "WM_NCLBUTTONDOWN";
    case 0x00A2: return "WM_NCLBUTTONUP";
    case 0x00A3: return "WM_NCLBUTTONDBLCLK";
    case 0x00A4: return "WM_NCRBUTTONDOWN";
    case 0x00A5: return "WM_NCRBUTTONUP";
    case 0x00A6: return "WM_NCRBUTTONDBLCLK";
    case 0x00A7: return "WM_NCMBUTTONDOWN";
    case 0x00A8: return "WM_NCMBUTTONUP";
    case 0x00A9: return "WM_NCMBUTTONDBLCLK";
    case 0x00AB: return "WM_NCXBUTTONDOWN";
    case 0x00AC: return "WM_NCXBUTTONUP";
    case 0x00AD: return "WM_NCXBUTTONDBLCLK";
    case 0x00FE: return "WM_INPUT_DEVICE_CHANGE";
    case 0x00FF: return "WM_INPUT";
    case 0x0100: return "WM_KEYDOWN (WM_KEYFIRST)";
    // case 0x0100: return "WM_KEYFIRST";
    case 0x0101: return "WM_KEYUP";
    case 0x0102: return "WM_CHAR";
    case 0x0103: return "WM_DEADCHAR";
    case 0x0104: return "WM_SYSKEYDOWN";
    case 0x0105: return "WM_SYSKEYUP";
    case 0x0106: return "WM_SYSCHAR";
    case 0x0107: return "WM_SYSDEADCHAR";
    case 0x0109: return "WM_UNICHAR (WM_KEYLAST)";
    // case 0x0109: return "WM_UNICHAR";
    case 0x010D: return "WM_IME_STARTCOMPOSITION";
    case 0x010E: return "WM_IME_ENDCOMPOSITION";
    case 0x010F: return "WM_IME_COMPOSITION (WM_IME_KEYLAST)";
    // case 0x010F: return "WM_IME_KEYLAST";
    case 0x0110: return "WM_INITDIALOG";
    case 0x0111: return "WM_COMMAND";
    case 0x0112: return "WM_SYSCOMMAND";
    case 0x0113: return "WM_TIMER";
    case 0x0114: return "WM_HSCROLL";
    case 0x0115: return "WM_VSCROLL";
    case 0x0116: return "WM_INITMENU";
    case 0x0117: return "WM_INITMENUPOPUP";
    case 0x0119: return "WM_GESTURE";
    case 0x011A: return "WM_GESTURENOTIFY";
    case 0x011F: return "WM_MENUSELECT";
    case 0x0120: return "WM_MENUCHAR";
    case 0x0121: return "WM_ENTERIDLE";
    case 0x0122: return "WM_MENURBUTTONUP";
    case 0x0123: return "WM_MENUDRAG";
    case 0x0124: return "WM_MENUGETOBJECT";
    case 0x0125: return "WM_UNINITMENUPOPUP";
    case 0x0126: return "WM_MENUCOMMAND";
    case 0x0127: return "WM_CHANGEUISTATE";
    case 0x0128: return "WM_UPDATEUISTATE";
    case 0x0129: return "WM_QUERYUISTATE";
    case 0x0132: return "WM_CTLCOLORMSGBOX";
    case 0x0133: return "WM_CTLCOLOREDIT";
    case 0x0134: return "WM_CTLCOLORLISTBOX";
    case 0x0135: return "WM_CTLCOLORBTN";
    case 0x0136: return "WM_CTLCOLORDLG";
    case 0x0137: return "WM_CTLCOLORSCROLLBAR";
    case 0x0138: return "WM_CTLCOLORSTATIC";
    case 0x0200: return "WM_MOUSEMOVE (WM_MOUSEFIRST)";
    // case 0x0200: return "WM_MOUSEFIRST";
    case 0x0201: return "WM_LBUTTONDOWN";
    case 0x0202: return "WM_LBUTTONUP";
    case 0x0203: return "WM_LBUTTONDBLCLK";
    case 0x0204: return "WM_RBUTTONDOWN";
    case 0x0205: return "WM_RBUTTONUP";
    case 0x0206: return "WM_RBUTTONDBLCLK";
    case 0x0207: return "WM_MBUTTONDOWN";
    case 0x0208: return "WM_MBUTTONUP";
    case 0x0209: return "WM_MBUTTONDBLCLK";
    case 0x020A: return "WM_MOUSEWHEEL";
    case 0x020B: return "WM_XBUTTONDOWN";
    case 0x020C: return "WM_XBUTTONUP";
    case 0x020D: return "WM_XBUTTONDBLCLK";
    case 0x020E: return "WM_MOUSEHWHEEL (WM_MOUSELAST)";
    // case 0x020E: return "WM_MOUSELAST";
    case 0x0210: return "WM_PARENTNOTIFY";
    case 0x0211: return "WM_ENTERMENULOOP";
    case 0x0212: return "WM_EXITMENULOOP";
    case 0x0213: return "WM_NEXTMENU";
    case 0x0214: return "WM_SIZING";
    case 0x0215: return "WM_CAPTURECHANGED";
    case 0x0216: return "WM_MOVING";
    case 0x0218: return "WM_POWERBROADCAST";
    case 0x0219: return "WM_DEVICECHANGE";
    case 0x0220: return "WM_MDICREATE";
    case 0x0221: return "WM_MDIDESTROY";
    case 0x0222: return "WM_MDIACTIVATE";
    case 0x0223: return "WM_MDIRESTORE";
    case 0x0224: return "WM_MDINEXT";
    case 0x0225: return "WM_MDIMAXIMIZE";
    case 0x0226: return "WM_MDITILE";
    case 0x0227: return "WM_MDICASCADE";
    case 0x0228: return "WM_MDIICONARRANGE";
    case 0x0229: return "WM_MDIGETACTIVE";
    case 0x0230: return "WM_MDISETMENU";
    case 0x0231: return "WM_ENTERSIZEMOVE";
    case 0x0232: return "WM_EXITSIZEMOVE";
    case 0x0233: return "WM_DROPFILES";
    case 0x0234: return "WM_MDIREFRESHMENU";
    case 0x0240: return "WM_TOUCH";
    case 0x0241: return "WM_NCPOINTERUPDATE";
    case 0x0242: return "WM_NCPOINTERDOWN";
    case 0x0243: return "WM_NCPOINTERUP";
    case 0x0245: return "WM_POINTERUPDATE";
    case 0x0246: return "WM_POINTERDOWN";
    case 0x0247: return "WM_POINTERUP";
    case 0x0249: return "WM_POINTERENTER";
    case 0x024A: return "WM_POINTERLEAVE";
    case 0x024B: return "WM_POINTERACTIVATE";
    case 0x024C: return "WM_POINTERCAPTURECHANGED";
    case 0x024D: return "WM_TOUCHHITTESTING";
    case 0x024E: return "WM_POINTERWHEEL";
    case 0x024F: return "WM_POINTERHWHEEL";
    case 0x0251: return "WM_POINTERROUTEDTO";
    case 0x0252: return "WM_POINTERROUTEDAWAY";
    case 0x0253: return "WM_POINTERROUTEDRELEASED";
    case 0x0281: return "WM_IME_SETCONTEXT";
    case 0x0282: return "WM_IME_NOTIFY";
    case 0x0283: return "WM_IME_CONTROL";
    case 0x0284: return "WM_IME_COMPOSITIONFULL";
    case 0x0285: return "WM_IME_SELECT";
    case 0x0286: return "WM_IME_CHAR";
    case 0x0288: return "WM_IME_REQUEST";
    case 0x0290: return "WM_IME_KEYDOWN";
    case 0x0291: return "WM_IME_KEYUP";
    case 0x02A0: return "WM_NCMOUSEHOVER";
    case 0x02A1: return "WM_MOUSEHOVER";
    case 0x02A2: return "WM_NCMOUSELEAVE";
    case 0x02A3: return "WM_MOUSELEAVE";
    case 0x02B1: return "WM_WTSSESSION_CHANGE";
    case 0x02c0: return "WM_TABLET_FIRST";
    case 0x02df: return "WM_TABLET_LAST";
    case 0x02E0: return "WM_DPICHANGED";
    case 0x02E2: return "WM_DPICHANGED_BEFOREPARENT";
    case 0x02E3: return "WM_DPICHANGED_AFTERPARENT";
    case 0x02E4: return "WM_GETDPISCALEDSIZE";
    case 0x0300: return "WM_CUT";
    case 0x0301: return "WM_COPY";
    case 0x0302: return "WM_PASTE";
    case 0x0303: return "WM_CLEAR";
    case 0x0304: return "WM_UNDO";
    case 0x0305: return "WM_RENDERFORMAT";
    case 0x0306: return "WM_RENDERALLFORMATS";
    case 0x0307: return "WM_DESTROYCLIPBOARD";
    case 0x0308: return "WM_DRAWCLIPBOARD";
    case 0x0309: return "WM_PAINTCLIPBOARD";
    case 0x030A: return "WM_VSCROLLCLIPBOARD";
    case 0x030B: return "WM_SIZECLIPBOARD";
    case 0x030C: return "WM_ASKCBFORMATNAME";
    case 0x030D: return "WM_CHANGECBCHAIN";
    case 0x030E: return "WM_HSCROLLCLIPBOARD";
    case 0x030F: return "WM_QUERYNEWPALETTE";
    case 0x0310: return "WM_PALETTEISCHANGING";
    case 0x0311: return "WM_PALETTECHANGED";
    case 0x0312: return "WM_HOTKEY";
    case 0x0317: return "WM_PRINT";
    case 0x0318: return "WM_PRINTCLIENT";
    case 0x0319: return "WM_APPCOMMAND";
    case 0x031A: return "WM_THEMECHANGED";
    case 0x031D: return "WM_CLIPBOARDUPDATE";
    case 0x031E: return "WM_DWMCOMPOSITIONCHANGED";
    case 0x031F: return "WM_DWMNCRENDERINGCHANGED";
    case 0x0320: return "WM_DWMCOLORIZATIONCOLORCHANGED";
    case 0x0321: return "WM_DWMWINDOWMAXIMIZEDCHANGE";
    case 0x0323: return "WM_DWMSENDICONICTHUMBNAIL";
    case 0x0326: return "WM_DWMSENDICONICLIVEPREVIEWBITMAP";
    case 0x033F: return "WM_GETTITLEBARINFOEX";
    case 0x0345: return "WM_TOOLTIPDISMISS";
    case 0x0358: return "WM_HANDHELDFIRST";
    case 0x035F: return "WM_HANDHELDLAST";
    case 0x0360: return "WM_AFXFIRST";
    case 0x037F: return "WM_AFXLAST";
    case 0x0380: return "WM_PENWINFIRST";
    case 0x038F: return "WM_PENWINLAST";
    case 0x0400: return "WM_USER";
    case 0x238: return "WM_POINTERDEVICECHANGE";
    case 0x239: return "WM_POINTERDEVICEINRANGE";
    case 0x23A: return "WM_POINTERDEVICEOUTOFRANGE";
    case 0x8000: return "WM_APP";
    default: return "UNKNOWN";
    }
    // clang-format on
}


[[noreturn]] void ix_win_abort_with_info(const char *file, int line, const char *msg)
{
    CHAR buf[128] = {};
    const DWORD err = GetLastError();
    const DWORD ret = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr,
        err,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        buf,
        sizeof(buf),
        nullptr
    );
    if (ret == 0)
    {
        ix_log_fatal("\n%s:%d: %s failed (Something went wrong: FormatMessage() failed)", file, line, msg);
        ix_abort();
    }

    // trim the newline
    ix_ASSERT(ret > 0);
    DWORD i = ret - 1;
    while ((buf[i] == '\r') || (buf[i] == '\n'))
    {
        buf[i] = '\0';
        i -= 1;
    }

    ix_log_fatal("\n%s:%d: %s failed (%s)", file, line, msg, buf);
    ix_abort();
}


#endif
// FILE END: ./src/ix/ix_util_win.cpp
// FILE BEGIN: ./external/sokol_time/sokol_time.c
#define SOKOL_TIME_IMPL
// FILE END: ./external/sokol_time/sokol_time.c
