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
Sequence<typename C::iterator> makeSequence(C &c);

template<class Iter>
const Sequence<Iter> makeSequence(
        const Iter &begin,
        const Iter &end
);

/// yet to implement:
/// - sequence over array
/// - move
/// - flatten / flat_map
/// - inner_product
template<class Iter>
class Sequence
{

    Iter m_begin;
    Iter m_end;

public:

    using value_type = typename Iter::value_type;

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
                return makeSequence(end(), end());
            }
        }

        return makeSequence(iter, end());
    }

    Sequence<DiscreteRangeIterator<Iter>> range(const size_t count) const
    {
        return makeSequence(
                makeDiscreteSequence(m_begin, 0),
                makeDiscreteSequence(m_end, count)
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
    value_type lastElement() const
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
        return makeSequence(
                makeZipSequence(begin(), rhs.begin()),
                makeZipSequence(end(), rhs.end())
        );
    }

    template<class R, class F>
    Sequence<MapIterator<R, Iter, F>> map(
            F function
    ) const
    {
        return makeSequence(
                makeMapSequence<R>(m_begin, function),
                makeMapSequence<R>(m_end, function)
        );
    }

    template<class R, class F>
    Sequence<Map2Iterator<R, Iter, F>> map2(
            F function
    ) const
    {
        return makeSequence(
                makeMap2Sequence<R>(begin(), function),
                makeMap2Sequence<R>(end(), function)
        );
    }

    template<class F>
    Sequence<InspectIterator<Iter, F>> inspect(F function) const
    {
        return makeSequence(
                makeInspectSequence(m_begin, function),
                makeInspectSequence(m_end, function)
        );
    }

    template<class IterRhs>
    Sequence<ChainIterator<Iter, IterRhs>> chain(
            Sequence<IterRhs> &rhs
    )
    {
        return makeSequence(
                makeChainSequence(begin(), end(), rhs.begin()),
                makeChainSequence(end(), end(), rhs.end(), rhs.begin())
        );
    }

    template<class F>
    Sequence<FilterIterator<Iter, F>> filter(
            F function
    ) const
    {
        return makeSequence(
                makeFilterSequence(m_begin, m_end, function),
                makeFilterSequence(m_end, m_end, function)
        );
    }

//				decltype(auto) flatten() const
//				{
//					return makeSequence(
//
//							);
//				}

    template<class F>
    void forEach(F function) const
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
    void emplaceTo(C &c)
    {
        for (const value_type &el : *this)
        {
            c.emplace_back(el);
        }
    }

};

template<class Iter>
const Sequence<Iter> makeSequence(
        const Iter &begin,
        const Iter &end
)
{
    return Sequence<Iter>{begin, end};
}

template<class C>
Sequence<typename C::iterator> makeSequence(C &c)
{
    return makeSequence(c.begin(), c.end());
}

}
