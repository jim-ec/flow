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

    using value_type = iter_value_type_t<Iter>;

    DiscreteRangeIterator() = default;
    DiscreteRangeIterator(const DiscreteRangeIterator &rhs) = default;
    DiscreteRangeIterator(DiscreteRangeIterator &&rhs) noexcept = default;

    DiscreteRangeIterator &operator=(const DiscreteRangeIterator &rhs) = default;
    DiscreteRangeIterator &operator=(DiscreteRangeIterator &&rhs) noexcept = default;

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

    DiscreteRangeIterator operator+(const size_t offset) const
    {
        DiscreteRangeIterator result{m_iter, m_index};
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
DiscreteRangeIterator<Iter> make_discrete_range_iter(
        const Iter &iter,
        const size_t index
)
{
    return DiscreteRangeIterator<Iter>{iter, index};
}
}
