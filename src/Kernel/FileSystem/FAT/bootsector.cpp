#include "../../include/string.h"
#include "../../include/FileSystem/FAT/bootsector.h"
#include "../../include/FileSystem/FAT/filesystem.h"
#include "../../include/FileSystem/FAT/filesystem2.h"
#include "../../include/types.h"
#include "../../include/kernel.h"
#include "../../include/harddrive.h"
#include "../../include/console.h"

void MasterBootRecord::Initialize_Partition(HardDrive* hd) {
    Assembly _asm_;
    MBR mbr;
    Console console;
    Hex hex;

    hd->Read28(0, (uint8*)&mbr, sizeof(MBR));
    console.WriteLine("MBR: ");
    for (int i=446;i<446+4*16+2;i++) {
        hex.printfHex(((uint8*)&mbr)[i]);
        console.WriteChr(' ');
    }
    console.WriteChr('\n');
    if (mbr.magic_number != 0xAA55) {
        console.WriteLine("ERROR: illegal_MBR");
        console.Write(itos(mbr.magic_number));
        hex.printfHex(mbr.magic_number);
        return;
    }
    for (int i =0;i<4;i++){

        FAT32_Creation fat32;

        if (mbr.primaryPartition[i].partitionid == 0x00)
            continue;

        console.Write("Partition: ");
        hex.printfHex(i&0xFF);

        if (mbr.primaryPartition[i].bootable == 0x80) {
            console.WriteLine("Bootable: ");
            console.WriteChr('\n');
        } else {
            console.WriteLine("Not Bootable: ");
            console.WriteChr('\n');
        }
        console.WriteChr('\n');
        hex.printfHex(mbr.primaryPartition[i].partitionid);
        hex.printfHex(mbr.primaryPartition[i].bootable);
        //fat32.ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
        fat32.ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
    }
}