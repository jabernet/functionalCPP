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

    void reserve(...)
	{
	}

	template<template<typename, typename ...> class ContainerType, typename ValueType, typename MapFnType, typename ResultType = decltype(std::declval<MapFnType>()(std::declval<ValueType>())), typename... MoreTypes>
	ContainerType<ResultType> map(MapFnType fun, const ContainerType<ValueType, MoreTypes...>& input)
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
		typedef ContainerType<ResultType> OutputType;
		OutputType output;
		reserve(output, input);
		for (const ValueType& value : input)
		{
			output.push_back((value.*fun)());
		}
		return output;
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