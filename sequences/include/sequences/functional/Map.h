//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

#include <sequences/core/TypeTraits.h>
#include <sequences/core/ISequence.h>

namespace sequences
{
    /// Maps each sequence element through a function.
	/// Arity: 1 -> 1
    template<class Seq, class Fn>
    class Map : public ISequence<Map<Seq, Fn>>
    {
    public:

        static inline bool constexpr finite = Seq::finite;
        using domain_type = typename Seq::output_type;
        using output_type = function_return_type<Fn, domain_type>;

        static_assert(!std::is_lvalue_reference_v<output_type>, "The mapped type must be owned.");
        static_assert(!std::is_rvalue_reference_v<output_type>, "The mapped type must be owned.");

        Map(
            Seq const &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<output_type> next()
        {
            std::optional<typename Seq::output_type> k = base.next();
            if (k.has_value())
            {
                return fn(std::move(*k));
            }
            return {};
        }

    private:
        Seq base;
        Fn fn;
    };

    template<class Fn>
    auto map(Fn fn)
    {
        return [=](auto seq) {
            return Map{seq, fn};
        };
    }
}
