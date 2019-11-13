//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
// The function's return type must be default constructable and assignable.
template<class Iter, class Fn>
class Map
{
public:
    using value_type = decltype(std::declval<Fn>()(
        // This statically calls the function type with the iterator's
        // value type, to get the functions return type since
        // no `std::function` but raw template types are used.
        // This means that the iterator's value type must be convertible
        // to the function's argument type, like:
        // `T -> T` or `T -> const T&`, but not `T -> T&`.
        std::declval<iter_value_type_t<Iter>>()
    ));

private:
    Iter m_iter;
    Fn m_fn;
    mutable value_type m_cache{};

public:

    Map() = default;

    Map(const Map &rhs) = default;

    Map(Map &&rhs) noexcept = default;

    Map &operator=(const Map &rhs) = default;

    Map &operator=(Map &&rhs) noexcept = default;

    Map(
            const Iter &iter,
            Fn fn
    ) :
            m_iter{iter},
            m_fn{fn}
    {
    }

    value_type &operator*()
    {
        m_cache = m_fn(*m_iter);
        return m_cache;
    }

    const value_type &operator*() const
    {
        m_cache = m_fn(*m_iter);
        return m_cache;
    }

    Map &operator++()
    {
        ++m_iter;
        return *this;
    }

    Map operator+(const size_t offset) const
    {
        Map result{m_iter, m_fn};
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

template<class Iter, class Fn>
Map<Iter, Fn> make_map(
        const Iter &iter,
        Fn fn
)
{
    return Map<Iter, Fn>{iter, fn};
}

}
