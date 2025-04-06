.PHONY: build install

DESTDIR:=/opt/CppOSEngine

HEADER_PATH:=/usr/include
src:=$(shell pwd)/src
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
	-fno-leading-underscore \
	-mno-red-zone \
	-Wwrite-strings \
	-Wno-unused-variable \
	-pedantic \
	-finline-functions \
	-nostdinc \
	-ffreestanding  \
	-Wno-unused-parameter \
	-fno-permissive

Cobject := $(patsubst ${src}/Kernel/%.cpp, ${src}/Kernel/object/%.o, $(shell find ${src}/Kernel -name *.cpp))
asmobj := ${src}/Kernel/object/boot.o

$(Cobject): ${src}/Kernel/object/%.o : ${src}/Kernel/%.cpp
	@if [ ! -d $(shell dirname $@) ]; then mkdir -p $(shell dirname $@); fi
	${CXX_COMPILER} ${gccMAINcommandline} $< -o $@ ${gccEXTRAcommandline}

$(asmobj): ${src}/Kernel/object/%.o: ${src}/%.asm
	@if [ ! -d $(shell dirname $@) ]; then mkdir -p $(shell dirname $@); fi
	nasm -f elf32 $< -o $@


all: build install

build: $(asmobj) $(Cobject)
	ar rv libCppOSEngine.a $(asmobj) $(Cobject)


install:
	mkdir -p ${DESTDIR}
	mkdir -p /usr/include/Kernel

	find src/Kernel/include -type f -exec install -Dm 755 "{}" "/usr/include/Kernel" \;

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

test:
	@make -f tests/Makefile

uninstall:
	@echo Removing all...
	@rm -rvf /usr/include/Kernel
	@rm -rvf /opt/CppOSEngine

clean:
	@echo Cleaning...
	@rm -rvf ${src}/Kernel/object
	@rm -rvf ./libCppOSEngine.a