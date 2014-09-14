#include "applicator.hpp"

using namespace functional_impl::helpers;

void testIntValue(int i)
{
}

void testIntConstRef(const int& i)
{
}

void testIntRef(int& i)
{
}

int testIntValueReturn(int i)
{
    return i;
}

int testIntConstRefReturn(const int& i)
{
    return i;
}

int testIntRefReturn(int& i)
{
    return i;
}

auto testIntValueL = [](int i)
{
};

auto testIntRefL = [](int& i)
{
};

auto testIntConstRefL = [](const int& i)
{
};

auto testIntValueReturnL = [](int i) -> int
{
    return i;
};

auto testIntRefReturnL = [](int& i) -> int
{
    return i;
};

auto testIntConstRefReturnL = [](const int& i) -> int
{
    return i;
};

struct T
{
    void testIntValue(int i)
    {
    }

    void testIntValueConst(int i) const
    {
    }

    void testIntRef(int& i)
    {
    }

    void testIntRefConst(int& i) const
    {
    }

    void testIntConstRef(const int& i)
    {
    }

    void testIntConstRefConst(const int& i) const
    {
    }

    int testIntValueReturn(int i)
    {
        return i;
    }

    int testIntValueConstReturn(int i) const
    {
        return i;
    }

    int testIntRefReturn(int& i)
    {
        return i;
    }

    int testIntRefConstReturn(int& i) const
    {
        return i;
    }

    int testIntConstRefReturn(const int& i)
    {
        return i;
    }

    int testIntConstRefConstReturn(const int& i) const
    {
        return i;
    }

};



void test()
{
    T t;
    int i = 0;

    // void return

// pass by value

    Applicator<void(*)(int)> { testIntValue } (1);
    Applicator<decltype(testIntValue)> { testIntValue } (1);
    Applicator<decltype(&testIntValue)> { testIntValue } (1);

    Applicator<decltype(testIntValueL)> { testIntValueL } (1);

    Applicator<decltype(&T::testIntValue)> { &T::testIntValue } (t, 1);
    Applicator<decltype(&T::testIntValueConst)> { &T::testIntValueConst } (t, 1);

// pass by ref

    Applicator<void(*)(int&)> { testIntRef } (i);
    Applicator<decltype(testIntRef)> { testIntRef } (i);
    Applicator<decltype(&testIntRef)> { testIntRef } (i);

    Applicator<decltype(testIntRefL)> { testIntRefL } (i);

    Applicator<decltype(&T::testIntRef)> { &T::testIntRef } (t, i);
    Applicator<decltype(&T::testIntRefConst)> { &T::testIntRefConst } (t, i);

// pass by const ref

    Applicator<void(*)(const int&)> { testIntConstRef } (i);
    Applicator<decltype(testIntConstRef)> { testIntConstRef } (i);
    Applicator<decltype(&testIntConstRef)> { testIntConstRef } (i);

    Applicator<decltype(testIntConstRefL)> { testIntConstRefL } (i);

    Applicator<decltype(&T::testIntConstRef)> { &T::testIntConstRef } (t, i);
    Applicator<decltype(&T::testIntConstRefConst)> { &T::testIntConstRefConst } (t, i);

// pass by const ref, rvalue

    Applicator<void(*)(const int&)> { testIntConstRef } (1);
    Applicator<decltype(testIntConstRef)> { testIntConstRef } (1);
    Applicator<decltype(&testIntConstRef)> { testIntConstRef } (1);

    Applicator<decltype(testIntConstRefL)> { testIntConstRefL } (1);

    Applicator<decltype(&T::testIntConstRef)> { &T::testIntConstRef } (t, 1);
    Applicator<decltype(&T::testIntConstRefConst)> { &T::testIntConstRefConst } (t, 1);


    // int return

// pass by value

    i = Applicator<int(*)(int)> { testIntValueReturn } (1);
    i = Applicator<decltype(testIntValueReturn)> { testIntValueReturn } (1);
    i = Applicator<decltype(&testIntValueReturn)> { testIntValueReturn } (1);

    i = Applicator<decltype(testIntValueReturnL)> { testIntValueReturnL } (1);

    i = Applicator<decltype(&T::testIntValueReturn)> { &T::testIntValueReturn } (t, 1);
    i = Applicator<decltype(&T::testIntValueConstReturn)> { &T::testIntValueConstReturn } (t, 1);

// pass by ref

    i = Applicator<int(*)(int&)> { testIntRefReturn } (i);
    i = Applicator<decltype(testIntRefReturn)> { testIntRefReturn } (i);
    i = Applicator<decltype(&testIntRefReturn)> { testIntRefReturn } (i);

    i = Applicator<decltype(testIntRefReturnL)> { testIntRefReturnL } (i);

    i = Applicator<decltype(&T::testIntRefReturn)> { &T::testIntRefReturn } (t, i);
    i = Applicator<decltype(&T::testIntRefConstReturn)> { &T::testIntRefConstReturn } (t, i);

// pass by const ref

    i = Applicator<int(*)(const int&)> { testIntConstRefReturn } (i);
    i = Applicator<decltype(testIntConstRefReturn)> { testIntConstRefReturn } (i);
    i = Applicator<decltype(&testIntConstRefReturn)> { testIntConstRefReturn } (i);

    i = Applicator<decltype(testIntConstRefReturnL)> { testIntConstRefReturnL } (i);

    i = Applicator<decltype(&T::testIntConstRefReturn)> { &T::testIntConstRefReturn } (t, i);
    i = Applicator<decltype(&T::testIntConstRefConstReturn)> { &T::testIntConstRefConstReturn } (t, i);

// pass by const ref, rvalue

    i = Applicator<int(*)(const int&)> { testIntConstRefReturn } (1);
    i = Applicator<decltype(testIntConstRefReturn)> { testIntConstRefReturn } (1);
    i = Applicator<decltype(&testIntConstRefReturn)> { testIntConstRefReturn } (1);

    i = Applicator<decltype(testIntConstRefReturnL)> { testIntConstRefReturnL } (1);

    i = Applicator<decltype(&T::testIntConstRefReturn)> { &T::testIntConstRefReturn } (t, 1);
    i = Applicator<decltype(&T::testIntConstRefConstReturn)> { &T::testIntConstRefConstReturn } (t, 1);

}