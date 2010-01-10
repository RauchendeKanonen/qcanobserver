#ifndef WINGETTIMEOFDAY_H
#define WINGETTIMEOFDAY_H
#include <windows.h>
#include <time.h>

struct timezone
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};


int gettimeofday(struct timeval *tv, struct timezone *tz);


#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

#endif // WINGETTIMEOFDAY_H
