// src/core/include/quantum/core/Export.h

#pragma once

#ifdef _WIN32
  #ifdef QUANTUM_CORE_BUILD
    #define QUANTUM_CORE_API __declspec(dllexport)
  #else
    #define QUANTUM_CORE_API __declspec(dllexport)
  #endif // QUANTUM_CORE_BUILD
#else
  #ifdef QUANTUM_CORE_BUILD
    #define QUANTUM_CORE_API __attribute__((visibility("default")))
  #else
    #define QUANTUM_CORE_API
  #endif // QUANTUM_CORE_BUILD
#endif // _WIN32
