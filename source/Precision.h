#ifndef SM64DS_PRECISION_INCLUDED
#define SM64DS_PRECISION_INCLUDED

#include "SM64DS_Common.h"


extern "C" uint64_t Div64(uint64_t n, uint64_t d);
extern "C" int DivQ24(int n, int d);


constexpr uint64_t fac(uint64_t n) // works if n > 0
{
    for (uint64_t i = n - 1; i > 1; i--)
    {
        n *= i;
    }
    return n;
}


Fix24i cos(short a)
{
    int b = a;
    if (b < 0) b = -b;

    bool flipped;

    if (b > 0x4000)
    {
        b = 0x8000 - b;
        flipped = true;
    }
    else flipped = false;

    uint64_t x = (b * 0x3243F6A8885A3 + (uint64_t(1) << 31)) >> 32; // Q31

    constexpr int64_t h = int64_t(1) << 31;

    uint64_t y2 = x * x;         // Q62
    uint64_t x2 = (y2 + h) >> 32; // Q30
    uint64_t y4 = x2 * x2;        // Q60
    uint64_t x4 = (y4 + h) >> 32; // Q28
    uint64_t y6 = x2 * x4;        // Q58
    uint64_t x6 = (y6 + h) >> 32; // Q26
    uint64_t y8 = x4 * x4;        // Q56
    uint64_t x8 = (y8 + h) >> 32; // Q24
    uint64_t y10 = x4 * x6;        // Q54
    uint64_t y12 = x6 * x6;        // Q52
    uint64_t y14 = x6 * x8;        // Q50
    uint64_t y16 = x8 * x8;        // Q48

    uint64_t resQ48 = (uint64_t(1) << 48)
        - (((Div64(y2 >> 1, fac(2)) << 1) + (uint64_t(1) << (23 + 14))) >> 14)
        + (((Div64(y4 >> 1, fac(4)) << 1) + (uint64_t(1) << (23 + 12))) >> 12)
        - (((Div64(y6 >> 1, fac(6)) << 1) + (uint64_t(1) << (23 + 10))) >> 10)
        + (((Div64(y8 >> 1, fac(8)) << 1) + (uint64_t(1) << (23 + 8))) >> 8)
        - (((Div64(y10 >> 1, fac(10)) << 1) + (uint64_t(1) << (23 + 6))) >> 6)
        + (((Div64(y12 >> 1, fac(12)) << 1) + (uint64_t(1) << (23 + 4))) >> 4)
        - (((Div64(y14 >> 1, fac(14)) << 1) + (uint64_t(1) << (23 + 2))) >> 2)
        + ((Div64(y16 >> 1, fac(16)) << 1));

    Fix24i resQ24((resQ48 + (1 << 23)) >> 24, true);

    return flipped ? -resQ24 : resQ24;
}

inline Fix24i sin(short a)
{
    return cos(a - 0x4000);
}

#endif	// SM64DS_PRECISION_INCLUDED