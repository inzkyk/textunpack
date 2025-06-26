#pragma once

#include "ix.hpp"
#include "ix_memory.hpp"
#include "ix_type_traits.hpp"
#include "ix_utility.hpp"

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
