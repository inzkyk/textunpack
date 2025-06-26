#pragma once

#include "ix.hpp"
#include "ix_Vector.hpp"
#include "ix_print.hpp"

// This header file is needed by almost all of the other files, so take extra care to reduce compile time.

class ix_TestContext;
class ix_TestCase;

namespace ix_global
{
extern ix_TestContext test_context;
} // namespace ix_global

int ix_do_test(int argc, const char *const *argv);
int ix_test_consume(const int *, int);
int ix_test_register(const ix_TestCase &test_case);

struct ix_TestOption
{
    const char *filter = nullptr;
    uint64_t abort_after = 1;
    bool print_execution_time = false;
    bool debug_break_on_error = true;
    bool ignore_time = false; // For tests (of this testing module)
};

class ix_TestCase
{
    const char *m_name;
    const char *m_file;
    int m_line;
    void (*m_body)(ix_TestContext &);
    uint64_t m_execution_time;

    friend class ix_TestContext;

  public:
    ix_TestCase(const char *name, void (*body)(ix_TestContext &), const char *file, int line);
};

class ix_TestContext
{
    ix_TestOption m_option;
    uint64_t m_num_assertions_executed;
    uint64_t m_num_assertions_failed;
    ix_TestCase *m_current_test_case;
    ix_Writer *m_writer;
    ix_Vector<ix_TestCase> m_test_cases;

  public:
    ix_TestContext() = default;

    void register_test_case(const ix_TestCase &test_case);
    bool expect(bool v, const char *expr, int line);
    int execute(ix_Writer &writer, const ix_TestOption &option);
    void print_expect_eq_failure(
        size_t (*print)(ix_Writer &writer, const void *),
        const void *lhs,
        const char *lhs_str,
        const void *rhs,
        const char *rhs_str,
        int line
    );

    void print_expect_neq_failure(
        size_t (*print)(ix_Writer &writer, const void *),
        const void *lhs,
        const char *lhs_str,
        const void *rhs,
        const char *rhs_str,
        int line
    );

    ix_DISABLE_GCC_WARNING_BEGIN
    ix_DISABLE_GCC_WARNING("-Wfloat-equal")
    ix_DISABLE_CLANG_WARNING_BEGIN
    ix_DISABLE_CLANG_WARNING("-Wfloat-equal")

    template <typename T>
    bool expect_eq(const T &lhs, const T &rhs, const char *lhs_str, const char *rhs_str, int line)
    {
        const bool too_many_failures = (m_option.abort_after <= m_num_assertions_failed);
        if (too_many_failures)
        {
            return false;
        }

        m_num_assertions_executed += 1;

        if (ix_LIKELY(lhs == rhs))
        {
            return false;
        }

        if constexpr (ix_Print<T>::is_implemented)
        {
            print_expect_eq_failure(
                [](ix_Writer &writer, const void *p) -> size_t {
                    return ix_Print<T>::print(writer, *static_cast<const T *>(p));
                },
                static_cast<const void *>(&lhs),
                lhs_str,
                static_cast<const void *>(&rhs),
                rhs_str,
                line
            );
        }
        else
        {
            print_expect_eq_failure(nullptr, nullptr, lhs_str, nullptr, rhs_str, line);
        }

        m_num_assertions_failed += 1;
        return m_option.debug_break_on_error;
    }

    template <typename T>
    bool expect_neq(const T &lhs, const T &rhs, const char *lhs_str, const char *rhs_str, int line)
    {
        const bool too_many_failures = (m_option.abort_after <= m_num_assertions_failed);
        if (too_many_failures)
        {
            return false;
        }

        m_num_assertions_executed += 1;

        if (ix_LIKELY(!(lhs == rhs)))
        {
            return false;
        }

        if constexpr (ix_Print<T>::is_implemented)
        {
            print_expect_neq_failure(
                [](ix_Writer &writer, const void *p) -> size_t {
                    return ix_Print<T>::print(writer, *static_cast<const T *>(p));
                },
                static_cast<const void *>(&lhs),
                lhs_str,
                static_cast<const void *>(&rhs),
                rhs_str,
                line
            );
        }
        else
        {
            print_expect_neq_failure(nullptr, nullptr, lhs_str, nullptr, rhs_str, line);
        }

        m_num_assertions_failed += 1;
        return m_option.debug_break_on_error;
    }

    ix_DISABLE_GCC_WARNING_END
    ix_DISABLE_CLANG_WARNING_END
};

struct ix_TestString
{
    const char *s;
};

bool operator==(ix_TestString a, ix_TestString b);

template <>
struct ix_Print<ix_TestString>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, ix_TestString x);
};

struct ix_TestFloat
{
    float v;
};

bool operator==(ix_TestFloat a, ix_TestFloat b);
bool operator==(float a, ix_TestFloat b);

template <>
struct ix_Print<ix_TestFloat>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, ix_TestFloat x);
};

struct ix_TestDouble
{
    double v;
};

bool operator==(ix_TestDouble a, ix_TestDouble b);
bool operator==(double a, ix_TestDouble b);

template <>
struct ix_Print<ix_TestDouble>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, ix_TestDouble x);
};

#if ix_DO_TEST

// NOLINTBEGIN(fuchsia-statically-constructed-objects,bugprone-macro-parentheses)
#define ix_KEEP_ALIVE(name, expr)                           \
    ix_DISABLE_CLANG_WARNING_BEGIN                          \
    ix_DISABLE_CLANG_WARNING("-Wglobal-constructors")       \
    static const int name = ix_test_consume(&(name), expr); \
    ix_DISABLE_CLANG_WARNING_END
// NOLINTEND(fuchsia-statically-constructed-objects,bugprone-macro-parentheses)

#define ix_DECLARE_AND_REGISTER_TEST(f, name)                      \
    static void f(ix_TestContext &);                               \
    ix_KEEP_ALIVE(                                                 \
        ix_PP_ADD_UNIQUE_ID(ix_TEST_TEMP_VAR_),                    \
        ix_test_register(ix_TestCase(name, f, __FILE__, __LINE__)) \
    );                                                             \
    static void f(ix_TestContext &)

#if ix_COMPILER(MSVC)
#define ix_TEST_DEBUG_BREAK() __debugbreak()
#else
#define ix_TEST_DEBUG_BREAK()
#endif

#define ix_TEST_CASE(name) ix_DECLARE_AND_REGISTER_TEST(ix_PP_ADD_UNIQUE_ID(ix_TEST_BODY_), name)

#define ix_EXPECT(...)                                                           \
    do                                                                           \
    {                                                                            \
        if (ix_global::test_context.expect(__VA_ARGS__, #__VA_ARGS__, __LINE__)) \
        {                                                                        \
            ix_TEST_DEBUG_BREAK();                                               \
        }                                                                        \
    } while (0)

#define ix_EXPECT_EQ(lhs, rhs)                                                                                     \
    do                                                                                                             \
    {                                                                                                              \
        if (ix_global::test_context.expect_eq((lhs), ix_remove_cvref_t<decltype(lhs)>(rhs), #lhs, #rhs, __LINE__)) \
        {                                                                                                          \
            ix_TEST_DEBUG_BREAK();                                                                                 \
        }                                                                                                          \
    } while (0)

#define ix_EXPECT_EQF(lhs, rhs)                                                                       \
    do                                                                                                \
    {                                                                                                 \
        if (ix_global::test_context                                                                   \
                .expect_eq<ix_TestFloat>(ix_TestFloat{lhs}, ix_TestFloat{rhs}, #lhs, #rhs, __LINE__)) \
        {                                                                                             \
            ix_TEST_DEBUG_BREAK();                                                                    \
        }                                                                                             \
    } while (0)

#define ix_EXPECT_EQD(lhs, rhs)                                                                          \
    do                                                                                                   \
    {                                                                                                    \
        if (ix_global::test_context                                                                      \
                .expect_eq<ix_TestDouble>(ix_TestDouble{lhs}, ix_TestDouble{rhs}, #lhs, #rhs, __LINE__)) \
        {                                                                                                \
            ix_TEST_DEBUG_BREAK();                                                                       \
        }                                                                                                \
    } while (0)

#define ix_EXPECT_EQSTR(lhs, rhs)                                                                        \
    do                                                                                                   \
    {                                                                                                    \
        if (ix_global::test_context                                                                      \
                .expect_eq<ix_TestString>(ix_TestString{lhs}, ix_TestString{rhs}, #lhs, #rhs, __LINE__)) \
        {                                                                                                \
            ix_TEST_DEBUG_BREAK();                                                                       \
        }                                                                                                \
    } while (0)

#define ix_EXPECT_NEQ(lhs, rhs)                                                                                     \
    do                                                                                                              \
    {                                                                                                               \
        if (ix_global::test_context.expect_neq((lhs), ix_remove_cvref_t<decltype(lhs)>(rhs), #lhs, #rhs, __LINE__)) \
        {                                                                                                           \
            ix_TEST_DEBUG_BREAK();                                                                                  \
        }                                                                                                           \
    } while (0)

#define ix_EXPECT_NEQF(lhs, rhs)                                                                       \
    do                                                                                                 \
    {                                                                                                  \
        if (ix_global::test_context                                                                    \
                .expect_neq<ix_TestFloat>(ix_TestFloat{lhs}, ix_TestFloat{rhs}, #lhs, #rhs, __LINE__)) \
        {                                                                                              \
            ix_TEST_DEBUG_BREAK();                                                                     \
        }                                                                                              \
    } while (0)

#define ix_EXPECT_NEQD(lhs, rhs)                                                                          \
    do                                                                                                    \
    {                                                                                                     \
        if (ix_global::test_context                                                                       \
                .expect_neq<ix_TestDouble>(ix_TestDouble{lhs}, ix_TestDouble{rhs}, #lhs, #rhs, __LINE__)) \
        {                                                                                                 \
            ix_TEST_DEBUG_BREAK();                                                                        \
        }                                                                                                 \
    } while (0)

#define ix_EXPECT_NEQSTR(lhs, rhs)                                                                        \
    do                                                                                                    \
    {                                                                                                     \
        if (ix_global::test_context                                                                       \
                .expect_neq<ix_TestString>(ix_TestString{lhs}, ix_TestString{rhs}, #lhs, #rhs, __LINE__)) \
        {                                                                                                 \
            ix_TEST_DEBUG_BREAK();                                                                        \
        }                                                                                                 \
    } while (0)

#endif
