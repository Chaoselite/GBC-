#ifndef CGB_CPU_H
#define CGB_CPU_H
#include <fstream>
#include <iostream>
#include "Utilities.h"
#include "MemoryModule.h"
#include "assert.h"

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

enum INT_TYPE
{
    VBlank,
    LCDSTAT,
    Timer,
    Serial,
    Joypad
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

    bool IME = 0;

    void PowerOnSetup(void);

    uint8_t Fetch(void);

    void ExecInterrupt(void);

    void ExecOP(uint8_t OP);

    void ExecCBOP(uint8_t OP);

    uint16_t SP        =0xfffe;

    uint16_t PC        =0x100;

    char registers[7]   ={0};

    float ClockSpd      =8.338; // this is double speed

    bool isRunning = true;

    char *BIOS;

    char *Cartridge;

    MemoryModule *Memory;

    uint8_t FlaggedOP(OPFlag OFlags, uint8_t Value, uint8_t Value2);

    void SetFlag(Flags Flag, bool Value);

    bool GetFlag(Flags Flag);

    void StackPush(uint8_t Data);

    uint8_t StackPop(void);

    void Call(uint16_t Address);

    void Ret(void);

    void SetInterrupt(INT_TYPE INT, bool Value);

    void EnableInterrupt(INT_TYPE INT, bool Value);

    bool GetInterrupt(INT_TYPE INT);

    bool IsEnableInterrupt(INT_TYPE INT);

};

#endif // CGB_CPU_H
