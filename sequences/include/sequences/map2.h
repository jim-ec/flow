//
// Created by jim on 20.05.19.
//

#pragma once

#include "tools.h"

namespace sequences
{
template<class R, class Iter, class F>
class Map2Iterator
{
public:
    using value_type = R;
    using pair_type = typename Iter::value_type;
//				using pair_type_left = typename Iter::value_type_left;
//				using pair_type_right = typename Iter::value_type_right;

private:
    Iter m_iter;
    F m_function;
    mutable value_type m_cache;

public:

    Map2Iterator(
            const Iter &iter,
            F function
    ) :
            m_iter{iter},
            m_function{function}
    {
    }

    value_type &operator*()
    {
        const pair_type &pair = *m_iter;
        m_cache = m_function(*pair.m_first, *pair.m_second);
        return m_cache;
    }

    const value_type &operator*() const
    {
        pair_type &pair = *m_iter;
        m_cache = m_function(*pair.m_first, *pair.m_second);
        return m_cache;
    }

    Map2Iterator &operator++()
    {
        ++m_iter;
        return *this;
    }

    Map2Iterator operator+(const size_t offset) const
    {
        Map2Iterator result{m_iter, m_function};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const Map2Iterator &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const Map2Iterator &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class R, class Iter, class F>
Map2Iterator<R, Iter, F> makeMap2Sequence(
        const Iter &iter,
        F f
)
{
    return Map2Iterator<R, Iter, F>{iter, f};
}

}
