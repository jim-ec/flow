//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences
{
    /// Yields only elements of the base sequence where the test function returns `true`.
    template<class Seq, class Fn>
    class Filter
    {
    public:

        static inline bool constexpr finite = Seq::finite;
        using output_type = typename Seq::output_type;

        Filter(
            Seq const &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<output_type> next()
        {
            for (;;)
            {
                std::optional<output_type> k = base.next();
                if (!k.has_value())
                {
                    return {};
                }
                if (fn(*k))
                {
                    return std::move(k);
                }
            }
        }

    private:
        Seq base;
        Fn fn;
    };
}
