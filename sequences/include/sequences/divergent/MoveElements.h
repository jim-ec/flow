//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences
{
    /// Yields all elements of the given container by move.
    /// This destroys the elements hold the underlying container.
	/// Arity: 0 -> 1
	// TODO: template over container instead of element
    template<class O>
    class MoveElements
    {
    public:
        static inline bool const finite = true;
        using output_type = O;

        explicit MoveElements(std::vector<O> &&xs) :
            xs(xs),
            k(0)
        {}

        std::optional<O> next()
        {
            if (k >= xs.size())
            {
                return {};
            }
            O state(std::move(xs[k]));
            ++k;
            return state;
        }

    private:
        std::vector<O> &xs;
        size_t k;
    };
}
