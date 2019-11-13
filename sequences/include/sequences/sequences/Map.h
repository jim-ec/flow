//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

#include "TypeTraits.h"

namespace sequences
{
    /// Maps each sequence element through a function.
    template<class Seq, class Fn>
    class Map
    {
    public:

        using output_type = function_return_type<Fn, typename Seq::output_type>;

        Map(
            Seq const &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<output_type> next()
        {
            std::optional<typename Seq::output_type> k = base.next();
            if (k.has_value())
            {
                return fn(std::move(*k));
            }
            return {};
        }

    private:
        Seq base;
        Fn fn;
    };
}
