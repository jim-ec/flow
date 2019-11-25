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
    /// Yields all elements by reference of the given container.
    /// Because references cannot be used with `std::optional`, pointers are used instead.
    /// As the pointer are not `const`, it is possible to mutate the underlying container values.
    /// The container is not owned by this sequence.
    /// When returning an `RefElements` sequence from a scope i.e. exceeding its lifetime,
    /// the returned elements are dangled references and contain undefined values.
    /// Arity: 0 -> 1
    template<class C>
    class RefElements {
    public:
        static inline bool constexpr finite = true;
        using value_type = typename C::value_type;
        using output_type = value_type *;
        using iterator_type = typename C::iterator;

        RefElements(RefElements const &rhs) = default;

        RefElements(RefElements &&rhs) noexcept = default;

        explicit RefElements(C &xs) :
            xs(xs),
            iterator(this->xs.begin()),
            end(this->xs.end())
        {}

        /// Since this sequence type does not own the underlying container,
        /// it cannot take ownership of it.
        explicit RefElements(C &&xs) = delete;

        std::optional<output_type> next() {
            if (iterator != end) {
                output_type const el_ptr(&*iterator);
                ++iterator;
                return el_ptr;
            }
            else {
                return {};
            }
        }

    private:
        C &xs;
        iterator_type iterator;
        iterator_type end;
    };

    /// Creates an `RefElements` flow.
    template<class C>
    auto ref_elements(C &c) {
        return Flow(RefElements(c));
    }
}
