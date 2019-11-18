//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences {

    /// Yields all elements of the given container by move.
    /// This destroys the elements hold the underlying container.
    /// Arity: 0 -> 1
    template<class C>
    class MoveElements {
    public:
        static inline bool constexpr finite = true;
        using output_type = typename C::value_type;
        using iterator_type = typename C::iterator;

        explicit MoveElements(C &&xs) :
                xs(std::move(xs)),
                iterator(xs.begin()),
                end(xs.end()) {}

        std::optional<output_type> next() {
            if (iterator != end) {
                output_type el(std::move(*iterator));
                ++iterator;
                return el;
            }
            else {
                return {};
            }
        }

    private:
        C const &xs;
        iterator_type iterator;
        iterator_type end;
    };
}
