#include "CartLoader.h"

CartLoader::CartLoader()
{
    CHead = new Header;
}

CartLoader::~CartLoader()
{
    delete [] Cartridge;
    delete CHead;
}

void CartLoader::Load(std::string filepath)
{
    std::ifstream is;
    is.open (filepath, std::ifstream::binary );
    if(!is)
    {
            std::cout<<"File couldn't open"<<std::endl;

    }
    else
    {
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
    std::cout<<length<<std::endl;
    is.read (Cartridge, length);
    GetHeader();
    }
}

void CartLoader::GetHeader(void)
{

    CHead->Entry[0]   =Cartridge[0x0100];
    CHead->Entry[1]   =Cartridge[0x0101];
    CHead->Entry[2]   =Cartridge[0x0102];
    CHead->Entry[3]   =Cartridge[0x0103];
    CHead->NLogo[0]   =Cartridge[0x0104];
    CHead->NLogo[1]   =Cartridge[0x0105];
    CHead->NLogo[2]   =Cartridge[0x0106];
    CHead->NLogo[3]   =Cartridge[0x0107];
    CHead->NLogo[4]   =Cartridge[0x0108];
    CHead->NLogo[5]   =Cartridge[0x0109];
    CHead->NLogo[6]   =Cartridge[0x010a];
    CHead->NLogo[7]   =Cartridge[0x010b];
    CHead->NLogo[8]   =Cartridge[0x010c];
    CHead->NLogo[9]   =Cartridge[0x010d];
    CHead->NLogo[10]  =Cartridge[0x010e];
    CHead->NLogo[11]  =Cartridge[0x010f];
    CHead->NLogo[12]  =Cartridge[0x0110];
    CHead->NLogo[13]  =Cartridge[0x0111];
    CHead->NLogo[14]  =Cartridge[0x0112];
    CHead->NLogo[15]  =Cartridge[0x0113];
    CHead->NLogo[16]  =Cartridge[0x0114];
    CHead->NLogo[17]  =Cartridge[0x0115];
    CHead->NLogo[18]  =Cartridge[0x0116];
    CHead->NLogo[19]  =Cartridge[0x0117];
    CHead->NLogo[20]  =Cartridge[0x0118];
    CHead->NLogo[21]  =Cartridge[0x0119];
    CHead->NLogo[22]  =Cartridge[0x011a];
    CHead->NLogo[23]  =Cartridge[0x011b];
    CHead->NLogo[24]  =Cartridge[0x011c];
    CHead->NLogo[25]  =Cartridge[0x011d];
    CHead->NLogo[26]  =Cartridge[0x011e];
    CHead->NLogo[27]  =Cartridge[0x011f];
    CHead->NLogo[28]  =Cartridge[0x0120];
    CHead->NLogo[29]  =Cartridge[0x0121];
    CHead->NLogo[30]  =Cartridge[0x0122];
    CHead->NLogo[31]  =Cartridge[0x0123];
    CHead->NLogo[32]  =Cartridge[0x0124];
    CHead->NLogo[33]  =Cartridge[0x0125];
    CHead->NLogo[34]  =Cartridge[0x0126];
    CHead->NLogo[35]  =Cartridge[0x0127];
    CHead->NLogo[36]  =Cartridge[0x0128];
    CHead->NLogo[37]  =Cartridge[0x0129];
    CHead->NLogo[38]  =Cartridge[0x012a];
    CHead->NLogo[39]  =Cartridge[0x012b];
    CHead->NLogo[40]  =Cartridge[0x012c];
    CHead->NLogo[41]  =Cartridge[0x012d];
    CHead->NLogo[42]  =Cartridge[0x012e];
    CHead->NLogo[43]  =Cartridge[0x012f];
    CHead->NLogo[44]  =Cartridge[0x0130];
    CHead->NLogo[45]  =Cartridge[0x0131];
    CHead->NLogo[46]  =Cartridge[0x0132];
    CHead->NLogo[47]  =Cartridge[0x0133];
    CHead->Title[0]   =Cartridge[0x0134];
    CHead->Title[1]   =Cartridge[0x0135];
    CHead->Title[2]   =Cartridge[0x0136];
    CHead->Title[3]   =Cartridge[0x0137];
    CHead->Title[4]   =Cartridge[0x0138];
    CHead->Title[5]   =Cartridge[0x0139];
    CHead->Title[6]   =Cartridge[0x013a];
    CHead->Title[7]   =Cartridge[0x013b];
    CHead->Title[8]   =Cartridge[0x013c];
    CHead->Title[9]   =Cartridge[0x013d];
    CHead->Title[10]  =Cartridge[0x013e];
    CHead->MCode[0]   =Cartridge[0x013f];
    CHead->MCode[1]   =Cartridge[0x0140];
    CHead->MCode[2]   =Cartridge[0x0141];
    CHead->MCode[3]   =Cartridge[0x0142];
    CHead->CGBFlag    =Cartridge[0x0143];
    CHead->NLC[0]     =Cartridge[0x0144];
    CHead->NLC[1]     =Cartridge[0x0145];
    CHead->SGB        =Cartridge[0x0146];
    CHead->CartType   =Cartridge[0x0147];
    CHead->ROMSize    =Cartridge[0x0148];
    CHead->RAMSize    =Cartridge[0x0149];
    CHead->Region     =Cartridge[0x014a];
    CHead->OLC        =Cartridge[0x014b];
    CHead->MROMVer    =Cartridge[0x014c];
    CHead->Checksum   =Cartridge[0x014d];

}
