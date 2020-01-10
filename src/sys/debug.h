

#ifndef NDEBUG

inline void fakeprintf(char*f,...){}
#define PRINTF fakeprintf

#else

#define PRINTF printf

#endif
