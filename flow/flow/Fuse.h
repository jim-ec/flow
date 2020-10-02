#pragma once

#include <flow/Maybe.h>

namespace flow
{
    /// Ensures the further calls to `next()` on a sequence return `None`s
    /// after it has been returned once from the base sequence.
	/// Arity: 1 -> 1
    template<class S>
    class Fuse
    {
    public:
        using ElementType = typename S::ElementType;

        explicit Fuse(S &&sequence):
            sequence(std::move(sequence)),
            exhausted(false)
        {}
        
        Maybe<ElementType> probe()
        {
            if (exhausted)
            {
                // The base sequence is already exhausted from previous calls.
                return None();
            }
            
            Maybe<ElementType> element = sequence.next();
            
            if (element.hasValue())
            {
                return element;
            }
            else
            {
                // The base sequence has become exhausted.
                exhausted = true;
                return None();
            }
        }

    private:
        S sequence;
        bool exhausted;
    };

    auto fuse()
    {
        return [] (auto &&sequence)
        {
            return Fuse(std::move(sequence));
        };
    }
}
