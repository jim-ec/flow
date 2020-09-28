#pragma once

#include <optional>

namespace flow
{
    /// Yields only elements of the base sequence where the predicate function returns `true`.
	/// Arity: 1 -> 1
    template<class S, class F>
    class Filter
    {
    public:
        constexpr static inline bool finite = S::finite;
        using ElementType = typename S::ElementType;

        Filter(S const &base, F predicate):
            base(base),
            predicate(predicate)
        {}

        std::optional<ElementType> next()
        {
            for (;;)
            {
                std::optional<ElementType> state(base.next());
                if (!state.has_value())
                {
                    return {};
                }
                if (predicate(*state))
                {
                    return state;
                }
            }
        }

    private:
        S base;
        F predicate;
    };

    template<class F>
    auto filter(F predicate)
    {
        return [=] (auto &&sequence)
        {
            return Filter(std::forward<decltype(sequence)>(sequence), predicate);
        };
    }
}
