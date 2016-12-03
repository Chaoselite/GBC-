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
    return Result;
}

inline bool getbit8(uint8_t Data, unsigned Position)
{
    std::bitset<8> Work (Data);
    return Work.test(Position);
}

inline uint8_t trunc4high(uint8_t Data)
{
    uint8_t Result = Data;
    return Result >> 4;
}

inline uint8_t trunc4low(uint8_t Data)
{
    uint8_t Result = 0;
    std::bitset<8> Work (Data);
    Work.set(4, 0);
    Work.set(5, 0);
    Work.set(6, 0);
    Work.set(7, 0);
    Result = Work.to_ulong();
    return Result;
}

inline uint8_t concat8(uint8_t High, uint8_t Low)
{
    uint8_t Result = 0;
    Result = High << 4;
    Result += Low;

    return Result;
}

inline uint8_t or8bit(uint8_t Data1, uint8_t Data2)
{
    uint8_t Result = 0;
    std::bitset<8> Work1 (Data1);
    std::bitset<8> Work2 (Data2);
    std::bitset<8> WorkResult (Result);
    for (int i = 0; i != 8; i++)
    {
        if(Work1.test(i) || Work2.test(i))
        {
            WorkResult.set(i, 1);

        }
        else
        {
            WorkResult.set(i, 0);
        }
    }
    Result = WorkResult.to_ulong();
    return Result;
}

inline uint8_t xor8bit(uint8_t Data1, uint8_t Data2)
{
    uint8_t Result = 0;
    std::bitset<8> Work1 (Data1);
    std::bitset<8> Work2 (Data2);
    std::bitset<8> WorkResult (Result);
    for (int i = 0; i != 8; i++)
    {
        if(Work1.test(i) != Work2.test(i))
        {
            WorkResult.set(i, 1);

        }
        else
        {
            WorkResult.set(i, 0);
        }
    }
    Result = WorkResult.to_ulong();
    return Result;
}

inline uint8_t and8bit(uint8_t Data1, uint8_t Data2)
{
    uint8_t Result = 0;
    std::bitset<8> Work1 (Data1);
    std::bitset<8> Work2 (Data2);
    std::bitset<8> WorkResult (Result);
    for (int i = 0; i != 8; i++)
    {
        if(Work1.test(i) && Work2.test(i))
        {
            WorkResult.set(i, 1);

        }
        else
        {
            WorkResult.set(i, 0);
        }
    }
    Result = WorkResult.to_ulong();
    return Result;
}


#endif // UTILITIES_H_INCLUDED
