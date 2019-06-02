//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter, class Fn>
class OnEachPair
{
public:
    using value_type = iter_value_type_t<Iter>;

private:
    Iter m_iter;
    Fn m_fn;

public:

    OnEachPair() = default;
    OnEachPair(const OnEachPair &rhs) = default;
    OnEachPair(OnEachPair &&rhs) noexcept = default;

    OnEachPair &operator=(const OnEachPair &rhs) = default;
    OnEachPair &operator=(OnEachPair &&rhs) noexcept = default;

    OnEachPair(
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
        m_fn(n.m_first, n.m_second);
        return n;
    }

    const value_type &operator*()
    {
        value_type &n = *m_iter;
        m_fn(n.m_first, n.m_second);
        return n;
    }

    OnEachPair &operator++()
    {
        ++m_iter;
        return *this;
    }

    OnEachPair operator+(const size_t offset) const
    {
        OnEachPair result{m_iter, m_fn};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const OnEachPair &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const OnEachPair &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter, class Fn>
OnEachPair<Iter, Fn> make_on_each_pair(
        const Iter &iter,
        Fn fn
)
{
    return OnEachPair<Iter, Fn>{iter, fn};
}

}
