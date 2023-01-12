#include "time.h"
#include <Arduino.h>

// rtcTime_t when the system was started.
// Calculated in rtcSet().
static rtcTime_t started = 0;

// For the "modulo 4" arithmetic to work, we need a leap base year
#define REFERENCE_YEAR 2000
// Offset (seconds) from the UNIX epoch (1970-01-01) to 2000-01-01
#define EPOCH_2000_OFFSET 946684800

#define MILLIS_PER_SECOND 1000

static const uint16_t days[4][12] =
{
    {   0,  31,     60,     91,     121,    152,    182,    213,    244,    274,    305,    335},
    { 366,  397,    425,    456,    486,    517,    547,    578,    609,    639,    670,    700},
    { 731,  762,    790,    821,    851,    882,    912,    943,    974,    1004,   1035,   1065},
    {1096,  1127,   1155,   1186,   1216,   1247,   1277,   1308,   1339,   1369,   1400,   1430},
};

bool rtcHasTime(void)
{
    return started != 0;
}

rtcTime_t rtcTimeMake(int32_t secs, uint16_t millis)
{
    return ((rtcTime_t)secs) * MILLIS_PER_SECOND + millis;
}

static rtcTime_t dateTimeToRtcTime(const dateTime_t *dt)
{
    unsigned int second = dt->seconds;  // 0-59
    unsigned int minute = dt->minutes;  // 0-59
    unsigned int hour = dt->hours;      // 0-23
    unsigned int day = dt->day - 1;     // 0-30
    unsigned int month = dt->month - 1; // 0-11
    unsigned int year = dt->year - REFERENCE_YEAR; // 0-99
    int32_t unixTime = (((year / 4 * (365 * 4 + 1) + days[year % 4][month] + day) * 24 + hour) * 60 + minute) * 60 + second + EPOCH_2000_OFFSET;
    return rtcTimeMake(unixTime, dt->millis);
}

bool rtcSet(rtcTime_t *t)
{
    started = *t - millis();
    return true;
}

bool rtcSetDateTime(dateTime_t *dt)
{
    rtcTime_t t = dateTimeToRtcTime(dt);
    return rtcSet(&t);
}