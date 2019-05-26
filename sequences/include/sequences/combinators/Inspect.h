//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter, class F>
class Inspect
{
public:
    using value_type = iter_value_type_t<Iter>;

private:
    Iter m_iter;
    F m_function;

public:

    Inspect() = default;
    Inspect(const Inspect &rhs) = default;
    Inspect(Inspect &&rhs) noexcept = default;

    Inspect &operator=(const Inspect &rhs) = default;
    Inspect &operator=(Inspect &&rhs) noexcept = default;

    Inspect(
            const Iter &iter,
            F function
    ) :
            m_iter{iter},
            m_function{function}
    {
    }

    const value_type &operator*() const
    {
        const value_type &n = *m_iter;
        m_function(n);
        return n;
    }

    const value_type &operator*()
    {
        value_type &n = *m_iter;
        m_function(n);
        return n;
    }

    Inspect &operator++()
    {
        ++m_iter;
        return *this;
    }

    Inspect operator+(const size_t offset) const
    {
        Inspect result{m_iter, m_function};
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

template<class Iter, class F>
Inspect<Iter, F> make_inspect(
        const Iter &iter,
        F f
)
{
    return Inspect<Iter, F>{iter, f};
}

}
