#pragma once

#include <optional>

#include <flow/details.h>

namespace flow
{
    /// Arity: 2 -> 1
    template<class L, class R>
    class Zip
    {
    public:
        using ElementType = std::tuple<typename L::ElementType, typename R::ElementType>;

        explicit Zip(L &&left, R &&right):
            left(std::move(left)),
            right(std::move(right))
        {
        }
        
        bool probe()
        {
            return left.probe() && right.probe();
        }

        ElementType next()
        {
            return {left.next(), right.next()};
        }

    private:
        L left;
        R right;
    };

    template<class S>
    auto zip(S &&right)
    {
        return [right = std::move(right)] (auto &&left) mutable
        {
            return Zip(std::move(left), std::move(right));
        };
    }
}
