#include <iostream>
#include "CGB_CPU.h"
#include "CartLoader.h"
#include "MemoryModule.h"
using namespace std;

int main()
{
    std::string ROMFILEPATH;
    CGB_CPU cpu;
    CartLoader CL;
    MemoryModule RAM(8191);
    std::cout << "Select a ROM to load" << std::endl;
    std::cin >> ROMFILEPATH;
    CL.Load(ROMFILEPATH);
    std::string title;
    for (int i = 0; i != 10; i++)
    {
        title += CL.CHead->Title[i];
    }
    cout << title << endl;
    return 0;
}
