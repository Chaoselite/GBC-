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
    int i;
    if(Address > 0x4000 && Address < 0x7fff)
    {
        ROMBankSwap(Data);
        std::cout << "MBC switch to bank " << Data << std::endl;
        std::cin >> i;
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

void MemoryModule::ROMBankSwap(uint8_t DataPosition)
{
    LoadROMChunk(0x4000, 16383, (DataPosition * 16383), Cartridge);
}

void MemoryModule::SetCartridge(char *Cart)
{
    Cartridge = Cart;
}

