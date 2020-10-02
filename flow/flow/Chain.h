#pragma once

#include <flow/Maybe.h>

namespace flow
{
    /// First yields all elements from the first sequence,
    /// then yields elements from the second flow.
    /// The first flow should be finite.
    /// Elements from the continuation flow must be convertible to the output
    /// type of the drain sequence.
    /// Arity: 2 -> 1
    template<class D, class C>
    class Chain
    {
    public:
        using ElementType = typename D::ElementType;
        
        explicit Chain(D &&drainingSequence, C &&continuationSequence):
            drainingSequence(std::move(drainingSequence)),
            continuationSequence(std::move(continuationSequence)),
            draining(true)
        {}
        
        bool probe()
        {
            if (draining)
            {
                // Check whether there is another element in the draining sequence.
                if (drainingSequence.probe())
                {
                    return true;
                }
                else {
                    // The draining sequence is exhausted.
                    // From here on, yield from the continuation sequence.
                    draining = false;
                }
            }
            
            return continuationSequence.probe();
        }

        Maybe<ElementType> next()
        {
            if (draining)
            {
                // Check whether there is another element in the draining sequence.
                Maybe<ElementType> nextElement = drainingSequence.next();
                if (nextElement.hasValue())
                {
                    return nextElement;
                }
                else {
                    // The draining sequence is exhausted.
                    // From here on, yield from the continuation sequence.
                    draining = false;
                }
            }
            
            return continuationSequence.next();
        }

    private:
        D drainingSequence;
        C continuationSequence;
        bool draining;
    };

    template<class C>
    auto chain(C &&continuationSequence)
    {
        return [=] (auto &&drainingSequence) mutable
        {
            return Chain(std::move(drainingSequence), std::move(continuationSequence));
        };
    }

}
