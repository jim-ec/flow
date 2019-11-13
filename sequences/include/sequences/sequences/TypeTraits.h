//
// Created by jim on 11/13/19.
//

#pragma once

#include <type_traits>

namespace sequences
{
    template<class Fn, class ArgumentType>
    using function_return_type = decltype(std::declval<Fn>()(
        // This statically calls the function type with the iterator's
        // value type, to get the functions return type since
        // no `std::function` but raw template types are used.
        // This means that the iterator's value type must be convertible
        // to the function's argument type, like:
        // `T -> T` or `T -> const T&`, but not `T -> T&`.
        std::declval<ArgumentType>()
    ));
}
