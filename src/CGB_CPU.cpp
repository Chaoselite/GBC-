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
        if(InterruptWait)
        {
            //ExecInterrupt(Interrupt);
        }
        else
        {
            ExecOP(Fetch());
        }
    }
}

uint8_t CGB_CPU::Fetch(void)
{
   uint8_t Data = Memory->ReadFromAddress(PC);
   PC++;
   return Data;
}

bool CGB_CPU::ExecInterrupt(uint8_t Interrupt)
{

}

void CGB_CPU::ExecOP(uint8_t OP)
{
    bool OCarry = 0;
    bool OHCarry = 0;
    bool OZero = 0;
    bool ONeg = 0;
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
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::B] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::B], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x05:
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::B] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::B], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x06:
        registers[REG_TYPE::B] = Fetch();

    break;

    case 0x07:
        data8h = registers[REG_TYPE::A];
        OCarry = getbit8(data8h, 7);
        data8h << 1;
        setbit8(data8h, 0, OCarry);
        registers[REG_TYPE::A] = data8h;
        SetFlag(Flags::fC, OCarry);
        SetFlag(Flags::fZ, 0);
        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fH, 0);

    break;

    case 0x08:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, SP);

    break;

    case 0x09:
        OZero = GetFlag(Flags::fZ);
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::B];
        FlaggedOP(OPFlag::ADD, data8h, data8l);

        data16a = concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]);
        data16b = concat16(registers[REG_TYPE::B], registers[REG_TYPE::C]);
        data16a += data16b;

        registers[REG_TYPE::H] = trunc8high(data16a);
        registers[REG_TYPE::L] = trunc8low(data16a);

        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fZ, OZero);

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
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::C] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::C], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x0d:
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::C] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::C], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x0e:
        registers[REG_TYPE::C] = Fetch();
    break;

    case 0x0f:
        data8h = registers[REG_TYPE::A];
        OCarry = getbit8(data8h, 0);
        data8h >> 1;
        setbit8(data8h, 7, OCarry);
        registers[REG_TYPE::A] = data8h;
        SetFlag(Flags::fC, OCarry);
        SetFlag(Flags::fZ, 0);
        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fH, 0);

    break;

    case 0x10:
        isRunning = false;

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
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::D] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::D], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x15:
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::D] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::D], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x16:
        registers[REG_TYPE::D] = Fetch();

    break;

    case 0x17:
        data8h = registers[REG_TYPE::A];
        OCarry = getbit8(data8h, 7);
        data8h << 1;
        setbit8(data8h, 0, GetFlag(Flags::fC));
        registers[REG_TYPE::A] = data8h;
        SetFlag(Flags::fC, OCarry);
        SetFlag(Flags::fZ, 0);
        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fH, 0);

    break;

    case 0x18:
        r8 = Fetch();
        PC += r8;

    break;

    case 0x19:

        OZero = GetFlag(Flags::fZ);
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::D];
        FlaggedOP(OPFlag::ADD, data8h, data8l);

        data16a = concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]);
        data16b = concat16(registers[REG_TYPE::D], registers[REG_TYPE::E]);
        data16a += data16b;

        registers[REG_TYPE::H] = trunc8high(data16a);
        registers[REG_TYPE::L] = trunc8low(data16a);

        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fZ, OZero);

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
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::E] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::E], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x1d:
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::E] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::E], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x1e:
        registers[REG_TYPE::E] = Fetch();
    break;

    case 0x1f:
        data8h = registers[REG_TYPE::A];
        OCarry = getbit8(data8h, 0);
        data8h >> 1;
        setbit8(data8h, 7, GetFlag(Flags::fC));
        registers[REG_TYPE::A] = data8h;
        SetFlag(Flags::fC, OCarry);
        SetFlag(Flags::fZ, 0);
        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fH, 0);

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
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::H] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::H], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x25:
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::H] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::H], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x26:
        registers[REG_TYPE::H] = Fetch();

    break;

    case 0x27:
        data8h = registers[REG_TYPE::A];
        if(GetFlag(Flags::fH) || trunc4low(data8h) > 0x9)
        {
            data8h += 0x06;
        }
        if(GetFlag(Flags::fC) || trunc4high(data8h) > 0x9)
        {
            data8h += 0x60;
        }

    break;

    case 0x28:
        if(getbit8(registers[REG_TYPE::F], 7))
        {
            r8 = Fetch();
            PC += r8;
        }

    break;

    case 0x29:
        OZero = GetFlag(Flags::fZ);
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::H];
        FlaggedOP(OPFlag::ADD, data8h, data8l);

        data16a = concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]);
        data16b = concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]);
        data16a += data16b;

        registers[REG_TYPE::H] = trunc8high(data16a);
        registers[REG_TYPE::L] = trunc8low(data16a);

        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fZ, OZero);

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
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::L] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::L], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x2d:
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::L] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::L], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x2e:
        registers[REG_TYPE::L] = Fetch();
    break;

    case 0x2f:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], 0xff);
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fN, 1);

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
        OCarry = GetFlag(Flags::fC);
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, FlaggedOP(OPFlag::ADD, Memory->ReadFromAddress(data16a), 0x1));
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x35:
        OCarry = GetFlag(Flags::fC);
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, FlaggedOP(OPFlag::SUB, Memory->ReadFromAddress(data16a), 0x1));
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x36:
        data8h = registers[REG_TYPE::H];
        data8l = registers[REG_TYPE::L];
        data16a = concat16(data8h, data8l);
        Memory->WriteToAddress(data16a, Fetch());
    break;

    case 0x37:
        SetFlag(Flags::fC, 1);
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0x38:
        if(getbit8(registers[REG_TYPE::F], 4))
        {
            r8 = Fetch();
            PC += r8;
        }

    break;

    case 0x39:
        OZero = GetFlag(Flags::fZ);
        data8h = registers[REG_TYPE::H];
        data8l = trunc8high(SP);
        FlaggedOP(OPFlag::ADD, data8h, data8l);

        data16a = concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]);
        data16b = SP;
        data16a += data16b;

        registers[REG_TYPE::H] = trunc8high(data16a);
        registers[REG_TYPE::L] = trunc8low(data16a);

        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fZ, OZero);

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
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::A], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x3d:
        OCarry = GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], 0x1);
        SetFlag(Flags::fC, OCarry);

    break;

    case 0x3e:
        registers[REG_TYPE::A] = Fetch();
    break;

    case 0x3f:
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fN, 0);
        if(GetFlag(Flags::fC))
        {
            SetFlag(Flags::fC, 0);
        }
        else
        {
            SetFlag(Flags::fC, 1);
        }

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
        InterruptWait = 1;

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

    case 0x80:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::B];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x81:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::C];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x82:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::D];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x83:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::E];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x84:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::H];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x85:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::L];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x86:
        data8h = registers[REG_TYPE::A];
        data8l = Memory->ReadFromAddress(concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]));
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x87:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::A];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x88:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::B] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x89:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::C] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x8a:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::D] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x8b:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::E] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x8c:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::H] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x8d:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::L] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x8e:
        data8h = registers[REG_TYPE::A];
        data8l = Memory->ReadFromAddress(concat16(registers[REG_TYPE::H], registers[REG_TYPE::L])) + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x8f:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::A] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, data8h, data8l);

    break;

    case 0x90:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::B];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x91:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::C];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x92:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::D];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x93:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::E];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x94:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::H];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x95:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::L];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x96:
        data8h = registers[REG_TYPE::A];
        data8l = Memory->ReadFromAddress(concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]));
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x97:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::A];
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x98:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::B] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x99:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::C] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x9a:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::D] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x9b:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::E] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x9c:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::H] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x9d:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::L] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x9e:
        data8h = registers[REG_TYPE::A];
        data8l = Memory->ReadFromAddress(concat16(registers[REG_TYPE::H], registers[REG_TYPE::L])) + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0x9f:
        data8h = registers[REG_TYPE::A];
        data8l = registers[REG_TYPE::A] + GetFlag(Flags::fC);
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, data8h, data8l);

    break;

    case 0xa0:
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A], registers[REG_TYPE::B]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa1:
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A], registers[REG_TYPE::C]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa2:
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A], registers[REG_TYPE::D]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa3:
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A], registers[REG_TYPE::E]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa4:
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A], registers[REG_TYPE::H]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa5:
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A], registers[REG_TYPE::L]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa6:
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A],
                                         Memory->ReadFromAddress(concat16(registers[REG_TYPE::H], registers[REG_TYPE::L])));
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa7:
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A], registers[REG_TYPE::A]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa8:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], registers[REG_TYPE::B]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xa9:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], registers[REG_TYPE::C]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xaa:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], registers[REG_TYPE::D]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xab:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], registers[REG_TYPE::E]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xac:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], registers[REG_TYPE::H]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xad:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], registers[REG_TYPE::L]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xae:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A],
                                         Memory->ReadFromAddress(concat16(registers[REG_TYPE::H], registers[REG_TYPE::L])));
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xaf:
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], registers[REG_TYPE::A]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb0:
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A], registers[REG_TYPE::B]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb1:
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A], registers[REG_TYPE::C]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb2:
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A], registers[REG_TYPE::D]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb3:
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A], registers[REG_TYPE::E]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb4:
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A], registers[REG_TYPE::H]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb5:
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A], registers[REG_TYPE::L]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb6:
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A],
                                        Memory->ReadFromAddress(concat16(registers[REG_TYPE::H], registers[REG_TYPE::L])));
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb7:
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A], registers[REG_TYPE::A]);
        SetFlag(Flags::fZ, (registers[REG_TYPE::A] == 0));
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fC, 0);
        SetFlag(Flags::fN, 0);

    break;

    case 0xb8:
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], registers[REG_TYPE::B]);

    break;

    case 0xb9:
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], registers[REG_TYPE::C]);

    break;

    case 0xba:
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], registers[REG_TYPE::D]);

    break;

    case 0xbb:
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], registers[REG_TYPE::E]);

    break;

    case 0xbc:
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], registers[REG_TYPE::H]);

    break;

    case 0xbd:
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], registers[REG_TYPE::L]);

    break;

    case 0xbe:
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A],
                               Memory->ReadFromAddress(concat16( registers[REG_TYPE::H],  registers[REG_TYPE::L])));

    break;

    case 0xbf:
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], registers[REG_TYPE::A]);

    break;

    case 0xc0:
        if(!GetFlag(Flags::fZ))
        {
            Ret();
        }

    break;

    case 0xc1:
        registers[REG_TYPE::C] = StackPop();
        registers[REG_TYPE::B] = StackPop();

    break;

    case 0xc2:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        if(!GetFlag(Flags::fZ))
        {
            PC = data16a;
        }

    break;

    case 0xc3:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        PC = data16a;

    break;

    case 0xc4:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        if(!GetFlag(Flags::fZ))
        {
            Call(data16a);
        }

    break;

    case 0xc5:
        StackPush(registers[REG_TYPE::B]);
        StackPush(registers[REG_TYPE::C]);
    break;

    case 0xc6:
        data8h = fetch();
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::A], data8h);

    break;

    case 0xc7:
        Call(0x0000);

    break;

    case 0xc8:
        if(GetFlag(Flags::fZ))
        {
            Ret();
        }

    break;

    case 0xc9:
        Ret();

    break;

    case 0xca:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        if(GetFlag(Flags::fZ))
        {
            PC = data16a;
        }

    break;

    case 0xcb:
        std::cout << "CB OP" << std::endl;
        ExecCBOP(Fetch());
    break;

    case 0xcc:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        if(GetFlag(Flags::fZ))
        {
            Call(data16a);
        }

    break;

    case 0xcd:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        Call(data16a);

    break;

    case 0xce:
        data8h = fetch();
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::ADD, registers[REG_TYPE::A], data8h + GetFlag(Flags::fC));

    break;

    case 0xcf:
        Call(0x0008);

    break;

    case 0xd0:
        if(!GetFlag(Flags::fC))
        {
            Ret();
        }

    break;

    case 0xd1:
        registers[REG_TYPE::E] = StackPop();
        registers[REG_TYPE::D] = StackPop();

    break;

    case 0xd2:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        if(!GetFlag(Flags::fC))
        {
            PC = data16a;
        }

    break;

    case 0xd4:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        if(!GetFlag(Flags::fC))
        {
            Call(data16a);
        }

    break;

    case 0xd5:
        StackPush(registers[REG_TYPE::D]);
        StackPush(registers[REG_TYPE::E]);

    break;

    case 0xd6:
        data8h = fetch();
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], data8h);

    break;

    case 0xd7:
        Call(0x0010);

    break;

    case 0xd8:
        if(GetFlag(Flags::fC))
        {
            Ret();
        }

    break;

    case 0xd9:
        Ret();
        SetInterrupt(1);

    break;

    case 0xda:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        if(GetFlag(Flags::fC))
        {
            PC = data16a;
        }

    break;

    case 0xdc:
        data8l = Fetch();
        data8h = Fetch();
        data16a = concat16(data8h, data8l);
        if(GetFlag(Flags::fC))
        {
            Call(data16a);
        }

    break;

    case 0xde:
        data8h = fetch();
        registers[REG_TYPE::A] = FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], data8h + GetFlag(Flags::fC));

    break;

    case 0xdf:
        Call(0x0018);

    break;

    case 0xe0:

    break;

    case 0xe1:
        registers[REG_TYPE::L] = StackPop();
        registers[REG_TYPE::H] = StackPop();

    break;

    case 0xe2:

    break;

    case 0xe5:
        StackPush(registers[REG_TYPE::H]);
        StackPush(registers[REG_TYPE::L]);

    break;

    case 0xe6:
        data8h = fetch();
        registers[REG_TYPE::A] = and8bit(registers[REG_TYPE::A], data8h);
        SetFlag(Flags::fZ, registers[REG_TYPE::A] == 0);
        SetFlag(Flags::fH, 1);
        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fC, 0);


    break;

    case 0xe7:
        Call(0x0020);

    break;

    case 0xe8:
        r8 = Fetch();
        data8l = trunc8low(SP);
        FlaggedOP(OPFlag::ADD, data8l, r8);
        SetFlag(Flags::fZ, 0);
        SetFlag(Flags::fN, 0);
        SP = SP + r8;

    break;

    case 0xe9:
        PC = concat16(registers[REG_TYPE::H], registers[REG_TYPE::L]);

    break;

    case 0xea:

    break;

    case 0xee:
        data8h = fetch();
        registers[REG_TYPE::A] = xor8bit(registers[REG_TYPE::A], data8h);
        SetFlag(Flags::fZ, registers[REG_TYPE::A] == 0);
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fC, 0);

    break;

    case 0xef:
        Call(0x0028);

    break;

    case 0xf0:

    break;

    case 0xf1:
        registers[REG_TYPE::F] = StackPop();
        registers[REG_TYPE::A] = StackPop();

    break;

    case 0xf2:

    break;

    case 0xf3:
        SetInterrupt(0);

    break;

    case 0xf5:
        StackPush(registers[REG_TYPE::A]);
        StackPush(registers[REG_TYPE::F]);

    break;

    case 0xf6:
        data8h = fetch();
        registers[REG_TYPE::A] = or8bit(registers[REG_TYPE::A], data8h);
        SetFlag(Flags::fZ, registers[REG_TYPE::A] == 0);
        SetFlag(Flags::fH, 0);
        SetFlag(Flags::fN, 0);
        SetFlag(Flags::fC, 0);

    break;

    case 0xf7:
        Call(0x0030);

    break;

    case 0xf8:

    break;

    case 0xf9:

    break;

    case 0xfa:

    break;

    case 0xfb:
        SetInterrupt(1);

    break;

    case 0xfe:
        data8h = fetch();
        FlaggedOP(OPFlag::SUB, registers[REG_TYPE::A], data8h);

    break;

    case 0xff:
        Call(0x0038);

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

uint8_t CGB_CPU::FlaggedOP(OPFlag OFlags, uint8_t Value, uint8_t Value2)
{
    uint8_t Result = 0;
    uint8_t Data = Value;
    uint8_t Data2 = Value2;
    bool Carry = 0;
    bool HCarry = 0;
    bool Zero = 0;

    uint8_t Datah4 = trunc4high(Data);
    uint8_t Datal4 = trunc4low(Data);
    uint8_t Data2h4 = trunc4high(Data2);
    uint8_t Data2l4 = trunc4low(Data2);


    if(OFlags == OPFlag::ADD)
    {
        Result = Data + Data2;
        HCarry = 0xf < Data2l4 + Datal4;
        Carry = 0xf < Data2h4 + Datah4;

        SetFlag(Flags::fN, 0);
    }
    if(OFlags == OPFlag::SUB) //this may be a problem
    {
        Result = Data - Data2;
        HCarry = 0x0 > Datal4 - Data2l4;
        Carry = 0x0 > Datah4 - Data2h4;

        SetFlag(Flags::fN, 1);
    }
    if(Result == 0)
    {
        SetFlag(Flags::fZ, 1);
    }

    SetFlag(Flags::fH, HCarry);
    SetFlag(Flags::fC, Carry);

    return Result;
}

 void CGB_CPU::SetFlag(Flags Flag, bool Value)
 {
     if(Flag == Flags::fZ)
     {
        setbit8(registers[REG_TYPE::F], 7, Value);
     }
     if(Flag == Flags::fN)
     {
        setbit8(registers[REG_TYPE::F], 6, Value);
     }
     if (Flag == Flags::fH)
     {
        setbit8(registers[REG_TYPE::F], 5, Value);
     }
     if (Flag == Flags::fC)
     {
        setbit8(registers[REG_TYPE::F], 4, Value);
     }
 }

bool CGB_CPU::GetFlag(Flags Flag)
{
    bool Result = 0;
    if(Flag == Flags::fZ)
    {
        getbit8(registers[REG_TYPE::F], 7);
    }
    if(Flag == Flags::fN)
    {
        getbit8(registers[REG_TYPE::F], 6);
    }
    if (Flag == Flags::fH)
    {
        getbit8(registers[REG_TYPE::F], 5);
    }
    if (Flag == Flags::fC)
    {
        getbit8(registers[REG_TYPE::F], 4);
    }
    return Result;
}

void CGB_CPU::StackPush(uint8_t Data)
{
    Memory->WriteToAddress(SP, Data);
    SP--;
}

uint8_t CGB_CPU::StackPop(void)
{
    SP++;
    return Memory->ReadFromAddress(SP);
}

void CGB_CPU::Call(uint16_t Address)
{
    uint8_t PCh = trunc8high(PC);
    uint8_t PCl = trunc8low(PC);
    StackPush(PCh);
    StackPush(PCl);
    PC = Address;
}

void CGB_CPU::Ret(void)
{
    uint8_t PCl = StackPop();
    uint8_t PCh = StackPop();
    PC = concat16(PCh, PCl);
}
