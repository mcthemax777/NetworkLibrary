#pragma once

///windows flag
#define PLATFORM_WINDOWS    0

///linux flag
#define PLATFORM_LINUX      1

///mac flag
#define PLATFORM_MAC        2

///set flag
#define OS_PLATFORM         PLATFORM_WINDOWS


#if OS_PLATFORM == PLATFORM_LINUX

#elif OS_PLATFORM == PLATFORM_WINDOWS

#elif OS_PLATFORM == PLATFORM_MAC

#endif