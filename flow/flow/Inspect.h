#pragma once

#include <optional>

namespace flow
{
    /// Calls a function on sequence elements, but passes the elements unchanged otherwise.
	/// Arity: 1 -> 1
    template<class S, class F>
    class Inspect
    {
    public:
        static inline bool constexpr finite = S::finite;
        using ElementType = typename S::ElementType;

        Inspect(S const &base, F function):
            base(base),
            function(function)
        {
        }

        std::optional<ElementType> next()
        {
            std::optional<ElementType> state(base.next());
            if (state.has_value())
            {
                // Call inspection functional on contained element,
                // but ensure that the element is not modified.
                function(static_cast<ElementType const &>(state.value()));
            }
            return state;
        }

    private:
        S base;
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
