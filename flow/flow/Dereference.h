#pragma once

#include <flow/Map.h>

namespace flow
{
    /// Dereferences the values behind element pointers.
	/// Arity: 1 -> 1
    auto dereference()
    {
        return map([] (auto *pointer)
        {
            return *pointer;
        });
    }
}
