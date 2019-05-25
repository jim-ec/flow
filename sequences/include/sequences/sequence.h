//
// Created by jimec on 19.05.2019.
//

#pragma once

#include "sequences/combinators/map.h"
#include "sequences/combinators/map2.h"
#include "sequences/combinators/filter.h"
#include "sequences/combinators/zip.h"
#include "sequences/combinators/discrete_range.h"
#include "sequences/combinators/chain.h"
#include "sequences/combinators/inspect.h"
#include "sequences/combinators/flatten.h"

namespace sequences
{

template<class Iter>
class Sequence;

/// Creates a sequence from a collection defining a @a begin() and @a end() function.
template<class C>
Sequence<typename C::iterator> make_sequence(C &c);

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

    using value_type = typename Iter::value_type;

    Sequence() = default;
    Sequence(const Sequence &rhs) = default;
    Sequence(Sequence &&rhs) noexcept = default;

    Sequence &operator=(const Sequence &rhs) = default;
    Sequence &operator=(Sequence &&rhs) noexcept = default;

    Sequence(const Iter &begin, const Iter &end) :
            m_begin{begin},
            m_end{end}
    {}

    Sequence &skip(const size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_begin;
            if (m_begin == end())
            {
                break;
            }
        }
        return *this;
    }

    Sequence skipped(const size_t offset) const
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

    Sequence<DiscreteRangeIterator<Iter>> range(const size_t count) const
    {
        return make_sequence(
                make_discrete_range_iter(m_begin, 0),
                make_discrete_range_iter(m_end, count)
        );
    }

    size_t count() const
    {
        size_t result = 0;
        auto iter = begin();
        while (iter != end())
        {
            ++result;
            ++iter;
        }
        return result;
    }

    Sequence &close()
    {
        while (m_begin != end())
        {
            *m_begin;
            ++m_begin;
        }
        return *this;
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

    template<class IterRhs>
    Sequence<ZipIterator<Iter, IterRhs>> zip(
            const Sequence<IterRhs> &rhs
    ) const
    {
        return make_sequence(
                make_zip_iter(begin(), rhs.begin()),
                make_zip_iter(end(), rhs.end())
        );
    }

    template<class F>
    Sequence<MapIterator<Iter, F>> map(
            F function
    ) const
    {
        return make_sequence(
                make_map_iter(m_begin, function),
                make_map_iter(m_end, function)
        );
    }

    template<class R, class F>
    Sequence<Map2Iterator<R, Iter, F>> map2(
            F function
    ) const
    {
        return make_sequence(
                make_map2_iter<R>(begin(), function),
                make_map2_iter<R>(end(), function)
        );
    }

    template<class F>
    Sequence<InspectIterator<Iter, F>> inspect(F function) const
    {
        return make_sequence(
                make_inspect_iter(m_begin, function),
                make_inspect_iter(m_end, function)
        );
    }

    template<class IterRhs>
    Sequence<ChainIterator<Iter, IterRhs>> chain(
            Sequence<IterRhs> &rhs
    )
    {
        return make_sequence(
                make_chain_iter(begin(), end(), rhs.begin(), rhs.begin()),
                make_chain_iter(end(), end(), rhs.end(), rhs.begin())
        );
    }

    template<class F>
    Sequence<FilterIterator<Iter, F>> filter(
            F function
    ) const
    {
        return make_sequence(
                make_filter_iter(m_begin, m_end, function),
                make_filter_iter(m_end, m_end, function)
        );
    }

//				decltype(auto) flatten() const
//				{
//					return make_sequence(
//
//							);
//				}

    template<class F>
    void for_each(F function) const
    {
        for (const auto &el : *this)
        {
            function(el);
        }
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

    /// Returns the current first iteration of the sequence.
    /// Shrinks the sequence to right.
    /// This must not be called on an empty sequence.
    value_type next()
    {
        auto iter{begin()};
        ++m_begin;
        return *iter;
    }

    /// Returns the value of the first sequence element.
    /// This should never be called if the sequence is empty.
    typename Iter::value_type operator*()
    {
        return *begin();
    }

    bool empty()
    {
        return begin() == end();
    }

    template<class C>
    void emplace_to(C &c)
    {
        for (const value_type &el : *this)
        {
            c.emplace_back(el);
        }
    }

};

template<class Iter>
const Sequence<Iter> make_sequence(
        const Iter &begin,
        const Iter &end
)
{
    return Sequence<Iter>{begin, end};
}

template<class C>
Sequence<typename C::iterator> make_sequence(C &c)
{
    return make_sequence(c.begin(), c.end());
}

}
