//
// Created by jim on 11/13/19.
//

#pragma once

namespace flow
{
    /// Yields up to a fixed amount of elements out of a base sequence.
	/// Arity: 1 -> 1
    template<class S>
    class Take
    {
    public:
        using ElementType = typename S::ElementType;

        Take(S &&sequence, size_t const n):
            sequence(std::move(sequence)),
            k(0),
            n(n)
        {
        }
        
        bool probe()
        {
            if (k < n)
            {
                ++k;
                return sequence.probe();
            }
            else
            {
                return false;
            }
        }

        ElementType next()
        {
            return sequence.next();
        }

    private:
        S sequence;
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
