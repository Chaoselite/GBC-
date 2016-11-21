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



    uint16_t a= 0xff00;
    uint8_t b= 0xaa;

    a += b;
    std::cout << "Select a ROM to load" <<std::endl;
    std::cin >> ROMFILEPATH;
    CL.Load(ROMFILEPATH);

    cpu.SetCartridge(CL.Cartridge);
    cpu.Run();

    return 0;
}
