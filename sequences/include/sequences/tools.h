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
