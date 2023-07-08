#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "../../types.h"
#include "../../harddrive.h"
#include "bootsector.h"
#include "filesystem2.h"

typedef struct BIOSBlock32{
	uint8 jump[3];
	uint8 softName[8];
	uint16 bytesPerSector;
	uint8 sectorsPerCluster;
	uint16 reservedSectors;
	uint8 fatcopies;
	uint16 rootDirectoryEn;
	uint16 totalSectors;
	uint8 mediaType;
	uint16 fatSectorCount; //will be 0
	uint16 sectorsPerTrack;
	uint16 headCount;
	uint32 hiddenSectors;
	uint32 totalSectorCount;
	uint32 tableSize;
	uint32 extensionFlags; //.exe, .png, ...
	uint16 fatversion;
	uint32 rootCluster;
	uint16 fatInfo;
	uint8 reserved0[12];
	uint8 driveNumber; //0: | 1: | 2: | ...
	uint8 reserved;
	uint8 bootSign;
	uint32 volid;
	uint8 volLabel[11];
	uint8 fatType[8];
}__attribute__((packed)) BIOSBlock32_t;

typedef struct DirectoryEn {
	uint8 name[8];
	uint8 extension[3];
	uint8 attribytes;
	uint8 reserved;
	uint8 TimeDate;
	uint16 Time;
	uint16 Date;
	uint16 aTime;

	uint16 firstClusterHigh;

	uint16 wTime;
	uint16 wDate;

	uint16 firstClusterLow;
	uint32 size;

}__attribute__((__packed__)) DirectoryEn_t;

class FAT32_Creation {
    public:
        void ReadBiosBlock(HardDrive* hd, uint32 offset);
};

#endif