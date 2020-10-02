#pragma once

#include <flow/details.h>

namespace flow
{
    template<class S, class F, class T>
    T fold(S sequence, T const &initial, F function) {
        T acc = initial;
        
        for (Maybe<typename S::ElementType> maybe = sequence.next(); maybe.hasValue(); maybe = sequence.next())
        {
            details::reinitialize(acc, function(std::move(acc), maybe.value()));
        }
        
        return acc;
    }

    template<class S, class F, class T = typename S::ElementType>
    Maybe<T>
    fold(S sequence, F function) {
        Maybe<typename S::ElementType> maybe = sequence.next();
        
        if (!maybe.hasValue())
        {
            return None();
        }
        
        T acc = maybe.value();

        for (Maybe<typename S::ElementType> maybe = sequence.next(); maybe.hasValue(); maybe = sequence.next())
        {
            details::reinitialize(acc, function(std::move(acc), maybe.value()));
        }
        
        return acc;
    }
}
