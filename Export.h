// src/core/include/quantum/core/Export.h

#pragma once

// Export/import macro
#ifdef _WIN32
  #ifdef QUANTUM_BUILD_SHARED
    #define QUANTUM_EXPORT __declspec(dllexport)
  #else
    #define QUANTUM_EXPORT __declspec(dllimport)
  #endif
#else
  #ifdef QUANTUM_BUILD_SHARED
    #define QUANTUM_EXPORT __attribute__((visibility("default")))
  #else
    #define QUANTUM_EXPORT
  #endif
#endif

// Macros for each module
#define QUANTUM_CORE_API  QUANTUM_EXPORT
#define QUANTUM_RHI_API   QUANTUM_EXPORT
