#pragma once

#include <flow/Maybe.h>
#include <flow/details.h>

namespace flow
{
    /// Yields only elements of the base sequence where the predicate function returns `true`.
	/// Arity: 1 -> 1
    template<class S, class F>
    class Filter
    {
    public:
        using ElementType = typename S::ElementType;

        Filter(S &&sequence, F predicate):
            sequence(std::move(sequence)),
            predicate(predicate)
        {}
        
        Maybe<ElementType> next()
        {
            for (;;)
            {
                Maybe<ElementType> nextElement = sequence.next();
                
                if (!nextElement.holdsValue())
                {
                    // Sequence is exhausted.
                    return None();
                }
                
                // If the predicate validates the element, return it.
                // Otherwise, continue on the next element.
                if (predicate(nextElement.value()))
                {
                    return nextElement;
                }
            }
        }
        
    private:
        S sequence;
        F predicate;
    };

    template<class F>
    auto filter(F predicate)
    {
        return [=] (auto &&sequence)
        {
            return Filter(std::move(sequence), predicate);
        };
    }
}
