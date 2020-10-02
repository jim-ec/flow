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

        Maybe<ElementType> next()
        {
            if (iterator != end)
            {
                ElementType element = &*iterator;
                ++iterator;
                return element;
            }
            else
            {
                return None();
            }
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
