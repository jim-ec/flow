#pragma once

#include <optional>

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
        
        Then(S &&base, F function):
            base(std::move(base)),
            function(function)
        {
        }
        
        std::optional<ElementType> next()
        {
            std::optional<typename S::ElementType> nextElement(base.next());
            
            // Return none if the base sequence is exhausted.
            if (!nextElement.has_value())
            {
                return {};
            }
            
            auto nextOptional = nextElement.value();
            if (nextOptional.has_value())
            {
                // Map only some(some(x)) values.
                return function(std::move(nextOptional.value()));
            }
            else {
                // Pass through some(none) elements.
                return ElementType{};
            }
        }
        
    private:
        S base;
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
