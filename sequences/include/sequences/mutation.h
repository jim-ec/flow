//
// Created by jim on 20.05.19.
//

#pragma once

#include <limits>

#include "tools.h"
#include "sequence.h"

namespace sequences
{
/// A mutation is an iterator-like class, generating value using a function
/// and a carry value.
/// In each iteration, the generator function is called with the current carry.
/// The result is than saved back to the carry.
/// A mutation is unequal to everything, and equal to nothing.
/// This allows endless iteration using the traditional *while iter != end_iter*
/// construct.
template<class T, class F>
class Mutation
{
public:
    using value_type = T;
    using iterator = Mutation;

    F m_function;
    value_type m_carry;

    explicit Mutation(
            T &&init,
            F f
    ) :
            m_function{f},
            m_carry{std::forward<T>(init)}
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
        m_carry = m_function(m_carry);
        return *this;
    }

    Mutation operator+(const size_t offset) const
    {
        Mutation generator{value_type{m_carry}, m_function};
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

template<class T, class F>
Sequence <Mutation<T, F>>
makeMutation(T &&init, F f)
{
    Mutation<T, F> mutation{std::forward<T>(init), f};
    return makeSequence(mutation, mutation);
}

template<class T = int>
decltype(auto) makeMutationLinear(T &&init = T{0}, T step = T{1})
{
    return makeMutation(std::forward<T>(init), [step](T n) { return n + step; });
}

}
