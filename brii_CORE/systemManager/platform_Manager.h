#ifndef PLATFORM_MANAGER_H
#define PLATFORM_MANAGER_H 

#include "logger.h"

 #if defined(_WIN32)

 #include <windows.h>
 #define PLATFORM_NAME "Windows"

 #elif defined(__linux__) && defined(__unix) || defined(__unix) && defined(__MACH__)

 #include <unistd.h>
 #include <pthread.h>
 #include <fcntl.h>
 #define PLATFORM_NAME "Linux"

 #else
  #error "OPERATING SYSTEM NOT COMPATIBLE WITH THE ENGINE"

 #endif


#endif
