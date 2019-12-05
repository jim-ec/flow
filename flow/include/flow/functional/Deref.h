//
// Created by jim on 11/13/19.
//

#pragma once

#include <flow/functional/Map.h>

namespace flow
{
    /// Dereferences the values behind element pointers.
	/// Arity: 1 -> 1
    auto deref() {
        return map([] (auto *ptr) { return *ptr; });
    }
}
