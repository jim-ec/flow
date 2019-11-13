//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{
    /// Yields all elements of the given container.
    template<class O>
    class Elements
    {
        std::vector<O> const &xs;
        int k;
    public:
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
            O state = xs[k];
            ++k;
            return std::move(state);
        }
    };
}
