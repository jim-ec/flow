//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{
    /// Yields up to a fixed amount of elements out of a base sequence.
    template<class Seq>
    class Take
    {
        Seq &base;
        size_t k;
        size_t n;

    public:

        using output_type = typename Seq::output_type;

        Take(
            Seq &base,
            size_t const n
        ) :
            base{base},
            k{0},
            n{n}
        {}

        std::optional<output_type> next()
        {
            if (k < n)
            {
                ++k;
                return base.next();
            }
            else
            {
                return {};
            }
        }
    };
}
