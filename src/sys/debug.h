

#ifndef NDEBUG

inline void fakeprintf(char*f,...){}
#define PRINTF fakeprintf

#else

#include <stdio.h>
#define PRINTF printf

#endif
