//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter, class Fn>
class Filter
{
public:
    using value_type = iter_value_type_t<Iter>;

private:
    Iter m_iter;
    Iter m_end;
    Fn m_fn;
    value_type m_cache{};

public:

    Filter() = default;
    Filter(const Filter &rhs) = default;
    Filter(Filter &&rhs) noexcept = default;

    Filter &operator=(const Filter &rhs) = default;
    Filter &operator=(Filter &&rhs) noexcept = default;

    Filter(
            const Iter &iter,
            const Iter &end,
            Fn fn
    ) :
            m_iter{iter},
            m_end{end},
            m_fn{fn},
            m_cache{}
    {
        if (m_iter != m_end)
        {
            m_cache = *m_iter;
            while (!m_fn(m_cache))
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

    Filter &operator++()
    {
        do
        {
            ++m_iter;

            if (m_end == m_iter)
            {
                break;
            }

            m_cache = *m_iter;
        } while (!m_fn(m_cache));

        return *this;
    }

    Filter operator+(const size_t offset) const
    {
        Filter result{m_iter, m_fn};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const Filter &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const Filter &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter, class Fn>
Filter<Iter, Fn> make_filter(
        const Iter &begin,
        const Iter &end,
        Fn fn
)
{
    return Filter<Iter, Fn>{begin, end, fn};
}

}
