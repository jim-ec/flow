#pragma once

//#include <flow/Map.h>
#include <flow/Maybe.h>
#include <flow/details.h>

namespace flow
{
    /// Yields only elements of the base sequence where the predicate function returns `true`.
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
                
                if (!nextElement.hasValue())
                {
                    // Sequence is exhausted.
                    return None();
                }
                
                // If the predicate validates the element, return it.
                // Otherwise, continue on the next element.
                if (predicate(reinterpret_cast<ElementType const &>(nextElement.value())))
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
    
    /// Removes empty maybes from a sequence and unwraps the remaining elements.
//    auto filterMaybes()
//    {
//        return filter([] (auto const &maybe) { return maybe.hasValue(); })
//            | map([] (auto &&maybe) { return maybe.value(); });
//    }
}
