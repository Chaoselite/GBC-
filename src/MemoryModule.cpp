#include "MemoryModule.h"

MemoryModule::MemoryModule(const unsigned Size)
{
    Memory = new char[Size];
}

MemoryModule::~MemoryModule()
{
    delete [] Memory;
}

char MemoryModule::ReadFromAddress(char Address)
{
    return Memory[Address];
}

void MemoryModule::WriteToAddress(char Address, char Data)
{
    Memory[Address] = Data;
}
