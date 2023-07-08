#include "../../include/string.h"
#include "../../include/FileSystem/FAT/filesystem.h"
#include "../../include/FileSystem/FAT/bootsector.h"
#include "../../include/types.h"
#include "../../include/kernel.h"
#include "../../include/harddrive.h"
#include "../../include/console.h"

void FAT32_Creation::ReadBiosBlock(HardDrive *hd, uint32 offset) {
    BIOSBlock32 biosblock;
    Hex hex;
    Console console;
    hd->Read28(offset, (uint8 *)&biosblock, sizeof(BIOSBlock32));

    for (int i=0x00;i<sizeof(BIOSBlock32);i++) {
        hex.printfHex(((uint8*)&biosblock)[i]);
        console.WriteChr(' ');
    }
    console.WriteLine("\n");

    uint32 fatStart = offset + biosblock.reservedSectors;
    uint32 fatsize = biosblock.tableSize;
    uint32 dataStart = fatStart + fatsize*biosblock.fatcopies;
    uint32 rootstart = dataStart + biosblock.sectorsPerCluster*(biosblock.rootCluster-2);

    DirectoryEn dirent[16];

    hd->Read28(rootstart, (uint8*)&dirent[0], 16*sizeof(DirectoryEn));

    for (int i=0;i<16;i++) {
        if (dirent[i].name[0] == 0x00) break;
        if ((dirent[i].attribytes & 0x0F) == 0x0F) continue;

        char* test = "        ";

        for (int a=0;a<8;a++) {
            test[a] = dirent[i].name[a];
        }
        console.WriteLine(test);

        if ((dirent[i].attribytes & 0x10) == 0x10)
            continue;

        uint32 fileCluster = ((uint32)dirent[i].firstClusterHigh) << 16 | ((uint32)dirent[i].firstClusterLow);
        uint32 fileSector  = dataStart + biosblock.sectorsPerCluster * (fileCluster-2);

        uint8 buffer[513];

        hd->Read28(fileSector, buffer, 512);
        buffer[dirent[i].size] = '\0';
        console.WriteLine((char*)buffer);

    }

}