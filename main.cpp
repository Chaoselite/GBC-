#include <iostream>
#include "CGB_CPU.h"
#include "CartLoader.h"
#include "MemoryModule.h"
using namespace std;

int main()
{
    std::string ROMFILEPATH, BIOSPATH;
    std::string title;
    CGB_CPU cpu;
    CartLoader CL;


    //Unit test here

    uint8_t Data = 0x29;
    uint8_t Data2 = 0xe1;
    bool Carry = 0;
    bool HCarry = 0;

    uint8_t Datah4 = trunc4high(Data);
    uint8_t Datal4 = trunc4low(Data);
    uint8_t Data2h4 = trunc4high(Data2);
    uint8_t Data2l4 = trunc4low(Data2);

    HCarry = 0xf < (Data2l4 + Datal4);
    Carry = 0xf < (Data2h4 + Datah4);

    std::cout << "Half Carry " << HCarry << " " <<  Data2l4 + Datal4 << std::endl;
    std::cout << "Carry " << Carry << " " << Data2h4 + Datah4 << std::endl;

    //End unit test


    std::cout << "Select a ROM to load" <<std::endl;
    std::cin >> ROMFILEPATH;
    CL.Load(ROMFILEPATH);

    cpu.SetCartridge(CL.Cartridge);
    cpu.Run();

    return 0;
}
