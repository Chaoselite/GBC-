#include "CGB_CPU.h"

CGB_CPU::CGB_CPU()
{
    BIOS = new char[2304];
    Memory = new MemoryModule(65535);
}

CGB_CPU::~CGB_CPU()
{
    delete [] BIOS;
    delete Memory;
}

void CGB_CPU::SetCartridge(char *Cart)
{
    Cartridge = Cart;
    Memory->LoadROMChunk(0x100, 32511, 0x100, Cartridge);//load header bank 1 and switchable
}

void CGB_CPU::LoadBIOS(std::string Filepath)
{
    std::ifstream is;
    is.open (Filepath.c_str(), std::ifstream::binary );
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
    while(isRunning)
    {
        ExecOP(Fetch());
    }
}

uint8_t CGB_CPU::Fetch(void)
{
   uint8_t Data = Memory->ReadFromAddress(PC);
   PC++;
   if (PC > 0xffff)
   {
       isRunning = false;
   }
   return Data;
}

void CGB_CPU::ExecOP(uint8_t OP)
{
    uint16_t data16a = 0;
    uint16_t data16b = 0;
    uint8_t data8h = 0;
    uint8_t data8l = 0;
    int8_t r8 = 0;
    char input;

    switch (OP)
    {
    case 0x00:
        std::cout << "NOP" << std::endl;
    break;

    case 0x01:
        registers[REG_TYPE::C] = Fetch();
        registers[REG_TYPE::B] = Fetch();
    break;

    case 0x02:
        data8h = registers[REG_TYPE::B];
        data8l = registers[REG_TYPE::C];
        data16a = concat16(data8h, data8l);

        Memory->WriteToAddress(data16a, registers[REG_TYPE::A]);
    break;

    case 0x03:
        data8h = registers[REG_TYPE::B];
        data8l = registers[REG_TYPE::C];
        data16a = concat16(data8h, data8l);
        data16a++;
        registers[REG_TYPE::B] = trunc8high(data16a);
        registers[REG_TYPE::C] = trunc8low(data16a);

    break;

    case 0x04:

    break;

    case 0x05:

    break;

    case 0x06:
        registers[REG_TYPE::B] = Fetch();

    break;

    case 0x07:

    break;

    case 0x08:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, SP);

    break;

    case 0x09:

    break;

    case 0x0a:
        data16a = concat16(registers[REG_TYPE::B], registers[REG_TYPE::C]);
        registers[REG_TYPE::A] = Memory->ReadFromAddress(data16a);
    break;

    case 0x0b:
        data8h = registers[REG_TYPE::B];
        data8l = registers[REG_TYPE::C];
        data16a = concat16(data8h, data8l);
        data16a--;
        registers[REG_TYPE::B] = trunc8high(data16a);
        registers[REG_TYPE::C] = trunc8low(data16a);

    break;

    case 0x0c:

    break;

    case 0x0d:

    break;

    case 0x0e:
        registers[REG_TYPE::C] = Fetch();
    break;

    case 0x0f:

    break;

    case 0x10:
        //staph
    break;

    case 0x11:
        registers[REG_TYPE::E] = Fetch();
        registers[REG_TYPE::D] = Fetch();
    break;

    case 0x12:
        data8h = registers[REG_TYPE::D];
        data8l = registers[REG_TYPE::E];
        data16a = concat16(data8h, data8l);

        Memory->WriteToAddress(data16a, registers[REG_TYPE::A]);
    break;

    case 0x13:
        data8h = registers[REG_TYPE::D];
        data8l = registers[REG_TYPE::E];
        data16a = concat16(data8h, data8l);
        data16a++;
        registers[REG_TYPE::B] = trunc8high(data16a);
        registers[REG_TYPE::C] = trunc8low(data16a);

    break;

    case 0x14:

    break;

    case 0x15:

    break;

    case 0x16:
        registers[REG_TYPE::D] = Fetch();

    break;

    case 0x17:

    break;

    case 0x18:
        r8 = Fetch();
        PC += r8;

    break;

    case 0x19:

    break;

    case 0x1a:
        data16a = concat16(registers[REG_TYPE::D], registers[REG_TYPE::E]);
        registers[REG_TYPE::A] = Memory->ReadFromAddress(data16a);
    break;

    case 0x1b:
        data8h = registers[REG_TYPE::D];
        data8l = registers[REG_TYPE::E];
        data16a = concat16(data8h, data8l);
        data16a--;
        registers[REG_TYPE::B] = trunc8high(data16a);
        registers[REG_TYPE::C] = trunc8low(data16a);

    break;

    case 0x1c:

    break;

    case 0x1d:

    break;

    case 0x1e:
        registers[REG_TYPE::E] = Fetch();
    break;

    case 0x1f:

    break;

    case 0x20:
        if(!getbit8(registers[REG_TYPE::F], 7))
        {
            r8 = Fetch();
            PC += r8;
        }
    break;

    case 0x21:
        registers[REG_TYPE::L] = Fetch();
        registers[REG_TYPE::H] = Fetch();
    break;

    case 0x22:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        data16a++;
        Memory->WriteToAddress(data16a, registers[REG_TYPE::A]);
    break;

    case 0x23:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        data16a++;
        registers[REG_TYPE::B] = trunc8high(data16a);
        registers[REG_TYPE::C] = trunc8low(data16a);

    break;

    case 0x24:

    break;

    case 0x25:

    break;

    case 0x26:
        registers[REG_TYPE::H] = Fetch();

    break;

    case 0x27:

    break;

    case 0x28:
        if(getbit8(registers[REG_TYPE::F], 7))
        {
            r8 = Fetch();
            PC += r8;
        }

    break;

    case 0x29:

    break;

    case 0x2a:
        data16a = concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]);
        data16a++;
        registers[REG_TYPE::A] = Memory->ReadFromAddress(data16a);
    break;

    case 0x2b:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        data16a--;
        registers[REG_TYPE::B] = trunc8high(data16a);
        registers[REG_TYPE::C] = trunc8low(data16a);

    break;

    case 0x2c:

    break;

    case 0x2d:

    break;

    case 0x2e:
        registers[REG_TYPE::L] = Fetch();
    break;

    case 0x2f:

    break;

    case 0x30:
        if(!getbit8(registers[REG_TYPE::F], 4))
        {
            r8 = Fetch();
            PC += r8;
        }
    break;

    case 0x31:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        SP = data16a;
    break;

    case 0x32:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        data16a--;
        Memory->WriteToAddress(data16a, registers[REG_TYPE::A]);
    break;

    case 0x33:
        SP++;
    break;

    case 0x34:

    break;

    case 0x35:

    break;

    case 0x36:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, Fetch());
    break;

    case 0x37:

    break;

    case 0x38:
        if(getbit8(registers[REG_TYPE::F], 4))
        {
            r8 = Fetch();
            PC += r8;
        }

    break;

    case 0x39:

    break;

    case 0x3a:
        data16a = concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]);
        data16a--;
        registers[REG_TYPE::A] = Memory->ReadFromAddress(data16a);
    break;

    case 0x3b:
        SP--;
    break;

    case 0x3c:

    break;

    case 0x3d:

    break;

    case 0x3e:
        registers[REG_TYPE::A] = Fetch();
    break;

    case 0x3f:

    break;

    case 0x40:
        data8h = registers[REG_TYPE::B];
        registers[REG_TYPE::B] = data8h;
    break;

    case 0x41:
        data8h = registers[REG_TYPE::C];
        registers[REG_TYPE::B] = data8h;
    break;

    case 0x42:
        data8h = registers[REG_TYPE::D];
        registers[REG_TYPE::B] = data8h;
    break;

    case 0x43:
        data8h = registers[REG_TYPE::E];
        registers[REG_TYPE::B] = data8h;
    break;

    case 0x44:
        data8h = registers[REG_TYPE::H];
        registers[REG_TYPE::B] = data8h;
    break;

    case 0x45:
        data8h = registers[REG_TYPE::L];
        registers[REG_TYPE::B] = data8h;
    break;

    case 0x46:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        registers[REG_TYPE::B] = Memory->ReadFromAddress(data16a);
    break;

    case 0x47:
        data8h = registers[REG_TYPE::A];
        registers[REG_TYPE::B] = data8h;
    break;

    case 0x48:
        data8h = registers[REG_TYPE::B];
        registers[REG_TYPE::C] = data8h;

    break;

    case 0x49:
        data8h = registers[REG_TYPE::C];
        registers[REG_TYPE::C] = data8h;
    break;

    case 0x4a:
        data8h = registers[REG_TYPE::D];
        registers[REG_TYPE::C] = data8h;
    break;

    case 0x4b:
        data8h = registers[REG_TYPE::E];
        registers[REG_TYPE::C] = data8h;
    break;

    case 0x4c:
        data8h = registers[REG_TYPE::H];
        registers[REG_TYPE::C] = data8h;
    break;

    case 0x4d:
        data8h = registers[REG_TYPE::L];
        registers[REG_TYPE::C] = data8h;
    break;

    case 0x4e:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        registers[REG_TYPE::C] = Memory->ReadFromAddress(data16a);
    break;

    case 0x4f:
        data8h = registers[REG_TYPE::A];
        registers[REG_TYPE::C] = data8h;
    break;

    case 0x50:
        data8h = registers[REG_TYPE::B];
        registers[REG_TYPE::D] = data8h;
    break;

    case 0x51:
        data8h = registers[REG_TYPE::C];
        registers[REG_TYPE::D] = data8h;
    break;

    case 0x52:
        data8h = registers[REG_TYPE::D];
        registers[REG_TYPE::D] = data8h;
    break;

    case 0x53:
        data8h = registers[REG_TYPE::E];
        registers[REG_TYPE::D] = data8h;
    break;

    case 0x54:
        data8h = registers[REG_TYPE::H];
        registers[REG_TYPE::D] = data8h;
    break;

    case 0x55:
        data8h = registers[REG_TYPE::L];
        registers[REG_TYPE::D] = data8h;
    break;

    case 0x56:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        registers[REG_TYPE::D] = Memory->ReadFromAddress(data16a);
    break;

    case 0x57:
        data8h = registers[REG_TYPE::A];
        registers[REG_TYPE::D] = data8h;
    break;

    case 0x58:
        data8h = registers[REG_TYPE::B];
        registers[REG_TYPE::E] = data8h;

    break;

    case 0x59:
        data8h = registers[REG_TYPE::C];
        registers[REG_TYPE::E] = data8h;
    break;

    case 0x5a:
        data8h = registers[REG_TYPE::D];
        registers[REG_TYPE::E] = data8h;
    break;

    case 0x5b:
        data8h = registers[REG_TYPE::E];
        registers[REG_TYPE::E] = data8h;
    break;

    case 0x5c:
        data8h = registers[REG_TYPE::H];
        registers[REG_TYPE::E] = data8h;
    break;

    case 0x5d:
        data8h = registers[REG_TYPE::L];
        registers[REG_TYPE::E] = data8h;
    break;

    case 0x5e:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        registers[REG_TYPE::E] = Memory->ReadFromAddress(data16a);
    break;

    case 0x5f:
        data8h = registers[REG_TYPE::A];
        registers[REG_TYPE::E] = data8h;
    break;

    case 0x60:
        data8h = registers[REG_TYPE::B];
        registers[REG_TYPE::H] = data8h;
    break;

    case 0x61:
        data8h = registers[REG_TYPE::C];
        registers[REG_TYPE::H] = data8h;
    break;

    case 0x62:
        data8h = registers[REG_TYPE::D];
        registers[REG_TYPE::H] = data8h;
    break;

    case 0x63:
        data8h = registers[REG_TYPE::E];
        registers[REG_TYPE::H] = data8h;
    break;

    case 0x64:
        data8h = registers[REG_TYPE::H];
        registers[REG_TYPE::H] = data8h;
    break;

    case 0x65:
        data8h = registers[REG_TYPE::L];
        registers[REG_TYPE::H] = data8h;
    break;

    case 0x66:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        registers[REG_TYPE::H] = Memory->ReadFromAddress(data16a);
    break;

    case 0x67:
        data8h = registers[REG_TYPE::A];
        registers[REG_TYPE::H] = data8h;
    break;

    case 0x68:
        data8h = registers[REG_TYPE::B];
        registers[REG_TYPE::L] = data8h;

    break;

    case 0x69:
        data8h = registers[REG_TYPE::C];
        registers[REG_TYPE::L] = data8h;
    break;

    case 0x6a:
        data8h = registers[REG_TYPE::D];
        registers[REG_TYPE::L] = data8h;
    break;

    case 0x6b:
        data8h = registers[REG_TYPE::E];
        registers[REG_TYPE::L] = data8h;
    break;

    case 0x6c:
        data8h = registers[REG_TYPE::H];
        registers[REG_TYPE::L] = data8h;
    break;

    case 0x6d:
        data8h = registers[REG_TYPE::L];
        registers[REG_TYPE::L] = data8h;
    break;

    case 0x6e:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        registers[REG_TYPE::L] = Memory->ReadFromAddress(data16a);
    break;

    case 0x6f:
        data8h = registers[REG_TYPE::A];
        registers[REG_TYPE::L] = data8h;
    break;

    case 0x70:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, registers[REG_TYPE::B]);
    break;

    case 0x71:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, registers[REG_TYPE::C]);
    break;

    case 0x72:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, registers[REG_TYPE::D]);
    break;

    case 0x73:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, registers[REG_TYPE::E]);
    break;

    case 0x74:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, registers[REG_TYPE::H]);
    break;

    case 0x75:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, registers[REG_TYPE::L]);
    break;

    case 0x76:
        //HALT
    break;

    case 0x77:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, registers[REG_TYPE::A]);
    break;

    case 0x78:
        data8h = registers[REG_TYPE::B];
        registers[REG_TYPE::A] = data8h;

    break;

    case 0x79:
        data8h = registers[REG_TYPE::C];
        registers[REG_TYPE::A] = data8h;
    break;

    case 0x7a:
        data8h = registers[REG_TYPE::D];
        registers[REG_TYPE::A] = data8h;
    break;

    case 0x7b:
        data8h = registers[REG_TYPE::E];
        registers[REG_TYPE::A] = data8h;
    break;

    case 0x7c:
        data8h = registers[REG_TYPE::H];
        registers[REG_TYPE::A] = data8h;
    break;

    case 0x7d:
        data8h = registers[REG_TYPE::L];
        registers[REG_TYPE::A] = data8h;
    break;

    case 0x7e:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        registers[REG_TYPE::A] = Memory->ReadFromAddress(data16a);
    break;

    case 0x7f:
        data8h = registers[REG_TYPE::A];
        registers[REG_TYPE::A] = data8h;
    break;

    case 0xcb:
        std::cout << "CB OP" << std::endl;
        isRunning = false;
    break;

    default :

        std::cout << "Invalid OP: " << OP << " at location: " << PC << std::endl;
        std::cin >> input;
    break;
    }
}

void CGB_CPU::ExecCBOP(uint8_t OP)
{

}

uint16_t CGB_CPU::FlaggedOP(OPFlag Flags, uint16_t Value, uint16_t Value2)
{
    uint16_t Result =0;
    bool Carry =0;
    bool HalfCarry =0;
    bool Zero =0;
    bool Subtract = 0;

    if(Flags == OPFlag::ADD)
    {

    }
    else if (Flags == OPFlag::SUB)
    {
        Subtract = 1;
    }
    else
    {

    }
}

 void CGB_CPU::SetFlag(Flags Flag, bool Value)
 {
     if(Flag == Flags::fZ)
     {
        setbit8(registers[REG_TYPE::F], 7, Value);
     }
     if(Flag == Flags::fN)
     {

     }
     if (Flag == Flags::fH)
     {

     }
     if (Flag == Flags::fC)
     {

     }
 }

