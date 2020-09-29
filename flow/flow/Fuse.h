#pragma once

namespace flow
{
    /// Ensures the further calls to `next()` on a sequence return `None`s
    /// after it has been returned once from the base sequence.
	/// Arity: 1 -> 1
    template<class S>
    class Fuse
    {
    public:
        // TODO: Optimized implementation for infinite flow?
        static inline bool constexpr finite = S::finite;
        using ElementType = typename S::ElementType;

        explicit Fuse(S const &sequence):
            sequence(sequence),
            exhausted(false)
        {}
        
        bool probe()
        {
            if (exhausted)
            {
                // The base sequence is already exhausted from previous calls.
                return false;
            }
            else if (!sequence.probe())
            {
                // The base sequence has become exhausted.
                exhausted = true;
                return false;
            }
            else
            {
                return true;
            }
        }

        ElementType next()
        {
            return sequence.next();
        }

    private:
        S sequence;
        bool exhausted;
    };

    auto fuse()
    {
        return [] (auto &&sequence)
        {
            return Fuse(std::forward<decltype(sequence)>(sequence));
        };
    }
}
