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
        
        bool probe()
        {
            return left.probe() && right.probe();
        }

        ElementType next()
        {
            return {std::tuple(left.next(), right.next())};
        }

    private:
        L left;
        R right;
    };

    template<class S>
    auto zip(S right)
    {
        return [=] (auto const &left)
        {
            return Zip(left, right);
        };
    }
}
