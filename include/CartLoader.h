#ifndef CARTLOADER_H
#define CARTLOADER_H
#include <fstream>
#include <iostream>
#include <string>
#include "Types.h"

struct Header
{
    char Entry[3]   ={0}; //0100-0103 Contains jump to main program
    char NLogo[47]  ={0}; //0104-0133 Nintendo Logo
    char Title[10]  ={0}; //134-013F Title
    char MCode[3]   ={0}; //013F-0142 Manufacturer Code
    char CGBFlag    =0; //0143 Used to enable Color functions
    char NLC[1]     ={0}; //New Licensee Code (Company/Publisher)
    char SGB        =0; //0146 Used to enable Super GB functions
    char CartType   =0; //0147 Specifies Memory Bank Controller & cart hardware
    char ROMSize    =0; //0148
    char RAMSize    =0; //0149
    char Region     =0; //014a Japanese or not
    char OLC        =0; //014b Old Licensee Code, value of 0x33 means use NLC instead
    char MROMVer    =0; //014c Version number of the game typically 0x00
    char Checksum   =0; //014d lower 8 bits must match calculated checksum
    //014E-014F Global checksum, GB does not check this so I won't either

};

class CartLoader
{
public:
    CartLoader();
    virtual ~CartLoader();

    void Load(std::string filepath);

    char *Cartridge = new char[8388607]; //8MB max
    Header *CHead;

private:
    void GetHeader(void);

};

#endif // CARTLOADER_H
