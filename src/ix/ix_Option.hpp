#pragma once

#include "ix.hpp"
#include "ix_Result.hpp"
#include "ix_StringView.hpp"
#include "ix_assert.hpp"
#include "ix_utility.hpp"

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

extern template class ix_Option<size_t>;
