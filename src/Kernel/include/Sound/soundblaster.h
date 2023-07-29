#ifndef __SOUNDBLASTER_H__
#define __SOUNDBLASTER_H__

#define MIXERPORT     0x224
#define MIXERDATAPORT 0x225
#define RESET         0x226
#define READ          0x22A
#define WRITE         0x22C
#define READ_STATUS   0x22E
#define INTERRUPT     0X22F

#define MONO   0x0
#define STEREO 0x1


class SoundBlasterDriver {
    public:
        SoundBlasterDriver(int channel_number, int channel);
        ~SoundBlasterDriver();
        void Start();
        void Disable();
        void Reset();
        void Test();
        void Test(int inthz);
};

#endif