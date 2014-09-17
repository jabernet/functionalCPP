/*
Copyright (c) 2013 Janick Bernet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _FUNCTIONAL_IMPL_HPP_
#define _FUNCTIONAL_IMPL_HPP_

#if defined(__GNUC__)
    #define forceinline __attribute__((always_inline)) inline
    #define noinline __attribute__((noinline)) 
    #define PACKED __attribute__ ((__packed__))
#elif defined(_MSC_VER)
    #define forceinline __forceinline 
    #define noinline __declspec(noinline) 
    #define PACKED
#else 
    #define forceinline inline 
    #define noinline 
    #define PACKED
#endif

#include <cstddef>
#include <tuple>
#include <utility>
#include <assert.h>

#include "applicator.hpp"

namespace functional
{
    // a struct indicating that the type should be derived instead of manually specified
    struct _Derived {};
}

namespace functional_impl
{
    using namespace functional;

    // maximum number of elements in tuple/array that will be 'unrolled' using recursive templates at compile time
    static constexpr std::size_t UNROLL_MAX = 100;

    namespace helpers
    {
        // helper struct to allow recursion using overload resolution
        template<std::size_t>
        struct index {} PACKED;

        // helper struct to derive input type, output type and output container given input container, explcit output container (or _Derived) and function type
        template<
            typename ResultContainerTypeExplicit,
            typename InputContainerType,
            typename FunType>
        struct result_t;

        // specialization for generic containers
        template<
            typename ResultContainerExplicit,
            template<typename...> class ContainerType,
            typename Fun,
            typename ValueType,
            typename... MoreTypes>
        struct result_t<
            ResultContainerExplicit,
            ContainerType<ValueType, MoreTypes...>,
            Fun>
        {
            typedef ValueType value_type;
            typedef decltype(std::declval<helpers::Applicator<Fun>>()(std::declval<ValueType>())) result_type;
            typedef typename std::conditional<
                std::is_same<ResultContainerExplicit, _Derived>::value,
                ContainerType<result_type>,
                ResultContainerExplicit>::type container_type;
            static_assert(std::is_convertible<result_type, typename container_type::value_type>::value, "ResultContainer does not have proper value type.");
        };

        // specialization for std array
        template<
            typename ResultContainerExplicit,
            typename Fun,
            typename ValueType,
            size_t size>
        struct result_t<
            ResultContainerExplicit,
            std::array<ValueType, size>,
            Fun>
        {
            typedef ValueType value_type;
            typedef decltype(std::declval<helpers::Applicator<Fun>>()(std::declval<ValueType>())) result_type;
            typedef typename std::conditional<
                std::is_same<ResultContainerExplicit, _Derived>::value,
                std::array<result_type, size>,
                ResultContainerExplicit>::type container_type;
            static_assert(std::is_convertible<result_type, typename container_type::value_type>::value, "ResultContainer does not have proper value type.");
        };

        // helper struct to do accumulation for different containers
        template<typename ContainerT>
        struct Accumulator
        {
            ContainerT container;
            
            // specialization for types that have reserve method (using SFINAE)
            template<typename COut, typename CIn>
            forceinline auto reserve(COut& outc, const CIn& inc) -> decltype(outc.reserve(inc.size()))
            {
                outc.reserve(inc.size());
            }

            // specialization for types that don't have reserve method
            template <typename... T>
            forceinline void reserve(T...)
            {
            }

            template<typename T>
            forceinline void accumulate(const T& t)
            {
                container.push_back(t);
            }
        };

        // helper struct to do accumulation for different containers
        template<typename ValueT, size_t size>
        struct Accumulator<std::array<ValueT, size>>
        {
            std::array<ValueT, size> container;
            int pos = { 0 };

            template<typename CIn>
            forceinline void reserve(const CIn& input)
            {
                assert(input.size() <= size);
            }

            template<typename InT, size_t size_in>
            forceinline void reserve(const std::array<InT, size_in>&)
            {
                static_assert(size_in <= size, "Result array cannot all elements of input array.");
            }

            template<typename T>
            forceinline void accumulate(const T& t)
            {
                container[pos++] = t;
            }
        };


        template<std::size_t N, typename Fun, typename Iteratable, std::size_t I = N>
        forceinline auto apply(Fun fun, Iteratable& inout, index<I> = index<N>())
            -> typename std::enable_if<(N <= UNROLL_MAX)>::type
        {
            helpers::Applicator<Fun>{fun}(std::get<N-I>(inout));

            apply<N>(fun, inout, index<I-1>());
        }

        template<std::size_t N, typename Fun, typename Iteratable>
        forceinline auto apply(Fun, Iteratable&, index<0>)
            -> typename std::enable_if<(N <= UNROLL_MAX)>::type
        {
        }

        template<std::size_t N, typename Fun, typename Iteratable>
        forceinline auto apply(Fun fun, Iteratable& inout)
            -> typename std::enable_if<(N > UNROLL_MAX)>::type
        {
            for(int i = 0; i < N; ++i)
            {
                helpers::Applicator<Fun>{fun}(inout[0]);
            }
        }
    };

    // generic implementation of apply for any kind of iteratable (vector, list, etc.)
    template<typename Fun, typename Iteratable>
    forceinline void apply(Fun fun, Iteratable& inout)
    {
        for (auto& value : inout)
        {
            helpers::Applicator<Fun>{fun}(value);
        }
    }

    // specialization for tuple (compile time sized)
    template<typename Fun, typename... Args>
    forceinline void apply(Fun fun, std::tuple<Args...>& inout)
    {
        helpers::apply<sizeof...(Args)>(fun, inout);
    }

    // specialization for array (compile time sized)
    template<typename Fun, typename T, size_t size>
    forceinline void apply(Fun fun, std::array<T, size>& inout)
    {
        helpers::apply<size>(fun, inout);
    }

    template<
        typename ResultContainerTypeExplicit,
        typename InputContainerType,
        typename Fun,
        typename ResultHelperT = typename helpers::result_t<ResultContainerTypeExplicit, InputContainerType, Fun>,
        typename ValueType = typename ResultHelperT::value_type,
        typename ResultType = typename ResultHelperT::result_type,
        typename ResultContainer = typename ResultHelperT::container_type>
    forceinline ResultContainer map(Fun fun, const InputContainerType& input)
    {
        helpers::Accumulator<ResultContainer> accumulator;
        accumulator.reserve(input);
        apply(
            [&](const ValueType& value)
            { 
                accumulator.accumulate(helpers::Applicator<Fun>{fun}(value));
            },
            input);
        return accumulator.container;
    }

    template<typename ResultType, template<typename...> class Iteratable, typename InValue, typename Fun, typename... ExtraArgs>
    forceinline ResultType foldr(Fun fun, ResultType neutralValue, const Iteratable<InValue, ExtraArgs...>& iteratable)
    {
        ResultType res = neutralValue;
        for (InValue value : iteratable)
        {
            res = helpers::Applicator<Fun>{fun}(value, res);
        }
        return res;
    }

    template<typename ResultType, template<typename...> class Iteratable, typename Fun, typename InValue, typename... ExtraArgs>
    forceinline ResultType foldl(Fun fun, ResultType neutralValue, const Iteratable<InValue, ExtraArgs...>& iteratable)
    {
        ResultType res = neutralValue;
        for (InValue value : iteratable)
        {
            res = helpers::Applicator<Fun>{fun}(res, value);
        }
        return res;
    }

    template<typename Fun, template<typename...> class Container, typename LhsValue, typename RhsValue, typename ResultType = decltype(std::declval<Fun>()(std::declval<LhsValue>(), std::declval<RhsValue>())), typename... ExtraArgs1, typename... ExtraArgs2, typename OutContainer = Container<ResultType>>
    forceinline OutContainer zipWith(Fun fun, const Container<LhsValue, ExtraArgs1...>& lhs, const Container<RhsValue, ExtraArgs2...>& rhs)
    {
        OutContainer out;
        auto lhsIt = lhs.begin();
        auto rhsIt = rhs.begin();
        while (lhsIt != lhs.end() && rhsIt != rhs.end())
        {
            out.push_back(helpers::Applicator<Fun>{fun}(*lhsIt, *rhsIt));
            ++lhsIt; ++rhsIt;
        }
        return out;
    }

    template<template<typename...> class Container, typename LhsValue, typename RhsValue, typename... ExtraArgs1, typename... ExtraArgs2, typename OutContainer = Container<std::pair<LhsValue, RhsValue>>>
    forceinline OutContainer zip(const Container<LhsValue, ExtraArgs1...>& lhs, const Container<RhsValue, ExtraArgs2...>& rhs)
    {
        return zipWith([](LhsValue l, RhsValue r) { return std::make_pair(l, r); }, lhs, rhs);
    }

    namespace helpers
    {
        template<typename Fun>
        struct Curry
        {
            Applicator<Fun> f;

            template<typename... Args>
            auto operator () (const Args&... args) const -> decltype(f(std::make_tuple(args...)))
            {
                return f(std::make_tuple(args...));
            }            
        };


        // below sequence generation code, in lieu of C++14 std::integer_sequence, is blatantly copied from 
        // http://stackoverflow.com/questions/17424477/implementation-c14-make-integer-sequence

        template<class T> using Invoke = typename T::type;

        template<unsigned...> struct seq{ using type = seq; };

        template<class S1, class S2> struct concat;

        template<unsigned... I1, unsigned... I2>
        struct concat<seq<I1...>, seq<I2...>>
            : seq<I1..., (sizeof...(I1)+I2)...>{};

        template<class S1, class S2>
        using Concat = Invoke<concat<S1, S2>>;

        template<unsigned N> struct gen_seq;
        template<unsigned N> using GenSeq = Invoke<gen_seq<N>>;

        template<unsigned N>
        struct gen_seq : Concat<GenSeq<N / 2>, GenSeq<N - N / 2>>{};

        template<> struct gen_seq<0> : seq<>{};
        template<> struct gen_seq<1> : seq<0>{};

        
        template<typename Fun, typename... Args, std::size_t... Is>
        forceinline auto uncurryTuple(Fun& f, const std::tuple<Args...>& args, seq<Is...>) -> decltype(f(std::get<Is>(args)...))
        {
            return f(std::get<Is>(args)...);
        }

        template<typename Fun>
        struct UnCurry
        {
            Applicator<Fun> f;

            template<typename... Args>
            auto operator () (const std::tuple<Args...>& args) const -> decltype(uncurryTuple(f, args, gen_seq<sizeof...(Args)>()))
            {
                return uncurryTuple(f, args, gen_seq<sizeof...(Args)>());
            }
        };
    }

    template<typename Fun>
    forceinline helpers::Curry<Fun> curry(Fun f)
    {
        return helpers::Curry<Fun> { { std::move(f) } };
    }

    template<typename Fun>
    forceinline helpers::UnCurry<Fun> uncurry(Fun f)
    {
        return helpers::UnCurry<Fun> { { std::move(f) } };
    }
};

#undef forceinline
#undef noinline
#undef PACKED

#endif // _FUNCTIONAL_IMPL_HPP_