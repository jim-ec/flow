//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
// The function's return type must be default constructable and assignable.
template<class Iter, class F>
class MapIterator
{
public:
    using value_type = decltype(std::declval<F>()(
            std::declval<iter_value_type_t<Iter>>()
    ));

private:
    Iter m_iter;
    F m_function;
    mutable value_type m_cache;

public:

    MapIterator() = default;

    MapIterator(const MapIterator &rhs) = default;

    MapIterator(MapIterator &&rhs) noexcept = default;

    MapIterator &operator=(const MapIterator &rhs) = default;

    MapIterator &operator=(MapIterator &&rhs) noexcept = default;

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

template<class Iter, class F>
MapIterator<Iter, F> make_map_iter(
        const Iter &iter,
        F f
)
{
    return MapIterator<Iter, F>{iter, f};
}

}
