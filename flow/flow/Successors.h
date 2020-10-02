#pragma once

#include <flow/Maybe.h>

namespace flow
{
    /// Successors based on some initial value.
	/// Arity: 0 -> 1
    template<class T>
    class Successors
    {
    public:
        using ElementType = T;

        explicit Successors(T const &n):
            n(n)
        {
        }
        
        bool probe()
        {
            return true;
        }

        Maybe<T> next()
        {
            T nextElement = n;
            ++n;
            return nextElement;
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
