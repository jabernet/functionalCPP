#include <fstream>
#include <iostream>
#include <iomanip> 
#include <vector>
#include <list>
#include <stdint.h>
#include <tuple>
#include <string>

#include "functional.hpp"

template<typename T>
T factorial(T n)
{
	return functional::foldl([](T a, T b) { return a*b; }, (T)1, functional::range((T)1, n + 1));
}

int main(const int argc, const char* argv[])
{
	std::vector<int> v { 1, 2, 3, 4, };
	std::vector<int> v1 { 1, 2, 3, 4, };

	std::list<int> l { 1, 2, 3, 4 };

	auto sum1 = functional::foldr([] (int a, int b) { return a + b; }, 0, v);
	auto sum2 = functional::foldl([] (int a, int b) { return a + b; }, 0, v);

	auto fac1 = functional::foldl([](int a, int b) { return a * b; }, 1, v);

	auto vs = functional::map([](int a) { return std::to_string(a); }, v);
	auto ls = functional::map([](int a) { return std::to_string(a); }, l);

	std::cout << sum1 << std::endl;
	std::cout << sum2 << std::endl;

	std::cout << fac1 << std::endl;
	std::cout << factorial(4) << std::endl;
	std::cout << factorial(uint64_t(20)) << std::endl;

	auto v12 = functional::zip(v, v1);
	auto v1and2 = functional::zipWith(v, v1, [](int a, int b) { return a + b; });

	return 0;
}
