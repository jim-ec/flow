//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

#include <sequences/core/TypeTraits.h>

namespace sequences
{
    /// This is not a sequence but a series with a custom reduction function.
    /// The reduction function is a binary function.
    /// Since sequences do not know direction, there is no distinction between
    /// left and right fold.
    template<class Seq, class Fn>
    class Fold
    {
    public:

        static_assert(Seq::finite, "Sequence to be fold must be finite.");
        using seq_output_type = typename Seq::output_type;

        Fold(
            Seq const &base,
            Fn fn
        ) :
            seq{base},
            fn{fn}
        {}

        /// Reduce the fold using a initial accumulator.
        /// The reduction type may differ from the sequence element type.
        template<class ReductionType = typename Seq::output_type>
        function_return_type<Fn, ReductionType, seq_output_type>
        reduce(ReductionType const &init)
        {
            ReductionType acc{init};

            for(;;) {
                std::optional<seq_output_type> state{seq.next()};

                if (!state.has_value())
                {
                    // Underlying sequence is exhausted.
                    return acc;
                }

                // Next reduction step.
                ReductionType next_reduction = fn(std::move(acc), *state);
                acc.~ReductionType();
                new (&acc) ReductionType(std::move(next_reduction));
            }
        }

        /// Try to reduce the fold without an initial accumulator.
        /// The reduction type must be the same as the sequence element type.
        std::optional<function_return_type<Fn, seq_output_type, seq_output_type>>
        reduce_maybe()
        {
            // Initial try to populate the accumulator.
            std::optional<seq_output_type> acc = seq.next();
            if (!acc.has_value())
            {
                return {};
            }

            for(;;) {
                std::optional<seq_output_type> state{seq.next()};

                if (!state.has_value())
                {
                    // Underlying sequence is exhausted.
                    return acc;
                }

                // Next reduction step.
                seq_output_type next_reduction = fn(std::move(*acc), *state);
                acc.reset();
                acc.emplace(std::move(next_reduction));
            }
        }

    private:
        Seq seq;
        Fn fn;
    };
}
