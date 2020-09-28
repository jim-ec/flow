#pragma once

#include <optional>

namespace flow
{
    template<class S, class F, class T>
    T fold(S sequence, T const &initial, F function) {
        using ElementType = typename S::ElementType;

        T acc(initial);

        for(;;) {
            std::optional<ElementType> state(sequence.next());

            if (!state.has_value())
            {
                // Underlying sequence is exhausted.
                return acc;
            }

            // Next reduction step.
            T nextReduction(function(std::move(acc), *state));
            acc.~T();
            new (&acc) T(std::move(nextReduction));
        }
    }

    template<class S, class F>
    std::optional<typename S::ElementType>
    fold_maybe(S sequence, F function) {
        using ElementType = typename S::ElementType;

        // Initial attempt to populate the accumulator.
        std::optional<ElementType> acc(sequence.next());
        if (!acc.has_value())
        {
            return {};
        }

        for(;;) {
            std::optional<ElementType> state(sequence.next());

            if (!state.has_value())
            {
                // Underlying sequence is exhausted.
                return acc;
            }

            // Next reduction step.
            ElementType nextReduction(function(std::move(*acc), *state));
            acc.reset();
            acc.emplace(std::move(nextReduction));
        }
    }
}
