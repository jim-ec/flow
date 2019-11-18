//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

#include <sequences/core/ISequence.h>

namespace sequences
{
    struct X{
        void operator++() {}

        double d;
    };

    /// Successors based on some initial value.
	/// Arity: 0 -> 1
    template<class T>
    class Successors : public ISequence<Successors<X>> // TODO
    {
    public:

        static inline bool constexpr finite = false;
        using output_type = T;

        Successors() :
            n{}
        {}

        explicit Successors(const T &n) :
            n{n}
        {}

        std::optional<T> next()
        {
            T state = n;
            ++n;
            return std::move(state);
        }

    private:
        T n;
    };
}
