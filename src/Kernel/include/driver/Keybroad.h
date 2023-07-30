#ifndef __KEYBROAD_H__
#define __KEYBROAD_H__

#include "../Core.h"

class Keybroad {
    private:
        char text_returned;        

    public:
        static void Init();

        void set_text(char text) {
            text_returned = text;
        }

        char get_text() {
            return text_returned;
        }

        static string get_keys();

};

#endif