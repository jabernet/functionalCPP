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

#include "applicator.hpp"


namespace functional_impl
{
    // maximum number of elements in tuple/array that will be 'unrolled' using recursive templates at compile time
    static constexpr std::size_t UNROLL_MAX = 100;

    namespace helpers
    {
        // helper struct to allow recursion using overload resolution
        template<std::size_t>
        struct index {} PACKED;

        template<std::size_t N, typename Fun, typename Gettable, std::size_t I = N>
        forceinline auto apply(Fun fun, const Gettable& input, index<I> = index<N>())
            -> typename std::enable_if<(N <= UNROLL_MAX)>::type
        {
            helpers::Applicator<Fun>{fun}(std::get<N-I>(input));
            apply<N>(fun, input, index<I-1>());
        }

        template<std::size_t N, typename Fun, typename Gettable>
        forceinline auto apply(Fun, const Gettable&, index<0>)
            -> typename std::enable_if<(N <= UNROLL_MAX)>::type
        {
        }

        template<std::size_t N, typename Fun, typename Gettable>
        forceinline auto apply(Fun fun, const Gettable& input)
            -> typename std::enable_if<(N > UNROLL_MAX)>::type
        {
            for(int i = 0; i < N; ++i)
            {
                helpers::Applicator<Fun>{fun}(input[0]);
            }
        }

        template < typename COut, typename CIn >
        forceinline auto reserve(COut& outc, CIn inc) -> decltype(outc.reserve(inc.size()))
        {
            outc.reserve(inc.size());
        }

        template < typename... T >
        forceinline void reserve(T...)
        {
        }
    };

    template<template<typename, typename ...> class Iteratable, typename ValueType, typename Fun, typename... MoreTypes>
    forceinline void apply(Fun fun, const Iteratable<ValueType, MoreTypes...>& input)
    {
        for (const ValueType& value : input)
        {
            helpers::Applicator<Fun>{fun}(value);
        }
    }

    template<typename Fun, template<typename...> class TupleType, typename... ValueTypes>
    forceinline void apply(Fun fun, const TupleType<ValueTypes...>& input)
    {
        helpers::apply<sizeof...(ValueTypes)>(fun, input);
    }

    template<typename Fun, template<typename,std::size_t> class Array, typename ValueType, std::size_t size>
    forceinline void apply(Fun fun, const Array<ValueType, size>& input)
    {
        helpers::apply<size>(fun, input);
    }

    template<template<typename, typename ...> class ContainerType, typename ValueType, typename Fun, typename ResultType = decltype(std::declval<helpers::Applicator<Fun>>()(std::declval<ValueType>())), typename... MoreTypes>
    forceinline auto map(Fun fun, const ContainerType<ValueType, MoreTypes...>& input)->ContainerType<ResultType>
    {
        ContainerType<ResultType> output;
        helpers::reserve(output, input);
        for (const ValueType& value : input)
        {
            output.push_back(helpers::Applicator<Fun>{fun}(value));
        }
        return output;
    }

    template<template<typename...> class Iteratable, typename InValue, typename OutValue, typename Fun, typename... ExtraArgs>
    forceinline OutValue foldr(Fun fun, OutValue neutralValue, const Iteratable<InValue, ExtraArgs...>& iteratable)
    {
        OutValue res = neutralValue;
        for (InValue value : iteratable)
        {
            res = helpers::Applicator<Fun>{fun}(value, res);
        }
        return res;
    }

    template<template<typename...> class Iteratable, typename Fun, typename InValue, typename OutValue, typename... ExtraArgs>
    forceinline OutValue foldl(Fun fun, OutValue neutralValue, const Iteratable<InValue, ExtraArgs...>& iteratable)
    {
        OutValue res = neutralValue;
        for (InValue value : iteratable)
        {
            res = helpers::Applicator<Fun>{fun}(res, value);
        }
        return res;
    }

    template<typename Fun, template<typename...> class Container, typename LhsValue, typename RhsValue, typename OutValue = decltype(std::declval<Fun>()(std::declval<LhsValue>(), std::declval<RhsValue>())), typename... ExtraArgs1, typename... ExtraArgs2, typename OutContainer = Container<OutValue>>
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

        // helper struct for indexes
        template <std::size_t... Is>
        struct indices {};

        // template to extract indeces struct from parameter pack
        template <std::size_t N, std::size_t... Is>
        struct build_indices
          : build_indices<N-1, N-1, Is...> {};

        // template to extract indeces struct from parameter pack
        template <std::size_t... Is>
        struct build_indices<0, Is...> : indices<Is...> {};

        template<typename Fun, typename... Args, std::size_t... Is>
        forceinline auto uncurryTuple(Fun& f, const std::tuple<Args...>& args, indices<Is...>) -> decltype(f(std::get<Is>(args)...))
        {
            return f(std::get<Is>(args)...);
        }

        template<typename Fun>
        struct UnCurry
        {
            Applicator<Fun> f;

            template<typename... Args>
            auto operator () (const std::tuple<Args...>& args) const -> decltype(uncurryTuple(f, args, build_indices<sizeof...(Args)>()))
            {
                return uncurryTuple(f, args, build_indices<sizeof...(Args)>());
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