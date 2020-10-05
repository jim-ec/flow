#pragma once

#include <flow/details.h>
#include <flow/Iterator.h>
#include <flow/Maybe.h>

namespace flow
{
    template<class F> struct Functor;
    
    /// A flow, dependend on a basing flow and on a function, which not only may map the values
    /// coming from the base flow to new values, but may also decide on how to iterate over the base flow.
    template<class B, class F>
    struct Flow2
    {
        B baseFlow;
        F function;
                
        Flow2(B const &baseFlow, F const &function): baseFlow(baseFlow), function(function)
        {
        }

        auto next()
        {
            if constexpr (flow::isMaybe<decltype(std::declval<F>()(std::declval<B&>()))>)
            {
                return function(baseFlow);
            }
            else
            {
                return some(function(baseFlow));
            }
        }
        
        /// Appends the given functor to this flow.
        template<class E>
        auto operator|(Functor<E> const &flowSegment)
        {
            return Flow2<Flow2<B, F>, E>(*this, flowSegment.function);
        }
    };

    /// A flow which does not yet have a base flow to iterate from.
    /// To become a flow, it has to be appended to either a flow, functor or generator.
    /// The type of elements this functor will produce is not known, because its closure can be a template.
    template<class F>
    struct Functor
    {
        F function;

        Functor(F const &function): function(function)
        {
        }

        /// Appends another functor to this one.
        /// This turns the given functor into a flow, which receives its elements from this functor.
        template<class E>
        auto operator|(Functor<E> const &functor)
        {
            return Flow2(*this, functor.function);
        }
    };
    
    template<class F>
    struct Generator
    {
        F function;
                
        Generator(F const &function): function(function) {}
        
        auto next()
        {
            if constexpr (flow::isMaybe<decltype(std::declval<F>()())>)
            {
                return function();
            }
            else
            {
                return some(function());
            }
        }
        
        /// Appends a functor to this generator.
        /// This will turn the given functor into a flow.
        template<class E>
        auto operator|(Functor<E> functor)
        {
            return Flow2(*this, functor.function);
        }
    };
    
    /// A flow merely acts as a wrapper for another sequence, while also offering a composition function to
    /// compose a new sequence out of the held sequence and another sequence.
    /// To compose two sequences, `a` and `b`, write `Flow(a) | b`.
    template<class S>
    class Flow
    {
    public:
        using ElementType = typename S::ElementType;

        Maybe<ElementType> next()
        {
            return sequence.next();
        }
        
        explicit Flow(S const &sequence):
            sequence(sequence)
        {
        }

        explicit Flow(S &&sequence):
            sequence(std::move(sequence))
        {
        }
        
        /// Sequence composition.
        template<class C>
        auto operator|(C sequenceConstructor) &&
        {
            return Flow<details::FunctionReturnType<C, S>>(sequenceConstructor(std::move(sequence)));
        }
        
        /// Sequence composition when flow is copied simultaneously.
        template<class C>
        auto operator|(C sequenceConstructor) const &
        {
            return Flow<details::FunctionReturnType<C, S>>(sequenceConstructor(S(sequence)));
        }
        
        Iterator<S> begin()
        {
            return Iterator<S>(sequence);
        }
        
        EndIterator end()
        {
            return EndIterator{};
        }

    private:
        S sequence;
    };
}
