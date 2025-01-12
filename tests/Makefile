test:
	g++ -m32 -c tests/main.cpp -o tests/main.o  -fno-stack-protector    -fno-use-cxa-atexit     -nostdlib       -fno-builtin   -fno-rtti       -fno-exceptions         -fno-leading-underscore         -mno-red-zone   -Wwrite-strings         -Wno-unused-variable    -pedantic      -finline-functions      -nostdinc       -ffreestanding          -Wno-unused-parameter   -fno-permissive -I/usr/include
	ld -T /opt/CppOSEngine/linker.ld -o tests/test.bin tests/main.o ./libCppOSEngine.a
	qemu-system-x86_64 -kernel tests/test.bin