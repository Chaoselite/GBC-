#ifndef CGB_CPU_H
#define CGB_CPU_H
#include <assert.h>
#include "Types.h"
#include <iostream>

enum REG_TYPE
{
    A =0,// Accumulator
    F =1,// Flags //7-(Z)Zero 6-(N)Subtract 5-(H)Half Carry 4-(C)Carry 3-0 not used(set to 0)
    B =2,
    C =3,
    D =4,
    E =5,
    H =6,
    L =7

};

class CGB_CPU
{
public:
    CGB_CPU();
    virtual ~CGB_CPU();

    uint16_t  SP =0;
    uint16_t  PC =0;
    char  addr_bus_16 =0; // pretty sure we don't need this
    char   addr_bus_8 =0; //this
    char   reg_status =0; //or this but we'll see
    char  registers[7] = {0};

    float   ClockSpd = 4.194304;

    void Run(void);

private:

    void ExecOP(uint16_t OP, char Data);

    void ExecCBOP(uint16_t OP, char Data);

};

#endif // CGB_CPU_H
