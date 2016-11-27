#include <iostream>
#include "CGB_CPU.h"
#include "CartLoader.h"
#include "MemoryModule.h"
using namespace std;

//TODO: add OPs 2f, 3f then continue to 80+
//Add MBC swapping
//Add restart and interrupt vectors
//Start VRAM interpreting

int main()
{
    std::string ROMFILEPATH, BIOSPATH;
    std::string title;
    CGB_CPU cpu;
    CartLoader CL;

    //Unit test here

    //End unit test

    std::cout << "Select a ROM to load" <<std::endl;
    std::cin >> ROMFILEPATH;
    CL.Load(ROMFILEPATH);

    cpu.SetCartridge(CL.Cartridge);
    cpu.Run();

    return 0;
}
