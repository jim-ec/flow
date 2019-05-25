//
// Created by jim on 20.05.19.
//

#pragma once

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

    Mutation() = default;
    Mutation(const Mutation &rhs) = default;
    Mutation(Mutation &&rhs) noexcept = default;

    Mutation &operator=(const Mutation &rhs) = default;
    Mutation &operator=(Mutation &&rhs) noexcept = default;

    explicit Mutation(
            const T &init,
            F f
    ) :
            m_function{f},
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
Sequence <Mutation<T, F>> make_mutation(const T &init, F f)
{
    Mutation<T, F> mutation{init, f};
    return make_sequence(mutation, mutation);
}

namespace impl
{

template<class T>
struct LinearMutation
{
    T step;

    LinearMutation(const LinearMutation &rhs) = default;

    LinearMutation(LinearMutation &&rhs) noexcept = default;

    LinearMutation &operator=(const LinearMutation &rhs) = default;

    LinearMutation &operator=(LinearMutation &&rhs) noexcept = default;

    int operator()(int n)
    {
        return n + step;
    }
};

}

template<class T = int>
Sequence <Mutation<T, impl::LinearMutation<T>>> make_mutation_linear(const T &init = T{0}, const T &step = T{1})
{
    return make_mutation(init, impl::LinearMutation<T>{step});
}

}
