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
class ChainIterator
{
    IterFirst m_first_iter;
    IterFirst m_first_end;
    IterSecond m_second_iter;
    IterSecond m_second_begin;

public:

    using value_type = typename IterFirst::value_type;

    ChainIterator(
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

    bool left_iter_is_valid() const
    {
        return m_first_iter != m_first_end;
    }

    value_type &operator*()
    {
        if (left_iter_is_valid())
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
        if (left_iter_is_valid())
        {
            return *m_first_iter;
        }
        else
        {
            return *m_second_iter;
        }
    }

    ChainIterator &operator++()
    {
        if (left_iter_is_valid())
        {
            ++m_first_iter;
        }
        else
        {
            ++m_second_iter;
        }
        return *this;
    }

    ChainIterator operator+(const size_t offset) const
    {
        ChainIterator result{m_first_iter, m_first_end, m_second_iter, m_second_begin};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const ChainIterator &rhs) const
    {
        return m_first_iter == rhs.m_first_iter && m_second_iter == rhs.m_second_iter;
    }

    bool operator!=(const ChainIterator &rhs) const
    {
        return !(*this == rhs);
    }
};

template<class IterLeft, class IterRight>
ChainIterator<IterLeft, IterRight> make_chain_iter(
        const IterLeft &first_iter,
        const IterLeft &first_end,
        const IterRight &second_iter,
        const IterRight &second_begin
)
{
    return ChainIterator<IterLeft, IterRight>{first_iter, first_end, second_iter, second_begin};
}

}
