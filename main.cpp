#include <fstream>
#include <iostream>
#include <iomanip> 
#include <vector>
#include <list>
#include <stdint.h>
#include <tuple>
#include <typeinfo>
#include <string>
#include <sstream>
#include <functional>
#include <array>

#include "functional.hpp"

template<typename T>
T factorial(T n)
{
    return functional::foldl([](T a, T b) { return a*b; }, (T)1, functional::range((T)1, n + 1));
}

template class std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

template<typename T>
std::string to_string(T i)
{
    std::stringstream stream;
    stream << i;
    return stream.str();
}

struct Printer
{
    template<typename T>
    void operator() (T t)
    {
        std::cout << t << " ";
    }
};

int main(const int argc, const char* argv[])
{
    std::vector<int> v { 1, 2, 3, 4, };
    std::vector<int> v1 { 1, 2, 3, 4, };

    std::list<int> l { 1, 2, 3, 4 };

    std::tuple<int, int, int, int> it(std::make_tuple(1, 2, 3, 4));
    std::tuple<int, std::string, int, std::string> mixt(std::make_tuple(1, "'2'", 3, "'4'"));

    std::array<int, 4> a { 1, 2, 3, 4};
    std::array<std::string, 500> ba;
    ba.fill(std::string("-"));

    auto sum1 = functional::foldr([] (int a, int b) { return a + b; }, 0, v);
    auto sum2 = functional::foldl([] (int a, int b) { return a + b; }, 0, v);
    auto sum3 = functional::foldl([] (int a, int b) { return a + b; }, 0, l);

    auto fac1 = functional::foldl([](int a, int b) { return a * b; }, 1, v);

    std::cout << sum1 << " " << sum2 << " " << sum3 << " " << fac1 << std::endl;
    
    auto vs = functional::map([](int a) { return to_string(a); }, v);

    std::cout << typeid(vs).name() << std::endl;
    std::cout << typeid(std::vector<std::string>).name() << std::endl;

    std::vector<std::string> vvs{ "1", "2", "3" };

    functional::apply([](int i) { printf("%d", i);  }, v1);
    functional::apply(&std::string::length, vs);
    struct Test
    { 
        static void d(std::string s)
        {
            std::cout << s << " ";
        }
    };
    functional::apply(Test::d, vs);
    std::cout << "apply tuple: ";
    functional::apply([] (int i) { std::cout << i << " "; }, it);
    std::cout << std::endl;
    std::cout << "apply mixed: ";
    functional::apply(Printer(), it);
    std::cout << std::endl;
    std::cout << "apply array: ";
    functional::apply([] (int i) { std::cout << i << " "; }, a);
    std::cout << std::endl;
    std::cout << "apply big array: ";
    functional::apply([] (std::string s) { std::cout << s << " "; }, ba);
    std::cout << std::endl;

    auto cs = functional::map(&std::string::length, vvs);

    std::cout << typeid(cs).name() << std::endl;

    auto ls = functional::map([](int a) { return to_string(a); }, l);

    std::cout << sum1 << std::endl;
    std::cout << sum2 << std::endl;

    std::cout << fac1 << std::endl;
    std::cout << factorial(4) << std::endl;
    std::cout << factorial(uint64_t(20)) << std::endl;

    auto v12 = functional::zip(v, v1);
    auto v1and2 = functional::zipWith([](int a, int b) { return a + b; }, v, v1);

    return 0;
}
