//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class Index
{
public:
    using iter_value_type = iter_value_type_t<Iter>;
    using value_type = Pair<size_t, iter_value_type>;

//private:
    Iter m_iter;
    size_t m_index;
    value_type m_cache;

public:

    Index() = default;

    Index(const Index &rhs) = default;

    Index(Index &&rhs) noexcept = default;

    Index &operator=(const Index &rhs) = default;

    Index &operator=(Index &&rhs) noexcept = default;

    Index(const Iter &iter, const size_t index) :
            m_iter{iter},
            m_index{index},
            m_cache{}
    {
    }

    value_type &operator*()
    {
        m_cache = make_pair(m_index, *m_iter);
        return m_cache;
    }

    const value_type &operator*() const
    {
        m_cache = make_pair(m_index, *m_iter);
        return m_cache;
    }

    Index &operator++()
    {
        ++m_iter;
        ++m_index;
        return *this;
    }

    Index operator+(const size_t offset) const
    {
        Index result{m_iter, m_index};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const Index &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const Index &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter>
Index<Iter> make_index(const Iter &iter)
{
    return Index<Iter>{iter, 0};
}
}
