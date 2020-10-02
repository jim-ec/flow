#pragma once

#include <flow/details.h>
#include <flow/SequenceIterator.h>
#include <flow/Maybe.h>

namespace flow
{
    /// A flow merely acts as a wrapper for another sequence, while also offering a composition function to
    /// compose a new sequence out of the held sequence and another sequence.
    /// To compose two sequences, `a` and `b`, write `Flow(a) | b`.
    template<class S>
    class Flow
    {
    public:
        using ElementType = typename S::ElementType;

        Maybe<ElementType> next()
        {
            return sequence.next();
        }
        
        explicit Flow(S const &sequence):
            sequence(sequence)
        {
        }

        explicit Flow(S &&sequence):
            sequence(std::move(sequence))
        {
        }
        
        /// Sequence composition.
        template<class C>
        auto operator|(C sequenceConstructor) &&
        {
            return Flow<details::FunctionReturnType<C, S>>(sequenceConstructor(std::move(sequence)));
        }
        
        /// Sequence composition when flow is copied simultaneously.
        template<class C>
        auto operator|(C sequenceConstructor) const &
        {
            return Flow<details::FunctionReturnType<C, S>>(sequenceConstructor(S(sequence)));
        }
        
        details::SequenceIterator<S> begin()
        {
            return details::SequenceIterator<S>(sequence);
        }
        
        details::SequenceEndIterator end()
        {
            return details::SequenceEndIterator{};
        }

    private:
        S sequence;
    };
}
