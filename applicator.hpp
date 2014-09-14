#ifndef _APPLICATOR_HPP_
#define _APPLICATOR_HPP_

#if defined(__GNUC__)
#define forceinline __attribute__((always_inline)) inline
#define noinline __attribute__((noinline)) 
#define PACKED __attribute__ ((__packed__))
#elif defined(_MSC_VER)
#define forceinline __forceinline 
#define noinline __declspec(noinline) 
#define PACKED
#else 
#define forceinline inline 
#define noinline 
#define PACKED
#endif

#include <utility>

namespace functional_impl
{
    namespace helpers
    {
        // helper struct to apply functors and function pointers
        template<typename Fun>
        struct Applicator
        {
            Fun f;

            template<typename... Args>
            forceinline auto operator () (Args&&... args) const -> decltype(f(std::forward<Args>(args)...))
            {
                return f(std::forward<Args>(args)...);
            }
        };

        // helper struct to map function types to function pointers
        template<typename ReturnType, typename... Args>
        struct Applicator<ReturnType(Args...)>
        {
            typedef ReturnType(*Fun)(Args...);

            Fun f;

            template<typename... Args>
            forceinline auto operator () (Args&&... args) const -> decltype(f(std::forward<Args>(args)...))
            {
                return f(std::forward<Args>(args)...);
            }
        };

        // specialization for member function pointers
        template<typename ReturnType, typename ValueType, typename... Args>
        struct Applicator<ReturnType(ValueType::*)(Args...)>
        {
            typedef ReturnType(ValueType::*Fun)(Args...);

            Fun f;

            template<typename... Args>
            forceinline auto operator () (ValueType& thisArg, Args&&... args) const -> decltype((thisArg.*f)(std::forward<Args>(args)...))
            {
                return (thisArg.*f)(std::forward<Args>(args)...);
            }
        };

        // specialization for const member function pointers
        template<typename ReturnType, typename ValueType, typename... Args>
        struct Applicator<ReturnType(ValueType::*)(Args...) const>
        {
            typedef ReturnType(ValueType::*Fun)(Args...) const;

            Fun f;

            template<typename... Args>
            forceinline auto operator () (const ValueType& thisArg, Args&&... args) const -> decltype((thisArg.*f)(std::forward<Args>(args)...))
            {
                return (thisArg.*f)(std::forward<Args>(args)...);
            }
        };
    };
};

#endif