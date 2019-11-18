//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences
{
    /// Ensures the further calls to `next()` on a sequence return `None`s
    /// after it has been returned once from the base sequence.
    template<class Seq>
    class Fuse
    {
    public:

        // TODO: Optimized implementation for infinite sequences?
        static inline bool constexpr finite = Seq::finite;
        using output_type = typename Seq::output_type;

        explicit Fuse(Seq const &base) :
            base{base},
            exhausted{false}
        {}

        std::optional<output_type> next()
        {
            if (exhausted)
            {
                // The base sequence is already exhausted from previous calls
                // to this functional.
                return {};
            }

            std::optional<output_type> state{base.next()};
            if (!state.has_value()) {
                // The base sequence has been exhausted.
                exhausted = true;
                return {};
            }

            return state;
        }

    private:
        Seq base;
        bool exhausted;
    };
}
