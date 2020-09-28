#pragma once

#include <tuple>

#include <flow/Map.h>

namespace flow
{
    /// (a{n}) = (n, b{n})
	/// Arity: 1 -> 1
    auto enumerate()
    {
        size_t k = 0;
        return map([=] (auto const &element) mutable
        {
            return std::tuple(k++, element);
        });
    }
}
