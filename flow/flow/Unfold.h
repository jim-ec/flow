#pragma once

#include <optional>

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
        /// This is simply assumed, as it is impossible to statically check if the
        /// unfolding function will ever return `None`.
        static inline bool constexpr finite = true;

        using FunctionReturnType = details::FunctionReturnType<F, T>;
        using output_type = typename FunctionReturnType::value_type::first_type;

        static_assert(std::is_same_v<T, typename FunctionReturnType::value_type::second_type>,
            "The function's second return value must have the same type as it's argument.");

        explicit Unfold(T const &init, F fn):
            fn(fn),
            state(init)
        {}

        std::optional<output_type> next()
        {
            FunctionReturnType nextOptionalValue(fn(std::move(state)));

            if (nextOptionalValue.has_value())
            {
                // The sequence is not terminated.
                auto &nextValue = nextOptionalValue.value();

                // Re-initialize new state.
                state.~T();
                new (&state) T(std::move(nextValue.second));

                // Yield the actual compute element.
                return std::move(nextValue.first);
            }
            else
            {
                // The sequence is terminated.
                return {};
            }
        }

    private:
        F fn;
        T state;
    };

    template<class T, class F>
    auto unfold(T const &init, F fn)
    {
        return Flow(Unfold(init, fn));
    }
}
