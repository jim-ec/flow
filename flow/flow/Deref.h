#pragma once

#include <flow/Map.h>

namespace flow
{
    /// Dereferences the values behind element pointers.
	/// Arity: 1 -> 1
    auto deref()
    {
        return map([] (auto *ptr)
        {
            return *ptr;
        });
    }
}
