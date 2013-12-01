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

namespace functional_impl
{
    // maximum number of elements in tuple/array that will be 'unrolled' using recursive templates at compile time
    static constexpr std::size_t UNROLL_MAX = 100;

    namespace helpers
    {
        // helper struct to apply function pointer, lambdas and member function pointers the same way
        template<typename Fun>
        struct Applicator
        {
            const Fun& f;

            template<typename... Args>
            auto operator () (const Args&... args) const -> decltype(f(args...))
            {
                return f(args...);
            }
        };

        // specialization for member function pointers
        template<typename ReturnType, typename ValueType>
        struct Applicator<ReturnType(ValueType::*)() const>
        {
            typedef ReturnType(ValueType::*Fun)() const;

            const Fun& f;

            template<typename Arg>
            auto operator () (const Arg& arg) const -> decltype((arg.*f)())
            {
                return (arg.*f)();
            }
        };

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
    };

    template<template<typename, typename ...> class Iteratable, typename ValueType, typename Fun, typename... MoreTypes>
    void apply(Fun fun, const Iteratable<ValueType, MoreTypes...>& input)
    {
        for (const ValueType& value : input)
        {
            helpers::Applicator<Fun>{fun}(value);
        }
    }

    template<typename Fun, template<typename...> class TupleType, typename... ValueTypes>
    void apply(Fun fun, const TupleType<ValueTypes...>& input)
    {
        helpers::apply<sizeof...(ValueTypes)>(fun, input);
    }

    template<typename Fun, template<typename,std::size_t> class Array, typename ValueType, std::size_t size>
    void apply(Fun fun, const Array<ValueType, size>& input)
    {
        helpers::apply<size>(fun, input);
    }

    template < typename COut, typename CIn >
    auto reserve(COut& outc, CIn inc) -> decltype(outc.reserve(inc.size()))
    {
        outc.reserve(inc.size());
    }

    template < typename... T >
    void reserve(T...)
    {
    }

    template<template<typename, typename ...> class ContainerType, typename ValueType, typename MapFnType, typename ResultType = decltype(std::declval<MapFnType>()(std::declval<ValueType>())), typename... MoreTypes>
    auto map(MapFnType fun, const ContainerType<ValueType, MoreTypes...>& input)->ContainerType<ResultType>
    {
        ContainerType<ResultType> output;
        reserve(output, input);
        for (const ValueType& value : input)
        {
            output.push_back(fun(value));
        }
        return output;
    }

    template<template<typename, typename ...> class ContainerType, typename ValueType, typename ResultType, typename... MoreTypes>
    auto map(ResultType(ValueType::*fun)() const, const ContainerType<ValueType, MoreTypes...>& input)->ContainerType<ResultType>
    {
        return map([fun] (ValueType v) { return (v.*fun)(); }, input);
    }

    template<template<typename...> class Iteratable, typename InValue, typename OutValue, typename Fun, typename... ExtraArgs>
    OutValue foldr(Fun f, OutValue neutralValue, const Iteratable<InValue, ExtraArgs...>& iteratable)
    {
        OutValue res = neutralValue;
        for (InValue value : iteratable)
        {
            res = f(value, res);
        }
        return res;
    }

    template<template<typename...> class Iteratable, typename Fun, typename InValue, typename OutValue, typename... ExtraArgs>
    OutValue foldl(Fun f, OutValue neutralValue, const Iteratable<InValue, ExtraArgs...>& iteratable)
    {
        OutValue res = neutralValue;
        for (InValue value : iteratable)
        {
            res = f(res, value);
        }
        return res;
    }

    template<typename Fun, template<typename...> class Container, typename LhsValue, typename RhsValue, typename OutValue = decltype(std::declval<Fun>()(std::declval<LhsValue>(), std::declval<RhsValue>())), typename... ExtraArgs1, typename... ExtraArgs2, typename OutContainer = Container<OutValue>>
    OutContainer zipWith(Fun f, const Container<LhsValue, ExtraArgs1...>& lhs, const Container<RhsValue, ExtraArgs2...>& rhs)
    {
        OutContainer out;
        auto lhsIt = lhs.begin();
        auto rhsIt = rhs.begin();
        while (lhsIt != lhs.end() && rhsIt != rhs.end())
        {
            out.push_back(f(*lhsIt, *rhsIt));
            ++lhsIt; ++rhsIt;
        }
        return out;
    }

    template<template<typename...> class Container, typename LhsValue, typename RhsValue, typename... ExtraArgs1, typename... ExtraArgs2, typename OutContainer = Container<std::pair<LhsValue, RhsValue>>>
    OutContainer zip(const Container<LhsValue, ExtraArgs1...>& lhs, const Container<RhsValue, ExtraArgs2...>& rhs)
    {
        return zipWith([](LhsValue l, RhsValue r) { return std::make_pair(l, r); }, lhs, rhs);
    }
};

#undef forceinline
#undef noinline
#undef PACKED

#endif // _FUNCTIONAL_IMPL_HPP_