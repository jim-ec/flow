//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter, class F>
class FilterIterator
{
public:
    using value_type = typename Iter::value_type;

private:
    Iter m_iter;
    Iter m_end;
    F m_function;
    value_type m_cache;

public:

    FilterIterator(
            const Iter &iter,
            const Iter &end,
            F function
    ) :
            m_iter{iter},
            m_end{end},
            m_function{function},
            m_cache{}
    {
        if (m_iter != m_end)
        {
            m_cache = *m_iter;
            while (!m_function(m_cache))
            {
                ++m_iter;

                if (m_end == m_iter)
                {
                    break;
                }

                m_cache = *m_iter;
            }
        }
    }

    value_type &operator*()
    {
        return m_cache;
    }

    const value_type &operator*() const
    {
        return m_cache;
    }

    FilterIterator &operator++()
    {
        do
        {
            ++m_iter;

            if (m_end == m_iter)
            {
                break;
            }

            m_cache = *m_iter;
        } while (!m_function(m_cache));

        return *this;
    }

    FilterIterator operator+(const size_t offset) const
    {
        FilterIterator result{m_iter, m_function};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const FilterIterator &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const FilterIterator &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter, class F>
FilterIterator<Iter, F> make_filter_iter(
        const Iter &begin,
        const Iter &end,
        F f
)
{
    return FilterIterator<Iter, F>{begin, end, f};
}

}
