#include "../../include/FileSystem/FAT/filesystem2.h"
#include "../../include/harddrive.h"
#include "../../include/console.h"

void FAT32_Creation2::ReadFat(HardDrive *hd, uint32 offset) {
    BiosParameterBlock BPB;
    Hex hex;
    Console console;
    hd->Read28(offset, (uint8 *)&BPB, sizeof(BiosParameterBlock));


    for (uint8 i=0; i<sizeof(BiosParameterBlock);i++) {
        hex.printfHex(((uint8*)&BPB)[i]);
    }
}