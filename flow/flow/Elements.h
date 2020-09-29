#pragma once

#include <optional>
#include <initializer_list>

#include <flow/Flow.h>

namespace flow
{
    /// Yields all elements of the given container.
    /// The container is owned by this sequence.
    /// This allows returning an `Elements` sequence from a scope without exceeding its lifetime.
    /// This is especially useful when mapping elements of a sequence to `Elements` flow.
    /// Arity: 0 -> 1
    template<class C>
    class Elements
    {
    public:
        static inline bool constexpr finite = true;
        using ElementType = typename C::value_type;
        using IteratorType = typename C::iterator;

        explicit Elements(C const &container):
            container(container),
            iterator(this->container.begin()),
            end(this->container.end())
        {}

        explicit Elements(C &&container):
            container(std::move(container)),
            iterator(this->container.begin()),
            end(this->container.end())
        {}

        std::optional<ElementType> next()
        {
            if (iterator != end)
            {
                // Because we own the container, we can move elements out of it.
                ElementType element(std::move(*std::move(iterator)));
                ++iterator;
                return std::move(element);
            }
            else
            {
                return {};
            }
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
