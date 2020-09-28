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
        using output_type = typename S::output_type;

        Inspect(S const &base, F fn):
            base(base),
            fn(fn)
        {
        }

        std::optional<output_type> next()
        {
            std::optional<output_type> state(base.next());
            if (state.has_value())
            {
                // Call inspection functional on contained element,
                // but ensure that the element is not modified.
                fn(static_cast<output_type const &>(state.value()));
            }
            return state;
        }

    private:
        S base;
        F fn;
    };

    template<class F>
    auto inspect(F fn)
    {
        return [=] (auto &&seq)
        {
            return Inspect(std::forward<decltype(seq)>(seq), fn);
        };
    }
}
