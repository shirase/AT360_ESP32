#pragma once

#include <stdint.h>

typedef struct _dateTime_s {
    // full year
    uint16_t year;
    // 1-12
    uint8_t month;
    // 1-31
    uint8_t day;
    // 0-23
    uint8_t hours;
    // 0-59
    uint8_t minutes;
    // 0-59
    uint8_t seconds;
    // 0-999
    uint16_t millis;
} dateTime_t;

// millisecond time
typedef uint32_t timeMs_t;
#define TIMEMS_MAX UINT32_MAX

// microsecond time
#ifdef USE_64BIT_TIME
typedef uint64_t timeUs_t;
#define TIMEUS_MAX UINT64_MAX
#else
typedef uint32_t timeUs_t;
#define TIMEUS_MAX UINT32_MAX
#endif

typedef int32_t timeDelta_t;
#define TIMEDELTA_MAX INT32_MAX

// Milliseconds since Jan 1 1970
typedef int64_t rtcTime_t;

bool rtcHasTime(void);
bool rtcSetDateTime(dateTime_t *dt);