#pragma once

#include <optional>

#include "flow/TypeTraits.h"

namespace flow
{
    /// Yields only elements of the base sequence where the predicate function returns `true`.
	/// Arity: 1 -> 1
    template<class S, class F>
    class Filter
    {
    public:
        constexpr static inline bool finite = S::finite;
        using ElementType = typename S::ElementType;

        Filter(S const &sequence, F predicate):
            sequence(sequence),
            predicate(predicate)
        {}
        
        bool probe()
        {
            for (;;)
            {
                if (!sequence.probe())
                {
                    // Sequence is exhausted.
                    return false;
                }
                
                // Call predicate on current element.
                // If it succeeds, this element is stored.
                ElementType nextElement = sequence.next();
                if (predicate(nextElement))
                {
                    details::reinitialize(this->nextElement, std::move(nextElement));
                    return true;
                }
            }
        }

        ElementType next()
        {
            return std::move(nextElement.value());
        }

    private:
        S sequence;
        std::optional<ElementType> nextElement;
        F predicate;
    };

    template<class F>
    auto filter(F predicate)
    {
        return [=] (auto &&sequence)
        {
            return Filter(std::forward<decltype(sequence)>(sequence), predicate);
        };
    }
}
