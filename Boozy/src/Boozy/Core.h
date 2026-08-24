#pragma once
#include <memory>

#ifdef BZ_PLATFORM_WINDOWS
    #ifdef BZ_DYNAMIC_LINK
        #ifdef BZ_BUILD_DLL
            #define BOOZY_API __declspec(dllexport)
        #else
            #define BOOZY_API __declspec(dllimport)
        #endif
    #else
        #define BOOZY_API
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

#define BZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Boozy {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;
}