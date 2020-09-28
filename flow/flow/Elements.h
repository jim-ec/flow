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

        Elements(Elements const &rhs):
            xs(rhs.xs),
            iterator(xs.begin()),
            end(xs.end())
        {}

        Elements(Elements &&rhs) noexcept:
            xs(std::move(rhs.xs)),
            iterator(xs.begin()),
            end(xs.end())
        {}

        explicit Elements(C const &xs):
            xs(xs),
            iterator(this->xs.begin()),
            end(this->xs.end())
        {}

        explicit Elements(C &&xs):
            xs(std::move(xs)),
            iterator(this->xs.begin()),
            end(this->xs.end())
        {}

        std::optional<ElementType> next()
        {
            if (iterator != end)
            {
                // Because we own the container, we can move elements out of it.
                ElementType el(std::move(*std::move(iterator)));
                ++iterator;
                return std::move(el);
            }
            else
            {
                return {};
            }
        }

    private:
        C xs;
        IteratorType iterator;
        IteratorType end;
    };

    /// Creates an `Elements` flow.
    template<class C>
    auto elements(C &&c)
    {
        return Flow(Elements(std::forward<C>(c)));
    }
}
