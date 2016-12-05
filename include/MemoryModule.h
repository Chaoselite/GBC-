#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H
#include <iostream>
#include "Types.h"

class MemoryModule
{
public:
    MemoryModule(const unsigned Size);
    virtual ~MemoryModule();

    char ReadFromAddress(uint16_t Address);

    void WriteToAddress(uint16_t Address, char Data);

    void LoadROMChunk(uint16_t Position, uint16_t Size, uint16_t DataPosition, char *Data);

    void ROMBankSwap(uint16_t DataPosition, char *Data);

private:

    char *Memory;

};

#endif // MEMORYMODULE_H
