#pragma once

#include <optional>

#include <flow/Fuse.h>
#include <flow/TypeTraits.h>

namespace flow
{
    /// Reduces the sequence depth by one.
	/// Arity: 1 -> 1
    template<class S>
    class Flatten
    {
    public:
        using SubSequenceType = typename S::ElementType;
        using ElementType = typename SubSequenceType::ElementType;

        explicit Flatten(S &&sequence):
            sequence(std::move(sequence)),
            currentSubSequence()
        {
        }
        
        bool probe()
        {
            // Try to get another element to return until there are no more elements.
            for (;;)
            {
                if (currentSubSequence.has_value())
                {
                    // Try to get the next value out of the current sub sequence.
                    if (currentSubSequence.value().probe())
                    {
                        details::reinitialize(nextElement, currentSubSequence->next());
                        return true;
                    }
                }
                
                // Current sub sequence is exhausted, go to next.
                if (sequence.probe())
                {
                    // Go to next subsequence.
                    details::reinitialize(currentSubSequence, sequence.next());
                }
                else
                {
                    // The base sequence is exhausted.
                    // There are ultimately no elements left.
                    return false;
                }
            }
        }
        
        ElementType next()
        {
            return std::move(nextElement.value());
        }

    private:
        S sequence;
        std::optional<ElementType> nextElement;
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
