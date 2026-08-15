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