#ifndef _FUNCTIONAL_HPP_
#define _FUNCTIONAL_HPP_


namespace functional
{
	template<template<typename, typename ...> class ContainerType, typename ValueType, typename MapFnType, typename ResultType = decltype(std::declval<MapFnType>()(std::declval<ValueType>())), typename... MoreTypes, typename OutputType = ContainerType<ResultType, MoreTypes...> >
	OutputType map(const ContainerType<ValueType, MoreTypes...>& input, MapFnType fun);

	template<template<typename, typename ...> class ContainerType, typename ValueType, typename ResultType, typename... MoreTypes>
	auto map(const ContainerType<ValueType, MoreTypes...>& input, ResultType(ValueType::*fun)() const)->ContainerType<ResultType, MoreTypes...>;

	//! foldr :: 
	template<template<typename...> class Iteratable, typename InValue, typename OutValue, typename Fun, typename... ExtraArgs>
	OutValue foldr(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable);

	//! foldl :: 
	template<template<typename...> class Iteratable, typename Fun, typename InValue, typename OutValue, typename... ExtraArgs>
	OutValue foldl(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable);

	//! zipWith :: [a] -> [b] -> (a -> b -> c) -> [c]
	template<typename OutContainer = Container<OutValue, ExtraArgs...>, template<typename...> class Container, typename Fun, typename LhsValue, typename RhsValue, typename OutValue = decltype(std::declval<Fun>()(std::declval<LhsValue>(), std::declval<RhsValue>())), typename... ExtraArgs>
	OutContainer zipWith(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs, Fun f);

	//! zip :: [a] -> [b] -> (a -> b -> c) -> [c]
	template<typename OutContainer = Container<std::pair<LhsValue, RhsValue>, ExtraArgs...>, template<typename...> class Container, typename LhsValue, typename RhsValue, typename... ExtraArgs>
	OutContainer zip(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs);

	template<typename T>
	class Range
	{
	public:
		Range(T from, T to)
			: m_from(from)
			, m_to(to)
		{
		}

		class Itr
		{
		public:
			bool operator!= (const Itr& other) const
			{
				return m_pos != other.m_pos;
			}

			T operator* () const
			{
				return m_pos;
			}

			const Itr& operator++ ()
			{
				++m_pos;
				return *this;
			}

		private:
			friend class Range<T>;

			Itr(T pos)
				: m_pos(pos)
			{
			}

			T m_pos;
		};

		Itr begin() const { return Itr(m_from); }
		Itr end() const { return Itr(m_to); }

	private:
		const T m_from;
		const T m_to;
	};

	template<typename T>
	Range<T> range(T from, T to)
	{
		return Range<T>(from, to);
	}
};

template < typename COut, typename CIn >
auto reserve(COut& outc, CIn inc) -> decltype(outc.reserve(inc.size()))
{
	outc.reserve(inc.size());
}

static void reserve(...)
{
}

template<template<typename, typename ...> class ContainerType, typename ValueType, typename MapFnType, typename ResultType, typename... MoreTypes, typename OutputType>
OutputType functional::map(const ContainerType<ValueType, MoreTypes...>& input, MapFnType fun)
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
auto functional::map(const ContainerType<ValueType, MoreTypes...>& input, ResultType(ValueType::*fun)() const) -> ContainerType<ResultType, MoreTypes...>
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
OutValue functional::foldr(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable)
{
	OutValue res = neutralValue;
	for (InValue value : iteratable)
	{
		res = f(value, res);
	}
	return res;
}

template<template<typename...> class Iteratable, typename Fun, typename InValue, typename OutValue, typename... ExtraArgs>
OutValue functional::foldl(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable)
{
	OutValue res = neutralValue;
	for (InValue value : iteratable)
	{
		res = f(res, value);
	}
	return res;
}

template<typename OutContainer = Container<OutValue, ExtraArgs...>, template<typename...> class Container, typename Fun, typename LhsValue, typename RhsValue, typename OutValue, typename... ExtraArgs>
OutContainer functional::zipWith(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs, Fun f)
{
	OutContainer out;
	auto lhsIt = lhs.begin();
	auto rhsIt = rhs.begin();
	while (lhsIt != lhs.end() && rhsIt != rhs.end())
	{
		out.push_back(f(*lhsIt, *rhsIt))
	}
	return out;
}

template<typename OutContainer = Container<std::pair<LhsValue, RhsValue>, ExtraArgs...>, template<typename...> class Container, typename LhsValue, typename RhsValue, typename... ExtraArgs>
OutContainer functional::zip(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs)
{
	return zipWith(lhs, rhs, [](LhsValue l, RhsValue r) { return std::make_pair(l, r); });
}


#endif