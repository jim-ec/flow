//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class DiscreteRangeIterator
{
    Iter m_iter;
    size_t m_index;

public:

    using value_type = typename Iter::value_type;

    DiscreteRangeIterator(
            const Iter &iter,
            const size_t index
    ) :
            m_iter{iter},
            m_index{index}
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

    DiscreteRangeIterator &operator++()
    {
        ++m_iter;
        ++m_index;
        return *this;
    }

    DiscreteRangeIterator<Iter> operator+(const size_t offset) const
    {
        DiscreteRangeIterator<Iter> result{m_iter, m_index};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const DiscreteRangeIterator &rhs) const
    {
        return m_index == rhs.m_index || m_iter == rhs.m_iter;
    }

    bool operator!=(const DiscreteRangeIterator &rhs) const
    {
        return m_index != rhs.m_index && m_iter != rhs.m_iter;
    }
};

template<class Iter>
DiscreteRangeIterator<Iter> makeDiscreteSequence(
        const Iter &iter,
        const size_t index
)
{
    return DiscreteRangeIterator<Iter>{iter, index};
}
}
