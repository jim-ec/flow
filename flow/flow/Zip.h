#pragma once

#include <flow/details.h>
#include <flow/Maybe.h>

namespace flow
{
    template<class L, class R>
    class Zip
    {
    public:
        using ElementType = std::pair<typename L::ElementType, typename R::ElementType>;

        explicit Zip(L &&left, R &&right):
            left(std::move(left)),
            right(std::move(right))
        {
        }

        Maybe<ElementType> next()
        {
            Maybe<typename L::ElementType> nextLeft = left.next();
            if (nextLeft.holdsValue())
            {
                Maybe<typename R::ElementType> nextRight = right.next();
                if (nextRight.holdsValue())
                {
                    return std::pair(nextLeft.value(), nextRight.value());
                }
            }
            return None();
        }

    private:
        L left;
        R right;
    };

    template<class S>
    auto zip(S &&right)
    {
        return [=] (auto &&left) mutable
        {
            return Zip(std::move(left), std::move(right));
        };
    }
}
