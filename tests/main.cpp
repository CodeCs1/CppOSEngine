#include <Core.h>

class Main : public kernel {
  public:
    void BeforeStart() override { }
    void Start() override {
      Console::Clear();
      Console::WriteLine("Hello World");
      Console::WriteLine("This is a test of the kernel.");
      string2 str = Console::ReadLine();
      Console::WriteLine("You entered: ");
      Console::WriteLine(str);
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
  kernel->Start();
  kernel->AfterStart();
}
