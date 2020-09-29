#pragma once

#include <flow/Fuse.h>

namespace flow
{
    /// Before yielding the next element, skip `n - 1` elements.
	/// Arity: 1 -> 1
    template<class S>
    class Stride
    {
    public:
        using ElementType = typename S::ElementType;

        Stride(S &&sequence, size_t const n):
            sequence(Fuse(std::move(sequence))),
            n(n)
        {}
        
        bool probe()
        {
            // Skip `n` elements.
            // Because base is fused, it guarantees that consecutive calls to
            // `next()` return `None`.
            for (size_t k = 0; k < n - 1; ++k) {
                sequence.probe();
            }
            return sequence.probe();
        }

        ElementType next()
        {
            return sequence.next();
        }

    private:
        Fuse<S> sequence;
        size_t n;
    };

    auto stride(size_t const n)
    {
        return [=] (auto &&sequence)
        {
            return Stride(std::forward<decltype(sequence)>(sequence), n);
        };
    }
}
