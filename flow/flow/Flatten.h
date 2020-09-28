#pragma once

#include <optional>

#include <flow/Fuse.h>

namespace flow
{
    /// Reduces the sequence depth by one.
	/// Arity: 1 -> 1
    template<class S>
    class Flatten
    {
    public:
        static inline bool constexpr finite = S::finite;
        using sub_sequence_type = typename S::output_type;
        using output_type = typename sub_sequence_type::output_type;

        explicit Flatten(S &&base):
            base(std::move(base)),
            current_sub_sequence()
        {
        }

        std::optional<output_type> next()
        {
            // Try to get another element to return until there are no more elements.
            for (;;)
            {
                if (current_sub_sequence) {
                    // Try to get the next value out of the current sub sequence.
                    std::optional<output_type> state(current_sub_sequence->next());
                    if (state.has_value())
                    {
                        return state;
                    }
                }

                // Current sub sequence is exhausted, go to next.
                std::optional<sub_sequence_type> next_sub_sequence(base.next());
                if (next_sub_sequence.has_value())
                {
                    // Go to next subsequence.
                    current_sub_sequence.emplace(next_sub_sequence.value());
                }
                else
                {
                    // The base sequence is exhausted.
                    // There are ultimately no elements left.
                    return {};
                }
            }

        }

    private:
        S base;
        std::optional<sub_sequence_type> current_sub_sequence;
    };

    auto flatten()
    {
        return [] (auto &&seq)
        {
            return Flatten(std::forward<decltype(seq)>(seq));
        };
    }

}
