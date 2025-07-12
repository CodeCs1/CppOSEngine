#include<Core.h>


class Main : public kernel {
  public:
    void BeforeStart() override {
	} 
    void Start() override {
    	Console::WriteLine("hello from C++OSEngine Test");
    	string2 s = Console::ReadLine();
		Console::WriteLine(s);
    	Console::WriteLine("End of script!");
    }
};

CALL_MAIN_KERNEL(Main)
