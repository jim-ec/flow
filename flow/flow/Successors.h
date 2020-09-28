#pragma once

#include <optional>

namespace flow
{
    /// Successors based on some initial value.
	/// Arity: 0 -> 1
    template<class T>
    class Successors
    {
    public:
        static inline bool constexpr finite = false;
        using output_type = T;

        explicit Successors(T const &n):
            n(n)
        {
        }

        std::optional<T> next()
        {
            T state(n);
            ++n;
            return state;
        }

    private:
        T n;
    };

    template<class T>
    auto successors(T const &n)
    {
        return Flow(Successors(n));
    }
}