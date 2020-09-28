//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace flow
{
    /// Yields up to a fixed amount of elements out of a base sequence.
	/// Arity: 1 -> 1
    template<class S>
    class Take
    {
    public:
        static inline bool constexpr finite = true;
        using ElementType = typename S::ElementType;

        Take(S const &base, size_t const n):
            base(base),
            k(0),
            n(n)
        {
        }

        std::optional<ElementType> next()
        {
            if (k < n)
            {
                ++k;
                return base.next();
            }
            else
            {
                return {};
            }
        }

    private:
        S base;
        size_t k;
        size_t n;
    };

    auto take(size_t const n)
    {
        return [=] (auto &&sequence)
        {
            return Take(std::forward<decltype(sequence)>(sequence), n);
        };
    }
}
