//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class StepBy
{
public:
    using value_type = iter_value_type_t<Iter>;

private:
    Iter m_iter;
    Iter m_end;
    size_t m_step{};

public:

    StepBy() = default;

    StepBy(const StepBy &rhs) = default;

    StepBy(StepBy &&rhs) noexcept = default;

    StepBy &operator=(const StepBy &rhs) = default;

    StepBy &operator=(StepBy &&rhs) noexcept = default;

    StepBy(const Iter &iter, const Iter &end, const size_t step) :
            m_iter{iter},
            m_end{end},
            m_step{step}
    {
    }

    value_type &
    operator*()
    {
        return *m_iter;
    }

    const value_type &
    operator*() const
    {
        return *m_iter;
    }

    StepBy &
    operator++()
    {
        for (int i = 0; i < m_step; ++i)
        {
            ++m_iter;
            if (m_iter == m_end)
            {
                break;
            }
        }
        return *this;
    }

    StepBy
    operator+(const size_t offset) const
    {
        StepBy result{m_iter, m_step};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool
    operator==(const StepBy &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool
    operator!=(const StepBy &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter>
StepBy<Iter>
make_step_by(const Iter &begin, const Iter &end, const size_t step)
{
    return StepBy<Iter>{begin, end, step};
}

}
