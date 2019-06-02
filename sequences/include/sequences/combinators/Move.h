//
// Created by jim on 20.05.19.
//

#pragma once

#include "sequences/tools.h"

namespace sequences
{
template<class Iter>
class Move
{
public:
    using value_type = iter_value_type_t<Iter>&&;

private:
    Iter m_iter;

public:

    Move() = default;

    Move(const Move &rhs) = default;

    Move(Move &&rhs) noexcept = default;

    Move &operator=(const Move &rhs) = default;

    Move &operator=(Move &&rhs) noexcept = default;

    Move(const Iter &iter) :
            m_iter{iter}
    {
    }

    value_type operator*()
    {
        return std::move(*m_iter);
    }

    Move &operator++()
    {
        ++m_iter;
        return *this;
    }

    Move operator+(const size_t offset) const
    {
        Move result{m_iter};
        for (size_t i = 0; i < offset; i++)
        {
            ++result.m_iter;
        }
        return result;
    }

    bool operator==(const Move &rhs) const
    {
        return m_iter == rhs.m_iter;
    }

    bool operator!=(const Move &rhs) const
    {
        return m_iter != rhs.m_iter;
    }
};

template<class Iter>
Move<Iter> make_move(const Iter &iter)
{
    return Move<Iter>{iter};
}

}
