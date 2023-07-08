#include "../../include/FileSystem/FAT/filesystem2.h"
#include "../../include/harddrive.h"
#include "../../include/console.h"

void FAT32_Creation2::ReadFat(HardDrive* hd, uint32 offset) {
    FAT32_Filesystem_t fat32;
    Hex hex;
    Console console;

    hd->Read28(offset,(uint8*)&fat32, sizeof(FAT32_Filesystem_t));

    for(int i=0;i<sizeof(FAT32_Filesystem_t);i++){
        hex.printfHex(((uint8*)&fat32)[i]);
        console.WriteChr(' ');
    }

}