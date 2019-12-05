//
// Created by jim on 11/13/19.
//

#pragma once

#include <optional>

#include <flow/core/TypeTraits.h>

namespace flow {

    /// Zips `n` flow into one flow of `n`-tuples.
    /// Arity: n -> 1
    template<class... Seqs>
    class Merge {
        template<class Seq>
        using to_seq_output_tuple_type = std::optional<typename Seq::output_type>;

        template<class T>
        using id_type = T;

        template<class T>
        using to_output_type = typename T::output_type;

        /// A tuple of the sequence types.
        using seqs_tuple_type = std::tuple<Seqs...>;

        /// A tuple of optionals for each sequence, containing their output type.
        using seqs_output_tuple_type = tuple_type_map<to_seq_output_tuple_type, seqs_tuple_type>;

        template<size_t N = 0>
        constexpr static bool
        any_finite() {
            return std::tuple_element_t<N, seqs_tuple_type>::finite ||
                   any_finite<N + 1>();
        }

        template<>
        constexpr static bool
        any_finite<std::tuple_size_v<seqs_tuple_type>>() {
            return false;
        }

    public:

        static inline bool constexpr finite = any_finite();

        /// A tuple of the output type for each sequence.
        using output_type = tuple_type_map<to_output_type, seqs_tuple_type>;

        explicit Merge(seqs_tuple_type const &bases) :
                bases(bases) {}

        std::optional<output_type> next() {
            // Create tuple containing the next element for each sequence.
            // Then check is any entry is none, if so, this sequence is exhausted.
            // Otherwise, return the tuple.

            seqs_output_tuple_type base_outputs;
            init_tuple(base_outputs, bases);

            if (any_none(base_outputs)) {
                return {};
            }
            else {
                output_type outputs;
                move_to_result_output(outputs, base_outputs);
                return outputs;
            }
        }

    private:
        template<size_t N = 0>
        void
        init_tuple(
                seqs_output_tuple_type &outputs,
                seqs_tuple_type &seqs
        ) {
            // TODO: Use in-place dtor/ctor instead of assignment
            std::get<N>(outputs) = std::get<N>(seqs).next();
            init_tuple<N + 1>(outputs, seqs);
        }

        template<>
        void
        init_tuple<std::tuple_size_v<seqs_tuple_type>>(
                seqs_output_tuple_type &,
                seqs_tuple_type &
        ) {
        }

        template<size_t N = 0>
        bool
        any_none(
                seqs_output_tuple_type &outputs
        ) {
            if (!std::get<N>(outputs).has_value()) {
                return true;
            }
            else {
                return any_none<N + 1>(outputs);
            }
        }

        template<>
        bool
        any_none<std::tuple_size_v<seqs_tuple_type>>(
                seqs_output_tuple_type &
        ) {
            return false;
        }

        template<size_t N = 0>
        void
        move_to_result_output(
                output_type &outputs,
                seqs_output_tuple_type &base_outputs
        ) {
            std::get<N>(outputs) = *std::get<N>(base_outputs);
            move_to_result_output<N + 1>(outputs, base_outputs);
        }

        template<>
        void
        move_to_result_output<std::tuple_size_v<seqs_tuple_type>>(
                output_type &,
                seqs_output_tuple_type &
        ) {
        }

    private:
        seqs_tuple_type bases;
    };

    template<class... Seqs>
    auto merge(Seqs... seqs) {
        return [=](auto const &seq) {
            return Merge(std::make_tuple(seq, seqs...));
        };
    }
}
