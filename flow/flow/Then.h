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
        
        Then(S &&base, F fn):
            base(std::move(base)),
            fn(fn)
        {
        }
        
        std::optional<ElementType> next()
        {
            std::optional<typename S::ElementType> k(base.next());
            
            // Return none if the base sequence is exhausted.
            if (!k.has_value())
            {
                return {};
            }
            
            auto nextOptional = k.value();
            if (nextOptional.has_value())
            {
                // Map only some(some(x)) values.
                return fn(std::move(nextOptional.value()));
            }
            else {
                // Pass through some(none) elements.
                auto x = ElementType{};
                return x;
            }
        }
        
    private:
        S base;
        F fn;
    };
    
    template<class F>
    auto then(F fn)
    {
        return [=] (auto &&seq)
        {
            return Then(std::forward<decltype(seq)>(seq),fn);
        };
    }
}
