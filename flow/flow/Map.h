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

        Map(S &&base, F function):
            base(std::move(base)),
            function(function)
        {
        }

        std::optional<ElementType> next()
        {
            std::optional<FunctionInputType> nextElement(base.next());
            if (nextElement.has_value())
            {
                return function(std::move(nextElement.value()));
            }
            return {};
        }

    private:
        S base;
        F function;
    };

    template<class F>
    auto map(F function)
    {
        return [=] (auto &&sequence)
        {
            return Map(std::forward<decltype(sequence)>(sequence), function);
        };
    }
}
