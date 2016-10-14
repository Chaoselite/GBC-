#include "CGB_CPU.h"

CGB_CPU::CGB_CPU()
{
    //ctor
}

CGB_CPU::~CGB_CPU()
{
    //dtor
}

void CGB_CPU::Run(void)
{
    ExecOP(0xcb, 0x00);
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
