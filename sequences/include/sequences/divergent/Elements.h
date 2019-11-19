//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>
#include <initializer_list>

#include <sequences/Flow.h>
#include <sequences/functional/Deref.h>

namespace sequences {
    /// Yields all elements of the given container.
    /// Arity: 0 -> 1
    template<class C>
    class RefElements {
    public:
        static inline bool constexpr finite = true;
        using value_type = typename C::value_type;
        using output_type = value_type const *;
        using iterator_type = typename C::const_iterator;

        explicit RefElements(C const &xs) :
                xs(xs),
                iterator(xs.begin()),
                end(xs.end()) {}

//        template<class T>
//        Elements(std::initializer_list<T> const &xs) :
//                Elements(xs)
//        {}

        std::optional<output_type> next() {
            if (iterator != end) {
                output_type el = &*iterator;
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

//    /// Deduce the correct container type when constructing from an initializer list.
//    template<class T>
//    Elements(std::initializer_list<T> const &xs) -> Elements<std::initializer_list<T>>;

    template<class C>
    auto ref_elements(C const &c) {
        return Flow(RefElements(c));
    }

    template<class C>
    auto elements(C const &c) {
        return Flow(RefElements(c)) | deref();
    }
}
