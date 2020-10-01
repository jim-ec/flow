#pragma once

#include <flow/TypeTraits.h>

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
        
        bool probe()
        {
            return sequence.probe();
        }

        ElementType next()
        {
            return function(sequence.next());
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
            return Map(std::forward<decltype(sequence)>(sequence), function);
        };
    }
    
    /// Dereferences the values behind element pointers.
    /// Arity: 1 -> 1
    auto dereference()
    {
        return map([] (auto *pointer)
                   {
            return *pointer;
        });
    }
}
