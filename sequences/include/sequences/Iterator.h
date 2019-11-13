//
// Created by jim on 11/13/19.
//

#pragma once

#include "sequences/Sequence.h"

namespace sequences
{
    /// A forward iterator wrapping a sequence.
    /// Unlike a sequence, this can be used in the range-based for loop language construct.
    /// The main purpose of this class is that the actual sequences do not need to implement the
    /// impractical iterator interface, such as comparing to check whether a sequence is still valid.
    class Iterator {

    public:
    };
}
