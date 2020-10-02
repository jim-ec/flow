#pragma once

#include <flow/details.h>
#include <flow/Maybe.h>

namespace flow
{
    /// Maps each sequence element through a function.
    /// Arity: 1 -> 1
    template<class S, class F>
    class Map
    {
    public:
        using FunctionInputType = typename S::ElementType;
        using ElementType = details::FunctionReturnType<F, FunctionInputType>;

        static_assert(!std::is_lvalue_reference_v<ElementType>, "The mapped type must be owned.");
        static_assert(!std::is_rvalue_reference_v<ElementType>, "The mapped type must be owned.");

        Map(S &&sequence, F function):
            sequence(std::move(sequence)),
            function(function)
        {
        }

        Maybe<ElementType> next()
        {
            Maybe<FunctionInputType> functionInput = sequence.next();
            if (functionInput.holdsValue())
            {
                return function(functionInput.value());
            }
            else
            {
                return None();
            }
        }

    private:
        S sequence;
        F function;
    };

    template<class F>
    auto map(F function)
    {
        return [=] (auto &&sequence)
        {
            return Map(std::move(sequence), function);
        };
    }
    
    /// Dereferences the values behind element pointers.
    /// Arity: 1 -> 1
    auto dereference()
    {
        return map([] (auto *pointer) { return *pointer; });
    }
    
    template<class F>
    auto then(F function)
    {
        return map([=] (auto &&inputMaybe)
        {
            if (inputMaybe.holdsValue())
            {
                return Maybe(function(std::move(inputMaybe.value())));
            }
            else
            {
                using OutputType = decltype(function(std::move(inputMaybe.value())));
                return Maybe<OutputType>(None());
            }
        });
    }
    
    auto enumerate()
    {
        size_t k = 0;
        return map([=] (auto const &element) mutable
        {
            return std::tuple(k++, element);
        });
    }
}
