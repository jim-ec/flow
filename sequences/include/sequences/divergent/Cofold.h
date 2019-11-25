//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences
{
    /// Generates a sequence out of a unfolding function.
    /// This function returns a maybe type, containing both the next element
    /// and the state passing to the next function invocation.
    /// `Fn: T -> Optional<(Output, T)>`
	/// Arity: 0 -> 1
    ///
    /// Concept:
    /// `Fn` must map to a type, which has a member type called `value_type`.
    /// This member type must itself expose two member types, called `first_type`
    /// and `second_type`.
    /// This sequence generates elements of type `first_type`.
    /// `Fn` must take a single owned parameter whose type is `second_type`.
    template<class T, class Fn>
    class Cofold
    {
    public:

        /// This is simply assumed, as it is impossible to check if the
        /// cofolding function will ever return `None`.
        static inline bool constexpr finite = true;

        using seed_type = T;
        using fn_domain_type = function_return_type<Fn, T>;
        using fn_domain_inner_type = typename fn_domain_type::value_type;
        using output_type = typename fn_domain_inner_type::first_type;

        static_assert(std::is_same_v<seed_type, typename fn_domain_inner_type::second_type>,
            "The functional's second return value must have the same type as it's argument.");

        explicit Cofold(seed_type const &init, Fn fn) :
            fn(fn),
            state(init)
        {}

        std::optional<output_type> next()
        {
            fn_domain_type result(fn(std::move(state)));

            if (result.has_value())
            {
                // The sequence is not terminated.
                fn_domain_inner_type &result_value = result.value();

                // Re-initialize new state.
                state.~seed_type();
                new (&state) seed_type(std::move(result_value.second));

                // Yield the actual compute element.
                return std::move(result_value.first);
            }
            else
            {
                // The sequence is terminated.
                return {};
            }
        }

    private:
        Fn fn;
        seed_type state;
    };

    template<class T, class Fn>
    auto cofold(T const &init, Fn fn) {
        return Flow(Cofold(init, fn));
    }
}
