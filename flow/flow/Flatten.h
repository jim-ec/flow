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
        using SubSequenceType = typename S::ElementType;
        using ElementType = typename SubSequenceType::ElementType;

        explicit Flatten(S &&base):
            base(std::move(base)),
            currentSubSequence()
        {
        }

        std::optional<ElementType> next()
        {
            // Try to get another element to return until there are no more elements.
            for (;;)
            {
                if (currentSubSequence)
                {
                    // Try to get the next value out of the current sub sequence.
                    std::optional<ElementType> state(currentSubSequence->next());
                    if (state.has_value())
                    {
                        return state;
                    }
                }

                // Current sub sequence is exhausted, go to next.
                std::optional<SubSequenceType> nextSubSequence(base.next());
                if (nextSubSequence.has_value())
                {
                    // Go to next subsequence.
                    currentSubSequence.emplace(nextSubSequence.value());
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
        std::optional<SubSequenceType> currentSubSequence;
    };

    auto flatten()
    {
        return [] (auto &&sequence)
        {
            return Flatten(std::forward<decltype(sequence)>(sequence));
        };
    }

}
