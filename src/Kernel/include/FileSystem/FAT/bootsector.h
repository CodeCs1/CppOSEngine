#ifndef __BOOTSECTOR_H__
#define __BOOTSECTOR_H__

//This OS ONLY Support FAT32 System only

#include "../../types.h"
#include "../../harddrive.h"

struct PartitionEntry {
	uint8 bootable;
	uint8 start_head;
	uint8 start_sector:6;
	uint16 start_cylinder:10;
	uint8 partitionid;
	uint8 end_head;
	uint8 end_sector:6;
	uint16 end_cylinder:10;

	uint32 start_lba;
	uint32 len;
}__attribute__((packed));

struct MBR {
	uint8 bootloader[440];
	uint32 signature;
	uint16 unused;

	PartitionEntry primaryPartition[4];
	uint16 magic_number;
}__attribute__((packed));

class MasterBootRecord {
    public:
        void Initialize_Partition(HardDrive* hd);
		void ReadBiosBlock(HardDrive* hd, uint32 offset);
};

#endif