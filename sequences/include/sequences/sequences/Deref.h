//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

#include "TypeTraits.h"

namespace sequences
{
    /// Dereferences the values behind element pointers.
    template<class Seq>
    class Deref
    {
    public:
        static_assert(std::is_pointer_v<typename Seq::output_type>,
            "Deref sequence expects underlying elements to be pointers.");

        static inline bool constexpr finite = Seq::finite;
        using output_type = std::remove_pointer_t<typename Seq::output_type>;

        explicit Deref(Seq const &base) :
            base{base}
        {}

        std::optional<output_type> next()
        {
            std::optional<typename Seq::output_type> state = base.next();
            if (state.has_value())
            {
                return {*state.value()};
            }
            else
            {
                return {};
            }
        }

    private:
        Seq base;
    };
}
