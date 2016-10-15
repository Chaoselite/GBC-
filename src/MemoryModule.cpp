#include "MemoryModule.h"

MemoryModule::MemoryModule(const unsigned Size)
{
    Memory = new char[Size];
}

MemoryModule::~MemoryModule()
{
    delete [] Memory;
}

char MemoryModule::ReadFromAddress(int16_t Address)
{
    return Memory[Address];
}

void MemoryModule::WriteToAddress(int16_t Address, char Data)
{
    Memory[Address] = Data;
}
