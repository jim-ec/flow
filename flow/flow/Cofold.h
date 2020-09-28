#pragma once

#include <optional>

namespace flow
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
    template<class T, class F>
    class Cofold
    {
    public:
        /// This is simply assumed, as it is impossible to check if the
        /// cofolding function will ever return `None`.
        static inline bool constexpr finite = true;

        using SeedType = T;
        using DomainType = details::FunctionReturnType<F, T>;
        using fn_domain_inner_type = typename DomainType::value_type;
        using output_type = typename fn_domain_inner_type::first_type;

        static_assert(std::is_same_v<SeedType, typename fn_domain_inner_type::second_type>,
            "The function's second return value must have the same type as it's argument.");

        explicit Cofold(SeedType const &init, F fn):
            fn(fn),
            state(init)
        {}

        std::optional<output_type> next()
        {
            DomainType result(fn(std::move(state)));

            if (result.has_value())
            {
                // The sequence is not terminated.
                fn_domain_inner_type &result_value = result.value();

                // Re-initialize new state.
                state.~SeedType();
                new (&state) SeedType(std::move(result_value.second));

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
        F fn;
        SeedType state;
    };

    template<class T, class F>
    auto cofold(T const &init, F fn)
    {
        return Flow(Cofold(init, fn));
    }
}
