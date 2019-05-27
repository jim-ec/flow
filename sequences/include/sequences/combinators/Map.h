//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
// The function's return type must be default constructable and assignable.
template<class Iter, class F>
class Map
{
public:
    using value_type = decltype(std::declval<F>()(
            std::declval<iter_value_type_t<Iter>>()
    ));

private:
    Iter m_iter;
    F m_function;
    mutable value_type m_cache{};

public:

    Map() = default;

    Map(const Map &rhs) = default;

    Map(Map &&rhs) noexcept = default;

    Map &operator=(const Map &rhs) = default;

    Map &operator=(Map &&rhs) noexcept = default;

    Map(
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

    Map &operator++()
    {
        ++m_iter;
        return *this;
    }

    Map operator+(const size_t offset) const
    {
        Map result{m_iter, m_function};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const Map &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const Map &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter, class F>
Map<Iter, F> make_map(
        const Iter &iter,
        F f
)
{
    return Map<Iter, F>{iter, f};
}

}
