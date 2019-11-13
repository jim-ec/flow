//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{
    /// Maps each sequence element through a function.
    template<class Seq, class Fn>
    class Map
    {
        Seq &base;
        Fn fn;

    public:

        using output_type = typename Seq::output_type;

        Map(
            Seq &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<output_type> next()
        {
            auto k = base.next();
            if (k)
            {
                return fn(std::move(*k));
            }
            return {};
        }
    };
}
