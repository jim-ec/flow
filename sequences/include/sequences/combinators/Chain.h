//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{

/// An iterator first iterating over all elements yielded by the first iterator,
/// and then iterating over all elements yielded by the second iterator.
/// The first and second iterator types need not to be same, but the yielded element type must be the same.
template<class IterFirst, class IterSecond>
class Chain
{
    IterFirst m_first_iter;
    IterFirst m_first_end;
    IterSecond m_second_iter;
    IterSecond m_second_begin;

public:

    using value_type = iter_value_type_t<IterFirst>;

    Chain() = default;
    Chain(const Chain &rhs) = default;
    Chain(Chain &&rhs) noexcept = default;

    Chain &operator=(const Chain &rhs) = default;
    Chain &operator=(Chain &&rhs) noexcept = default;

    Chain(
            const IterFirst &first_iter,
            const IterFirst &first_end,
            const IterSecond &second_iter,
            const IterSecond &second_begin
    ) :
            m_first_iter{first_iter},
            m_first_end{first_end},
            m_second_iter{second_iter},
            m_second_begin{second_begin}
    {
    }

    bool first_iter_is_valid() const
    {
        return m_first_iter != m_first_end;
    }

    value_type &operator*()
    {
        if (first_iter_is_valid())
        {
            return *m_first_iter;
        }
        else
        {
            return *m_second_iter;
        }
    }

    const value_type &operator*() const
    {
        if (first_iter_is_valid())
        {
            return *m_first_iter;
        }
        else
        {
            return *m_second_iter;
        }
    }

    Chain &operator++()
    {
        if (first_iter_is_valid())
        {
            ++m_first_iter;
        }
        else
        {
            ++m_second_iter;
        }
        return *this;
    }

    Chain operator+(const size_t offset) const
    {
        Chain result{m_first_iter, m_first_end, m_second_iter, m_second_begin};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const Chain &rhs) const
    {
        return m_first_iter == rhs.m_first_iter && m_second_iter == rhs.m_second_iter;
    }

    bool operator!=(const Chain &rhs) const
    {
        return !(*this == rhs);
    }
};

template<class IterLeft, class IterRight>
Chain<IterLeft, IterRight> make_chain(
        const IterLeft &first_iter,
        const IterLeft &first_end,
        const IterRight &second_iter,
        const IterRight &second_begin
)
{
    return Chain<IterLeft, IterRight>{first_iter, first_end, second_iter, second_begin};
}

}
