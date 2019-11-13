//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{
    /// Yields all elements of the given container.
    template<class O>
    class Elements
    {
        std::vector<O> const &xs;
        int k;
    public:
        using output_type = O;

        explicit Elements(std::vector<O> const &xs) :
            xs{xs},
            k{0}
        {}

        std::optional<O> next()
        {
            if (k >= xs.size())
            {
                return {};
            }
            O state = xs[k];
            ++k;
            return state;
        }
    };

    /// Successors based on some initial value.
    template<class T>
    class Successors
    {
        T n;
    public:
        using output_type = int;

        Successors() :
            n{}
        {}

        std::optional<T> next()
        {
            T state = n;
            ++n;
            return std::move(state);
        }
    };

    /// Maps each sequence element through a function.
    template<class Seq, class Fn>
    class Map
    {
        Seq &base;
        Fn fn;

    public:

        using output_type = typename Seq::output_type;

        Map(
            Seq &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<output_type> next()
        {
            auto k = base.next();
            if (k)
            {
                return fn(std::move(*k));
            }
            return {};
        }
    };

    /// Yields only elements of the base sequence where the test function returns `true`.
    template<class Seq, class Fn>
    class Filter
    {
        Seq &base;
        Fn fn;

    public:

        using output_type = typename Seq::output_type;

        Filter(
            Seq &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<output_type> next()
        {
            for (;;)
            {
                auto k = base.next();
                if (!k)
                {
                    return {};
                }
                if (fn(*k))
                {
                    return std::move(k);
                }
            }
        }
    };

    /// Yields up to a fixed amount of elements out of a base sequence.
    template<class Seq>
    class Take
    {
        Seq &base;
        size_t k;
        size_t n;

    public:

        using output_type = typename Seq::output_type;

        Take(
            Seq &base,
            size_t const n
        ) :
            base{base},
            k{0},
            n{n}
        {}

        std::optional<output_type> next()
        {
            if (k < n)
            {
                ++k;
                return base.next();
            }
            else
            {
                return {};
            }
        }
    };

    /// Reduces the sequence order by one.
    template<class Seq>
    class Flatten
    {

    public:
        using sub_sequence_type = typename Seq::output_type;
        using output_type = typename sub_sequence_type::output_type;

        explicit Flatten(
            Seq &base
        ) :
            base{base},
            current_sub_sequence{base.next()}
        {}

        std::optional<output_type> next()
        {
            // Try to get another element to return until there are no more elements.
            for (;;)
            {
                if (current_sub_sequence)
                {
                    // Try to get the next value out of the current sub sequence.
                    auto el = current_sub_sequence->next();
                    if (el)
                    {
                        return std::move(el);
                    }
                    else
                    {
                        // Current sub sequence is exhausted, go to next.
                        std::optional<sub_sequence_type> next = base.next();
                        if (next)
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
        Seq &base;
        std::optional<sub_sequence_type> current_sub_sequence;
    };

}
