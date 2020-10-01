#pragma once

#include <flow/Flow.h>

namespace flow
{
    /// Yields all elements of the given container.
    /// The container is owned by this sequence and therefore copied during initialization.
    /// This allows keeping this sequence beyond the lifetime of the container the sequence was created from.
    /// Since each sequence usually copies, moves or creates new elements during each iteration,
    /// the container copy cost might be neglected if the whole container is used during iteration.
    template<class C>
    class Elements
    {
    public:
        using ElementType = typename C::value_type;
        using IteratorType = typename C::iterator;

        explicit Elements(C const &container):
            container(container),
            iterator(this->container.begin()),
            end(this->container.end())
        {
        }

        explicit Elements(C &&container):
            container(std::move(container)),
            iterator(this->container.begin()),
            end(this->container.end())
        {
        }
        
        explicit Elements(Elements const &elements):
            Elements(elements.container)
        {
        }
        
        explicit Elements(Elements &&elements):
            Elements(std::move(elements.container))
        {
        }
        
        bool probe()
        {
            return iterator != end;
        }

        ElementType next()
        {
            // Because we own the container, we can move elements out of it.
            ElementType element(std::move(*iterator));
            ++iterator;
            return element;
        }

    private:
        C container;
        IteratorType iterator;
        IteratorType end;
    };

    template<class C>
    auto elements(C &&container)
    {
        return Flow(Elements(std::forward<C>(container)));
    }
}
