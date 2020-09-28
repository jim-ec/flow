#pragma once

#include <optional>
#include <initializer_list>

#include <flow/Flow.h>

namespace flow
{
    /// Yields all elements by reference of the given container.
    /// Because references cannot be used with `std::optional`, pointers are used instead.
    /// As the pointer are not `const`, it is possible to mutate the underlying container values.
    /// The container is not owned by this sequence.
    /// When returning an `RefElements` sequence from a scope i.e. exceeding its lifetime,
    /// the returned elements are dangled references and contain undefined values.
    /// Arity: 0 -> 1
    template<class C>
    class Reference
    {
    public:
        static inline bool constexpr finite = true;
        using output_type = typename C::value_type *;
        using IteratorType = typename C::iterator;

        Reference(Reference const &rhs) = default;

        Reference(Reference &&rhs) noexcept = default;

        explicit Reference(C &xs):
            xs(xs),
            iterator(this->xs.begin()),
            end(this->xs.end())
        {
        }

        /// Since this sequence type does not own the underlying container,
        /// it cannot take ownership of it.
        explicit Reference(C &&xs) = delete;

        std::optional<output_type> next()
        {
            if (iterator != end)
            {
                output_type const el_ptr(&*iterator);
                ++iterator;
                return el_ptr;
            }
            else
            {
                return {};
            }
        }

    private:
        C &xs;
        IteratorType iterator;
        IteratorType end;
    };

    template<class C>
    auto reference(C &c)
    {
        return Flow(Reference(c));
    }
}
