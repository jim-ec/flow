#pragma once

#include <flow/Fuse.h>
#include <flow/Maybe.h>
#include <flow/details.h>

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
            currentSubSequence(None())
        {
        }
        
        Maybe<ElementType> next()
        {
            // Try to get another element to return until there are no more elements.
            for (;;)
            {
                if (currentSubSequence.hasValue())
                {
                    // Try to get the next value out of the current sub sequence.
                    Maybe<ElementType> nextElement = currentSubSequence.value().next();
                    if (nextElement.hasValue())
                    {
                        return nextElement;
                    }
                }
                
                // Current sub sequence is exhausted, go to next.
                if (!(currentSubSequence = sequence.next()).hasValue())
                {
                    // The base sequence is exhausted.
                    // There are ultimately no elements left.
                    return None();
                }
            }
        }

    private:
        S sequence;
        Maybe<SubSequenceType> currentSubSequence;
    };

    auto flatten()
    {
        return [] (auto &&sequence)
        {
            return Flatten(std::move(sequence));
        };
    }

}
