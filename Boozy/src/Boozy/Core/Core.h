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
    #define BZ_DEBUGBREAK() __debugbreak()
#elif defined(BZ_PLATFORM_LINUX)
    #define BOOZY_API
    #define BZ_DEBUGBREAK() __builtin_trap()
#else
    #error Boozy: unsupported platform!
#endif

#ifdef BZ_ENABLE_ASSERTS
    #define BZ_ASSERT(x, ...) do { if(!(x)) { BZ_ERROR("Assertion Failed: {}", __VA_ARGS__); BZ_DEBUGBREAK(); } } while(0)
#else
    #define BZ_ASSERT(x, ...) do { (void)sizeof(x); } while(0) // 引用一次变量防止 release 配置下警告未使用的变量
#endif

#define BIT(x) (1 << x)

#define BZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Boozy {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;
}