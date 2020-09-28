#pragma once

#include <optional>

#include <flow/TypeTraits.h>

namespace flow
{
    /// Maps each sequence element through a function.
    /// Arity: 1 -> 1
    template<class S, class F>
    class Map
    {
    public:
        static inline bool constexpr finite = S::finite;
        using FunctionInputType = typename S::ElementType;
        using ElementType = details::FunctionReturnType<F, FunctionInputType>;

        static_assert(!std::is_lvalue_reference_v<ElementType>, "The mapped type must be owned.");
        static_assert(!std::is_rvalue_reference_v<ElementType>, "The mapped type must be owned.");

        Map(S &&base, F fn):
            base(std::move(base)),
            fn(fn)
        {
        }

        std::optional<ElementType> next()
        {
            std::optional<FunctionInputType> k(base.next());
            if (k.has_value())
            {
                return fn(std::move(k.value()));
            }
            return {};
        }

    private:
        S base;
        F fn;
    };

    template<class F>
    auto map(F fn)
    {
        return [=] (auto &&seq)
        {
            return Map(std::forward<decltype(seq)>(seq),fn);
        };
    }
}
