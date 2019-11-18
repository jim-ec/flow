//
// Created by jim on 11/13/19.
//

#pragma once

#include <tuple>

#include <sequences/functional/Map.h>

namespace sequences
{
    /// (a{n}) = (n, b{n})
	/// Arity: 1 -> 1
    auto enumerate() {
        size_t k = 0;
        return map([=] (auto const &el) mutable { return std::tuple(k++, el); });
    }
}
