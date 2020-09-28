#pragma once

#include <optional>

#include <flow/TypeTraits.h>

namespace flow
{
    /// Arity: 2 -> 1
    template<class L, class R>
    class Zip
    {
    public:
        static inline bool constexpr finite = L::finite || R::finite;
        using ElementType = std::tuple<typename L::ElementType, typename R::ElementType>;

        explicit Zip(L const &left, R const &right):
            left(left),
            right(right)
        {
        }

        std::optional<ElementType> next()
        {
            // Create tuple containing the next element for each sequence.
            // Then check is any entry is none, if so, this sequence is exhausted.
            // Otherwise, return the tuple.
            
            auto nextLeft = left.next();
            auto nextRight = right.next();

            if (!nextLeft.has_value() || !nextRight.has_value())
            {
                return {};
            }
            else
            {
                return {std::tuple(std::move(*nextLeft), std::move(*nextRight))};
            }
        }

    private:
        L left;
        R right;
    };

    template<class Seq>
    auto zip(Seq right)
    {
        return [=] (auto const &left)
        {
            return Zip(left, right);
        };
    }
}
