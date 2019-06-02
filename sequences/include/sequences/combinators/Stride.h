//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class Stride
{
public:
    using value_type = iter_value_type_t<Iter>;

private:
    Iter m_iter;
    Iter m_end;
    size_t m_stride{};

public:

    Stride() = default;

    Stride(const Stride &rhs) = default;

    Stride(Stride &&rhs) noexcept = default;

    Stride &operator=(const Stride &rhs) = default;

    Stride &operator=(Stride &&rhs) noexcept = default;

    Stride(const Iter &iter, const Iter &end, const size_t stride) :
            m_iter{iter},
            m_end{end},
            m_stride{stride}
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

    Stride &
    operator++()
    {
        for (int i = 0; i < m_stride; ++i)
        {
            ++m_iter;
            if (m_iter == m_end)
            {
                break;
            }
        }
        return *this;
    }

    Stride
    operator+(const size_t offset) const
    {
        Stride result{m_iter, m_stride};
        for (size_t i = 0; i < offset; i++)
        {
            ++result;
        }
        return result;
    }

    bool
    operator==(const Stride &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool
    operator!=(const Stride &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter>
Stride<Iter>
make_stride(const Iter &begin, const Iter &end, const size_t stride)
{
    return Stride<Iter>{begin, end, stride};
}

}
