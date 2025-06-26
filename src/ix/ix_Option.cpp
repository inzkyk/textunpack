#include "ix_Option.hpp"

template class ix_Option<size_t>;

#if ix_DO_TEST
#include "ix_DumbString.hpp"
#include "ix_Vector.hpp"
#include "ix_test.hpp"

ix_TEST_CASE("ix_Option<int>")
{
    ix_Option<int> o(ix_ERROR_SOMETHING_WENT_WRONG);
    ix_EXPECT(o.is_error());

    o = 10;
    ix_EXPECT(o.is_ok());
    ix_EXPECT(o.unwrap() == 10);
    o.unwrap() = 100;
    ix_EXPECT(o.unwrap() == 100);
}

ix_TEST_CASE("ix_Option<ix_Vector<int>>")
{
    ix_Option<ix_Vector<int>> o(ix_ERROR_SOMETHING_WENT_WRONG);
    ix_EXPECT(o.is_error());

    o = ix_Vector<int>{1, 2};
    ix_EXPECT(o.is_ok());
    ix_EXPECT(o.unwrap().size() == 2);
    o.unwrap().push_back(3);
    ix_EXPECT(o.unwrap().size() == 3);

    o = ix_Vector<int>{1, 2, 3, 4};
    ix_EXPECT(o.is_ok());
    ix_EXPECT(o.unwrap().size() == 4);

    o = ix_ERROR_SOMETHING_WENT_WRONG;
    ix_EXPECT(o.is_error());
}

ix_TEST_CASE("ix_Option: unwrap")
{
    auto f = []() -> ix_Option<ix_DumbString> { return ix_DumbString("foo"); };

    ix_Option<ix_DumbString> x = f();

    ix_EXPECT_EQSTR(x.unwrap().data(), "foo");
    ix_EXPECT_EQSTR(f().unwrap().data(), "foo");
    x.unwrap() = ix_DumbString("bar");
    ix_EXPECT_EQSTR(x.unwrap().data(), "bar");
}

ix_TEST_CASE("ix_Option: unwrap_or")
{
    auto f = []() -> ix_Option<ix_DumbString> { return ix_DumbString("foo"); };
    auto g = []() -> ix_Option<ix_DumbString> { return ix_ERROR_SOMETHING_WENT_WRONG; };

    const ix_Option<ix_DumbString> x = f();
    const ix_Option<ix_DumbString> y = g();

    ix_EXPECT_EQSTR(x.unwrap_or(ix_DumbString("bar")).data(), "foo");
    ix_EXPECT_EQSTR(y.unwrap_or(ix_DumbString("bar")).data(), "bar");

    ix_EXPECT_EQSTR(f().unwrap_or(ix_DumbString("bar")).data(), "foo");
    ix_EXPECT_EQSTR(g().unwrap_or(ix_DumbString("bar")).data(), "bar");
}
#endif
