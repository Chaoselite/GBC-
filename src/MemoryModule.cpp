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

void MemoryModule::LoadROMChunk(int16_t Position, int16_t Size, int16_t DataPosition, char *Data)
{
    int16_t ReadData = 0;
    int16_t CurrentPosition = Position;
    int16_t CurrentDataPosition = DataPosition;

    while(ReadData != Size)
    {
        Memory[CurrentPosition] = Data[CurrentDataPosition];
        CurrentDataPosition++;
        CurrentPosition++;
        ReadData++;
    }
}

void MemoryModule::ROMBankSwap(int16_t DataPosition, char *Data)
{
    LoadROMChunk(0x4000, 16383, DataPosition, Data);
}

