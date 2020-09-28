//
// Created by jim on 11/13/19.
//

#pragma once

#include <type_traits>

namespace flow::details
{
    template<class F, class T>
    using FunctionReturnType = decltype(std::declval<F>()(
        // This statically calls the functional type with the iterator's
        // value type, to get the functions return type since
        // no `std::function` but raw template types are used.
        // This means that the iterator's value type must be convertible
        // to the functional's argument type, like:
        // `T -> T` or `T -> const T&`, but not `T -> T&`.
        std::declval<T>()
    ));
}
