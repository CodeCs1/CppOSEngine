.PHONY: build install

DESTDIR:=/opt/CppOSEngine

HEADER_PATH:=/usr/include

src:=./src

INCLUDE_FOLDER=${src}/Kernel/include

CXX_COMPILER=g++

gccMAINcommandline+= \
	-m32 \
	-c

gccEXTRAcommandline+= \
	-fno-stack-protector \
	-fno-use-cxa-atexit \
	-nostdlib \
	-fno-builtin \
	-fno-rtti \
	-fno-exceptions \
	-fno-leading-underscore

filesystem_include += ${INCLUDE_FOLDER}/FileSystem/FAT/*.h 

graphics_include += ${INCLUDE_FOLDER}/Graphics/*.h 

sound_include += ${INCLUDE_FOLDER}/Sound/*.h

include += \
	${INCLUDE_FOLDER}/*.h \

object += \
	${src}/Kernel/object/boot.o \
	${src}/Kernel/object/console.o \
	${src}/Kernel/object/FatFS.o \
	${src}/Kernel/object/FatFS2.o \
	${src}/Kernel/object/kernel.o \
	${src}/Kernel/object/strings.o \
	${src}/Kernel/object/utils.o \
	${src}/Kernel/object/ACPI.o \
	${src}/Kernel/object/Mouse.o \
	${src}/Kernel/object/MBR.o \
	${src}/Kernel/object/graphics.o \
	${src}/Kernel/object/harddrive.o \
	${src}/Kernel/object/sound.o \
	${src}/Kernel/object/soundblaster16.o

asm += \
	${src}/boot.asm

all: build install

build:
	mkdir -p ${src}/Kernel/object
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/console.cpp -o ${src}/Kernel/object/console.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/kernel.cpp -o ${src}/Kernel/object/kernel.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/strings.cpp -o ${src}/Kernel/object/strings.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/utils.cpp -o ${src}/Kernel/object/utils.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/ACPI.cpp -o ${src}/Kernel/object/ACPI.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/Graphics/Mouse.cpp -o ${src}/Kernel/object/Mouse.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/FileSystem/FAT/bootsector.cpp -o ${src}/Kernel/object/MBR.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/FileSystem/FAT/filesystem.cpp -o ${src}/Kernel/object/FatFS.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/FileSystem/FAT/filesystem2.cpp -o ${src}/Kernel/object/FatFS2.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/harddrive.cpp -o ${src}/Kernel/object/harddrive.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/sound/sound.cpp -o ${src}/Kernel/object/sound.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/sound/soundblaster.cpp -o ${src}/Kernel/object/soundblaster16.o ${gccEXTRAcommandline}
	${CXX_COMPILER} ${gccMAINcommandline} ${src}/Kernel/Graphics/graphics.cpp -o ${src}/Kernel/object/graphics.o ${gccEXTRAcommandline}
	@echo Building Assembly...
	nasm -f elf32 ${asm} -o ${src}/Kernel/object/boot.o

install:
	mkdir -p ${DESTDIR}
	mkdir -p /usr/include/Kernel
	install ${object} ${DESTDIR}
	install ${include} /usr/include/Kernel
	mkdir -p /usr/include/Kernel/FileSystem/FAT
	mkdir -p /usr/include/Kernel/Graphics
	mkdir -p /usr/include/Kernel/Sound
	install ${filesystem_include} /usr/include/Kernel/FileSystem/FAT
	install ${graphics_include} /usr/include/Kernel/Graphics
	install ${sound_include} /usr/include/Kernel/Sound
	@if [ -f ${DESTDIR}/linker.ld ]; then echo "removing linker.ld"; rm -rf ${DESTDIR}/linker.ld ; fi
	@echo adding linker.ld...
	@echo "ENTRY(start)" >> ${DESTDIR}/linker.ld
	@echo "OUTPUT_FORMAT(elf32-i386)" >> ${DESTDIR}/linker.ld
	@echo "OUTPUT_ARCH(i386:i386)" >> ${DESTDIR}/linker.ld
	@echo "SECTIONS" >> ${DESTDIR}/linker.ld
	@echo "{" >> ${DESTDIR}/linker.ld
	@echo "	. = 0x0100000;" >> ${DESTDIR}/linker.ld
	@echo "	.text :" >> ${DESTDIR}/linker.ld
	@echo "	{" >> ${DESTDIR}/linker.ld
	@echo "		*(.multiboot)" >> ${DESTDIR}/linker.ld
	@echo "		*(.text*)" >> ${DESTDIR}/linker.ld
	@echo "		*(.rodata)" >> ${DESTDIR}/linker.ld
	@echo "	}" >> ${DESTDIR}/linker.ld
	@echo "	.data  :" >> ${DESTDIR}/linker.ld
	@echo "	{" >> ${DESTDIR}/linker.ld
	@echo "		start_ctors = .; ">> ${DESTDIR}/linker.ld
	@echo "		KEEP(*( .init_array ));" >> ${DESTDIR}/linker.ld
	@echo "		KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));" >> ${DESTDIR}/linker.ld
	@echo "		end_ctors = .;" >> ${DESTDIR}/linker.ld
	@echo "		*(.data)" >> ${DESTDIR}/linker.ld
	@echo "	}" >> ${DESTDIR}/linker.ld
	@echo "	.bss  : ">> ${DESTDIR}/linker.ld
	@echo "	{" >> ${DESTDIR}/linker.ld
	@echo "		*(.bss)" >> ${DESTDIR}/linker.ld
	@echo "	}" >> ${DESTDIR}/linker.ld 
	@echo "	/DISCARD/ : { *(.fini_array*) *(.comment) }" >> ${DESTDIR}/linker.ld
	@echo "	}" >> ${DESTDIR}/linker.ld

uninstall:
	@echo Removing all...
	@rm -rvf /usr/include/Kernel
	@rm -rvf /opt/CppOSEngine

clean:
	@echo Cleaning...
	@rm -rvf ${src}/Kernel/object