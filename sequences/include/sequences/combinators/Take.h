//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class Take
{
    Iter m_iter;
    size_t m_index{};

public:

    using value_type = iter_value_type_t<Iter>;

    Take() = default;
    Take(const Take &rhs) = default;
    Take(Take &&rhs) noexcept = default;

    Take &operator=(const Take &rhs) = default;
    Take &operator=(Take &&rhs) noexcept = default;

    Take(
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

    Take &operator++()
    {
        ++m_iter;
        ++m_index;
        return *this;
    }

    Take operator+(const size_t offset) const
    {
        Take result{m_iter, m_index};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const Take &rhs) const
    {
        return m_index == rhs.m_index || m_iter == rhs.m_iter;
    }

    bool operator!=(const Take &rhs) const
    {
        return m_index != rhs.m_index && m_iter != rhs.m_iter;
    }
};

template<class Iter>
Take<Iter> make_take(
        const Iter &iter,
        const size_t index
)
{
    return Take<Iter>{iter, index};
}
}
