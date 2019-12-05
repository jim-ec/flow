//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace flow
{
    template<class Seq, class Fn, class T>
    T fold(Seq seq, T const &init, Fn fn) {
        using output_type = typename Seq::output_type;

        T acc(init);

        for(;;) {
            std::optional<output_type> state(seq.next());

            if (!state.has_value())
            {
                // Underlying sequence is exhausted.
                return acc;
            }

            // Next reduction step.
            T next_reduction(fn(std::move(acc), *state));
            acc.~T();
            new (&acc) T(std::move(next_reduction));
        }
    }

    template<class Seq, class Fn>
    std::optional<typename Seq::output_type>
    fold_maybe(Seq seq, Fn fn) {
        using output_type = typename Seq::output_type;

        // Initial attempt to populate the accumulator.
        std::optional<output_type> acc(seq.next());
        if (!acc.has_value())
        {
            return {};
        }

        for(;;) {
            std::optional<output_type> state(seq.next());

            if (!state.has_value())
            {
                // Underlying sequence is exhausted.
                return acc;
            }

            // Next reduction step.
            output_type next_reduction(fn(std::move(*acc), *state));
            acc.reset();
            acc.emplace(std::move(next_reduction));
        }
    }
}
