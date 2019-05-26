//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{

template<class Iter>
class Flatten
{
public:
    using child_iter_type = typename Iter::value_type::iter_type;
    using value_type = iter_value_type_t<child_iter_type>;

private:

    Iter m_base_iter;
    Iter m_base_end;
    child_iter_type m_child_iter;
    child_iter_type m_child_end;

public:

    Flatten() = default;

    Flatten(const Flatten &rhs) = default;

    Flatten(Flatten &&rhs) noexcept = default;

    Flatten &operator=(const Flatten &rhs) = default;

    Flatten &operator=(Flatten &&rhs) noexcept = default;

    Flatten(
            const Iter &iter,
            const Iter &end,
            const child_iter_type &child_iter,
            const child_iter_type &child_end
    ) :
            m_base_iter{iter},
            m_base_end{end},
            m_child_iter{child_iter},
            m_child_end{child_end}
    {}

    value_type &operator*()
    {
        return *m_child_iter;
    }

    const value_type &operator*() const
    {
        return *m_child_iter;
    }

    Flatten &operator++()
    {
        ++m_child_iter;
        if (m_child_iter == m_child_end)
        {
            // Go to next child.
            ++m_base_iter;
            if(m_base_iter != m_base_end)
            {
                m_child_iter = (*m_base_iter).begin();
                m_child_end = (*m_base_iter).end();
            }
        }
        return *this;
    }

    Flatten operator+(const size_t offset) const
    {
        Flatten result{m_base_iter, m_child_iter, m_child_end};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool operator==(const Flatten &rhs) const
    {
        return m_base_iter == rhs.m_base_iter;
    }

    bool operator!=(const Flatten &rhs) const
    {
        return !(*this == rhs);
    }
};

template<class Iter>
Flatten<Iter> make_flatten(
        const Iter &iter,
        const Iter &end,
        const typename Flatten<Iter>::child_iter_type &child_iter,
        const typename Flatten<Iter>::child_iter_type &child_end
)
{
    return Flatten<Iter>{iter, end, child_iter, child_end};
}

}
