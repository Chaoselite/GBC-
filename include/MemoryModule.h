#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H


class MemoryModule
{
public:
    MemoryModule(const unsigned Size);
    virtual ~MemoryModule();

    char ReadFromAddress(char Address);

    void WriteToAddress(char Address, char Data);

private:

    char *Memory;

};

#endif // MEMORYMODULE_H
