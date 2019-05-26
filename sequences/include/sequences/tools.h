//
// Created by jimec on 19.05.2019.
//

#pragma once

#include <limits>
#include <cstddef>
#include <utility>
#include <type_traits>

namespace sequences
{

/// Use this type to get an iterator-type's value-type.
template<class Iter>
struct iter_value_type
{
    using type = typename Iter::value_type;
};

/// The value type of an iterator which is a raw pointer is
/// simply the dereferenced type. E.g. the value type of
/// the iterator type `int *` is `int`.
template<class T>
struct iter_value_type<T *>
{
    using type = T;
};

/// Short cut for `typename iter_value_type<T>::type`.
template<class T>
using iter_value_type_t = typename iter_value_type<T>::type;

template<class T, class U>
struct Pair
{
    T m_first;
    U m_second;

    bool operator==(const Pair &rhs) const
    {
        return m_first == rhs.m_first && m_second == rhs.m_second;
    }

    template<class A, class B>
    bool operator==(const Pair<A &, B &> &rhs) const
    {
        return m_first == rhs.m_first && m_second == rhs.m_second;
    }

    template<class A, class B>
    bool operator==(const Pair<A *, B *> &rhs) const
    {
        return m_first == *rhs.m_first && m_second == *rhs.m_second;
    }

    template<class A, class B>
    Pair &operator=(const Pair<A, B> &rhs)
    {
        m_first = rhs.m_first;
        m_second = rhs.m_second;
        return *this;
    }

    template<class A, class B>
    Pair &operator=(const Pair<A *, B *> &rhs)
    {
        m_first = *rhs.m_first;
        m_second = *rhs.m_second;
        return *this;
    }

};

template<class T, class U>
Pair<T, U> make_pair(T a, U b)
{
    return Pair<T, U>{a, b};
}

}
