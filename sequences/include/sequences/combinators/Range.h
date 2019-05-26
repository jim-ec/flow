//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class Range
{
    Iter m_iter;
    size_t m_index;

public:

    using value_type = iter_value_type_t<Iter>;

    Range() = default;
    Range(const Range &rhs) = default;
    Range(Range &&rhs) noexcept = default;

    Range &operator=(const Range &rhs) = default;
    Range &operator=(Range &&rhs) noexcept = default;

    Range(
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

    Range &operator++()
    {
        ++m_iter;
        ++m_index;
        return *this;
    }

    Range operator+(const size_t offset) const
    {
        Range result{m_iter, m_index};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const Range &rhs) const
    {
        return m_index == rhs.m_index || m_iter == rhs.m_iter;
    }

    bool operator!=(const Range &rhs) const
    {
        return m_index != rhs.m_index && m_iter != rhs.m_iter;
    }
};

template<class Iter>
Range<Iter> make_range(
        const Iter &iter,
        const size_t index
)
{
    return Range<Iter>{iter, index};
}
}
