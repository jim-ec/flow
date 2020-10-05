#pragma once

#include <flow/details.h>
#include <flow/Maybe.h>
#include <flow/Flow.h>

namespace flow
{
    /// Maps each sequence element through a function.
    /// Arity: 1 -> 1
    template<class S, class F>
    class Map
    {
    public:
        using FunctionInputType = typename S::ElementType;
        using ElementType = details::FunctionReturnType<F, FunctionInputType>;

        static_assert(!std::is_lvalue_reference_v<ElementType>, "The mapped type must be owned.");
        static_assert(!std::is_rvalue_reference_v<ElementType>, "The mapped type must be owned.");

        Map(S &&sequence, F function):
            sequence(std::move(sequence)),
            function(function)
        {
        }

        Maybe<ElementType> next()
        {
            Maybe<FunctionInputType> functionInput = sequence.next();
            if (functionInput.hasValue())
            {
                return function(functionInput.value());
            }
            else
            {
                return None();
            }
        }

    private:
        S sequence;
        F function;
    };

    template<class F>
    auto map(F function)
    {
        return [=] (auto &&sequence)
        {
            return Map(std::move(sequence), function);
        };
    }
    
    template<class T>
    auto elements2(std::vector<T> const &container)
    {
        size_t i = 0;
        
        auto f = [=] () mutable -> int
        {
            return container[i++];
//            if (i < container.size())
//            {
//                return container[i++];
//            }
//            else
//            {
//                return None();
//            }
        };
        
        return Generator(f);
    }
    
    template<class F>
    auto map2(F function)
    {
        return Functor([=] (auto &flow)
        {
            auto maybe = flow.next();
            return maybeIf(maybe.hasValue(), function(maybe.value()));
        });
    }
    
    auto take2(size_t n)
    {
        size_t k = 0;
        return Functor([=] (auto &flow) mutable
        {
            auto maybe = flow.next();
            return maybeIf(maybe.hasValue() && k++ < n, maybe.value());
        });
    }
    
    /// Dereferences the values behind element pointers.
    auto dereference()
    {
        return map([] (auto *pointer) { return *pointer; });
    }
    
    /// Maps maybes which have a value through a function.
    /// Empty maybes are kept.
    template<class F>
    auto then(F function)
    {
        return map([=] (auto &&inputMaybe)
        {
            if (inputMaybe.hasValue())
            {
                return Maybe(function(std::move(inputMaybe.value())));
            }
            else
            {
                using OutputType = decltype(function(std::move(inputMaybe.value())));
                return Maybe<OutputType>(None());
            }
        });
    }
    
    /// Identifies each element with a growing index.
    auto enumerate()
    {
        size_t k = 0;
        return map([=] (auto const &element) mutable
        {
            return std::tuple(k++, element);
        });
    }
}
