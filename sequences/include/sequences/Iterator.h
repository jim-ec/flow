//
// Created by jim on 11/13/19.
//

#pragma once

namespace sequences
{
    template<class Seq>
    class YieldIterator
    {
    public:

        using value_type = typename Seq::output_type;

        explicit YieldIterator(
            Seq const &seq,
            bool initialize
        ) :
            seq{seq},
            element{initialize ? this->seq.next() : std::optional<value_type>{}}
        {}

        YieldIterator &operator++()
        {
            element.reset();
            std::optional<value_type> el = seq.next();
            if (el.has_value()) {
                element.emplace(*el);
            }
            return *this;
        }

        value_type &operator*()
        {
            return *element;
        }

        value_type const &operator*() const
        {
            return *element;
        }

        bool operator!=(YieldIterator const&)
        {
            return element.has_value();
        }

    private:
        Seq seq;
        std::optional<value_type> element;
    };

    /// A forward iterator wrapping a sequence.
    /// Unlike a sequence, this can be used in the range-based for loop language construct.
    /// The main purpose of this class is that the actual sequences do not need to implement the
    /// impractical iterator interface, such as comparing to check whether a sequence is still valid.
    template<class Seq>
    class Exhaust
    {
    public:

        static_assert(Seq::finite, "Cannot exhaust an infinite sequence.");
        using value_type = typename Seq::output_type;

        explicit Exhaust(Seq const &seq) :
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
        Seq seq;
    };
}
