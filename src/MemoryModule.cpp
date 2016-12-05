#include "MemoryModule.h"

MemoryModule::MemoryModule(const unsigned Size)
{
    Memory = new char[Size];
}

MemoryModule::~MemoryModule()
{
    delete [] Memory;
}

char MemoryModule::ReadFromAddress(uint16_t Address)
{
    return Memory[Address];
}

void MemoryModule::WriteToAddress(uint16_t Address, char Data)
{
    std::cout << "Writing to " << Address << std::endl;

    if(Address > 0x4000 && Address < 0x7fff)
    {
        std::cout << "MBC switch to bank " << Data << std::endl;
    }
    else
    {
        Memory[Address] = Data;
    }
}

void MemoryModule::LoadROMChunk(uint16_t Position, uint16_t Size, uint16_t DataPosition, char *Data)
{
    uint16_t ReadData = 0;
    uint16_t CurrentPosition = Position;
    uint16_t CurrentDataPosition = DataPosition;

    while(ReadData != Size)
    {
        Memory[CurrentPosition] = Data[CurrentDataPosition];
        CurrentDataPosition++;
        CurrentPosition++;
        ReadData++;
    }
}

void MemoryModule::ROMBankSwap(uint16_t DataPosition, char *Data)
{
    LoadROMChunk(0x4000, 16383, DataPosition, Data);
}

