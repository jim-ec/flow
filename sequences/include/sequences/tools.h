//
// Created by jimec on 19.05.2019.
//

#pragma once

#include <limits>
#include <cstddef>
#include <utility>
#include <type_traits>

namespace sequences
{

/// Use this type to get an iterator-type's value-type.
template<class Iter>
struct iter_value_type
{
    using type = typename Iter::value_type;
};

/// The value type of an iterator which is a raw pointer is
/// simply the dereferenced type. E.g. the value type of
/// the iterator type `int *` is `int`.
template<class T>
struct iter_value_type<T *>
{
    using type = T;
};

/// Short cut for `typename iter_value_type<T>::type`.
template<class T>
using iter_value_type_t = typename iter_value_type<T>::type;

namespace impl
{

template<class T>
struct LinearMutation
{
    T step;

    LinearMutation() = default;

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

}
