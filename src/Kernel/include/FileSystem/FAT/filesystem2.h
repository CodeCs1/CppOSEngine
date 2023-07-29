#ifndef __FILESYSTEM2_H__
#define __FILESYSTEM2_H__

#include "../../types.h"
#include "../../harddrive.h"

struct BiosParameterBlock {
    uint8 BytePerLocalSector[3];
    uint8 OEM_id[8];
    uint16 Bytespersector;
    uint8 sectorspercluster;
    uint16 reserved;
    uint16 FAT : 2;
    uint16 RootEntry;
    uint16 TotalSectors;
    uint16 MDT;
    uint16 NumberTrack;
    uint16 HeadsOrSides;
    uint16 HiddenSectors;
    uint16 LargesectorCount;
}__attribute__((packed));

struct FAT32 {
    uint8 Sectors;
    uint8 Flags;
    uint8 Version;
    uint8 Cluster;
    uint8 Sector;
    uint8 RootSectors;
    uint16 Reserved;
    uint8 DriveNumber;
    uint8 FlagsNT;
    uint8 Sign;
    uint8 Volume_ID;
    uint16 Label;
    uint8 Sys_Id;
    uint32 bootcode;
    uint8 BootSign; //MUST be 510 (+2)
}__attribute__((packed));

class FAT32_Creation2 {
    public:
        void ReadFat(HardDrive* hd, uint32 offset);
};

#endif