#include "include/harddrive.h"
#include "include/kernel.h"
#include "include/types.h"
#include "include/console.h"
#include "include/string.h"

//bool master;
uint16 bytePerSector;

/*
void HardDrive::Initialize(uint16 port, bool usemaster)
{
    Assembly _asm_;
    Console console;
    _asm_.inw(port); //data
    _asm_.in(port+0x1); //error
    _asm_.in(port+0x2); //sectorcount
    _asm_.in(port+0x3); //lbalow
    _asm_.in(port+0x4); //lbamid
    _asm_.in(port+0x5); //lbahigh
    _asm_.in(port+0x6); //device
    _asm_.in(port+0x7); //command
    _asm_.in(port+0x206); //control
    this->usemaster=master;
}
*/

#define ERROR 0x1
#define SECTORCOUNT 0x2
#define LBALOW 0x3
#define LBAMID 0x4
#define LBAHI 0x5
#define DEVICE 0x6
#define COMMAND 0x7
#define CONTROL 0x0C

Assembly _asm_;

HardDrive::HardDrive(uint16 port, bool usemaster)
{
    _asm_.inw(port);
    _asm_.in(port+ERROR);
    _asm_.in(port+SECTORCOUNT);
    _asm_.in(port+LBALOW);
    _asm_.in(port+LBAMID);
    _asm_.in(port+LBAHI);
    _asm_.in(port+DEVICE);
    _asm_.in(port+COMMAND);
    _asm_.in(port+CONTROL);

    this->usemaster=usemaster;
    this->port = port;
}

HardDrive::~HardDrive(){}

void HardDrive::Identify() {
    Assembly _asm_;
    Console console;

    console.Write("Used Port: ");
    console.Write(itos(port));

    _asm_.out(port+DEVICE, usemaster ? 0xA0 : 0xB0);
    _asm_.out(port+CONTROL, 0);
    _asm_.out(port+DEVICE, 0xA0);
    uint8 status = _asm_.in(port+COMMAND);
    if (status == 0xFF)
        return;
    _asm_.out(port+DEVICE, usemaster ? 0xA0 : 0xB0);
    _asm_.out(port+SECTORCOUNT, 0);
    _asm_.out(port+LBALOW, 0);
    _asm_.out(port+LBAMID, 0);
    _asm_.out(port+LBAHI, 0);
    _asm_.out(port+COMMAND, 0xEC);

    status = _asm_.in(port+COMMAND);
    if (status == 0x00) {
        console.WriteLine("A HDD isn't found in this PC");
        return;
    }
    while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
    {
        status = _asm_.in(port+0x7);
    }

    if (status & 0x01) {
        console.WriteLine("Error in reading drive.");
        return;
    }
    for(uint16 i=0;i<256;i++)
    {
        uint16 data=_asm_.inw(port);
        char* test = "  \0";
        test[0] = (data>>8)&0xFF;
        test[1] = data & 0xFF;
        console.Write(test);
    }
}

void HardDrive::Read28(uint32 sector, uint8 *data, int count) {
    Assembly _asm_;
    Console console;
    if(sector > 0x0FFFFFFF)
        return;
    //if (sector & 0xF0000000)
    //    return
    _asm_.out(port+ERROR, 0);
    _asm_.out(port+DEVICE, (usemaster ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
    _asm_.out(port+SECTORCOUNT, 1);
    _asm_.out(port+LBALOW, sector & 0x000000FF);
    _asm_.out(port+LBAMID, (sector& 0x0000FF00) >> 8);
    _asm_.out(port+LBALOW, (sector& 0x00FF0000) >> 16);
    _asm_.out(port+COMMAND, 0x21);

    uint8 status = _asm_.in(port+COMMAND);
    while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
    {
        status = _asm_.in(port+COMMAND);
    }

    if (status & 0x01) {
        console.WriteLine("Error in reading drive.");
        return;
    }
    console.WriteLine("Reading from Drive: ");
    for(int i=0;i<count;i+=2)
    {
        uint16 wdata = _asm_.inw(port);
        data[i]=wdata & 0x00FF;
        if (i+1<count){
            data[i+1]=(wdata>>8) & 0x00FF;
        } else {
            data[1]='\0';
        }
        console.Write(itos(*data));
    }
    for(uint32 i=count+(count % 2);i<512;i+=2) {
        _asm_.inw(port);
    }  
}

void HardDrive::Write28(uint32 sector, uint8 *data, int count) {
    Assembly _asm_;
    Console console;

    if(sector > 0x0FFFFFFF)
        return;

/*
    if (sector & 0xF0000000)
        return;
        */
    if (count > bytePerSector)
        return;

    _asm_.out(port+0x6, (usemaster ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24) & 0x0F);
    _asm_.out(port+0x1, 0);
    _asm_.out(port+0x2, 1);
    _asm_.out(port+0x3, sector &0x000000FF);
    _asm_.out(port+0x4, (sector&0x0000FF00)>> 8);
    _asm_.out(port+0x5, (sector&0x00FF0000)>>16);
    _asm_.out(port+0x7, 0x30);

    console.WriteLine("Writing to Drive: ");
    for(uint16 i=0;i<bytePerSector;i+=2)
    {
        
        uint16 wdata = data[i];
        if (i+1<count)
            wdata |= ((uint16)data[i+1]) << 8;
        char* test = "   \0";
        _asm_.outw(port, wdata);
        test[1] = (wdata>>8)&0x00FF;
        test[0] = wdata & 0x00FF;
        console.Write(test);

    }
    for(uint16 i=count+(count % 2);i<bytePerSector;i+=2) {
        _asm_.outw(port, 0x0000);
    }
}

void HardDrive::Flush()
{
    Assembly _asm_;
    Console console;
    _asm_.out(port+0x6, usemaster ? 0xE0 : 0xF0);
    _asm_.out(port+0x7, 0xE7);

    uint8 status = _asm_.in(port+0x7);
    while(((status & 0x80) == 0x80) && (status & 0x01) != 0x01)
    {
        status = _asm_.in(port+0x7);
    }

    if (status & 0x01) {
        console.WriteLine("Error in flushing drive.");
        return;
    }
}