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

#ifndef _FUNCTIONAL_HPP_
#define _FUNCTIONAL_HPP_

#include "functional_impl.hpp"

namespace functional
{
	//! apply :: (a -> b) -> [a]
	template<typename Fun, typename Iteratable>
	void apply(Fun fun, const Iteratable& input);

	//! map :: (a -> b) -> [a] -> [b]
	template<typename Fun, typename Container>
	auto map(Fun fun, const Container& input) -> decltype(functional_impl::map(fun, input));

	//! foldr :: (a -> b -> b) -> b -> [a] -> b
	template<typename Fun, typename OutValue, typename Iteratable>
	OutValue foldr(Fun f, OutValue neutralValue, const Iteratable& iteratable);

	//! foldl :: (a -> b -> a) -> a -> [b] -> a
	template<typename Fun, typename OutValue, typename Iteratable>
	OutValue foldl(Fun f, OutValue neutralValue, const Iteratable& iteratable);

	//! zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
    template<typename Fun, typename LhsContainer, typename RhsContainer>
    auto zipWith(Fun f, const LhsContainer& lhs, const RhsContainer& rhs) -> decltype(functional_impl::zipWith(f, lhs, rhs));

	//! zip :: [a] -> [b] -> [(a,b)]
    template<typename LhsContainer, typename RhsContainer>
    auto zip(const LhsContainer& lhs, const RhsContainer& rhs) -> decltype(functional_impl::zip(lhs, rhs));

    

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

template<typename Fun, typename Iteratable>
void functional::apply(Fun fun, const Iteratable& input)
{
	return functional_impl::apply(fun, input);
}

template<typename Fun, typename Container>
auto functional::map(Fun fun, const Container& input) -> decltype(functional_impl::map(fun, input))
{
	return functional_impl::map(fun, input);
}

template<typename Fun, typename OutValue, typename Iteratable>
OutValue functional::foldr(Fun f, OutValue neutralValue, const Iteratable& iteratable)
{
	return functional_impl::foldr(f, neutralValue, iteratable);
}

template<typename Fun, typename OutValue, typename Iteratable>
OutValue functional::foldl(Fun f, OutValue neutralValue, const Iteratable& iteratable)
{
	return functional_impl::foldl(f, neutralValue, iteratable);
}

template<typename Fun, typename LhsContainer, typename RhsContainer>
auto functional::zipWith(Fun f, const LhsContainer& lhs, const RhsContainer& rhs) -> decltype(functional_impl::zipWith(f, lhs, rhs))
{
	return functional_impl::zipWith(f, lhs, rhs);
}

template<typename LhsContainer, typename RhsContainer>
auto functional::zip(const LhsContainer& lhs, const RhsContainer& rhs) -> decltype(functional_impl::zip(lhs, rhs))
{
	return functional_impl::zip(lhs, rhs);
}

#endif // _FUNCTIONAL_HPP_