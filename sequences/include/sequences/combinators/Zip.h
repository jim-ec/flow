//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class IterLeft, class IterRight>
class Zip
{
public:
    using value_type_left = iter_value_type_t<IterLeft>;
    using value_type_right = iter_value_type_t<IterRight>;
    using value_type = std::pair<value_type_left, value_type_right>;

private:
    IterLeft m_left;
    IterRight m_right;
    value_type m_cache{};

public:

    Zip() = default;
    Zip(const Zip &rhs) = default;
    Zip(Zip &&rhs) noexcept = default;

    Zip &operator=(const Zip &rhs) = default;
    Zip &operator=(Zip &&rhs) noexcept = default;

    Zip(
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
        m_cache = std::make_pair(*m_left, *m_right);
        return m_cache;
    }

    value_type &operator*()
    {
        m_cache = std::make_pair(*m_left, *m_right);
        return m_cache;
    }

    Zip operator++()
    {
        ++m_left;
        ++m_right;
        return *this;
    }

    bool operator==(const Zip &rhs) const
    {
        // This might look like a weird implementation, because intuitively two zip sequences
        // are the same if both iterator pairs equal, and not only one them.
        // The reason for this implementation is that this is simply the negation
        // of the != operator, so there are no cases where two zip sequences are neither equal
        // not not equal. And the != operator is chosen in such a way that if either the left
        // or the right iterator reaches end, the whole zip sequence is considered to reached the end.
        return m_left == rhs.m_left || m_right == rhs.m_right;
    }

    bool operator!=(const Zip &rhs) const
    {
        return m_left != rhs.m_left && m_right != rhs.m_right;
    }
};

template<class IterLeft, class IterRight>
const Zip<IterLeft, IterRight> make_zip(
        const IterLeft &left,
        const IterRight &right
)
{
    return Zip<IterLeft, IterRight>{left, right};
}

}
