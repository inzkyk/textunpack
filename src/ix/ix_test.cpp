#include "ix_test.hpp"
#include "ix_CmdArgsEater.hpp"
#include "ix_SystemManager.hpp"
#include "ix_Writer.hpp"
#include "ix_file.hpp"
#include "ix_floating_point.hpp"
#include "ix_initializer_list.hpp"
#include "ix_math.hpp"
#include "ix_scanf.hpp"
#include "ix_sort.hpp"
#include "ix_string.hpp"
#include "ix_time.hpp"

#include <inttypes.h>

// NOLINTBEGIN(fuchsia-statically-constructed-objects)
ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wglobal-constructors")
ix_DISABLE_CLANG_WARNING("-Wexit-time-destructors")
namespace ix_global
{
ix_TestContext test_context;
} // namespace ix_global
ix_DISABLE_CLANG_WARNING_END
// NOLINTEND(fuchsia-statically-constructed-objects)

static ix_Vector<ix_TestCase> &get_global_test_cases()
{
    ix_DISABLE_CLANG_WARNING_BEGIN
    ix_DISABLE_CLANG_WARNING("-Wexit-time-destructors")
    static ix_Vector<ix_TestCase> g_test_cases;
    ix_DISABLE_CLANG_WARNING_END
    return g_test_cases;
}

static ix_Option<ix_TestOption> parse_test_option(int argc, const char *const *argv, ix_Writer *err)
{
    if (argv == nullptr)
    {
        ix_ASSERT(argc == 0);
        return ix_TestOption();
    }

    ix_CmdArgsEater args(argc, argv);
    ix_TestOption option;

    const char *filter = args.eat_kv({"--filter", "-f"});
    if (filter != nullptr)
    {
        option.filter = filter;
    }

    const char *abort_after_str = args.eat_kv({"--abort-after", "-aa"});
    if (abort_after_str != nullptr)
    {
        ix_Option<uint64_t> abort_after = ix_string_convert<uint64_t>(abort_after_str);
        if (abort_after.is_error())
        {
            const ix_Option<size_t> result = err->write_stringf("[ix_do_test] Not a number: %s", abort_after_str);
            if (result.is_error())
            {
                return result.result_kind();
            }
            return abort_after.result_kind();
        }
        option.abort_after = abort_after.unwrap();
    }

    if (args.eat_boolean({"--print-execution-time", "-pet"}))
    {
        option.print_execution_time = true;
    }

    if (args.eat_boolean({"--no-debug-break", "-ndb"}))
    {
        option.debug_break_on_error = false;
    }

    return option;
}

#if ix_DO_TEST
static ix_Option<ix_TestOption> parse_option(std::initializer_list<const char *> args, ix_Writer *err)
{
    return parse_test_option(static_cast<int>(args.size()), args.begin(), err);
}

ix_TEST_CASE("parse_test_option")
{
    char buf[128];
    auto err = ix_Writer::from_existing_array(buf);

    {
        const ix_TestOption o;
        ix_EXPECT_EQ(o.filter, nullptr);
        ix_EXPECT_EQ(o.abort_after, 1);
        ix_EXPECT(!o.print_execution_time);
        ix_EXPECT(o.debug_break_on_error);
    }

    {
        const ix_TestOption o = parse_test_option(0, nullptr, nullptr).unwrap();
        ix_EXPECT_EQ(o.filter, nullptr);
        ix_EXPECT_EQ(o.abort_after, 1);
        ix_EXPECT(!o.print_execution_time);
        ix_EXPECT(o.debug_break_on_error);
    }

    {
        ix_Option<ix_TestOption> op = parse_option(
            {"ix_do_test", "--filter", "foo", "--abort-after", "20", "--print-execution-time", "--no-debug-break"},
            &err
        );
        ix_EXPECT(op.is_ok());
        err.end_string();
        ix_EXPECT_EQSTR(err.data(), "");
        const ix_TestOption &o = op.unwrap();
        ix_EXPECT_EQSTR(o.filter, "foo");
        ix_EXPECT_EQ(o.abort_after, 20);
        ix_EXPECT(o.print_execution_time);
        ix_EXPECT(!o.debug_break_on_error);
    }
    err.clear();
    {
        ix_Option<ix_TestOption> op = parse_option({"ix_do_test", "-f", "foo", "-aa", "20", "-pet", "-ndb"}, &err);
        ix_EXPECT(op.is_ok());
        err.end_string();
        ix_EXPECT_EQSTR(err.data(), "");
        const ix_TestOption &o = op.unwrap();
        ix_EXPECT_EQSTR(o.filter, "foo");
        ix_EXPECT_EQ(o.abort_after, 20);
        ix_EXPECT(o.print_execution_time);
        ix_EXPECT(!o.debug_break_on_error);
    }
    err.clear();
    {
        const ix_Option<ix_TestOption> op = parse_option({"ix_do_test", "-aa", "FOO"}, &err);
        ix_EXPECT(op.is_error());
        err.end_string();
        ix_EXPECT_EQSTR(err.data(), "[ix_do_test] Not a number: FOO");
    }
}
#endif

int ix_do_test(int argc, const char *const *argv)
{
    const bool standalone = !ix_SystemManager::is_initialized();
    if (standalone)
    {
        auto &sm = ix_SystemManager::initialize();
        sm.initialize_stdio().assert_ok();
        sm.initialize_time().assert_ok();
        sm.initialize_logger().assert_ok();
    }
    else
    {
        const auto &sm = ix_SystemManager::get();
        sm.assert_initialized("stdio");
        sm.assert_initialized("time");
        sm.assert_initialized("logger");
    }

    ix_Writer writer(1024, &ix_FileHandle::of_stderr());
    const ix_TestOption option = parse_test_option(argc, argv, &writer).unwrap();
    const int ret = ix_global::test_context.execute(writer, option);
    if (standalone)
    {
        ix_SystemManager::deinitialize();
    }
    return ret;
}

int ix_test_consume(const int *, int)
{
    return 0;
}

int ix_test_register(const ix_TestCase &test_case)
{
    get_global_test_cases().push_back(test_case);
    return 0;
}

ix_TestCase::ix_TestCase(const char *name, void (*body)(ix_TestContext &), const char *file, int line)
    : m_name(name),
      m_file(file),
      m_line(line),
      m_body(body),
      m_execution_time(0)
{
}

void ix_TestContext::register_test_case(const ix_TestCase &test_case)
{
    m_test_cases.push_back(test_case);
}

int ix_TestContext::execute(ix_Writer &writer, const ix_TestOption &option)
{
    const bool is_global = (this == &ix_global::test_context);
    ix_Vector<ix_TestCase> &global_test_cases = get_global_test_cases();
    if (is_global && !global_test_cases.empty())
    {
        m_test_cases = ix_move(global_test_cases);
    }

    m_writer = &writer;
    m_option = option;
    m_option.abort_after = m_option.abort_after == 0 ? ix_UINT64_MAX : m_option.abort_after;

    uint64_t num_test_cases_executed = 0;
    m_num_assertions_executed = 0;
    m_num_assertions_failed = 0;
    const uint64_t test_start = ix_time_now();
    for (ix_TestCase &tc : m_test_cases)
    {
        const bool skip = (option.filter != nullptr) && !ix_wildcard_match(tc.m_name, option.filter);
        if (skip)
        {
            continue;
        }

        num_test_cases_executed += 1;
        m_current_test_case = &tc;
        const uint64_t start = ix_time_now();
        tc.m_body(*this);
        const uint64_t end = ix_time_now();
        tc.m_execution_time = end - start;
        if (tc.m_execution_time == 0)
        {
            tc.m_execution_time = 1;
        }
        if (m_num_assertions_failed == m_option.abort_after)
        {
            break;
        }
    }
    const uint64_t test_end = ix_time_now();

    m_writer->write_stringf(
        "[ix_test] %" PRIu64 " test cases are registered.\n"
        "[ix_test] Executed %" PRIu64 " cases and %" PRIu64 " assertions in %.4f seconds.\n",
        m_test_cases.size(),
        num_test_cases_executed,
        m_num_assertions_executed,
        m_option.ignore_time ? 0 : ix_time_diff_to_sec(test_end - test_start)
    );

    const bool all_passed = (m_num_assertions_failed == 0);
    if (all_passed)
    {
        m_writer->write_stringf("[ix_test] All tests passed!\n");
    }
    else
    {
        m_writer->write_stringf("[ix_test] %" PRIu64 " assertions failed.\n", m_num_assertions_failed);
    }

    if (option.print_execution_time)
    {
        ix_sort(m_test_cases.span(), [](const ix_TestCase &x, const ix_TestCase &y) {
            return x.m_execution_time > y.m_execution_time;
        });

        for (const ix_TestCase &tc : m_test_cases)
        {
            const size_t line_log10 = ix_log10(static_cast<size_t>(tc.m_line));
            if (tc.m_execution_time > 0)
            {
                m_writer->write_stringf("%s:%d:", tc.m_file, tc.m_line);
                m_writer->write_char_repeat(' ', (line_log10 < 5) ? 5 - line_log10 : 1);
                m_writer->write_stringf("%fms   %s\n", ix_time_diff_to_ms(tc.m_execution_time), tc.m_name);
            }
            else
            {
                m_writer->write_stringf("%s:%d:", tc.m_file, tc.m_line);
                m_writer->write_char_repeat(' ', (line_log10 < 5) ? 5 - line_log10 : 1);
                m_writer->write_stringf("NOT EXECUTED   %s\n", tc.m_name);
            }
        }
    }

    return all_passed ? 0 : 1;
}

void ix_TestContext::print_expect_eq_failure(
    size_t (*print)(ix_Writer &writer, const void *),
    const void *lhs,
    const char *lhs_str,
    const void *rhs,
    const char *rhs_str,
    int line
)
{
    m_writer->write_stringf(
        "%s:%d: In \"%s\":\n"
        "%s != %s\n",
        m_current_test_case->m_file,
        line,
        m_current_test_case->m_name,
        lhs_str,
        rhs_str
    );
    if (print != nullptr)
    {
        m_writer->write_stringf("LHS = ");
        print(*m_writer, lhs);
        m_writer->write_stringf("\nRHS = ");
        print(*m_writer, rhs);
        m_writer->write_char('\n');
    }
}

void ix_TestContext::print_expect_neq_failure(
    size_t (*print)(ix_Writer &writer, const void *),
    const void *lhs,
    const char *lhs_str,
    const void *rhs,
    const char *rhs_str,
    int line
)
{
    m_writer->write_stringf(
        "%s:%d: In \"%s\":\n"
        "%s == %s\n",
        m_current_test_case->m_file,
        line,
        m_current_test_case->m_name,
        lhs_str,
        rhs_str
    );

    if (print != nullptr)
    {
        m_writer->write_stringf("LHS = ");
        print(*m_writer, lhs);
        m_writer->write_stringf("\nRHS = ");
        print(*m_writer, rhs);
        m_writer->write_char('\n');
    }
}

bool ix_TestContext::expect(bool v, const char *expr, int line)
{
    const bool too_many_failures = (m_option.abort_after <= m_num_assertions_failed);
    if (too_many_failures)
    {
        return false;
    }

    m_num_assertions_executed += 1;
    if (v)
    {
        return false;
    }

    m_num_assertions_failed += 1;
    m_writer->write_stringf(
        "%s:%d: In \"%s\":\n"
        "ix_EXPECT(%s) failed!\n",
        m_current_test_case->m_file,
        line,
        m_current_test_case->m_name,
        expr
    );

    return m_option.debug_break_on_error;
}

bool operator==(ix_TestString a, ix_TestString b)
{
    return (ix_strcmp(a.s, b.s) == 0);
}

size_t ix_Print<ix_TestString>::print(ix_Writer &writer, ix_TestString x)
{
    return writer.write_stringf("\"%s\"", x.s);
}

bool operator==(ix_TestFloat a, ix_TestFloat b)
{
    return ix_is_approximately_equal(a.v, b.v);
}

bool operator==(float a, ix_TestFloat b)
{
    return ix_is_approximately_equal(a, b.v);
}

size_t ix_Print<ix_TestFloat>::print(ix_Writer &writer, ix_TestFloat x)
{
    return ix_Print<float>::print(writer, x.v);
}

bool operator==(ix_TestDouble a, ix_TestDouble b)
{
    return ix_is_approximately_equal(a.v, b.v);
}

bool operator==(double a, ix_TestDouble b)
{
    return ix_is_approximately_equal(a, b.v);
}

size_t ix_Print<ix_TestDouble>::print(ix_Writer &writer, ix_TestDouble x)
{
    return ix_Print<double>::print(writer, x.v);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_EXPECT")
{
    ix_EXPECT(true);
    ix_EXPECT(2 == 1 + 1);
}

ix_TEST_CASE("ix_EXPECT")
{
    ix_EXPECT(true);
    ix_EXPECT(2 == 1 + 1);
}

ix_TEST_CASE("ix_EXPECT_EQ")
{
    ix_EXPECT_EQ(1, 1);
    ix_EXPECT_EQ(nullptr, nullptr);
}

ix_TEST_CASE("ix_EXPECT_EQ")
{
    ix_EXPECT_EQ(1, 1);
    ix_EXPECT_EQ(nullptr, nullptr);
}

ix_TEST_CASE("ix_EXPECT_EQF")
{
    ix_EXPECT_EQF(0.0, 0.0);
    ix_EXPECT_EQF(3.0, 3.0);
    ix_EXPECT_EQF(-3.0, -3.0);
}

ix_TEST_CASE("ix_EXPECT_EQSTR")
{
    ix_EXPECT_EQSTR("", "");
    ix_EXPECT_EQSTR("foo", "foo");
    ix_EXPECT_EQSTR("foobar", "foobar");
}

ix_TEST_CASE("ix_TestContext")
{
    ix_TestContext ctx;
    const ix_TestCase test_cases[] = {
        ix_TestCase(
            "expect (success)",
            [](ix_TestContext &c) { c.expect(true, "true", 0); },
            "tmp",
            0
        ),
        ix_TestCase(
            "expect (failure)",
            [](ix_TestContext &c) { c.expect(false, "false", 1); },
            "tmp",
            1
        ),
        ix_TestCase(
            "expect_eq (int, success)",
            [](ix_TestContext &c) { c.expect_eq(1, 1, "1", "1", 2); },
            "tmp",
            2
        ),
        ix_TestCase(
            "expect_eq (int, failure)",
            [](ix_TestContext &c) { c.expect_eq(1, 2, "1", "2", 3); },
            "tmp",
            3
        ),
        ix_TestCase(
            "expect_eq (float, success)",
            [](ix_TestContext &c) { c.expect_eq(ix_TestFloat{1.0F}, ix_TestFloat{1.0F}, "1.0F", "1.0F", 4); },
            "tmp",
            4
        ),
        ix_TestCase(
            "expect_eq (float, failure)",
            [](ix_TestContext &c) { c.expect_eq(ix_TestFloat{1.0F}, ix_TestFloat{2.0F}, "1.0F", "2.0F", 5); },
            "tmp",
            5
        ),
        ix_TestCase(
            "expect_eq (string, success)",
            [](ix_TestContext &c) { c.expect_eq(ix_TestString{"a"}, ix_TestString{"a"}, "\"a\"", "\"a\"", 6); },
            "tmp",
            6
        ),
        ix_TestCase(
            "expect_eq (string, failure)",
            [](ix_TestContext &c) { c.expect_eq(ix_TestString{"a"}, ix_TestString{"b"}, "\"a\"", "\"b\"", 7); },
            "tmp",
            7
        ),
        ix_TestCase(
            "expect_neq (int, failure)",
            [](ix_TestContext &c) { c.expect_neq(1, 1, "1", "1", 2); },
            "tmp",
            8
        ),
        ix_TestCase(
            "expect_neq (int, success)",
            [](ix_TestContext &c) { c.expect_neq(1, 2, "1", "2", 3); },
            "tmp",
            9
        ),
        ix_TestCase(
            "expect_neq (float, failure)",
            [](ix_TestContext &c) { c.expect_neq(ix_TestFloat{1.0F}, ix_TestFloat{1.0F}, "1.0F", "1.0F", 4); },
            "tmp",
            10
        ),
        ix_TestCase(
            "expect_neq (float, success)",
            [](ix_TestContext &c) { c.expect_neq(ix_TestFloat{1.0F}, ix_TestFloat{2.0F}, "1.0F", "2.0F", 5); },
            "tmp",
            11
        ),
        ix_TestCase(
            "expect_neq (string, failure)",
            [](ix_TestContext &c) { c.expect_neq(ix_TestString{"a"}, ix_TestString{"a"}, "\"a\"", "\"a\"", 6); },
            "tmp",
            12
        ),
        ix_TestCase(
            "expect_neq (string, success)",
            [](ix_TestContext &c) { c.expect_neq(ix_TestString{"a"}, ix_TestString{"b"}, "\"a\"", "\"b\"", 7); },
            "tmp",
            13
        ),

    };

    for (size_t i = 0; i < ix_LENGTH_OF(test_cases); i++)
    {
        ctx.register_test_case(test_cases[i]);
    }

    ix_Writer writer(1024);
    ix_TestOption option;
    option.ignore_time = true;
    option.debug_break_on_error = false;
    option.abort_after = 0;
    ctx.execute(writer, option);
    writer.end_string();
    const char *expected = R"*(tmp:1: In "expect (failure)":
ix_EXPECT(false) failed!
tmp:3: In "expect_eq (int, failure)":
1 != 2
LHS = 1
RHS = 2
tmp:5: In "expect_eq (float, failure)":
1.0F != 2.0F
LHS = 1.000000
RHS = 2.000000
tmp:7: In "expect_eq (string, failure)":
"a" != "b"
LHS = "a"
RHS = "b"
tmp:2: In "expect_neq (int, failure)":
1 == 1
LHS = 1
RHS = 1
tmp:4: In "expect_neq (float, failure)":
1.0F == 1.0F
LHS = 1.000000
RHS = 1.000000
tmp:6: In "expect_neq (string, failure)":
"a" == "a"
LHS = "a"
RHS = "a"
[ix_test] 14 test cases are registered.
[ix_test] Executed 14 cases and 14 assertions in 0.0000 seconds.
[ix_test] 7 assertions failed.
)*";
    ix_EXPECT_EQSTR(writer.data(), expected);

    option.abort_after = 1;
    writer.clear();
    ctx.execute(writer, option);
    writer.end_string();
    expected = R"*(tmp:1: In "expect (failure)":
ix_EXPECT(false) failed!
[ix_test] 14 test cases are registered.
[ix_test] Executed 2 cases and 2 assertions in 0.0000 seconds.
[ix_test] 1 assertions failed.
)*";
    ix_EXPECT_EQSTR(writer.data(), expected);

    option.abort_after = 0;
    option.filter = "*failure*";
    writer.clear();
    ctx.execute(writer, option);
    writer.end_string();
    expected = R"*(tmp:1: In "expect (failure)":
ix_EXPECT(false) failed!
tmp:3: In "expect_eq (int, failure)":
1 != 2
LHS = 1
RHS = 2
tmp:5: In "expect_eq (float, failure)":
1.0F != 2.0F
LHS = 1.000000
RHS = 2.000000
tmp:7: In "expect_eq (string, failure)":
"a" != "b"
LHS = "a"
RHS = "b"
tmp:2: In "expect_neq (int, failure)":
1 == 1
LHS = 1
RHS = 1
tmp:4: In "expect_neq (float, failure)":
1.0F == 1.0F
LHS = 1.000000
RHS = 1.000000
tmp:6: In "expect_neq (string, failure)":
"a" == "a"
LHS = "a"
RHS = "a"
[ix_test] 14 test cases are registered.
[ix_test] Executed 7 cases and 7 assertions in 0.0000 seconds.
[ix_test] 7 assertions failed.
)*";
    ix_EXPECT_EQSTR(writer.data(), expected);

    option.abort_after = 0;
    option.filter = "*success*";
    writer.clear();
    ctx.execute(writer, option);
    writer.end_string();
    expected = R"*([ix_test] 14 test cases are registered.
[ix_test] Executed 7 cases and 7 assertions in 0.0000 seconds.
[ix_test] All tests passed!
)*";
    ix_EXPECT_EQSTR(writer.data(), expected);
}
#endif

#if 0
/*
#else
#define ix_DECLARE_TEST(f) \
    template <typename>    \
    static void f(ix_TestContext &)
#define ix_TEST_CASE(name) ix_DECLARE_TEST(ix_PP_ADD_UNIQUE_ID(ix_TEST_BODY_))
#define ix_EXPECT(...) (void)(__VA_ARGS__)
#define ix_EXPECT_EQ(lhs, rhs) \
    do                         \
    {                          \
        (void)(lhs);           \
        (void)(rhs);           \
    } while (0)
#define ix_EXPECT_EQF(lhs, rhs) ix_EXPECT_EQ(lhs, rhs)
#define ix_EXPECT_EQD(lhs, rhs) ix_EXPECT_EQ(lhs, rhs)
#define ix_EXPECT_EQSTR(lhs, rhs) ix_EXPECT_EQ(lhs, rhs)
#define ix_EXPECT_NEQ(lhs, rhs) ix_EXPECT_EQ(lhs, rhs)
#define ix_EXPECT_NEQF(lhs, rhs) ix_EXPECT_EQ(lhs, rhs)
#define ix_EXPECT_NEQD(lhs, rhs) ix_EXPECT_EQ(lhs, rhs)
#define ix_EXPECT_NEQSTR(lhs, rhs) ix_EXPECT_EQ(lhs, rhs)
}
*/
#endif
