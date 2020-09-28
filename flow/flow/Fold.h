#pragma once

#include <optional>

namespace flow
{
    template<class S, class F, class T>
    T fold(S seq, T const &init, F fn) {
        using ElementType = typename S::ElementType;

        T acc(init);

        for(;;) {
            std::optional<ElementType> state(seq.next());

            if (!state.has_value())
            {
                // Underlying sequence is exhausted.
                return acc;
            }

            // Next reduction step.
            T nextReduction(fn(std::move(acc), *state));
            acc.~T();
            new (&acc) T(std::move(nextReduction));
        }
    }

    template<class S, class F>
    std::optional<typename S::ElementType>
    fold_maybe(S seq, F fn) {
        using ElementType = typename S::ElementType;

        // Initial attempt to populate the accumulator.
        std::optional<ElementType> acc(seq.next());
        if (!acc.has_value())
        {
            return {};
        }

        for(;;) {
            std::optional<ElementType> state(seq.next());

            if (!state.has_value())
            {
                // Underlying sequence is exhausted.
                return acc;
            }

            // Next reduction step.
            ElementType nextReduction(fn(std::move(*acc), *state));
            acc.reset();
            acc.emplace(std::move(nextReduction));
        }
    }
}
