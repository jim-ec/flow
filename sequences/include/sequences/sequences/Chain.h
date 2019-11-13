//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{
    /// First yields all elements from the first sequence,
    /// then yields elements from the second sequences.
    /// The first sequences should be finite.
    /// Elements from the continuation sequences must be convertible to the output
    /// type of the drain sequence.
    template<class DrainSeq, class ContSeq>
    class Chain
    {
    public:

        using output_type = typename DrainSeq::output_type;

        explicit Chain(
            DrainSeq const &drain_seq,
            ContSeq const &cont_seq
        ) :
            drain_seq{drain_seq},
            cont_seq{cont_seq},
            draining{true}
        {}

        std::optional<output_type> next()
        {
            if (draining) {
                // Try to get another element of the drain sequence.
                std::optional<output_type> element = drain_seq.next();
                if (element.has_value()) {
                    return std::move(element);
                }
                else {
                    // The draining sequence is exhausted.
                    // From here on, yield from the continuation sequence.
                    draining = false;
                }
            }

            return cont_seq.next();
        }

    private:
        DrainSeq drain_seq;
        ContSeq cont_seq;
        bool draining;
    };

}
