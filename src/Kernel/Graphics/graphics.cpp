#include "../include/Graphics/graphics.h"
#include "../include/kernel.h"

Graphics::Graphics() {
    Assembly _asm_;
    _asm_.in(0x3c2), //misc
    _asm_.in(0x3d4), //crtcIndex
    _asm_.in(0x3d5), //crtcData
    _asm_.in(0x3c4), //SequencerIndex
    _asm_.in(0x3c5), //SequencerData
    _asm_.in(0x3ce), //Graphics controller index
    _asm_.in(0x3cf), //Graphics controller DATA
    _asm_.in(0x3c0), //AttributeControllerIndex
    _asm_.in(0x3c1), //AttributeControllerReadPort
    _asm_.in(0x3c0), //AttributeControllerWrite
    _asm_.in(0x3da); //AttributeControllerReset
}

Graphics::~Graphics() {}

void Graphics::Initialize() {
    Assembly _asm_;
    _asm_.in(0x3c2), //misc
    _asm_.in(0x3d4), //crtcIndex
    _asm_.in(0x3d5), //crtcData
    _asm_.in(0x3c4), //SequencerIndex
    _asm_.in(0x3c5), //SequencerData
    _asm_.in(0x3ce), //Graphics controller index
    _asm_.in(0x3cf), //Graphics controller DATA
    _asm_.in(0x3c0), //AttributeControllerIndex
    _asm_.in(0x3c1), //AttributeControllerReadPort
    _asm_.in(0x3c0), //AttributeControllerWrite
    _asm_.in(0x3da); //AttributeControllerReset
}

void Graphics::WriteReg(uint8* reg) {
    Assembly _asm_;
    _asm_.out(0x3c2, *(reg)++);

    for (uint8 i=0; i<5;i++) {
        _asm_.out(0x3c4, i);
        _asm_.out(0x3c5,*(reg++));
    }
    _asm_.out(0x3d4, 0x03);
    _asm_.out(0x3d5, _asm_.in(0x3d5) | 0x80);
    _asm_.out(0x3d4, 0x11);
    _asm_.out(0x3d5, _asm_.in(0x3d5) & ~0x80);

    reg[0x03] = reg[0x03] | 0x80;
    reg[0x11] = reg[0x11] & ~0x80;

    for (uint8 i=0;i<25;i++) {
        _asm_.out(0x3d4, i);
        _asm_.out(0x3d5, *(reg++));
    }
    for (uint8 i=0;i<9;i++) {
        _asm_.out(0x3ce, i);
        _asm_.out(0x3cf, *(reg++));
    }
    for (uint8 i=0;i<21;i++){
        _asm_.in(0x3da);
        _asm_.out(0x3c0, i); //Index
        _asm_.out(0x3c0,*(reg++)); //Write
    }

    _asm_.in(0x3da);
    _asm_.out(0x3c0, 0x20);
}

bool Graphics::SupportsMode(uint32 width, uint32 height, uint32 colordep) {
    return width == 320 && height == 200 && colordep == 256;
}

bool Graphics::setmode(uint32 width, uint32 height, uint32 colordep) {
    if (!SupportsMode(width, height, colordep))
        return false;
    unsigned char g_320x200x256[] =
    {
    /* MISC */
        0x63,
    /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
        0xFF,
    /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
    /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00,	0x00
    };
    WriteReg(g_320x200x256);
    return true;
}

uint8* Graphics::GetFrameBuffSeg() {
    Assembly _asm_;
    _asm_.out(0x3ce, 0x06);
    uint8 segnum = _asm_.in(0x3cf) & (3<<2);
    switch(segnum) {
        default:
        case 0<<2:
            return (uint8*)0x00000;
        case 1<<2:
            return (uint8*)0xA0000;
        case 2<<2:
            return (uint8*)0xB0000;
        case 3<<2:
            return (uint8*)0xB8000;
    }
}

void Graphics::PutPixel(uint32 x, uint32 y, uint8 color_index) {
    if (x<0 || 320 <=x || y<0 || 200<=y)
        return;

    uint8* pixelAdd = GetFrameBuffSeg() + 320*y+x;
    *pixelAdd = color_index;
}

uint8 Graphics::GetColorIndex(uint8 r, uint8 g, uint8 b) {
    if (r == 0x00 && g == 0x00 && b == 0x00) return 0x00; //black
    if (r == 0x00 && g == 0x00 && b == 0xA8) return 0x01; // blue
    if (r == 0x00 && g == 0xA8 && b == 0x00) return 0x02; // green
    if (r == 0xA8 && g == 0x00 && b == 0x00) return 0x04; // red
    if (r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F; // white
    return 0x00;
}


void Graphics::PutPixel(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b) {
    PutPixel(x,y,GetColorIndex(r, g, b));
}
