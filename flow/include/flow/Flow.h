//
// Created by jim on 11/18/19.
//

#pragma once

#include <flow/core/TypeTraits.h>

namespace flow {

    /// A flow merely acts as a wrapper for another sequence, while also offering a composition function to
    /// compose a new sequence out of the hold sequence and another sequence.
    /// To compose two flow, `a` and `b`: `Sequence(a) | b`.
    template<class Seq>
    class Flow {
    public:

        using output_type = typename Seq::output_type;

        /// Whether this sequence is finite or not depends on the underlying sequence.
        static inline bool const finite = Seq::finite;

        /// The next element in this stream is simply the next element of the sequence.
        std::optional<output_type> next() {
            return seq.next();
        }

        explicit Flow(Seq const &seq) : seq(seq) {}

        explicit Flow(Seq &&seq) : seq(std::move(seq)) {}

        /// The sequence composition.
        /// Takes a sequence constructor, i.e. a function which expects this sequence and returns a new sequence
        /// based on it. The resulting sequence is returned.
        template<class Ctor>
        auto operator|(Ctor ctor) const &{
            using SeqType = Flow<function_return_type<Ctor, Seq>>;
            return SeqType(ctor(seq));
        }

        template<class Ctor>
        auto operator|(Ctor ctor) &&{
            using SeqType = Flow<function_return_type<Ctor, Seq>>;
            return SeqType(ctor(std::move(seq)));
        }

    private:
        Seq seq;
    };
}
