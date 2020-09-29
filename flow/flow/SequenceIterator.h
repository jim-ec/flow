#pragma once

namespace flow::details
{
    class SequenceEndIterator
    {
    };
    
    /// A forward iterator, yielding elements from a sequence.
    /// Unless the sequence is fused, the iterator should not be incremented
    /// after the contained element is `None`.
    template<class S>
    class SequenceIterator
    {
    public:
        /// The type of elements yielded by this iterator is simply the type of elements yielded by the
        /// underlying sequence.
        using value_type = typename S::ElementType;
        
        /// Constructs an iterator yielding elements from the given sequence.
        explicit SequenceIterator(S const &sequence):
            sequence(sequence),
            element((this->sequence.probe(), this->sequence.next()))
        {
        }
        
        /// Yields the next element from the sequence.
        /// Unless the sequence is fused, this should not be called anymore as soon as the element is `None`,
        /// which can be queried by using the comparision function.
        SequenceIterator &operator++()
        {
            if (sequence.probe())
            {
                details::reinitialize(element, sequence.next());
            }
            else
            {
                element.reset();
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
        S sequence;
        std::optional<value_type> element;
    };
}
