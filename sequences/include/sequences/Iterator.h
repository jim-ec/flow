//
// Created by jim on 11/13/19.
//

#pragma once

namespace sequences
{
	class SequenceEndIterator {
	};

	/// A forward iterator, yielding elements from a sequence.
	/// Unless the sequence is fused, the iterator should not be incremented
	/// after the contained element is `None`.
    template<class Seq>
    class SequenceIterator
    {
    public:

    	/// The type of elements yielded by this iterator is simply the type of elements yielded by the
    	/// underlying sequence.
        using value_type = typename Seq::output_type;

    	/// Constructs an iterator yielding elements from the given sequence.
        explicit SequenceIterator(Seq const &seq) :
            seq{seq},
            element{this->seq.next()}
        {}

        /// Yields the next element from the sequence.
        /// Unless the sequence is fused, this should not be called anymore as soon as the element is `None`,
        /// which can be queried by using the comparision function.
        SequenceIterator &operator++()
        {
            element.reset();
            std::optional<value_type> el = seq.next();
            if (el.has_value()) {
                element.emplace(*el);
            }
            return *this;
        }

        /// Returns a mutable reference to the hold element.
        /// This must not be called if the element is `None`.
        value_type &operator*()
        {
            return *element;
        }

		/// Returns a immutable reference to the hold element.
		/// This must not be called if the element is `None`.
        value_type const &operator*() const
        {
            return *element;
        }

        /// Compares against the end iterator sentinel.
        /// The result of the comparision does not depend on the actual given end-iterator but only whether the
        /// element owned by this iterator is not `None`.
        /// The interface is only implemented to make the range-based for-each loop construct working and to comply
        /// with C++'s convention of iterating over iterators.
        bool operator!=(SequenceEndIterator const&)
        {
            return element.has_value();
        }

    private:
        Seq seq;
        std::optional<value_type> element;
    };

    /// A container interface to a sequence.
    /// It allows to iterate over the elements of a sequence in a range-based for-loop construct,
    /// which is not possible with sequences directly, as they have a simpler interface.
    /// The main purpose of this class is that the actual sequences do not need to implement the impractical iterator
    /// interface, such as comparing to another iterator to check whether a sequence is still valid.
    template<class Seq>
    class ForEach
    {
    public:

        static_assert(Seq::finite, "Cannot exhaust an infinite sequence.");
        using value_type = typename Seq::output_type;

        explicit ForEach(Seq const &seq) :
            seq{seq}
        {}

        SequenceIterator<Seq> begin()
        {
            return SequenceIterator<Seq>{seq};
        }

		SequenceEndIterator end()
        {
            return SequenceEndIterator{};
        }

    private:
        Seq seq;
    };
}
