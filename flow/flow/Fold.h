#pragma once

#include <optional>

#include <flow/details.h>

namespace flow
{
    template<class S, class F, class T>
    T fold(S sequence, T const &initial, F function) {
        T acc = initial;

        while (sequence.probe())
        {
            details::reinitialize(acc, function(std::move(acc), sequence.next()));
        }
        
        return acc;
    }

    template<class S, class F, class T = typename S::ElementType>
    std::optional<T>
    fold(S sequence, F function) {
        if (!sequence.probe())
        {
            return {};
        }
        
        T acc = sequence.next();

        while (sequence.probe())
        {
            details::reinitialize(acc, function(std::move(acc), sequence.next()));
        }
        
        return acc;
    }
}
