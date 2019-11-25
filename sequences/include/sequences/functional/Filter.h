//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences
{
    /// Yields only elements of the base sequence where the test function returns `true`.
	/// Arity: 1 -> 1
    template<class Seq, class Fn>
    class Filter
    {
    public:

        constexpr static inline bool finite = Seq::finite;
        using output_type = typename Seq::output_type;

        Filter(
            Seq const &base,
            Fn fn
        ) :
            base(base),
            fn(fn)
        {}

        std::optional<output_type> next()
        {
            for (;;)
            {
                std::optional<output_type> state(base.next());
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
        Seq base;
        Fn fn;
    };

    template<class Fn>
    auto filter(Fn fn) {
        return [=](auto &&seq) {
            return Filter(std::forward<decltype(seq)>(seq), fn);
        };
    }
}
