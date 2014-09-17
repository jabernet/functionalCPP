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

// TODO:
// - write zipwith so that out container can be specified as first paramter

namespace functional
{
    //! apply :: (a -> b) -> [a]
    template<typename Fun, typename Iteratable>
    void apply(Fun fun, Iteratable& inout);

    //! map :: (a -> b) -> [a] -> [b]
    template<typename ResultContainer = _Derived, typename Fun, typename Container>
    auto map(Fun fun, const Container& input) -> decltype(functional_impl::map<ResultContainer>(fun, input));

    //! foldr :: (a -> b -> b) -> b -> [a] -> b
    template<typename ResultType, typename Fun, typename Iteratable>
    ResultType foldr(Fun f, ResultType neutralValue, const Iteratable& iteratable);

    //! foldl :: (a -> b -> a) -> a -> [b] -> a
    template<typename ResultType, typename Fun, typename Iteratable>
    ResultType foldl(Fun f, ResultType neutralValue, const Iteratable& iteratable);

    //! zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
    template<typename Fun, typename LhsContainer, typename RhsContainer>
    auto zipWith(Fun f, const LhsContainer& lhs, const RhsContainer& rhs) -> decltype(functional_impl::zipWith(f, lhs, rhs));

    //! zip :: [a] -> [b] -> [(a,b)]
    template<typename LhsContainer, typename RhsContainer>
    auto zip(const LhsContainer& lhs, const RhsContainer& rhs) -> decltype(functional_impl::zip(lhs, rhs));
  
    //! curry: ((a,b,...) -> c) -> (a -> b -> ... -> c)
    template<typename Fun>
    auto curry(Fun f) -> decltype(functional_impl::curry(f));

    //! uncurry: (a -> b -> ... -> c) -> ((a,b,...) -> c)
    template<typename Fun>
    auto uncurry(Fun f) -> decltype(functional_impl::uncurry(f));

    template<typename T>
    class Range;

    //! range :: a -> b -> [a..b]
    template<typename T>
    Range<T> range(T from, T to);
};

template<typename Fun, typename Iteratable>
void functional::apply(Fun fun, Iteratable& inout)
{
    return functional_impl::apply(fun, inout);
}

template<typename ResultContainer, typename Fun, typename Container>
auto functional::map(Fun fun, const Container& input) -> decltype(functional_impl::map<ResultContainer>(fun, input))
{
    return functional_impl::map<ResultContainer>(fun, input);
}

template<typename ResultType, typename Fun, typename Iteratable>
ResultType functional::foldr(Fun f, ResultType neutralValue, const Iteratable& iteratable)
{
    return functional_impl::foldr<ResultType>(f, neutralValue, iteratable);
}

template<typename ResultType, typename Fun, typename Iteratable>
ResultType functional::foldl(Fun f, ResultType neutralValue, const Iteratable& iteratable)
{
    return functional_impl::foldl<ResultType>(f, neutralValue, iteratable);
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

template<typename Fun>
auto functional::curry(Fun f) -> decltype(functional_impl::curry(f))
{
    return functional_impl::curry(f);
}

template<typename Fun>
auto functional::uncurry(Fun f) -> decltype(functional_impl::uncurry(f))
{
    return functional_impl::uncurry(f);
}

template<typename T>
class functional::Range
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
functional::Range<T> functional::range(T from, T to)
{
    return { from, to };
}

#endif // _FUNCTIONAL_HPP_