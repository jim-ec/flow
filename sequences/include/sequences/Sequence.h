//
// Created by jim on 11/18/19.
//

#pragma once

#include <sequences/functional/Map.h>

namespace sequences
{
	template<class Seq>
	class Sequence
	{
	public:

	    using output_type = typename Seq::output_type;
	    static inline bool const finite = Seq::finite;

	    std::optional<output_type> next()
        {
	        return seq.next();
        }

		explicit Sequence(
			Seq const &seq
		) :
			seq{seq}
		{}

		/// The sequence composition.
		/// Takes a sequence constructor, i.e. a function which expects this sequence and returns a new sequence
		/// based on it. The resulting sequence is returned.
		template<class Ctor>
		auto operator|(Ctor ctor) &&
		{
	        using SeqType = Sequence<function_return_type<Ctor, Seq>>;
			return SeqType{
                    ctor(std::move(seq))
			};
		}

	private:
		Seq seq;
	};
}
