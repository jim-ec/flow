//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

#include "TypeTraits.h"

namespace sequences
{
    /// (a{n}) = (n, b{n})
    template<class Seq>
    class Enumerate
    {
    public:

        static inline bool constexpr finite = Seq::finite;
        using output_type = std::tuple<size_t, typename Seq::output_type>;

        explicit Enumerate(Seq const &base) :
            base{base},
            k{0}
        {}

        std::optional<output_type> next()
        {
            std::optional<typename Seq::output_type> state = base.next();
            if (state.has_value())
            {
                return std::tuple{k++, std::move(*state)};
            }
            return {};
        }

    private:
        Seq base;
        size_t k;
    };
}
