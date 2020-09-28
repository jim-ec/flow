#pragma once

#include <optional>

namespace flow
{
    /// Cycles the underlying sequence, i.e. after the last element is returned,
    /// another call to `next()` will return the first element of the underlying sequence.
    /// This sequence will never return `None`, unless the base sequence is empty.
    /// In that case, this sequence will always return `None`.
    template<class S>
    class Cycle
    {
    public:
        static inline bool constexpr finite = false;
        using ElementType = typename S::ElementType;
        
        explicit Cycle(S const &sequence):
            base(sequence),
            sequence(sequence)
        {}
        
        std::optional<ElementType> next()
        {
            // Try to get another element of the sequence.
            std::optional<ElementType> element = sequence.next();
            
            if (element.has_value())
            {
                return std::move(element);
            }
            else
            {
                // The sequence is exhausted.
                // Restart by re-initializing the sequence to the base sequence.
                sequence.~S();
                new (&sequence) S(base);
                
                return sequence.next();
            }
        }
        
    private:
        S base;
        S sequence;
    };
    
    auto cycle()
    {
        return [=] (auto const &sequence)
        {
            return Cycle(sequence);
        };
    }
    
}
