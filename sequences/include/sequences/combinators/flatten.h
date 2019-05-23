//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{

/// F: Generates next iterator
template<class F, class G, class Iter>
class FlattenIterator
{
public:
    using value_type = typename Iter::value_type; // This is also an iterator type

private:

    Iter m_iter; // while flattening, this does not get ended
    Iter m_currentEnd;
    F m_nextBegin;
    G m_nextEnd;

public:

    FlattenIterator(
            F nextBegin,
            G nextEnd
    ) :
            m_iter{nextBegin()},
            m_currentEnd{nextEnd()},
            m_nextBegin{nextBegin},
            m_nextEnd{nextEnd}
    {
    }

    FlattenIterator(
            const Iter &iter,
            const Iter &currentEnd,
            F nextBegin,
            G nextEnd
    ) :
            m_iter{iter},
            m_currentEnd{currentEnd},
            m_nextBegin{nextBegin},
            m_nextEnd{nextEnd}
    {
    }

    value_type &operator*()
    {
        return *m_iter;
    }

    const value_type &operator*() const
    {
        return *m_iter;
    }

    FlattenIterator &operator++()
    {
        ++m_iter;
        if (m_iter == m_currentEnd)
        {
            m_iter = m_nextBegin();
            m_currentEnd = m_nextEnd();
        }
        return *this;
    }

    FlattenIterator operator+(const size_t offset) const
    {
        FlattenIterator result{m_iter, m_currentEnd, m_nextBegin, m_nextEnd};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const FlattenIterator &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const FlattenIterator &rhs) const
    {
        return !(*this == rhs);
    }
};

template<class F, class G, class Iter>
FlattenIterator<F, G, Iter>
makeChainSequence(
        F nextBegin,
        G nextEnd
)
{
    return FlattenIterator<F, G, Iter>{nextBegin, nextEnd};
}

template<class F, class G, class Iter>
FlattenIterator<F, G, Iter>
makeChainSequence(
        const Iter &iter,
        const Iter &currentEnd,
        F nextBegin,
        G nextEnd
)
{
    return FlattenIterator<F, G, Iter>{iter, currentEnd, nextBegin, nextEnd};
}

}

