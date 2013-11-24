#ifndef _FUNCTIONAL_HPP_
#define _FUNCTIONAL_HPP_

#include "functional_impl.hpp"

namespace functional
{
	//! apply :: (a -> b) -> [a]
	template<typename Fun, template<typename, typename ...> class Iteratable, typename ValueType, typename... MoreTypes>
	void apply(Fun fun, const Iteratable<ValueType, MoreTypes...>& input);

	//! map :: (a -> b) -> [a] -> [b]
	template<typename MapFnType, template<typename, typename ...> class ContainerType, typename ValueType, typename... MoreTypes>
	auto map(MapFnType fun, const ContainerType<ValueType, MoreTypes...>& input) -> decltype(functional_impl::map(fun, input));

	//! foldr :: (a -> b -> b) -> b -> [a] -> b
	template<template<typename...> class Iteratable, typename InValue, typename OutValue, typename Fun, typename... ExtraArgs>
	OutValue foldr(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable);

	//! foldl :: (a -> b -> a) -> a -> [b] -> a
	template<template<typename...> class Iteratable, typename Fun, typename InValue, typename OutValue, typename... ExtraArgs>
	OutValue foldl(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable);

	//! zipWith :: [a] -> [b] -> (a -> b -> c) -> [c]
	template<template<typename...> class Container, typename Fun, typename LhsValue, typename RhsValue, typename OutValue = decltype(std::declval<Fun>()(std::declval<LhsValue>(), std::declval<RhsValue>())), typename... ExtraArgs, typename OutContainer = Container<OutValue, ExtraArgs...>>
	OutContainer zipWith(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs, Fun f);

	//! zip :: [a] -> [b] -> (a -> b -> c) -> [c]
	template<template<typename...> class Container, typename LhsValue, typename RhsValue, typename... ExtraArgs, typename OutContainer = Container<std::pair<LhsValue, RhsValue>, ExtraArgs...>>
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

	//! range :: a -> b -> [a..b]
	template<typename T>
	Range<T> range(T from, T to)
	{
		return Range<T>(from, to);
	}
};

template<typename Fun, template<typename, typename ...> class Iteratable, typename ValueType, typename... MoreTypes>
void functional::apply(Fun fun, const Iteratable<ValueType, MoreTypes...>& input)
{
	return functional_impl::apply(fun, input);
}

template<typename MapFnType, template<typename, typename ...> class ContainerType, typename ValueType, typename... MoreTypes>
auto functional::map(MapFnType fun, const ContainerType<ValueType, MoreTypes...>& input) -> decltype(functional_impl::map(fun, input))
{
	return functional_impl::map(fun, input);
}

template<template<typename...> class Iteratable, typename InValue, typename OutValue, typename Fun, typename... ExtraArgs>
OutValue functional::foldr(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable)
{
	return functional_impl::foldr(f, neutralValue, iteratable);
}

template<template<typename...> class Iteratable, typename Fun, typename InValue, typename OutValue, typename... ExtraArgs>
OutValue functional::foldl(Fun f, OutValue neutralValue, Iteratable<InValue, ExtraArgs...> iteratable)
{
	return functional_impl::foldl(f, neutralValue, iteratable);
}

template<template<typename...> class Container, typename Fun, typename LhsValue, typename RhsValue, typename OutValue, typename... ExtraArgs, typename OutContainer>
OutContainer functional::zipWith(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs, Fun f)
{
	return functional_impl::zipWith(lhs, rhs, f);
}

template<template<typename...> class Container, typename LhsValue, typename RhsValue, typename... ExtraArgs, typename OutContainer>
OutContainer functional::zip(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs)
{
	return functional_impl::zip(lhs, rhs);
}

#endif // _FUNCTIONAL_HPP_