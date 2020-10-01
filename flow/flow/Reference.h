#pragma once

#include <functional>

#include <flow/Flow.h>

namespace flow
{
    /// Yields all elements by reference of the given container.
    /// As the pointer are not `const`, it is possible to mutate the underlying container values.
    /// The container is not owned by this sequence.
    /// When returning an `RefElements` sequence from a scope i.e. exceeding its lifetime,
    /// the returned elements are dangled references and contain undefined values.
    /// Arity: 0 -> 1
    template<class C>
    class Reference
    {
    private:
        using IteratorType = typename C::iterator;
        
    public:
        using ElementType = std::reference_wrapper<typename C::value_type>;

        explicit Reference(C &container):
            container(container),
            iterator(this->container.begin()),
            end(this->container.end())
        {
        }

        /// Since this sequence type does not own the underlying container,
        /// it cannot take ownership of it.
        explicit Reference(C &&container) noexcept = delete;
        
        bool probe()
        {
            return iterator != end;
        }

        ElementType next()
        {
            ElementType element = *iterator;
            ++iterator;
            return element;
        }

    private:
        C &container;
        IteratorType iterator;
        IteratorType end;
    };

    template<class C>
    auto reference(C &container)
    {
        return Flow(Reference(container));
    }
}
