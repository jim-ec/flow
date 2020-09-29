#pragma once

#include <optional>

#include <flow/TypeTraits.h>
#include <flow/SequenceIterator.h>

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
        static inline bool const finite = S::finite;

        std::optional<ElementType> yield()
        {
            if (sequence.probe())
            {
                return sequence.next();
            }
            else
            {
                return {};
            }
        }
        
        bool probe()
        {
            return sequence.probe();
        }

        ElementType next()
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
        /// This instance stays intact, the new flow contains a copy of this instance's sequence.
        template<class C>
        auto operator|(C sequenceConstructor) const &
        {
            return Flow<details::FunctionReturnType<C, S>>(sequenceConstructor(sequence));
        }

        /// Move sequence composition.
        /// Moves the held sequence into the new flow while wrapping it into new sequence.
        template<class C>
        auto operator|(C sequenceConstructor) &&
        {
            return Flow<details::FunctionReturnType<C, S>>(sequenceConstructor(std::move(sequence)));
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
