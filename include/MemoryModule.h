#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H
#include "Types.h"


class MemoryModule
{
public:
    MemoryModule(const unsigned Size);
    virtual ~MemoryModule();

    char ReadFromAddress(int16_t Address);

    void WriteToAddress(int16_t Address, char Data);

private:

    char *Memory;

};

#endif // MEMORYMODULE_H
