//For convenient, include this instead.
#include <Kernel/Core.h>

class Main : public kernel {
  public:
    void BeforeStart() override { }
    void Start() override {
        Console::Clear();
        Console::WriteLine("Hello World");
	asm("int3");
    }
    void AfterStart() override { }
};

extern "C" void main_kernel() {
  GDT::Init();
  Interrupt::idt_install();
  InterruptRequiests::Init();
  InterruptServiceRoutines::Init();  
  Main main;
  kernel* kernel = &main;
  kernel->BeforeStart();
  while(1) {
    kernel->Start();
  }
  kernel->AfterStart();
}
