//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter, class Fn>
class InspectPair
{
public:
    using value_type = iter_value_type_t<Iter>;

private:
    Iter m_iter;
    Fn m_fn;

public:

    InspectPair() = default;
    InspectPair(const InspectPair &rhs) = default;
    InspectPair(InspectPair &&rhs) noexcept = default;

    InspectPair &operator=(const InspectPair &rhs) = default;
    InspectPair &operator=(InspectPair &&rhs) noexcept = default;

    InspectPair(
            const Iter &iter,
            Fn fn
    ) :
            m_iter{iter},
            m_fn{fn}
    {
    }

    const value_type &operator*() const
    {
        const value_type &n = *m_iter;
        m_fn(n.first, n.second);
        return n;
    }

    const value_type &operator*()
    {
        value_type &n = *m_iter;
        m_fn(n.first, n.second);
        return n;
    }

    InspectPair &operator++()
    {
        ++m_iter;
        return *this;
    }

    InspectPair operator+(const size_t offset) const
    {
        InspectPair result{m_iter, m_fn};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const InspectPair &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const InspectPair &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter, class Fn>
InspectPair<Iter, Fn> make_inspect_pair(
        const Iter &iter,
        Fn fn
)
{
    return InspectPair<Iter, Fn>{iter, fn};
}

}
