#pragma once

#ifdef BZ_PLATFORM_WINDOWS
    #ifdef BZ_BUILD_DLL
        #define BOOZY_API __declspec(dllexport)
    #else
        #define BOOZY_API __declspec(dllimport)
    #endif
#else
    #error Boozy only supports Windows!
#endif

#ifdef BZ_ENABLE_ASSERTS
    #define BZ_ASSERT(x, ...) { if(!x) { BZ_ERROR("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
    #define BZ_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)