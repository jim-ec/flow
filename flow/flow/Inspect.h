#pragma once

#include <flow/Maybe.h>

namespace flow
{
    /// Calls a function on sequence elements, but passes the elements unchanged otherwise.
	/// Arity: 1 -> 1
    template<class S, class F>
    class Inspect
    {
    public:
        using ElementType = typename S::ElementType;

        Inspect(S &&sequence, F function):
            sequence(std::move(sequence)),
            function(function)
        {
        }

        Maybe<ElementType> next()
        {
            Maybe<ElementType> nextElement = sequence.next();
            
            if (nextElement.hasValue())
            {
                // Call inspection functional on contained element, but ensure that the element is not modified.
                function(static_cast<ElementType const &>(nextElement.value()));
            }
            
            return nextElement;
        }

    private:
        S sequence;
        F function;
    };

    template<class F>
    auto inspect(F function)
    {
        return [=] (auto &&sequence)
        {
            return Inspect(std::move(sequence), function);
        };
    }
}
