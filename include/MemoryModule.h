#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H
#include <iostream>
#include "Types.h"

class MemoryModule
{
public:
    MemoryModule(const unsigned Size);
    virtual ~MemoryModule();

    char ReadFromAddress(int16_t Address);

    void WriteToAddress(int16_t Address, char Data);

    void LoadROMChunk(int16_t Position, int16_t Size, int16_t DataPosition, char *Data);

    void ROMBankSwap(int16_t DataPosition, char *Data);

private:

    char *Memory;

};

#endif // MEMORYMODULE_H
