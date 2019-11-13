//
// Created by jim on 11/13/19.
//

#include <optional>

namespace sequences
{

    /// Interface for all sequences.
    template<class O>
    class Sequence
    {
    public:
        using output_type = O;

        virtual std::optional<O> next() = 0;
    };

    /// Yields all elements of the given container.
    template<class T>
    class Elements :
        public Sequence<T>
    {
        std::vector<T> const &xs;
        int k;
    public:

        explicit Elements(std::vector<T> const &xs) :
            xs{xs},
            k{0}
        {}

        std::optional<T> next() override
        {
            if (k >= xs.size())
            {
                return {};
            }
            T state = xs[k];
            ++k;
            return state;
        }
    };

    /// Successors based on some initial value.
    class Successors :
        public Sequence<int>
    {
        int n;
    public:

        Successors() :
            n{0}
        {}

        std::optional<int> next() override
        {
            if (n >= 10)
            {
                return {};
            }
            int state = n;
            ++n;
            return state;
        }
    };

    /// Maps each sequence element through a function.
    template<class Fn>
    class Map :
        public Sequence<int>
    {
        Sequence<int> &base;
        Fn fn;

    public:

        Map(
            Sequence<int> &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<int> next() override
        {
            auto k = base.next();
            if (k)
            {
                return fn(std::move(*k));
            }
            return {};
        }
    };

//    template<class F>
//    Map(Sequence<int> &, F) -> Map<F>;

    /// Yields only elements of the base sequence where the test function returns `true`.
    template<class Fn>
    class Filter :
        public Sequence<int>
    {
        Sequence<int> &base;
        Fn fn;

    public:

        Filter(
            Sequence<int> &base,
            Fn fn
        ) :
            base{base},
            fn{fn}
        {}

        std::optional<int> next() override
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
    class Take :
        public Sequence<int>
    {
        Sequence<int> &base;
        size_t k;
        size_t n;

    public:

        Take(
            Sequence<int> &base,
            size_t const n
        ) :
            base{base},
            k{0},
            n{n}
        {}

        std::optional<int> next() override
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
    class Flatten :
        public Sequence<int>
    {
        Sequence<Sequence<int>*> &base;
        std::optional<Sequence<int>*> current_sub_sequence;

    public:

        Flatten(
            Sequence<Sequence<int>*> &base
        ) :
            base{base},
            current_sub_sequence{base.next()}
        {}

        std::optional<int> next() override
        {
            // Try to get another element to return until there are no more elements.
            for (;;)
            {
                if (current_sub_sequence)
                {
                    // Try to get the next value out of the current sub sequence.
                    auto el = (*current_sub_sequence)->next();
                    if (el)
                    {
                        return std::move(el);
                    }
                    else
                    {
                        // Current sub sequence is exhausted, go to next.
                        current_sub_sequence = base.next();
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
    };

}
