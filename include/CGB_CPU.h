#ifndef CGB_CPU_H
#define CGB_CPU_H
#include <assert.h>
#include "Types.h"
#include <fstream>
#include <iostream>
#include "MemoryModule.h"

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

    void SetCartridge(char *Cart);

    void SetMemory(MemoryModule *Mem);

    void LoadBIOS(std::string Filepath);

    void Run(void);

    uint16_t  SP =0x0000;
    uint16_t  PC =0x0000;
    char  registers[7] = {0};
    float   ClockSpd = 4.194304;

private:

    char Fetch(uint16_t ADDR);

    void ExecOP(uint16_t OP, char Data);

    void ExecCBOP(uint16_t OP, char Data);

    char *BIOS;

    char *Cartridge;

    MemoryModule *Memory;

};

#endif // CGB_CPU_H
