//
// Created by jim on 11/13/19.
//

#pragma once

#include <type_traits>

namespace sequences
{
    /// Tests if `TT<Ts...>` is the same type as `T`.
    template<class T, template<class...> class TT, class... Ts>
    inline constexpr bool is_type_instance_of = std::is_same_v<T, TT<Ts...>>;

    template<class Fn, class... As>
    using function_return_type = decltype(std::declval<Fn>()(
        // This statically calls the function type with the iterator's
        // value type, to get the functions return type since
        // no `std::function` but raw template types are used.
        // This means that the iterator's value type must be convertible
        // to the function's argument type, like:
        // `T -> T` or `T -> const T&`, but not `T -> T&`.
        std::declval<As>()...
    ));

    namespace impl
    {
        template<template<class> class TypeMap, class Tuple, size_t... Is>
        auto
        tuple_type_map_(
            std::index_sequence<Is...>
        )
        {
            return std::make_tuple((TypeMap<std::tuple_element_t<Is, Tuple>>())...);
        }

        template<template<class> class TypeMap, class Tuple>
        auto
        tuple_type_map()
        {
            return tuple_type_map_<TypeMap, Tuple>(
                std::make_index_sequence<std::tuple_size<Tuple>::value>{}
            );
        }
    }

    /// Maps a tuple type (A, ...) to a tuple type (A::output_type, ...).
    template<template<class> class TypeMap, class Tuple>
    using tuple_type_map = decltype(impl::tuple_type_map<TypeMap, Tuple>());
}
