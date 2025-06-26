#include "ix_util_win.hpp"

#if ix_PLATFORM(WIN)
#include "ix_Option.hpp"
#include "ix_StackVector.hpp"
#include "ix_UniquePointer.hpp"
#include "ix_Vector.hpp"
#include "ix_assert.hpp"

#if ix_DO_TEST
#include "ix_test.hpp"
#endif

#define ix_WIN_INCLUDE_USER
#define ix_WIN_INCLUDE_WINMESSAGES
#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_MB
#define ix_WIN_INCLUDE_NLS
#include "ix_Windows.hpp"

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

#if ix_DO_TEST
ix_TEST_CASE("ix_win_utf8_to_wchar")
{
    wchar_t wstr[128];
    ix_Option<size_t> len;

    len = ix_win_utf8_to_wchar(nullptr, 0, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_utf8_to_wchar(nullptr, 0, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_utf8_to_wchar("", 0, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_utf8_to_wchar("", 0, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_utf8_to_wchar("", 1, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.unwrap(), 1);

    len = ix_win_utf8_to_wchar("", 1, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 1);

    len = ix_win_utf8_to_wchar("0123456789", 11, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.unwrap(), 11);

    len = ix_win_utf8_to_wchar("0123456789", 5, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.unwrap(), 5);

    len = ix_win_utf8_to_wchar("0123456789", 5, wstr, 5);
    ix_EXPECT_EQ(len.unwrap(), 5);

    len = ix_win_utf8_to_wchar("0123456789", 5, wstr, 2);
    ix_EXPECT_EQ(len.result_kind(), ix_ERROR_BUFFER_TOO_SMALL);

    len = ix_win_utf8_to_wchar("0123456789", 0, wstr, 0);
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_utf8_to_wchar("0123456789", 0, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_utf8_to_wchar("0123456789", 5, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 5);

    len = ix_win_utf8_to_wchar("0123456789", 11, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 11);

    ix_EXPECT_EQ(ix_strlen("こんにちは"), 15);
    len = ix_win_utf8_to_wchar("こんにちは", 16, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.unwrap(), 6);

    len = ix_win_utf8_to_wchar("こんにちは", 16, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 6);

    len = ix_win_utf8_to_wchar("こんにちは", 6, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.unwrap(), 2);

    len = ix_win_utf8_to_wchar("こんにちは", 6, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 2);

    len = ix_win_utf8_to_wchar("こんにちは", 5, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.result_kind(), ix_ERROR_INVALID_UTF8);

    wchar_t wstr_short[4];
    len = ix_win_utf8_to_wchar("こんにちは", 16, wstr_short, ix_LENGTH_OF(wstr_short));
    ix_EXPECT_EQ(len.result_kind(), ix_ERROR_BUFFER_TOO_SMALL);

    // 「こんにちは」の先頭 5 バイト
    const char *str = "\xE3\x81\x93\xE3\x82";
    len = ix_win_utf8_to_wchar(str, 6, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.result_kind(), ix_ERROR_INVALID_UTF8);

    str = "\xE3\x81\x93\xE3\x82";
    len = ix_win_utf8_to_wchar(str, ix_strlen(str) + 1, nullptr, 0);
    ix_EXPECT_EQ(len.result_kind(), ix_ERROR_INVALID_UTF8);

    str = "\xFF\xFF\xFF";
    len = ix_win_utf8_to_wchar(str, ix_strlen(str) + 1, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.result_kind(), ix_ERROR_INVALID_UTF8);

    str = "\xFF\xFF\xFF";
    len = ix_win_utf8_to_wchar(str, ix_strlen(str) + 1, nullptr, 0);
    ix_EXPECT_EQ(len.result_kind(), ix_ERROR_INVALID_UTF8);

    // サロゲートペア
    ix_EXPECT_EQ(ix_strlen("𩹉𠮷😟"), 12);
    str = "𩹉𠮷😟";
    len = ix_win_utf8_to_wchar(str, 13, wstr, ix_LENGTH_OF(wstr));
    ix_EXPECT_EQ(len.unwrap(), 7);

    str = "𩹉𠮷😟";
    len = ix_win_utf8_to_wchar(str, 13, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 7);

    len = ix_win_utf8_to_wchar("123", 4, wstr_short, ix_LENGTH_OF(wstr_short));
    ix_EXPECT_EQ(len.unwrap(), 4);
    ix_EXPECT_EQ(wstr_short[0], '1');
    ix_EXPECT_EQ(wstr_short[1], '2');
    ix_EXPECT_EQ(wstr_short[2], '3');
    ix_EXPECT_EQ(wstr_short[3], '\0');
}
#endif

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

#if ix_DO_TEST
ix_TEST_CASE("ix_win_wchar_to_utf8")
{
    char str[128];
    ix_Option<size_t> len;

    len = ix_win_wchar_to_utf8(nullptr, 0, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_wchar_to_utf8(nullptr, 0, str, ix_LENGTH_OF(str));
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_wchar_to_utf8(L"", 0, str, ix_LENGTH_OF(str));
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_wchar_to_utf8(L"", 0, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 0);

    len = ix_win_wchar_to_utf8(L"", 1, str, ix_LENGTH_OF(str));
    ix_EXPECT_EQ(len.unwrap(), 1);

    len = ix_win_wchar_to_utf8(L"", 1, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 1);

    len = ix_win_wchar_to_utf8(L"0123456789", 11, str, ix_LENGTH_OF(str));
    ix_EXPECT_EQ(len.unwrap(), 11);

    len = ix_win_wchar_to_utf8(L"0123456789", 11, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 11);

    len = ix_win_wchar_to_utf8(L"0123456789", 5, str, ix_LENGTH_OF(str));
    ix_EXPECT_EQ(len.unwrap(), 5);

    len = ix_win_wchar_to_utf8(L"0123456789", 5, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 5);

    len = ix_win_wchar_to_utf8(L"0123456789", 5, str, 5);
    ix_EXPECT_EQ(len.unwrap(), 5);

    len = ix_win_wchar_to_utf8(L"0123456789", 5, str, 0);
    ix_EXPECT_EQ(len.result_kind(), ix_ERROR_BUFFER_TOO_SMALL);

    len = ix_win_wchar_to_utf8(L"こんにちは", 6, str, ix_LENGTH_OF(str));
    ix_EXPECT_EQ(len.unwrap(), 16);

    len = ix_win_wchar_to_utf8(L"こんにちは", 6, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 16);

    len = ix_win_wchar_to_utf8(L"こんにちは", 2, str, ix_LENGTH_OF(str));
    ix_EXPECT_EQ(len.unwrap(), 6);

    len = ix_win_wchar_to_utf8(L"こんにちは", 2, nullptr, 0);
    ix_EXPECT_EQ(len.unwrap(), 6);
}

ix_TEST_CASE("ix_win_wchar_to_utf8/ix_win_utf8_to_wchar")
{
    char str[128];
    wchar_t wstr[128];
    ix_Option<size_t> len;

    auto f = [&](const char *input) {
        len = ix_win_utf8_to_wchar(input, ix_strlen(input) + 1, wstr, ix_LENGTH_OF(wstr));
        ix_EXPECT(len.is_ok());
        len = ix_win_wchar_to_utf8(wstr, len.unwrap(), str, ix_LENGTH_OF(str));
        ix_EXPECT(len.is_ok());
        ix_EXPECT_EQSTR(str, input);
    };

    f("");
    f("foo");
    f("こんにちは");
    f("𩹉𠮷😟");
}
#endif

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

#if ix_DO_TEST
ix_TEST_CASE("ix_win_utf8_to_wchar_dynamic")
{
    ix_Vector<wchar_t> vector;

    ix_EXPECT(ix_win_utf8_to_wchar_dynamic(nullptr, 0, vector).is_ok());
    ix_EXPECT(ix_win_utf8_to_wchar_dynamic("", 0, vector).is_ok());

    ix_EXPECT(ix_win_utf8_to_wchar_dynamic("", 1, vector).is_ok());
    ix_EXPECT(ix_wstrcmp(vector.data(), L"") == 0);

    ix_EXPECT(ix_win_utf8_to_wchar_dynamic("0123456789", 11, vector).is_ok());
    ix_EXPECT_EQ(vector.size(), 11);
    ix_EXPECT(ix_wstrcmp(vector.data(), L"0123456789") == 0);

    ix_EXPECT(ix_win_utf8_to_wchar_dynamic("こんにちは", 16, vector).is_ok());
    ix_EXPECT_EQ(vector.size(), 6);
    ix_EXPECT(ix_wstrcmp(vector.data(), L"こんにちは") == 0);

    ix_EXPECT(ix_win_utf8_to_wchar_dynamic("0123456789", 11, vector).is_ok());
    ix_EXPECT_EQ(vector.size(), 11);
    ix_EXPECT(ix_wstrcmp(vector.data(), L"0123456789") == 0);
}
#endif

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

#if ix_DO_TEST
ix_TEST_CASE("ix_win_wchar_to_utf8_dynamic")
{
    ix_Vector<char> vector;

    ix_EXPECT(ix_win_wchar_to_utf8_dynamic(nullptr, 0, vector).is_ok());
    ix_EXPECT(ix_win_wchar_to_utf8_dynamic(L"", 0, vector).is_ok());

    ix_EXPECT(ix_win_wchar_to_utf8_dynamic(L"", 1, vector).is_ok());
    ix_EXPECT_EQSTR(vector.data(), "");

    ix_EXPECT(ix_win_wchar_to_utf8_dynamic(L"0123456789", 11, vector).is_ok());
    ix_EXPECT_EQ(vector.size(), 11);
    ix_EXPECT_EQSTR(vector.data(), "0123456789");

    ix_EXPECT(ix_win_wchar_to_utf8_dynamic(L"こんにちはこんにちは", 11, vector).is_ok());
    ix_EXPECT_EQ(vector.size(), 31);
    ix_EXPECT_EQSTR(vector.data(), "こんにちはこんにちは");

    ix_EXPECT(ix_win_wchar_to_utf8_dynamic(L"0123456789", 11, vector).is_ok());
    ix_EXPECT_EQ(vector.size(), 11);
    ix_EXPECT_EQSTR(vector.data(), "0123456789");
}
#endif

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

#if ix_DO_TEST
ix_TEST_CASE("ix_win_get_args")
{
    ix_Vector<char> buf;
    auto f = [&](std::initializer_list<const wchar_t *> wargv_input,
                 std::initializer_list<const char *> argv_expected) {
        // buf.clear();
        const size_t uargc = wargv_input.size();
        const int argc = static_cast<int>(uargc);
        ix_EXPECT_EQ(argc, argv_expected.size());
        const ix_UniquePointer<const char *[]> argv = ix_win_wargs_to_args(argc, wargv_input.begin(), buf).unwrap();
        for (size_t i = 0; i < uargc; i++)
        {
            ix_EXPECT_EQSTR(argv[i], argv_expected.begin()[i]);
        }
    };

    f({}, {});
    f({L"foo"}, {"foo"});
    f({L"foo", L"bar", L"foobar"}, {"foo", "bar", "foobar"});
    f({L"ほげ", L"ふが", L"ほげふが"}, {"ほげ", "ふが", "ほげふが"});
    f({L"00！", L"01！", L"02！", L"03！", L"04！", L"05！", L"06！", L"07！", L"08！", L"09！",
       L"10！", L"11！", L"12！", L"13！", L"14！", L"15！", L"16！", L"17！", L"18！", L"19！"},
      {"00！", "01！", "02！", "03！", "04！", "05！", "06！", "07！", "08！", "09！",
       "10！", "11！", "12！", "13！", "14！", "15！", "16！", "17！", "18！", "19！"});

    buf.clear();
    f({}, {});
    f({L"foo"}, {"foo"});
    f({L"foo", L"bar", L"foobar"}, {"foo", "bar", "foobar"});
    f({L"ほげ", L"ふが", L"ほげふが"}, {"ほげ", "ふが", "ほげふが"});
    f({L"00！", L"01！", L"02！", L"03！", L"04！", L"05！", L"06！", L"07！", L"08！", L"09！",
       L"10！", L"11！", L"12！", L"13！", L"14！", L"15！", L"16！", L"17！", L"18！", L"19！"},
      {"00！", "01！", "02！", "03！", "04！", "05！", "06！", "07！", "08！", "09！",
       "10！", "11！", "12！", "13！", "14！", "15！", "16！", "17！", "18！", "19！"});
}
#endif

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

#if ix_DO_TEST
ix_TEST_CASE("ix_win_msg_to_string")
{
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NULL), "WM_NULL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CREATE), "WM_CREATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DESTROY), "WM_DESTROY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOVE), "WM_MOVE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SIZE), "WM_SIZE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ACTIVATE), "WM_ACTIVATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SETFOCUS), "WM_SETFOCUS");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_KILLFOCUS), "WM_KILLFOCUS");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ENABLE), "WM_ENABLE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SETREDRAW), "WM_SETREDRAW");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SETTEXT), "WM_SETTEXT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETTEXT), "WM_GETTEXT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETTEXTLENGTH), "WM_GETTEXTLENGTH");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PAINT), "WM_PAINT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CLOSE), "WM_CLOSE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_QUERYENDSESSION), "WM_QUERYENDSESSION");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_QUERYOPEN), "WM_QUERYOPEN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ENDSESSION), "WM_ENDSESSION");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_QUIT), "WM_QUIT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ERASEBKGND), "WM_ERASEBKGND");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SYSCOLORCHANGE), "WM_SYSCOLORCHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SHOWWINDOW), "WM_SHOWWINDOW");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_WININICHANGE), "WM_SETTINGCHANGE (WM_WININICHANGE)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SETTINGCHANGE), "WM_SETTINGCHANGE (WM_WININICHANGE)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DEVMODECHANGE), "WM_DEVMODECHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ACTIVATEAPP), "WM_ACTIVATEAPP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_FONTCHANGE), "WM_FONTCHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_TIMECHANGE), "WM_TIMECHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CANCELMODE), "WM_CANCELMODE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SETCURSOR), "WM_SETCURSOR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOUSEACTIVATE), "WM_MOUSEACTIVATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CHILDACTIVATE), "WM_CHILDACTIVATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_QUEUESYNC), "WM_QUEUESYNC");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETMINMAXINFO), "WM_GETMINMAXINFO");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PAINTICON), "WM_PAINTICON");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ICONERASEBKGND), "WM_ICONERASEBKGND");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NEXTDLGCTL), "WM_NEXTDLGCTL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SPOOLERSTATUS), "WM_SPOOLERSTATUS");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DRAWITEM), "WM_DRAWITEM");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MEASUREITEM), "WM_MEASUREITEM");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DELETEITEM), "WM_DELETEITEM");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_VKEYTOITEM), "WM_VKEYTOITEM");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CHARTOITEM), "WM_CHARTOITEM");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SETFONT), "WM_SETFONT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETFONT), "WM_GETFONT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SETHOTKEY), "WM_SETHOTKEY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETHOTKEY), "WM_GETHOTKEY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_QUERYDRAGICON), "WM_QUERYDRAGICON");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_COMPAREITEM), "WM_COMPAREITEM");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETOBJECT), "WM_GETOBJECT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_COMPACTING), "WM_COMPACTING");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_COMMNOTIFY), "WM_COMMNOTIFY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_WINDOWPOSCHANGING), "WM_WINDOWPOSCHANGING");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_WINDOWPOSCHANGED), "WM_WINDOWPOSCHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POWER), "WM_POWER");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_COPYDATA), "WM_COPYDATA");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CANCELJOURNAL), "WM_CANCELJOURNAL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NOTIFY), "WM_NOTIFY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_INPUTLANGCHANGEREQUEST), "WM_INPUTLANGCHANGEREQUEST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_INPUTLANGCHANGE), "WM_INPUTLANGCHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_TCARD), "WM_TCARD");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_HELP), "WM_HELP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_USERCHANGED), "WM_USERCHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NOTIFYFORMAT), "WM_NOTIFYFORMAT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CONTEXTMENU), "WM_CONTEXTMENU");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_STYLECHANGING), "WM_STYLECHANGING");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_STYLECHANGED), "WM_STYLECHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DISPLAYCHANGE), "WM_DISPLAYCHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETICON), "WM_GETICON");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SETICON), "WM_SETICON");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCCREATE), "WM_NCCREATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCDESTROY), "WM_NCDESTROY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCCALCSIZE), "WM_NCCALCSIZE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCHITTEST), "WM_NCHITTEST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCPAINT), "WM_NCPAINT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCACTIVATE), "WM_NCACTIVATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETDLGCODE), "WM_GETDLGCODE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SYNCPAINT), "WM_SYNCPAINT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCMOUSEMOVE), "WM_NCMOUSEMOVE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCLBUTTONDOWN), "WM_NCLBUTTONDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCLBUTTONUP), "WM_NCLBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCLBUTTONDBLCLK), "WM_NCLBUTTONDBLCLK");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCRBUTTONDOWN), "WM_NCRBUTTONDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCRBUTTONUP), "WM_NCRBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCRBUTTONDBLCLK), "WM_NCRBUTTONDBLCLK");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCMBUTTONDOWN), "WM_NCMBUTTONDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCMBUTTONUP), "WM_NCMBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCMBUTTONDBLCLK), "WM_NCMBUTTONDBLCLK");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCXBUTTONDOWN), "WM_NCXBUTTONDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCXBUTTONUP), "WM_NCXBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCXBUTTONDBLCLK), "WM_NCXBUTTONDBLCLK");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_INPUT_DEVICE_CHANGE), "WM_INPUT_DEVICE_CHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_INPUT), "WM_INPUT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_KEYFIRST), "WM_KEYDOWN (WM_KEYFIRST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_KEYDOWN), "WM_KEYDOWN (WM_KEYFIRST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_KEYUP), "WM_KEYUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CHAR), "WM_CHAR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DEADCHAR), "WM_DEADCHAR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SYSKEYDOWN), "WM_SYSKEYDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SYSKEYUP), "WM_SYSKEYUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SYSCHAR), "WM_SYSCHAR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SYSDEADCHAR), "WM_SYSDEADCHAR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_UNICHAR), "WM_UNICHAR (WM_KEYLAST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_KEYLAST), "WM_UNICHAR (WM_KEYLAST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_STARTCOMPOSITION), "WM_IME_STARTCOMPOSITION");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_ENDCOMPOSITION), "WM_IME_ENDCOMPOSITION");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_COMPOSITION), "WM_IME_COMPOSITION (WM_IME_KEYLAST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_KEYLAST), "WM_IME_COMPOSITION (WM_IME_KEYLAST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_INITDIALOG), "WM_INITDIALOG");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_COMMAND), "WM_COMMAND");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SYSCOMMAND), "WM_SYSCOMMAND");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_TIMER), "WM_TIMER");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_HSCROLL), "WM_HSCROLL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_VSCROLL), "WM_VSCROLL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_INITMENU), "WM_INITMENU");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_INITMENUPOPUP), "WM_INITMENUPOPUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GESTURE), "WM_GESTURE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GESTURENOTIFY), "WM_GESTURENOTIFY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MENUSELECT), "WM_MENUSELECT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MENUCHAR), "WM_MENUCHAR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ENTERIDLE), "WM_ENTERIDLE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MENURBUTTONUP), "WM_MENURBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MENUDRAG), "WM_MENUDRAG");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MENUGETOBJECT), "WM_MENUGETOBJECT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_UNINITMENUPOPUP), "WM_UNINITMENUPOPUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MENUCOMMAND), "WM_MENUCOMMAND");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CHANGEUISTATE), "WM_CHANGEUISTATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_UPDATEUISTATE), "WM_UPDATEUISTATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_QUERYUISTATE), "WM_QUERYUISTATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CTLCOLORMSGBOX), "WM_CTLCOLORMSGBOX");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CTLCOLOREDIT), "WM_CTLCOLOREDIT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CTLCOLORLISTBOX), "WM_CTLCOLORLISTBOX");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CTLCOLORBTN), "WM_CTLCOLORBTN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CTLCOLORDLG), "WM_CTLCOLORDLG");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CTLCOLORSCROLLBAR), "WM_CTLCOLORSCROLLBAR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CTLCOLORSTATIC), "WM_CTLCOLORSTATIC");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOUSEFIRST), "WM_MOUSEMOVE (WM_MOUSEFIRST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOUSEMOVE), "WM_MOUSEMOVE (WM_MOUSEFIRST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_LBUTTONDOWN), "WM_LBUTTONDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_LBUTTONUP), "WM_LBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_LBUTTONDBLCLK), "WM_LBUTTONDBLCLK");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_RBUTTONDOWN), "WM_RBUTTONDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_RBUTTONUP), "WM_RBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_RBUTTONDBLCLK), "WM_RBUTTONDBLCLK");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MBUTTONDOWN), "WM_MBUTTONDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MBUTTONUP), "WM_MBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MBUTTONDBLCLK), "WM_MBUTTONDBLCLK");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOUSEWHEEL), "WM_MOUSEWHEEL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_XBUTTONDOWN), "WM_XBUTTONDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_XBUTTONUP), "WM_XBUTTONUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_XBUTTONDBLCLK), "WM_XBUTTONDBLCLK");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOUSEHWHEEL), "WM_MOUSEHWHEEL (WM_MOUSELAST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOUSELAST), "WM_MOUSEHWHEEL (WM_MOUSELAST)");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PARENTNOTIFY), "WM_PARENTNOTIFY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ENTERMENULOOP), "WM_ENTERMENULOOP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_EXITMENULOOP), "WM_EXITMENULOOP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NEXTMENU), "WM_NEXTMENU");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SIZING), "WM_SIZING");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CAPTURECHANGED), "WM_CAPTURECHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOVING), "WM_MOVING");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POWERBROADCAST), "WM_POWERBROADCAST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DEVICECHANGE), "WM_DEVICECHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDICREATE), "WM_MDICREATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDIDESTROY), "WM_MDIDESTROY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDIACTIVATE), "WM_MDIACTIVATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDIRESTORE), "WM_MDIRESTORE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDINEXT), "WM_MDINEXT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDIMAXIMIZE), "WM_MDIMAXIMIZE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDITILE), "WM_MDITILE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDICASCADE), "WM_MDICASCADE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDIICONARRANGE), "WM_MDIICONARRANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDIGETACTIVE), "WM_MDIGETACTIVE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDISETMENU), "WM_MDISETMENU");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ENTERSIZEMOVE), "WM_ENTERSIZEMOVE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_EXITSIZEMOVE), "WM_EXITSIZEMOVE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DROPFILES), "WM_DROPFILES");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MDIREFRESHMENU), "WM_MDIREFRESHMENU");

#if !ix_COMPILER(GCC)
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERDEVICECHANGE), "WM_POINTERDEVICECHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERDEVICEINRANGE), "WM_POINTERDEVICEINRANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERDEVICEOUTOFRANGE), "WM_POINTERDEVICEOUTOFRANGE");
#endif

    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_TOUCH), "WM_TOUCH");

#if !ix_COMPILER(GCC)
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCPOINTERUPDATE), "WM_NCPOINTERUPDATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCPOINTERDOWN), "WM_NCPOINTERDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCPOINTERUP), "WM_NCPOINTERUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERUPDATE), "WM_POINTERUPDATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERDOWN), "WM_POINTERDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERUP), "WM_POINTERUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERENTER), "WM_POINTERENTER");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERLEAVE), "WM_POINTERLEAVE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERACTIVATE), "WM_POINTERACTIVATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERCAPTURECHANGED), "WM_POINTERCAPTURECHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_TOUCHHITTESTING), "WM_TOUCHHITTESTING");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERWHEEL), "WM_POINTERWHEEL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERHWHEEL), "WM_POINTERHWHEEL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERROUTEDTO), "WM_POINTERROUTEDTO");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERROUTEDAWAY), "WM_POINTERROUTEDAWAY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_POINTERROUTEDRELEASED), "WM_POINTERROUTEDRELEASED");
#endif

    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_SETCONTEXT), "WM_IME_SETCONTEXT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_NOTIFY), "WM_IME_NOTIFY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_CONTROL), "WM_IME_CONTROL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_COMPOSITIONFULL), "WM_IME_COMPOSITIONFULL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_SELECT), "WM_IME_SELECT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_CHAR), "WM_IME_CHAR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_REQUEST), "WM_IME_REQUEST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_KEYDOWN), "WM_IME_KEYDOWN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_IME_KEYUP), "WM_IME_KEYUP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOUSEHOVER), "WM_MOUSEHOVER");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_MOUSELEAVE), "WM_MOUSELEAVE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCMOUSEHOVER), "WM_NCMOUSEHOVER");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_NCMOUSELEAVE), "WM_NCMOUSELEAVE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_WTSSESSION_CHANGE), "WM_WTSSESSION_CHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_TABLET_FIRST), "WM_TABLET_FIRST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_TABLET_LAST), "WM_TABLET_LAST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DPICHANGED), "WM_DPICHANGED");

#if !ix_COMPILER(GCC)
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DPICHANGED_BEFOREPARENT), "WM_DPICHANGED_BEFOREPARENT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DPICHANGED_AFTERPARENT), "WM_DPICHANGED_AFTERPARENT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETDPISCALEDSIZE), "WM_GETDPISCALEDSIZE");
#endif

    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CUT), "WM_CUT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_COPY), "WM_COPY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PASTE), "WM_PASTE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CLEAR), "WM_CLEAR");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_UNDO), "WM_UNDO");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_RENDERFORMAT), "WM_RENDERFORMAT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_RENDERALLFORMATS), "WM_RENDERALLFORMATS");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DESTROYCLIPBOARD), "WM_DESTROYCLIPBOARD");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DRAWCLIPBOARD), "WM_DRAWCLIPBOARD");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PAINTCLIPBOARD), "WM_PAINTCLIPBOARD");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_VSCROLLCLIPBOARD), "WM_VSCROLLCLIPBOARD");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_SIZECLIPBOARD), "WM_SIZECLIPBOARD");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_ASKCBFORMATNAME), "WM_ASKCBFORMATNAME");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CHANGECBCHAIN), "WM_CHANGECBCHAIN");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_HSCROLLCLIPBOARD), "WM_HSCROLLCLIPBOARD");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_QUERYNEWPALETTE), "WM_QUERYNEWPALETTE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PALETTEISCHANGING), "WM_PALETTEISCHANGING");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PALETTECHANGED), "WM_PALETTECHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_HOTKEY), "WM_HOTKEY");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PRINT), "WM_PRINT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PRINTCLIENT), "WM_PRINTCLIENT");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_APPCOMMAND), "WM_APPCOMMAND");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_THEMECHANGED), "WM_THEMECHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_CLIPBOARDUPDATE), "WM_CLIPBOARDUPDATE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DWMCOMPOSITIONCHANGED), "WM_DWMCOMPOSITIONCHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DWMNCRENDERINGCHANGED), "WM_DWMNCRENDERINGCHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DWMCOLORIZATIONCOLORCHANGED), "WM_DWMCOLORIZATIONCOLORCHANGED");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DWMWINDOWMAXIMIZEDCHANGE), "WM_DWMWINDOWMAXIMIZEDCHANGE");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DWMSENDICONICTHUMBNAIL), "WM_DWMSENDICONICTHUMBNAIL");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_DWMSENDICONICLIVEPREVIEWBITMAP), "WM_DWMSENDICONICLIVEPREVIEWBITMAP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_GETTITLEBARINFOEX), "WM_GETTITLEBARINFOEX");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_HANDHELDFIRST), "WM_HANDHELDFIRST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_HANDHELDLAST), "WM_HANDHELDLAST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_AFXFIRST), "WM_AFXFIRST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_AFXLAST), "WM_AFXLAST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PENWINFIRST), "WM_PENWINFIRST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_PENWINLAST), "WM_PENWINLAST");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_APP), "WM_APP");
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_USER), "WM_USER");

#if defined(WM_TOOLTIPDISMISS)
    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_TOOLTIPDISMISS), "WM_TOOLTIPDISMISS");
#endif

    ix_EXPECT_EQSTR(ix_win_msg_to_string(WM_USER + 1), "UNKNOWN");
}
#endif

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

#if 0
std::initializer_list<UINT>{
    0x90,
    0x93,
    0x94,
    0xAE,
    0xB0,
    0xC096,
    WM_ACTIVATE,
    WM_ACTIVATEAPP,
    WM_CAPTURECHANGED,
    WM_CHAR,
    WM_CLOSE,
    WM_CONTEXTMENU,
    WM_CREATE,
    WM_DESTROY,
    WM_DWMNCRENDERINGCHANGED,
    WM_ENTERIDLE,
    WM_ENTERMENULOOP,
    WM_ENTERSIZEMOVE,
    WM_ERASEBKGND,
    WM_EXITMENULOOP,
    WM_EXITSIZEMOVE,
    WM_GETICON,
    WM_GETMINMAXINFO,
    WM_IME_NOTIFY,
    WM_IME_SETCONTEXT,
    WM_INITMENU,
    WM_INITMENUPOPUP,
    WM_KEYDOWN,
    WM_KEYDOWN,
    WM_KEYUP,
    WM_KILLFOCUS,
    WM_LBUTTONDOWN,
    WM_LBUTTONUP,
    WM_LBUTTONUP,
    WM_MENUSELECT,
    WM_MOUSEACTIVATE,
    WM_MOUSEMOVE,
    WM_MOVE,
    WM_MOVING,
    WM_MOVING,
    WM_NCACTIVATE,
    WM_NCCALCSIZE,
    WM_NCCREATE,
    WM_NCDESTROY,
    WM_NCHITTEST,
    WM_NCLBUTTONDBLCLK,
    WM_NCLBUTTONDOWN,
    WM_NCMOUSELEAVE,
    WM_NCMOUSEMOVE,
    WM_NCPAINT,
    WM_PAINT,
    WM_QUERYOPEN,
    WM_QUIT,
    WM_RBUTTONDOWN,
    WM_RBUTTONUP,
    WM_SETCURSOR,
    WM_SETFOCUS,
    WM_SHOWWINDOW,
    WM_SIZE,
    WM_SIZING,
    WM_SYSCOMMAND,
    WM_SYSKEYDOWN,
    WM_SYSKEYUP,
    WM_UNINITMENUPOPUP,
    WM_WINDOWPOSCHANGED,
    WM_WINDOWPOSCHANGING,
}

DISPLAY_DEVICE display_devices[4] = {};
for (DWORD i = 0; i < ix_LENGTH_OF(display_devices); i++)
{
    display_devices[i].cb = sizeof(DISPLAY_DEVICE);
    ix_WIN_ASSERT_OK(EnumDisplayDevices(nullptr, i, &display_devices[i], 0));
    DEVMODE setting = {};
    setting.dmSize = sizeof(DEVMODE);
    BOOL ret = EnumDisplaySettings(display_devices[i].DeviceName, ENUM_CURRENT_SETTINGS, &setting);
    if (ret == 0)
    {
        break;
    }
    ix_log_debug("Display device name  : %ls", display_devices[i].DeviceName);
    ix_log_debug("Display device string: %ls", display_devices[i].DeviceString);
    ix_log_debug("Display Width : %ld", setting.dmPelsWidth);
    ix_log_debug("Display Height: %ld", setting.dmPelsHeight);
}
#endif

#endif
