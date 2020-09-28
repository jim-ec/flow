#pragma once

#include <flow/TypeTraits.h>

namespace flow
{
    /// A flow merely acts as a wrapper for another sequence, while also offering a composition function to
    /// compose a new sequence out of the hold sequence and another sequence.
    /// To compose two flow, `a` and `b`: `Suence(a) | b`.
    template<class S>
    class Flow
    {
    public:
        using output_type = typename S::output_type;

        /// Whether this sequence is finite or not depends on the underlying sequence.
        static inline bool const finite = S::finite;

        /// The next element in this stream is simply the next element of the sequence.
        std::optional<output_type> next()
        {
            return seq.next();
        }

        explicit Flow(S const &seq):
            seq(seq)
        {
        }

        explicit Flow(S &&seq):
            seq(std::move(seq))
        {
        }

        /// The sequence composition.
        /// Takes a sequence constructor, i.e. a function which expects this sequence and returns a new sequence
        /// based on it. The resulting sequence is returned.
        template<class Ctor>
        auto operator|(Ctor ctor) const &
        {
            using SType = Flow<function_return_type<Ctor, S>>;
            return SType(ctor(seq));
        }

        template<class Ctor>
        auto operator|(Ctor ctor) &&
        {
            using SType = Flow<function_return_type<Ctor, S>>;
            return SType(ctor(std::move(seq)));
        }

    private:
        S seq;
    };
}
