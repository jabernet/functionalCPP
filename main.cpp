#include <fstream>
#include <iostream>
#include <iomanip> 
#include <vector>
#include <stdint.h>
#include <tuple>
#include <string>
#include <type_traits>

namespace functional
{
    template<template<typename,typename ...> class ContainerType, typename ValueType, typename MapFnType, typename ResultType = decltype(std::declval<MapFnType>()(std::declval<ValueType>())), typename... MoreTypes, typename OutputType = ContainerType<ResultType, MoreTypes...> >
    OutputType map(const ContainerType<ValueType, MoreTypes...>& input, MapFnType fun);

    template<template<typename,typename ...> class ContainerType, typename ValueType, typename ResultType, typename... MoreTypes>
	auto map(const ContainerType<ValueType, MoreTypes...>& input, ResultType(ValueType::*fun)() const)->ContainerType<ResultType, MoreTypes...>;
//
//    template<typename, typename V = bool>
//    struct _has_reserve: public std::false_type { };
//
//    template<typename Container>
//    struct _has_reserve<Container,
//        typename enable_if<
//is_same<decltype(declval<Container>().reserve(0)), void>::value,
//            bool
//            >::type
//        > : true_type { };
//
//    template<typename InputContainer, typename OutputContainer, bool hasReserve = _has_reserve<OutputContainer>::value>
//    struct _reserveHelper;
//
//    template<typename InputContainer, typename OutputContainer>
//    struct _reserveHelper<InputContainer, OutputContainer, true>
//    {
//        static void execute(OutputContainer & output, const InputContainer& input)
//        {
//            output.reserve(input.size());
//        }
//    };
//
//    template<typename InputContainer, typename OutputContainer>
//    struct _reserveHelper<InputContainer, OutputContainer, false>
//    {
//        static void execute(OutputContainer &, const InputContainer&)
//        {
//        }
//    };

    template<template<typename,typename ...> class ContainerType, typename ValueType, typename MapFnType, typename ResultType = decltype(std::declval<MapFnType>()(std::declval<ValueType>())), typename... MoreTypes, typename OutputType = ContainerType<ResultType, MoreTypes...> >
	OutputType map(const ContainerType<ValueType, MoreTypes...>& input, MapFnType fun)
    {
        typedef ContainerType<ValueType, MoreTypes...> InputType;
        OutputType output;
//        _reserveHelper<InputType, OutputType>::execute(output, input);
        for(const ValueType& value: input)
        {
            output.push_back(fun(value));
        }
        return output;
    }

    template<template<typename,typename ...> class ContainerType, typename ValueType, typename ResultType, typename... MoreTypes>
	auto map(const ContainerType<ValueType, MoreTypes...>& input, ResultType(ValueType::*fun)() const) -> ContainerType<ResultType, MoreTypes...>
    {
        typedef ContainerType<ValueType, MoreTypes...> InputType;
        typedef ContainerType<ResultType, MoreTypes...> OutputType;
        OutputType output;
//        _reserveHelper<InputType, OutputType>::execute(output, input);
        for(const ValueType& value: input)
        {
            output.push_back((value.*fun)());
        }
        return output;
    } 

	//! foldr :: 
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

	//! foldl :: 
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

	//! zipWith :: [a] -> [b] -> (a -> b -> c) -> [c]
	template<typename OutContainer = Container<OutValue, ExtraArgs...>, template<typename...> class Container, typename Fun, typename LhsValue, typename RhsValue, typename OutValue = decltype(std::declval<Fun>()(std::declval<LhsValue>(), std::declval<RhsValue>())), typename... ExtraArgs>
	OutContainer zipWith(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs, Fun f)
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

	//! zip :: [a] -> [b] -> (a -> b -> c) -> [c]
	template<typename OutContainer = Container<std::pair<LhsValue, RhsValue>, ExtraArgs...>, template<typename...> class Container, typename LhsValue, typename RhsValue, typename... ExtraArgs>
	OutContainer zip(Container<LhsValue, ExtraArgs...> lhs, Container<RhsValue, ExtraArgs...> rhs)
	{
		return zipWith(lhs, rhs, [](LhsValue l, RhsValue r) { return std::make_pair(l, r); });
	}

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

template<typename T>
T factorial(T n)
{
	return functional::foldl([](T a, T b) { return a*b; }, (T)1, functional::range((T)1, n + 1));
}

int main(const int argc, const char* argv[])
{
	std::vector<int> v { 1, 2, 3, 4, };
	std::vector<int> v1 { 1, 2, 3, 4, };

	auto sum1 = functional::foldr([] (int a, int b) { return a + b; }, 0, v);
	auto sum2 = functional::foldl([] (int a, int b) { return a + b; }, 0, v);

	auto fac1 = functional::foldl([](int a, int b) { return a * b; }, 1, v);

	auto vs = functional::map(v, [](int a) { return std::to_string(a); });

	std::cout << sum1 << std::endl;
	std::cout << sum2 << std::endl;

	std::cout << fac1 << std::endl;
	std::cout << factorial(4) << std::endl;
	std::cout << factorial(uint64_t(20)) << std::endl;


//	auto v12 = functional::zip(v, v1);
//	auto v1and2 = functional::zipWith(v, v1, [](int a, int b) { return a + b; });

	return 0;
}
