#include <fstream>
#include <iostream>
#include <iomanip> 
#include <vector>
#include <list>
#include <stdint.h>
#include <stdlib.h>
#include <tuple>
#include <typeinfo>
#include <string>
#include <sstream>
#include <functional>
#include <array>

#include "functional.hpp"

#if defined(__GNUC__)
    #define noinline __attribute__((noinline)) 
#elif defined(_MSC_VER)
    #define noinline __declspec(noinline) 
#else 
    #define noinline 
#endif

template<typename T>
T factorial(T n)
{
    return functional::foldl([](T a, T b) { return a*b; }, (T)1, functional::range((T)1, n + 1));
}

template class std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

class string: public std::string
{
public:
    using std::string::string;

    template<typename... Args>
    string(Args... args)
        : std::string(args...)
    {
    }

    void print() const { std::cout << *this << " "; }

    int to_int() const { return atoi(c_str()); }
};

template<typename T>
string to_string(T i)
{
    std::stringstream stream;
    stream << i;
    return stream.str();
}

void print(const std::string& s)
{
    std::cout << s << " ";
}

struct Printer
{
    template<typename T>
    void operator() (T t) const
    {
        std::cout << t << " ";
    }
};


std::vector<int> v { 1, 2, 3, 4, };
std::vector<string> vs { "1", "2", "3", "4" };

std::list<int> l { 1, 2, 3, 4 };
std::list<string> ls { "1", "2", "3", "4" };

std::tuple<int, int, int, int> t(std::make_tuple(1, 2, 3, 4));
std::tuple<string, string, string, string> ts(std::make_tuple("1", "2", "3", "4"));
std::tuple<int, string, int, string> tm(std::make_tuple(1, "'2'", 3, "'4'"));

std::array<int, 4> a { 1, 2, 3, 4};
std::array<string, 4> as { "1", "2", "3", "4" };
std::array<string, 500> ba;


noinline void testApplyVectorLambda()
{
    std::cout << "testApplyVectorLambda: ";
    functional::apply([](int i) { printf("%d ", i);  }, v);
    std::cout << std::endl;
}

noinline void testApplyVectorMemberFn()
{
    std::cout << "testApplyVectorMemberFn: ";
    functional::apply(&string::print, vs);
    std::cout << std::endl;
}

noinline void testApplyVectorFunctionPtr()
{
    std::cout << "testApplyVectorFunctionPtr: ";
    functional::apply(print, vs);
    std::cout << std::endl;
}

noinline void testApplyListLambda()
{
    std::cout << "testApplyListLambda: ";
    functional::apply([](int i) { printf("%d ", i);  }, l);
    std::cout << std::endl;
}

noinline void testApplyListMemberFn()
{
    std::cout << "testApplyListMemberFn: ";
    functional::apply(&string::print, ls);
    std::cout << std::endl;
}

noinline void testApplyListFunctionPtr()
{
    std::cout << "testApplyListFunctionPtr: ";
    functional::apply(print, ls);
    std::cout << std::endl;
}

noinline void testApplyTupleLambda()
{
    std::cout << "testApplyTupleLambda: ";
    functional::apply([](int i) { printf("%d ", i);  }, t);
    std::cout << std::endl;
}

noinline void testApplyTupleMemberFn()
{
    std::cout << "testApplyTupleMemberFn: ";
    functional::apply(&string::print, ts);
    std::cout << std::endl;
}

noinline void testApplyTupleFunctionPtr()
{
    std::cout << "testApplyTupleFunctionPtr: ";
    functional::apply(print, ts);
    std::cout << std::endl;
}

noinline void testApplyMixedTupleGenericFunctor()
{
    std::cout << "testApplyMixedTupleGenericFunctor: ";
    functional::apply(Printer(), tm);
    std::cout << std::endl;
}

noinline void testApplyArrayLambda()
{
    std::cout << "testApplyArrayLambda: ";
    functional::apply([](int i) { printf("%d ", i);  }, a);
    std::cout << std::endl;
}

noinline void testApplyArrayMemberFn()
{
    std::cout << "testApplyArrayMemberFn: ";
    functional::apply(&string::print, as);
    std::cout << std::endl;
}

noinline void testApplyArrayFunctionPtr()
{
    std::cout << "testApplyArrayFunctionPtr: ";
    functional::apply(print, as);
    std::cout << std::endl;
}

noinline void testApplyBigArrayLambda()
{
    std::cout << "testApplyBigArrayLambda: ";
    functional::apply([](const string& s) { std::cout << s << " ";  }, ba);
    std::cout << std::endl;
}

noinline void testApplyBigArrayMemberFn()
{
    std::cout << "testApplyBigArrayMemberFn: ";
    functional::apply(&string::print, ba);
    std::cout << std::endl;
}

noinline void testApplyBigArrayFunctionPtr()
{
    std::cout << "testApplyBigArrayFunctionPtr: ";
    functional::apply(print, ba);
    std::cout << std::endl;
}

noinline void testMapVectorLambda()
{
    std::cout << "testMapVectorLambda: ";
    auto res = functional::map([](int a) { return to_string(a); }, v);
    functional::apply(Printer(), res);
    std::cout << " : " << typeid(res).name() << std::endl;
}

noinline void testMapVectorMemberFn()
{
    std::cout << "testMapVectorMemberFn: ";
    auto res = functional::map(&string::to_int, vs);
    functional::apply(Printer(), res);
    std::cout << " : " << typeid(res).name() << std::endl;
}

noinline void testMapVectorFunctionPtr()
{
    std::cout << "testMapVectorFunctionPtr: ";
    auto res = functional::map(to_string<int>, v);
    functional::apply(Printer(), res);
    std::cout << " : " << typeid(res).name() << std::endl;
}

noinline void testMapListLambda()
{
    std::cout << "testMapListLambda: ";
    auto res = functional::map([](int a) { return to_string(a); }, l);
    functional::apply(Printer(), res);
    std::cout << " : " << typeid(res).name() << std::endl;
}

noinline void testMapListMemberFn()
{
    std::cout << "testMapListMemberFn: ";
    auto res = functional::map(&string::to_int, ls);
    functional::apply(Printer(), res);
    std::cout << " : " << typeid(res).name() << std::endl;
}

noinline void testMapListFunctionPtr()
{
    std::cout << "testMapListFunctionPtr: ";
    auto res = functional::map(to_string<int>, l);
    functional::apply(Printer(), res);
    std::cout << " : " << typeid(res).name() << std::endl;
}

int main(const int argc, const char* argv[])
{
    ba.fill(string("-"));

    testApplyVectorLambda();
    testApplyVectorMemberFn();
    testApplyVectorFunctionPtr();

    testApplyListLambda();
    testApplyListMemberFn();
    testApplyListFunctionPtr();

    testApplyTupleLambda();
    testApplyTupleMemberFn();
    testApplyTupleFunctionPtr();
    testApplyMixedTupleGenericFunctor();

    testApplyArrayLambda();
    testApplyArrayMemberFn();
    testApplyArrayFunctionPtr();

    testApplyBigArrayLambda();
    testApplyBigArrayMemberFn();
    testApplyBigArrayFunctionPtr();

    testMapVectorLambda();
    testMapVectorMemberFn();
    testMapVectorFunctionPtr();

    testMapListLambda();
    testMapListMemberFn();
    testMapListFunctionPtr();


    auto sum1 = functional::foldr([] (int a, int b) { return a + b; }, 0, v);
    auto sum2 = functional::foldl([] (int a, int b) { return a + b; }, 0, v);
    auto sum3 = functional::foldl([] (int a, int b) { return a + b; }, 0, l);

    auto fac1 = functional::foldl([](int a, int b) { return a * b; }, 1, v);

    std::cout << sum1 << " " << sum2 << " " << sum3 << " " << fac1 << std::endl;
    
    auto vs = functional::map([](int a) { return to_string(a); }, v);

    std::cout << typeid(vs).name() << std::endl;
    std::cout << typeid(std::vector<std::string>).name() << std::endl;

    std::vector<std::string> vvs{ "1", "2", "3" };

    auto cs = functional::map(&std::string::length, vvs);

    std::cout << typeid(cs).name() << std::endl;

    auto ls = functional::map([](int a) { return to_string(a); }, l);

    std::cout << sum1 << std::endl;
    std::cout << sum2 << std::endl;

    std::cout << fac1 << std::endl;
    std::cout << factorial(4) << std::endl;
    std::cout << factorial(uint64_t(20)) << std::endl;

    auto v12 = functional::zip(v, v);
    auto v1and2 = functional::zipWith([](int a, int b) { return a + b; }, v, v);

    return 0;
}
