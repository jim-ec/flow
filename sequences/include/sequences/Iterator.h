//
// Created by jim on 11/13/19.
//

#pragma once

#include "sequences/Sequence.h"

namespace sequences
{
    template<class Seq>
    class Iterator
    {
    public:

        using value_type = typename Seq::output_type;

        explicit Iterator(Seq &seq, bool initialize) :
            seq{seq},
            element{initialize ? seq.next() : std::optional<value_type>{}}
        {}

        Iterator &operator++()
        {
            element = seq.next();
            return *this;
        }

        value_type operator*()
        {
            return *element;
        }

        bool operator!=(const Iterator &)
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
    class IteratorRange
    {
    public:

        using value_type = typename Seq::output_type;

        explicit IteratorRange(Seq &seq) :
        seq{seq}
        {}

        Iterator<Seq> begin()
        {
            return Iterator<Seq>{seq, true};
        }

        Iterator<Seq> end()
        {
            return Iterator<Seq>{seq, false};
        }

    private:
        Seq &seq;
    };
}
