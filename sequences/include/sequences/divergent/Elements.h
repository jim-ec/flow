//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>
#include <initializer_list>

namespace sequences {
    /// Yields all elements of the given container.
    /// Arity: 0 -> 1
    template<class C>
    class Elements {
    public:
        static inline bool constexpr finite = true;
        using output_type = typename C::value_type;
        using iterator_type = typename C::iterator;

        explicit Elements(C const &xs) :
                xs{xs},
                iterator{xs.begin()},
                end{xs.end()} {}

        std::optional<output_type> next() {
            if (iterator != end) {
                output_type const &el = *iterator;
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

    template<class C>
    Elements<C> elements(C const &elements) {
        return [=](auto seq) {
            return Elements{elements};
        };
    }

    template<class T>
    Elements<std::initializer_list<T>> elements(
            std::initializer_list<T> const &elements
    ) {
        return [=](auto seq) {
            return Elements{elements};
        };
    }
}
