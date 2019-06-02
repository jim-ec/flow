//
// Created by jimec on 19.05.2019.
//

#pragma once

#include "sequences/combinators/Map.h"
#include "sequences/combinators/MapPair.h"
#include "sequences/combinators/Filter.h"
#include "sequences/combinators/Zip.h"
#include "sequences/combinators/Take.h"
#include "sequences/combinators/Chain.h"
#include "sequences/combinators/Inspect.h"
#include "sequences/combinators/InspectPair.h"
#include "sequences/combinators/Flatten.h"
#include "sequences/combinators/ByPtr.h"
#include "sequences/combinators/Stride.h"

#include "Mutation.h"

namespace sequences
{

template<class Iter>
class Sequence;

/// Creates a sequence from a collection defining a @a begin() and @a end() function.
template<class C>
Sequence<typename C::iterator> make_sequence(C &c);

template<class T, class Fn>
Sequence<Mutation<T, Fn>> make_mutation(const T &init, Fn fn);

template<class T = int>
Sequence<Mutation<T, impl::LinearMutation<T>>> make_mutation_linear(const T &init = T{0}, const T &step = T{1});

template<class Iter>
const Sequence<Iter> make_sequence(
        const Iter &begin,
        const Iter &end
);

template<class Iter>
class Sequence
{

    Iter m_begin;
    Iter m_end;

public:

    using iter_type = Iter;
    using value_type = iter_value_type_t<Iter>;

    Sequence() = default;

    Sequence(const Sequence &rhs) = default;

    Sequence(Sequence &&rhs) noexcept = default;

    Sequence &operator=(const Sequence &rhs) = default;

    Sequence &operator=(Sequence &&rhs) noexcept = default;

    Sequence(const Iter &begin, const Iter &end) :
            m_begin{begin},
            m_end{end}
    {}

    /// Creates an iterator that skips the first n elements.
    Sequence
    skip(const size_t offset) const
    {
        auto iter = begin();

        for (size_t i = 0; i < offset; ++i)
        {
            ++iter;
            if (iter == end())
            {
                return make_sequence(end(), end());
            }
        }

        return make_sequence(iter, end());
    }

    /// Creates a new sequence skipping the first elements as long as the given
    /// predicate returns `true`.
    /// If the sequence gets empty while skipping, an empty sequence is returned.
    template<class Fn>
    Sequence
    skip_while(Fn fn) const
    {
        auto iter = begin();

        for (; iter != m_end; ++iter)
        {
            if(!fn(*iter))
            {
                break;
            }
        }

        return make_sequence(iter, end());
    }

    /// Creates an iterator that yields its first n elements.
    ///
    /// Note: Sequence combinators are usually not commutative which may sometimes lead to
    /// surprising results especially when using `take()` in conjunction with a combinator
    /// altering the sequence's number of elements.
    ///
    /// ```
    /// auto is_even = [] (int n) { return n % 2 == 0; };
    ///
    /// make_mutation_linear().take(5).filter(is_even); // { 0 2 4 }, take the even numbers out of the first five numbers.
    /// make_mutation_linear().filter(is_even).take(5); // { 0 2 4 6 8 }, take the first five even numbers.
    /// ```
    Sequence<Take<Iter>>
    take(const size_t count) const
    {
        return make_sequence(
                make_take(m_begin, 0),
                make_take(m_end, count)
        );
    }

    /// Consumes the iterator, counting the number of iterations and returning it.
    size_t
    count()
    {
        size_t count = 0;
        while (m_begin != m_end)
        {
            ++count;
            ++m_begin;
        }
        return count;
    }

    const value_type &operator[](const size_t index) const
    {
        return *(m_begin + index);
    }

    Iter &begin()
    {
        return m_begin;
    }

    Iter &end()
    {
        return m_end;
    }

    const Iter &begin() const
    {
        return m_begin;
    }

    const Iter &end() const
    {
        return m_end;
    }

    template<class T>
    bool contains(const T &rhs) const
    {
        for (const T &el : *this)
        {
            if (el == rhs)
            {
                return true;
            }
        }
        return false;
    }

    /// Returns the last element.
    /// Must not be called if the sequence is empty.
    value_type last_element() const
    {
        auto iter = begin();
        while (iter + 1 != end())
        {
            iter++;
        }
        return *iter;
    }

    Sequence<ByPtr<Iter>>
    by_ptr() const
    {
        return make_sequence(
                make_by_ptr(begin()),
                make_by_ptr(end())
        );
    }

    template<class IterRhs>
    Sequence<Zip<Iter, IterRhs>>
    zip(const Sequence<IterRhs> &rhs) const
    {
        return make_sequence(
                make_zip(begin(), rhs.begin()),
                make_zip(end(), rhs.end())
        );
    }

    template<class Fn>
    Sequence<Map<Iter, Fn>>
    map(Fn fn) const
    {
        return make_sequence(
                make_map(m_begin, fn),
                make_map(m_end, fn)
        );
    }

    template<class Fn>
    Sequence<MapPair<Iter, Fn>>
    map_pair(Fn fn) const
    {
        return make_sequence(
                make_map_pair(begin(), fn),
                make_map_pair(end(), fn)
        );
    }

    template<class Fn>
    Sequence<Inspect<Iter, Fn>>
    inspect(Fn fn) const
    {
        return make_sequence(
                make_inspect(m_begin, fn),
                make_inspect(m_end, fn)
        );
    }

    template<class Fn>
    Sequence<InspectPair<Iter, Fn>>
    inspect_pair(Fn fn) const
    {
        return make_sequence(
                make_inspect_pair(m_begin, fn),
                make_inspect_pair(m_end, fn)
        );
    }

    template<class IterRhs>
    Sequence<Chain<Iter, IterRhs>>
    chain(Sequence<IterRhs> &rhs)
    {
        return make_sequence(
                make_chain(begin(), end(), rhs.begin(), rhs.begin()),
                make_chain(end(), end(), rhs.end(), rhs.begin())
        );
    }

    template<class Fn>
    Sequence<Filter<Iter, Fn>>
    filter(Fn fn) const
    {
        return make_sequence(
                make_filter(m_begin, m_end, fn),
                make_filter(m_end, m_end, fn)
        );
    }

    // The current Iter's value_type must be an iterator itself.
    Sequence<Flatten<Iter>>
    flatten() const
    {
        using ChildIter = const typename Flatten<Iter>::child_iter_type;
        return make_sequence(
                make_flatten(begin(), end(), (*begin()).begin(), (*begin()).end()),
                make_flatten(end(), end(), ChildIter{}, ChildIter{})
        );
    }

    template<class Fn>
    Sequence<Flatten<Map<Iter, Fn>>>
    flat_map(Fn fn) const
    {
        return map(fn).flatten();
    }

    Sequence<Stride<Iter>>
    stride(const size_t stride) const
    {
        return make_sequence(
                make_stride(m_begin, m_end, stride),
                make_stride(m_end, m_end, stride)
        );
    }

    /// Partitions the sequence into two collections.
    /// The first collection contains all elements where the predicate function
    /// return true, the second collection contains the rest.
    /// Different types for both collections are possible, but each type must be default-constructable and have
    /// an `push_back()` method. The value type of both collections must be the same as the sequence's value type.
    ///
    /// Sample usage: `auto v = seq.partition<std::vector<int>, std::list<int>>([] (int n) { return n % 2 == 0; });`
    template<class CollectionTrue, class CollectionFalse = CollectionTrue, class Fn>
    std::pair<CollectionTrue, CollectionFalse>
    partition(Fn fn)
    {
        std::pair<CollectionTrue, CollectionFalse> result;

        for (; m_begin != m_end; ++m_begin)
        {
            value_type &el = *m_begin;
            if (fn(el))
            {
                result.first.push_back(el);
            }
            else
            {
                result.second.push_back(el);
            }
        }

        return result;
    }

    /// Calls a function on each element, consuming the iterator.
    template<class Fn>
    void for_each(Fn fn)
    {
        for (; m_begin != m_end; ++m_begin)
        {
            fn(*m_begin);
        }
    }

    /// Enumerates all elements by mapping them to a pair, where the first entry
    /// is the index and the second entry the actual element.
    template<class Idx = size_t>
    Sequence<Zip<Mutation<Idx, impl::LinearMutation<Idx>>, Iter>>
    enumerate() const
    {
        return make_mutation_linear<Idx>().zip(*this);
    }

    /// Folds the sequence, using a accumulator function.
    /// The function should be of the type: (R, T) -> R.
    template<class R = value_type, class Fn>
    R fold(Fn fn, const R &init = R{}) const
    {
        R acc = init;
        for (const auto &el : *this)
        {
            acc = fn(acc, el);
        }
        return acc;
    }

    /// Sums all the elements.
    /// The following must be defined: `+: (R, T) -> R`
    template<class R = value_type>
    R sum(const R &init = R{}) const
    {
        R acc = init;
        for (const auto &el : *this)
        {
            acc = acc + el;
        }
        return acc;
    }

    /// Multiplies all the elements.
    /// The following must be defined: `*: (R, T) -> R`
    template<class R = value_type>
    R product(const R &init = R{1}) const
    {
        R acc = init;
        for (const auto &el : *this)
        {
            acc = acc * el;
        }
        return acc;
    }

    template<class Mul = value_type, class Sum = Mul, class IterRhs, class FnMul, class FnSum>
    Sum
    inner_product(
            const Sequence<IterRhs> &rhs,
            FnMul fnMul,
            FnSum fnSum,
            Mul init = Mul{}
    )
    {
        return zip(rhs)
                .map_pair(fnMul)
                .template fold<Sum>(fnSum, init);
    }

    template<class Mul = value_type, class Sum = Mul, class IterRhs>
    Sum
    inner_product(
            const Sequence<IterRhs> &rhs,
            Mul init = Mul{}
    )
    {
        return inner_product(rhs, [](int a, int b) { return a * b; }, [](int a, int b) { return a + b; }, init);
    }

    /// Two sequences are considered equal if they contain the same elements
    /// in the same order, where two elements are equal if there is a defined
    /// == operator of this value type over the other value type, and this
    /// operator returns true for all elements with same positions.
    ///
    /// The other sequence can be of a different iterator type,
    /// as long as this value type defines a matching == operator.
    template<class IterRhs>
    bool operator==(const Sequence<IterRhs> &rhs)
    {
        auto iter1 = begin();
        auto iter2 = rhs.begin();
        auto end1 = end();
        auto end2 = rhs.end();

        for (;;)
        {
            if (iter1 == end1 && iter2 != end2)
            {
                // This sequence has ended but the other one not.
                return false;
            }
            else if (iter1 != end1 && iter2 == end2)
            {
                // The other sequence has ended but this one not.
                return false;
            }
            else if (iter1 == end1 && iter2 == end2)
            {
                // The end is reached.
                return true;
            }
            else if (*iter1 != *iter2)
            {
                // The contents differ at the current position.
                return false;
            }

            ++iter1;
            ++iter2;
        }
    }

    /// Shrinks the sequence from left by one element, which is returned.
    /// This must not be called on an empty sequence.
    value_type
    next()
    {
        auto iter{begin()};
        ++m_begin;
        return *iter;
    }

    /// Returns the value of the first sequence element.
    /// This should never be called if the sequence is empty.
    value_type &
    value()
    {
        return *begin();
    }

    /// Returns the value of the first sequence element.
    /// This should never be called if the sequence is empty.
    const value_type &
    value() const
    {
        return *begin();
    }

    /// Returns the value of the first sequence element or [fallback]
    /// if the sequence is empty.
    value_type &
    value_or(value_type &fallback)
    {
        if (m_begin != m_end)
        {
            return *begin();
        }
        else
        {
            return fallback;
        }
    }

    /// Returns the value of the first sequence element or [fallback]
    /// if the sequence is empty.
    const value_type &
    value_or(const value_type &fallback) const
    {
        if (m_begin != m_end)
        {
            return *begin();
        }
        else
        {
            return fallback;
        }
    }

    /// Returns true if the sequence is empty.
    bool empty()
    {
        return begin() == end();
    }

    template<class C>
    void push_back_to(C &c)
    {
        for (const value_type &el : *this)
        {
            c.push_back(el);
        }
    }

};

template<class Iter>
const Sequence<Iter>
make_sequence(const Iter &begin, const Iter &end)
{
    return Sequence<Iter>{begin, end};
}

template<class C>
Sequence<typename C::iterator>
make_sequence(C &c)
{
    return make_sequence(c.begin(), c.end());
}

template<class T, size_t size>
Sequence<T *>
make_sequence(T(&array)[size])
{
    return make_sequence(&array[0], &array[size]);
}

template<class T, class Fn>
Sequence<Mutation<T, Fn>>
make_mutation(const T &init, Fn fn)
{
    Mutation<T, Fn> mutation{init, fn};
    return make_sequence(mutation, mutation);
}

template<class T>
Sequence<Mutation<T, impl::LinearMutation<T>>>
make_mutation_linear(const T &init, const T &step)
{
    return make_mutation(init, impl::LinearMutation<T>{step});
}

}
