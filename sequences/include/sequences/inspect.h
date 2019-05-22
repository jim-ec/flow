//
// Created by jim on 20.05.19.
//

#pragma once

#include "tools.h"

namespace sequences
{
template<class Iter, class F>
class InspectIterator
{
public:
    using value_type = typename Iter::value_type;

private:
    Iter m_iter;
    F m_function;

public:

    InspectIterator(
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

    InspectIterator &operator++()
    {
        ++m_iter;
        return *this;
    }

    InspectIterator operator+(const size_t offset) const
    {
        InspectIterator result{m_iter, m_function};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const InspectIterator &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const InspectIterator &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter, class F>
InspectIterator<Iter, F> makeInspectSequence(
        const Iter &iter,
        F f
)
{
    return InspectIterator<Iter, F>{iter, f};
}

}
