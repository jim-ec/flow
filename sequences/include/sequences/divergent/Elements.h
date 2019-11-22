//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>
#include <initializer_list>

#include <sequences/Flow.h>
#include <sequences/functional/Deref.h>
#include <sequences/core/Log.h>

namespace sequences {
    /// Yields all elements of the given container.
    /// Arity: 0 -> 1
    template<class C>
    class Elements {
    public:
        static inline bool constexpr finite = true;
        using value_type = typename C::value_type;
        using output_type = value_type;
        using iterator_type = typename C::iterator;

        Elements(Elements const &rhs) :
            xs(rhs.xs),
            iterator(xs.begin()),
            end(xs.end())
        {
            log("Elements(Elements const &)");
        }

        Elements(Elements &&rhs) noexcept :
            xs(std::move(rhs.xs)),
            iterator(xs.begin()),
            end(xs.end())
        {
            log("Elements(Elements &&)");
        }

        explicit Elements(C const &xs) :
            xs(xs),
            iterator(this->xs.begin()),
            end(this->xs.end()) {
            log("Elements(C const &)");
        }

        explicit Elements(C &&xs) :
            xs(std::move(xs)),
            iterator(this->xs.begin()),
            end(this->xs.end()) {
            log("Elements(C &&)");
        }

        ~Elements() {
            log("~Elements()");
        }

//        template<class T>
//        Elements(std::initializer_list<T> const &xs) :
//                Elements(xs)
//        {}

        std::optional<output_type> next() {
            if (iterator != end) {
                // Because we own the container, we can move elements out of it.
                log("Elements: Move next element out of container.");
                output_type el(std::move(*std::move(iterator)));
                ++iterator;
                log("Elements: Return element by move.");
                return std::move(el);
            }
            else {
                return {};
            }
        }

    private:
        C xs;
        iterator_type iterator;
        iterator_type end;
    };

//    /// Deduce the correct container type when constructing from an initializer list.
//    template<class T>
//    Elements(std::initializer_list<T> const &xs) -> Elements<std::initializer_list<T>>;

//    template<class C>
//    auto ref_elements(C const &c) {
//        return Flow(RefElements(c));
//    }

    template<class C>
    auto elements(C &&c) {
        return Flow(Elements(std::forward<C>(c)));
    }
}
