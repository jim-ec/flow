#pragma once

#include <flow/TypeTraits.h>

namespace flow
{
    /// Similar to a regular map, only that this sequence is optional aware.
    /// The base sequence has to produce optionals over some type.
    /// The mapping function maps values from that type to another type.
    /// This sequence than produces optionals over that new type.
    /// The mapping function is only called for input optionals which holds a value.
    /// Arity: 1 -> 1
    template<class S, class F>
    class Then
    {
    public:
        static inline bool constexpr finite = S::finite;
                
        using FunctionInputType = typename S::ElementType::value_type;
        using FunctionOutputType = details::FunctionReturnType<F, FunctionInputType>;
        using ElementType = std::optional<FunctionOutputType>;
        
        Then(S &&sequence, F function):
            sequence(std::move(sequence)),
            function(function)
        {
        }
        
        bool probe()
        {
            return sequence.probe();
        }
        
        ElementType next()
        {
            typename S::ElementType nextOptional = sequence.next();
            
            if (nextOptional.has_value())
            {
                // Map only some(some(x)) values.
                return function(std::move(nextOptional.value()));
            }
            else {
                // Pass through some(none) elements.
                return nextOptional;
            }
        }
        
    private:
        S sequence;
        F function;
    };
    
    template<class F>
    auto then(F function)
    {
        return [=] (auto &&sequence)
        {
            return Then(std::forward<decltype(sequence)>(sequence),function);
        };
    }
}
