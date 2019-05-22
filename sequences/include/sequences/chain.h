//
// Created by jim on 20.05.19.
//

#pragma once

#include "tools.h"

namespace sequences
{

/// An iterator first iterating over all elements yielded by the first iterator,
/// and then iterating over all elements yielded by the second iterator.
/// The first and second iterator types need not to be same, but the yielded element type must be the same.
template<class IterFirst, class IterSecond>
class ChainIterator
{
    IterFirst m_first;
    IterFirst m_end;
    IterSecond m_second;
    IterSecond m_begin;

public:

    using value_type = typename IterFirst::value_type;

    ChainIterator(
            const IterFirst &first,
            const IterFirst &end,
            const IterSecond &begin
    ) :
            m_first{first},
            m_end{end},
            m_second{begin},
            m_begin{begin}
    {
    }

    ChainIterator(
            const IterFirst &first,
            const IterFirst &end,
            const IterSecond &second,
            const IterSecond &begin
    ) :
            m_first{first},
            m_end{end},
            m_second{second},
            m_begin{begin}
    {
    }

    bool leftIterIsValid() const
    {
        return m_first != m_end;
    }

    value_type &operator*()
    {
        if (leftIterIsValid())
        {
            return *m_first;
        }
        else
        {
            return *m_second;
        }
    }

    const value_type &operator*() const
    {
        if (leftIterIsValid())
        {
            return *m_first;
        }
        else
        {
            return *m_second;
        }
    }

    ChainIterator &operator++()
    {
        if (leftIterIsValid())
        {
            ++m_first;
        }
        else
        {
            ++m_second;
        }
        return *this;
    }

    ChainIterator operator+(const size_t offset) const
    {
        ChainIterator result{m_first, m_end, m_second, m_begin};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const ChainIterator &rhs) const
    {
        return m_first == rhs.m_first && m_second == rhs.m_second;
    }

    bool operator!=(const ChainIterator &rhs) const
    {
        return !(*this == rhs);
    }
};

template<class IterLeft, class IterRight>
ChainIterator<IterLeft, IterRight>
makeChainSequence(
        const IterLeft &first,
        const IterLeft &end,
        const IterRight &begin
)
{
    return ChainIterator<IterLeft, IterRight>{first, end, begin};
}

template<class IterLeft, class IterRight>
ChainIterator<IterLeft, IterRight>
makeChainSequence(
        const IterLeft &first,
        const IterLeft &end,
        const IterRight &second,
        const IterRight &begin
)
{
    return ChainIterator<IterLeft, IterRight>{first, end, second, begin};
}

}
