//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{
    /// Before yielding the next element, skip `n - 1` elements.
    template<class Seq>
    class Stride
    {
    public:

        using output_type = typename Seq::output_type;

        Stride(
            Seq const &base,
            size_t const n
        ) :
            base{base},
            n{n}
        {}

        std::optional<output_type> next()
        {
            for(size_t k = 0; k < n; ++k) {
                std::optional<output_type> el = base.next();

                if (!el.has_value())
                {
                    // The base sequence is exhausted.
                    return {};
                }

                // Drop and forget about the element.
            }

            return base.next();
        }

    private:
        Seq base;
        size_t n;
    };
}
