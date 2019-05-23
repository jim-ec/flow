//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class R, class Iter, class F>
class MapIterator
{
public:
    using value_type = R;

private:
    Iter m_iter;
    F m_function;
    mutable R m_cache;

public:

    MapIterator(
            const Iter &iter,
            F function
    ) :
            m_iter{iter},
            m_function{function}
    {
    }

    value_type &operator*()
    {
        m_cache = m_function(*m_iter);
        return m_cache;
    }

    const value_type &operator*() const
    {
        m_cache = m_function(*m_iter);
        return m_cache;
    }

    MapIterator &operator++()
    {
        ++m_iter;
        return *this;
    }

    MapIterator operator+(const size_t offset) const
    {
        MapIterator result{m_iter, m_function};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const MapIterator &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const MapIterator &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class R, class Iter, class F>
MapIterator<R, Iter, F> makeMapSequence(
        const Iter &iter,
        F f
)
{
    return MapIterator<R, Iter, F>{iter, f};
}

}