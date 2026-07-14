#ifndef THREADS_PLATFORM_MANAGER_H
#define THREADS_PLATFORM_MANAGER_H

 #if defined(_WIN32)

 #include <windows.h>
 #define PLATFORM_NAME "Windows"

 #elif defined(__linux__)

 #include <unistd.h>
 #include <pthread.h>
 #define PLATFORM_NAME "Linux"

 #else
  #error "OPERATING SYSTEM NOT COMPATIBLE"

 #endif

#endif
