//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{
    /// Successors based on some initial value.
    template<class T>
    class Successors
    {
        T n;
    public:
        using output_type = int;

        Successors() :
            n{}
        {}

        std::optional<T> next()
        {
            T state = n;
            ++n;
            return std::move(state);
        }
    };
}
