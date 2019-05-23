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
    Iter m_current_end;
    F m_next_begin;
    G m_next_end;

public:

    FlattenIterator(
            F nextBegin,
            G nextEnd
    ) :
            m_iter{nextBegin()},
            m_current_end{nextEnd()},
            m_next_begin{nextBegin},
            m_next_end{nextEnd}
    {
    }

    FlattenIterator(
            const Iter &iter,
            const Iter &currentEnd,
            F nextBegin,
            G nextEnd
    ) :
            m_iter{iter},
            m_current_end{currentEnd},
            m_next_begin{nextBegin},
            m_next_end{nextEnd}
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
        if (m_iter == m_current_end)
        {
            m_iter = m_next_begin();
            m_current_end = m_next_end();
        }
        return *this;
    }

    FlattenIterator operator+(const size_t offset) const
    {
        FlattenIterator result{m_iter, m_current_end, m_next_begin, m_next_end};
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

}
