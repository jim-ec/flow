//
// Created by jim on 11/13/19.
//

#pragma once

#include "sequences/Sequence.h"

namespace sequences
{
    template<class Seq>
    class YieldIterator
    {
    public:

        using value_type = typename Seq::output_type;

        explicit YieldIterator(Seq &seq, bool initialize) :
            seq{seq},
            element{initialize ? seq.next() : std::optional<value_type>{}}
        {}

        YieldIterator &operator++()
        {
            element = seq.next();
            return *this;
        }

        value_type operator*()
        {
            return *element;
        }

        bool operator!=(const YieldIterator &)
        {
            return element.has_value();
        }

    private:
        Seq &seq;
        std::optional<value_type> element;
    };

    /// A forward iterator wrapping a sequence.
    /// Unlike a sequence, this can be used in the range-based for loop language construct.
    /// The main purpose of this class is that the actual sequences do not need to implement the
    /// impractical iterator interface, such as comparing to check whether a sequence is still valid.
    template<class Seq>
    class Over
    {
    public:

        using value_type = typename Seq::output_type;

        explicit Over(Seq &seq) :
        seq{seq}
        {}

        YieldIterator<Seq> begin()
        {
            return YieldIterator<Seq>{seq, true};
        }

        YieldIterator<Seq> end()
        {
            return YieldIterator<Seq>{seq, false};
        }

    private:
        Seq &seq;
    };
}
