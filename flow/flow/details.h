//
// Created by jim on 11/13/19.
//

#pragma once

#include <type_traits>

namespace flow::details
{
    template<class F, class T>
    using FunctionReturnType = decltype(std::declval<F>()(std::declval<T>()));
    
    template<class T, class E>
    void reinitialize(T &value, E &&constructorArgument)
    {
        value.~T();
        new (&value) T(std::forward<E>(constructorArgument));
    }
}
