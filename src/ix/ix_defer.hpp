#pragma once

#include "ix.hpp"
#include "ix_utility.hpp"

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
