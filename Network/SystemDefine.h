#pragma once

#define PLATFORM_WINDOWS    0
#define PLATFORM_LINUX      1
#define PLATFORM_MAC        2

#define OS_PLATFORM         PLATFORM_WINDOWS


#define SINGLE_THREAD       0
#define MULTI_THREAD        1

#define THREAD_TYPE         MULTI_THREAD



#if OS_PLATFORM == PLATFORM_LINUX

#elif OS_PLATFORM == PLATFORM_WINDOWS

#elif OS_PLATFORM == PLATFORM_MAC

#endif