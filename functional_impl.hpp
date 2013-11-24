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

	static void reserve(...)
	{
	}

	template<template<typename, typename ...> class ContainerType, typename ValueType, typename MapFnType, typename ResultType = decltype(std::declval<MapFnType>()(std::declval<ValueType>())), typename... MoreTypes, typename OutputType = ContainerType<ResultType, MoreTypes...> >
	OutputType map(MapFnType fun, const ContainerType<ValueType, MoreTypes...>& input)
	{
		typedef ContainerType<ValueType, MoreTypes...> InputType;
		OutputType output;
		reserve(output, input);
		for (const ValueType& value : input)
		{
			output.push_back(fun(value));
		}
		return output;
	}

	template<template<typename, typename ...> class ContainerType, typename ValueType, typename ResultType, typename... MoreTypes>
	auto map(ResultType(ValueType::*fun)() const, const ContainerType<ValueType, MoreTypes...>& input)->ContainerType<ResultType, MoreTypes...>
	{
		typedef ContainerType<ValueType, MoreTypes...> InputType;
		typedef ContainerType<ResultType, MoreTypes...> OutputType;
		OutputType output;
		reserve(output, input);
		for (const ValueType& value : input)
		{
			output.push_back((value.*fun)());
		}
		return output;
	}	

	template<template<typename...> class Iteratable, typename InValue, typename OutValue, typename Fun, typename... ExtraArgs>
	OutValue foldr(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable)
	{
		OutValue res = neutralValue;
		for (InValue value : iteratable)
		{
			res = f(value, res);
		}
		return res;
	}

	template<template<typename...> class Iteratable, typename Fun, typename InValue, typename OutValue, typename... ExtraArgs>
	OutValue foldl(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable)
	{
		OutValue res = neutralValue;
		for (InValue value : iteratable)
		{
			res = f(res, value);
		}
		return res;
	}

	template<template<typename...> class Container, typename Fun, typename LhsValue, typename RhsValue, typename OutValue = decltype(std::declval<Fun>()(std::declval<LhsValue>(), std::declval<RhsValue>())), typename... ExtraArgs, typename OutContainer = Container<OutValue, ExtraArgs...>>
	OutContainer zipWith(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs, Fun f)
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

	template<template<typename...> class Container, typename LhsValue, typename RhsValue, typename... ExtraArgs, typename OutContainer = Container<std::pair<LhsValue, RhsValue>, ExtraArgs...>>
	OutContainer zip(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs)
	{
		return zipWith(lhs, rhs, [](LhsValue l, RhsValue r) { return std::make_pair(l, r); });
	}
};

#endif // _FUNCTIONAL_IMPL_HPP_