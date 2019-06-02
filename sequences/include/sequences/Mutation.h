//
// Created by jim on 20.05.19.
//

#pragma once

#include "tools.h"

namespace sequences
{
/// A mutation is an iterator-like class, generating value using a function
/// and a carry value.
/// In each iteration, the generator function is called with the current carry.
/// The result is than saved back to the carry.
/// A mutation is unequal to everything, and equal to nothing.
/// This allows endless iteration using the traditional *while iter != end_iter*
/// construct.
template<class T, class Fn>
class Mutation
{
public:
    using value_type = T;
    using iterator = Mutation;

    Fn m_fn;
    value_type m_carry;

    Mutation() = default;
    Mutation(const Mutation &rhs) = default;
    Mutation(Mutation &&rhs) noexcept = default;

    Mutation &operator=(const Mutation &rhs) = default;
    Mutation &operator=(Mutation &&rhs) noexcept = default;

    explicit Mutation(
            const T &init,
            Fn fn
    ) :
            m_fn{fn},
            m_carry{init}
    {
    }

    Mutation &begin()
    {
        return *this;
    }

    Mutation &end()
    {
        return *this;
    }

    const Mutation &begin() const
    {
        return *this;
    }

    const Mutation &end() const
    {
        return *this;
    }

    value_type &operator*()
    {
        return m_carry;
    }

    const value_type &operator*() const
    {
        return m_carry;
    }

    Mutation &operator++()
    {
        m_carry = m_fn(m_carry);
        return *this;
    }

    Mutation operator+(const size_t offset) const
    {
        Mutation generator{value_type{m_carry}, m_fn};
        for (size_t i = 0; i < offset; i++)
        {
            ++generator;
        }
        return generator;
    }

    bool operator==(const Mutation &rhs) const
    {
        return false;
    }

    bool operator!=(const Mutation &rhs) const
    {
        return true;
    }

};

}
