//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

namespace sequences
{
    /// Reduces the sequence depth by one.
	/// Arity: 1 -> 1
    template<class Seq>
    class Flatten
    {
    public:

        static inline bool constexpr finite = Seq::finite;
        using sub_sequence_type = typename Seq::output_type;
        using output_type = typename sub_sequence_type::output_type;

        explicit Flatten(
            Seq const &base
        ) :
            base{base},
            current_sub_sequence{this->base.next()}
        {}

        std::optional<output_type> next()
        {
            // Try to get another element to return until there are no more elements.
            for (;;)
            {
                if (current_sub_sequence)
                {
                    // Try to get the next value out of the current sub sequence.
                    std::optional<output_type> el = current_sub_sequence->next();
                    if (el.has_value())
                    {
                        return std::move(el);
                    }
                    else
                    {
                        // Current sub sequence is exhausted, go to next.
                        std::optional<sub_sequence_type> next = base.next();
                        if (next.has_value())
                        {
                            current_sub_sequence.emplace(std::move(*next));
                        }
                        else
                        {
                            current_sub_sequence.reset();
                        }
                    }
                }
                else
                {
                    // The base sequence is exhausted.
                    // There are ultimately no elements left.
                    return {};
                }
            }

        }

    private:
        Seq base;
        std::optional<sub_sequence_type> current_sub_sequence;
    };

}
