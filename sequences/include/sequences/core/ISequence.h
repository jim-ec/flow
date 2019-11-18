//
// Created by jim on 11/18/19.
//

#pragma once

#include <sequences/core/TypeTraits.h>

namespace sequences
{
	template<class Self>
	struct ISequence
	{
//        static_assert(std::is_base_of_v<ISequence<Self>, Self>, "The sequence's self type must be a subtype.");
//        static_assert(std::is_same_v<ISequence, ISequence<Self>>);
//        static_assert(std::is_constructible_v<ISequence *, Self *>);

		/// The sequence composition.
		/// Takes a sequence constructor, i.e. a function which expects this sequence and returns a new sequence
		/// based on it. The resulting sequence is returned.
		template<class Ctor>
		auto operator|(Ctor ctor) const&
		{
	        using SeqType = function_return_type<Ctor, Self>;
			return SeqType{
                    ctor(*this)
			};
		}

        template<class Ctor>
        auto operator|(Ctor ctor) &&
        {
            using SeqType = function_return_type<Ctor, Self>;
            return SeqType{
                    ctor(std::move(static_cast<Self &>(*this)))
            };
        }
	};
}
