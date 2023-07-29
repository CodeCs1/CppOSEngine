#ifndef __HARDDRIVE_H__
#define __HARDDRIVE_H__

#include "types.h"

class HardDrive {
    protected:
        //data port = 16bit
        //error port = 8bit
        //sector port = 8bit
        //lbaLow port = 8bit
        //lbamid port = 8bit
        //lbahigh port = 8bit
        //device port = 8bit
        //command port = 8bit
        //control port = 8bit
        bool usemaster;
        uint16 bytePerSector;
        uint16 port;
    public:

        HardDrive(uint16 port, bool usemaster);
        ~HardDrive();
        void Identify();
        void Read28(uint32 sector, uint8 *data, int count=512);
        void Write28(uint32 sector, uint8 *data,int count);
        void Flush();
};

#endif