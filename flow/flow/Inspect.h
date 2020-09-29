#pragma once

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
        
        bool probe()
        {
            return sequence.probe();
        }

        ElementType next()
        {
            ElementType nextElement = sequence.next();
            
            // Call inspection functional on contained element, but ensure that the element is not modified.
            function(static_cast<ElementType const &>(nextElement));
            
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
            return Inspect(std::forward<decltype(sequence)>(sequence), function);
        };
    }
}
