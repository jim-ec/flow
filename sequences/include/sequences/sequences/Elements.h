//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences
{
    /// Yields all elements of the given container.
    template<class O>
    class Elements
    {
    public:
        static inline bool constexpr finite = true;
        using output_type = O;

        explicit Elements(std::vector<O> const &xs) :
            xs{xs},
            k{0}
        {}

        std::optional<O> next()
        {
            if (k >= xs.size())
            {
                return {};
            }
            O state{xs[k]};
            ++k;
            return std::move(state);
        }

    private:
        std::vector<O> const &xs;
        size_t k;
    };
}
