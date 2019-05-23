//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class IterLeft, class IterRight>
class ZipIterator
{
public:
    using value_type_left = typename IterLeft::value_type *;
    using value_type_right = typename IterRight::value_type *;
    using value_type = Pair<value_type_left, value_type_right>;

private:
    IterLeft m_left;
    IterRight m_right;
    value_type m_cache;

public:

    ZipIterator(
            const IterLeft &left,
            const IterRight &right
    ) :
            m_left{left},
            m_right{right},
            m_cache{}
    {
    }

    const value_type &operator*() const
    {
        m_cache = makePair(&*m_left, &*m_right);
        return m_cache;
    }

    value_type &operator*()
    {
        m_cache = makePair(&*m_left, &*m_right);
        return m_cache;
    }

    ZipIterator operator++()
    {
        ++m_left;
        ++m_right;
        return *this;
    }

    bool operator==(const ZipIterator &rhs) const
    {
        // This might look like a weird implementation, because intuitively two zip sequences
        // are the same if both iterator pairs equal, and not only one them.
        // The reason for this implementation is that this is simply the negation
        // of the != operator, so there are no cases where two zip sequences are neither equal
        // not not equal. And the != operator is choosen in such a way that if either the left
        // or the right iterator reaches end, the whole zip sequence is considered to reached the end.
        return m_left == rhs.m_left || m_right == rhs.m_right;
    }

    bool operator!=(const ZipIterator &rhs) const
    {
        return m_left != rhs.m_left && m_right != rhs.m_right;
    }
};

template<class IterLeft, class IterRight>
const ZipIterator<IterLeft, IterRight>
makeZipSequence(
        const IterLeft &left,
        const IterRight &right
)
{
    return ZipIterator<IterLeft, IterRight>{left, right};
}

}
