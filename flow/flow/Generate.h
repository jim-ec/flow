#pragma once

#include <flow/details.h>
#include <flow/Maybe.h>

namespace flow
{
    template<class G>
    class Generate
    {
    public:
        using ElementType = typename decltype(std::declval<G>()())::ValueType;

        explicit Generate(G generator):
            generator(generator)
        {}

        Maybe<ElementType> next()
        {
            return generator();
        }

    private:
        G generator;
    };

    template<class G>
    auto generate(G generator)
    {
        return Flow(Generate(generator));
    }
    
    auto successors(size_t i)
    {
        return generate([=] () mutable -> Maybe<int>
        {
            return i++;
        });
    }
}
