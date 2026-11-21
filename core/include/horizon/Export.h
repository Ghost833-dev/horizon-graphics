#pragma once

// Export/import macro
#ifdef _WIN32
  #ifdef HORIZON_BUILD_SHARED
    #define HORIZON_EXPORT __declspec(dllexport)
  #else
    #define HORIZON_EXPORT __declspec(dllimport)
  #endif
#else
  #ifdef HORIZON_BUILD_SHARED
    #define HORIZON_EXPORT __attribute__((visibility("default")))
  #else
    #define HORIZON_EXPORT
  #endif
#endif

// Macros for each module
#define HORIZON_CORE_API  HORIZON_EXPORT
#define HORIZON_RHI_API   HORIZON_EXPORT
