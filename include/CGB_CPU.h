#ifndef CGB_CPU_H
#define CGB_CPU_H
#include <fstream>
#include <iostream>
#include "Utilities.h"
#include "MemoryModule.h"

enum REG_TYPE
{
    A,// Accumulator
    F,// Flags //7-(Z)Zero 6-(N)Subtract 5-(H)Half Carry 4-(C)Carry 3-0 not used(set to 0)
    B,
    C,
    D,
    E,
    H,
    L
};

enum OPFlag
{
    ADD,
    SUB
};

enum Flags
{
    fZ,
    fN,
    fH,
    fC
};

class CGB_CPU
{
public:
    CGB_CPU();
    virtual ~CGB_CPU();

    void SetCartridge(char *Cart);

    void LoadBIOS(std::string Filepath);

    void Run(void);

private:

    uint8_t Fetch(void);

    void ExecOP(uint8_t OP);

    void ExecCBOP(uint8_t OP);

    uint16_t SP        =0xfffe;

    uint16_t PC        =0x0150;

    char registers[7]   ={0};

    float ClockSpd      =8.338; // this is double speed

    bool isRunning = true;

    char *BIOS;

    char *Cartridge;

    MemoryModule *Memory;

    uint8_t FlaggedOP(OPFlag OFlags, uint8_t Value, uint8_t Value2);

    void SetFlag(Flags Flag, bool Value);

};

#endif // CGB_CPU_H
