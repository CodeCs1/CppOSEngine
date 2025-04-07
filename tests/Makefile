test:
	g++ -m32 -c tests/main.cpp -o tests/main.o  -fno-stack-protector    -fno-use-cxa-atexit     -nostdlib       -fno-builtin   -fno-rtti       -fno-exceptions         -fno-leading-underscore         -mno-red-zone   -Wwrite-strings         -Wno-unused-variable    -pedantic      -finline-functions      -nostdinc       -ffreestanding          -Wno-unused-parameter   -fno-permissive -I./src/Kernel/include
	ld -T tests/linker.ld -o tests/test.bin tests/main.o ./libCppOSEngine.a
	mkdir -p tests/tests/boot/grub
	cp tests/test.bin tests/tests/boot/test.bin
	echo "set timeout=0" > tests/tests/boot/grub/grub.cfg
	echo "set default=0" >> tests/tests/boot/grub/grub.cfg
	echo "menuentry 'CppOS' {" >> tests/tests/boot/grub/grub.cfg
	echo "	multiboot /boot/test.bin" >> tests/tests/boot/grub/grub.cfg
	echo "	boot" >> tests/tests/boot/grub/grub.cfg
	echo "}" >> tests/tests/boot/grub/grub.cfg
	grub-mkrescue -o boot.iso tests/tests