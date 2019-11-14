//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences
{
    /// Successors based on some initial value.
    template<class T>
    class Successors
    {
    public:

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
