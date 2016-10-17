#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED
#include "stdint.h"
#include <bitset>

inline uint8_t trunc8high(uint16_t Value)
{
    uint16_t Result = Value;
    return Result  >> 8;
}

inline uint8_t trunc8low(uint16_t Value)
{
    uint16_t Result = Value;
    return Result << 8 >> 8;
}

inline uint16_t concat16(uint8_t High, uint8_t Low)
{
    uint16_t Result = 0;
    Result = High << 8;
    Result += Low;

    return Result;
}

inline uint8_t setbit8(uint8_t Data, unsigned Position, bool Value)
{
    uint8_t Result = 0;
    std::bitset<8> Work (Data);
    Work.set(Position, Value);
    Result = Work.to_ulong();
}

inline bool getbit8(uint8_t Data, unsigned Position)
{
    std::bitset<8> Work (Data);
    return Work.test(Position);
}

#endif // UTILITIES_H_INCLUDED
