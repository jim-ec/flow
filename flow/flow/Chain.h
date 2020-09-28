#pragma once

#include <optional>

namespace flow
{
    /// First yields all elements from the first sequence,
    /// then yields elements from the second flow.
    /// The first flow should be finite.
    /// Elements from the continuation flow must be convertible to the output
    /// type of the drain sequence.
    /// Arity: 2 -> 1
    template<class D, class C>
    class Chain
    {
    public:
        static_assert(D::finite, "Cannot enchain a sequence to an infinite sequence.");
        static inline bool constexpr finite = C::finite;
        using ElementType = typename D::ElementType;

        explicit Chain(D const &drainingSequence, C const &continuationSequence):
            drainingSequence(drainingSequence),
            continuationSequence(continuationSequence),
            draining(true)
        {}

        std::optional<ElementType> next()
        {
            if (draining)
            {
                // Try to get another element of the drain sequence.
                std::optional<ElementType> element = drainingSequence.next();
                if (element.has_value())
                {
                    return std::move(element);
                }
                else
                {
                    // The draining sequence is exhausted.
                    // From here on, yield from the continuation sequence.
                    draining = false;
                }
            }

            return continuationSequence.next();
        }

    private:
        D drainingSequence;
        C continuationSequence;
        bool draining;
    };

    template<class C>
    auto chain(C const &continuationSequence)
    {
        return [=] (auto const &drainingSequence)
        {
            return Chain(drainingSequence, continuationSequence);
        };
    }

}
