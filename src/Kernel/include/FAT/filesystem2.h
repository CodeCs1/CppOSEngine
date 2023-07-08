#ifndef __FILESYSTEM2_H__
#define __FILESYSTEM2_H__

#include "../../types.h"
#include "../../harddrive.h"

typedef struct FAT32_Filesystem {

    uint32  tablesize;
    uint16  extended_flags;
    uint16  fat_version;
    uint32  root_cluster;
    uint16  fat_info;
    uint16  backup_sector;
    uint8   reserved0[12];
    uint8   drive_number;
    uint8   reserved1;
    uint8   boot_sign;
    uint32  volume_id;
    uint8   volume_label[11];
    uint8   fat_type_label[8];

}__attribute__((packed)) FAT32_Filesystem_t;

typedef struct FAT_Filesystem {
    uint8   bootjump[3];
    uint8   OEM_name[8];
    uint16  bytesPerSector;
    uint8   sectorsPerCluster;
    uint16  reservedSectorCount;
    uint8   tableCount;
    uint16  rootEntry;
    uint16  totalSectorCount16;
    uint8   mediaType;
    uint16  tableSize16;
    uint16  sectorsPerTrack;
    uint16  headSideCount;
    uint32  hiddenSectors;
    uint32  totalSectors32;

}__attribute__((packed)) FAT_Filesystem_t;

typedef struct DirectoryEntry {
    uint16 filename[8];
    uint8  extension[3];
    uint8  attributes;
    uint8  reserved;
    uint16 ctime; //create time
    uint16 ptime; //past time 
    uint16 pdate; //past date
    uint16 ldate; //last accessed date

    uint16 firstClusterHigh;

    uint16 lmodtime;
    uint16 lmoddate;

    uint16 firstClusterLow;
    uint16 size;

}__attribute__((packed)) DirectoryEntry_t;

class FAT32_Creation2 {
    public:
        void ReadFat(HardDrive* hd, uint32 offset);
};

#endif