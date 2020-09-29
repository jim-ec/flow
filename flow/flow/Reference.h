#pragma once

#include <flow/Flow.h>

namespace flow
{
    /// Yields all elements by reference of the given container.
    /// Because references cannot be used with `std::optional`, pointers are used instead.
    /// As the pointer are not `const`, it is possible to mutate the underlying container values.
    /// The container is not owned by this sequence.
    /// When returning an `RefElements` sequence from a scope i.e. exceeding its lifetime,
    /// the returned elements are dangled references and contain undefined values.
    /// Arity: 0 -> 1
    template<class C>
    class Reference
    {
    public:
        static inline bool constexpr finite = true;
        using ElementType = typename C::value_type *;
        using IteratorType = typename C::iterator;

        explicit Reference(C &container):
            container(container),
            iterator(this->container.begin()),
            end(this->container.end())
        {
        }

        /// Since this sequence type does not own the underlying container,
        /// it cannot take ownership of it.
        explicit Reference(C &&container) = delete;
        
        bool probe()
        {
            return iterator != end;
        }

        ElementType next()
        {
            ElementType const elementPointer = &*iterator;
            ++iterator;
            return elementPointer;
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
