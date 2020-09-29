#pragma once

#include <optional>

#include <flow/TypeTraits.h>

namespace flow
{
    /// Generates a sequence out of a unfolding function.
    /// This function returns a maybe type, containing both the next element
    /// and the state passing to the next function invocation.
    /// `F: T -> Optional<(Output, T)>`
	/// Arity: 0 -> 1
    ///
    /// Concept:
    /// `F` must map to a type, which has a member type called `value_type`.
    /// This member type must itself expose two member types, called `first_type` and `second_type`.
    /// This sequence generates elements of type `first_type`.
    /// `F` must take a single owned parameter whose type is `second_type`.
    template<class T, class F>
    class Unfold
    {
    public:
        using FunctionReturnType = details::FunctionReturnType<F, T>;
        using ElementType = typename FunctionReturnType::value_type::first_type;

        static_assert(std::is_same_v<T, typename FunctionReturnType::value_type::second_type>,
            "The function's second return value must have the same type as it's argument.");

        explicit Unfold(T const &initialState, F function):
            function(function),
            state(initialState)
        {}

        bool probe()
        {
            FunctionReturnType nextOptionalValue = function(std::move(state));
            
            if (nextOptionalValue.has_value())
            {
                // The sequence is not terminated.
                auto &nextValue = nextOptionalValue.value();
                
                // Re-initialize new state.
                details::reinitialize(state, std::move(nextValue.second));
                
                // Yield the actually computed element.
                details::reinitialize(nextElement, std::move(nextValue.first));
                return true;
            }
            else
            {
                // The sequence is terminated.
                return false;
            }
        }
        
        ElementType next()
        {
            return std::move(nextElement.value());
        }

    private:
        F function;
        T state;
        std::optional<ElementType> nextElement;
    };

    template<class T, class F>
    auto unfold(T const &initialState, F function)
    {
        return Flow(Unfold(initialState, function));
    }
}
