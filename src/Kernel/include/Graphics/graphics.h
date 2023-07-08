#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "../types.h"

class Graphics {
    protected:
        virtual void WriteReg(uint8* reg);
        virtual uint8* GetFrameBuffSeg();

        virtual uint8 GetColorIndex(uint8 r, uint8 g, uint8 b);

    public:
        Graphics();
        ~Graphics();

        void Initialize();

        bool SupportsMode(uint32 width, uint32 height, uint32 colordep);
        bool setmode(uint32 width, uint32 height,uint32 colordep);
        void PutPixel(uint32 x, uint32 y, uint8 color_index);
        void PutPixel(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b);

};

#endif