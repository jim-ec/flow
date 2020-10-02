#pragma once

#include <flow/Maybe.h>

namespace flow
{
    class EndIterator
    {
    };
    
    /// A forward iterator, yielding elements from a sequence.
    /// When incrementing this iterator, the next element is stored.
    /// It can be accessed by using the dereference operator.
    template<class S>
    class Iterator
    {
    public:
        /// The type of elements yielded by this iterator is simply the type of elements yielded by the underlying sequence.
        using value_type = typename S::ElementType;
        
        /// Constructs an iterator yielding elements from the given sequence.
        explicit Iterator(S const &sequence):
            sequence(sequence),
            element(this->sequence.next())
        {
        }
        
        /// Yields the next element from the sequence.
        /// Unless the sequence is fused, this should not be called anymore as soon as the element is `None`,
        /// which can be queried by using the comparision function.
        Iterator &operator++()
        {
            element = sequence.next();
            return *this;
        }
        
        /// Returns a mutable reference to the hold element.
        /// This must not be called if the element is `None`.
        value_type &operator*()
        {
            return element.value();
        }
        
        /// Returns a immutable reference to the hold element.
        /// This must not be called if the element is `None`.
        value_type const &operator*() const
        {
            return element.value();
        }
        
        /// Compares against the end iterator sentinel.
        /// The result of the comparision does not depend on the actual given end-iterator but only whether the
        /// element owned by this iterator is not `None`.
        /// The interface is only implemented to make the range-based for-each loop construct working and to comply
        /// with C++'s convention of iterating over iterators.
        bool operator!=(EndIterator const&)
        {
            return element.hasValue();
        }
        
    private:
        S sequence;
        Maybe<value_type> element;
    };
}
