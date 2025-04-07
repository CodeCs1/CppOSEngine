#include <Core.h>
#include <pmm.h>

class Main : public kernel {
  public:
    void BeforeStart() override { }
    void Start() override {
      Console::WriteLine("Hello World");
      Console::WriteLine("This is a test of the kernel.");
      string2 str = Console::ReadLine();
      Console::WriteLine("You entered: ");
      Console::WriteLine(str);
    }
    void AfterStart() override { }
};

extern "C" uint32_t kern_sz;
extern "C" uint32_t kern_start;

extern "C" void main_kernel(uint32_t magic, Multiboot_info* info) {
  GDT::Init();
  Interrupt::idt_install();
  InterruptRequiests::Init();
  InterruptServiceRoutines::Init();  
  Main main;
  kernel* kernel = &main;
  if (magic != 0x2BADB002) {
    Console::Write("Invalid magic number: ");
    Hex::printfHex32(magic);
    return;
  }

  uint32_t memsz = 1024+info->mem_lower+info->mem_upper*64;

  Console::WriteLine("Kernel size: ");
  Hex::printfHex32(kern_start);

  PMM::Init(memsz, 0x100000);


  //uint32_t* test1 = (uint32_t*)PMM::Alloc(4096);
  Console::WriteLine("Allocated memory at: ");

  kernel->BeforeStart();
  kernel->Start();
  kernel->AfterStart();
}
