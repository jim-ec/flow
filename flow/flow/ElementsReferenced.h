#pragma once

#include <flow/Flow.h>

namespace flow
{
    /// Yields pointers to all elements of the given container, avoiding creating copying the original elements.
    /// Using this sequence beyond the given container's lifetime will yield dangling pointers.
    template<class C>
    class ElementsReferenced
    {
    private:
        using IteratorType = typename C::iterator;
        
    public:
        using ElementType = typename C::value_type *;

        explicit ElementsReferenced(C &container):
            container(container),
            iterator(this->container.begin()),
            end(this->container.end())
        {
        }

        /// Since this sequence type does not own the underlying container,
        /// it cannot take ownership of it.
        explicit ElementsReferenced(C &&container) noexcept = delete;
        
        bool probe()
        {
            return iterator != end;
        }

        ElementType next()
        {
            ElementType element = &*iterator;
            ++iterator;
            return element;
        }

    private:
        C &container;
        IteratorType iterator;
        IteratorType end;
    };

    template<class C>
    auto elementsReferenced(C &container)
    {
        return Flow(ElementsReferenced(container));
    }
}
