#include "CGB_CPU.h"

CGB_CPU::CGB_CPU()
{
    BIOS = new char[2304];
}

CGB_CPU::~CGB_CPU()
{
    delete [] BIOS;
}

void CGB_CPU::SetCartridge(char *Cart)
{
    Cartridge = Cart;
}

void CGB_CPU::SetMemory(MemoryModule *Mem)
{
    Memory = Mem;
}

void CGB_CPU::LoadBIOS(std::string Filepath)
{
    std::ifstream is;
    is.open (Filepath, std::ifstream::binary );
    if(!is)
    {
            std::cout<<"File couldn't open BIOS"<<std::endl;

    }
    else
    {
    int length = is.tellg();
    is.seekg (0, is.beg);
    is.read (BIOS, 2304);
    }
}

void CGB_CPU::Run(void)
{
    ExecOP(0xcb, 0x00);
}

char CGB_CPU::Fetch(uint16_t ADDR)
{

}

void CGB_CPU::ExecOP(uint16_t OP, char Data)
{
    switch (OP)
    {
    case 0xcb:
        std::cout << "CB OP" << std::endl;
        break;
    default :
        std::cout << "Invalid OP: " << OP << std::endl;
        break;
    }
}

void CGB_CPU::ExecCBOP(uint16_t OP, char Data)
{

}
