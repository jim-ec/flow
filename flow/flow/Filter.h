#pragma once

#include <optional>

namespace flow
{
    /// Yields only elements of the base sequence where the test function returns `true`.
	/// Arity: 1 -> 1
    template<class S, class F>
    class Filter
    {
    public:
        constexpr static inline bool finite = S::finite;
        using ElementType = typename S::ElementType;

        Filter(S const &base, F fn):
            base(base),
            fn(fn)
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
                if (fn(*state))
                {
                    return state;
                }
            }
        }

    private:
        S base;
        F fn;
    };

    template<class F>
    auto filter(F fn)
    {
        return [=] (auto &&seq)
        {
            return Filter(std::forward<decltype(seq)>(seq), fn);
        };
    }
}
