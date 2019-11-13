//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{
    /// Yields only elements of the base sequence where the test function returns `true`.
    template<class Seq, class Fn>
    class Filter
    {
        Seq &base;
        Fn fn;

    public:

        using output_type = typename Seq::output_type;

        Filter(
            Seq &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<output_type> next()
        {
            for (;;)
            {
                auto k = base.next();
                if (!k)
                {
                    return {};
                }
                if (fn(*k))
                {
                    return std::move(k);
                }
            }
        }
    };
}
