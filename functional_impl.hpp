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

namespace functional_impl
{
    template<template<typename, typename ...> class Iteratable, typename ValueType, typename Fun, typename... MoreTypes>
    void apply(Fun fun, const Iteratable<ValueType, MoreTypes...>& input)
    {
        for (const ValueType& value : input)
        {
            fun(value);
        }
    }

    template<template<typename, typename ...> class ContainerType, typename ValueType, typename ReturnType, typename... MoreTypes>
    void apply(ReturnType(ValueType::*fun)() const, const ContainerType<ValueType, MoreTypes...>& input)
    {
        for (const ValueType& value : input)
        {
            (value.*fun)();
        }
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

#endif // _FUNCTIONAL_IMPL_HPP_