//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter, class Fn>
class Inspect
{
public:
    using value_type = iter_value_type_t<Iter>;

private:
    Iter m_iter;
    Fn m_fn;

public:

    Inspect() = default;
    Inspect(const Inspect &rhs) = default;
    Inspect(Inspect &&rhs) noexcept = default;

    Inspect &operator=(const Inspect &rhs) = default;
    Inspect &operator=(Inspect &&rhs) noexcept = default;

    Inspect(
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
        m_fn(n);
        return n;
    }

    const value_type &operator*()
    {
        value_type &n = *m_iter;
        m_fn(n);
        return n;
    }

    Inspect &operator++()
    {
        ++m_iter;
        return *this;
    }

    Inspect operator+(const size_t offset) const
    {
        Inspect result{m_iter, m_fn};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const Inspect &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const Inspect &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter, class Fn>
Inspect<Iter, Fn> make_inspect(
        const Iter &iter,
        Fn fn
)
{
    return Inspect<Iter, Fn>{iter, fn};
}

}
