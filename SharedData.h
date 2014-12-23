#pragma once

static const string SHARED_DATA_KEY = "brocra";

// VFX Window Num
#define NUM_VFXWIN 8

// buffer size
#define WAVE_SIZE 256

// toggle num
#define NUM_TOGGLE 5

struct Rectangle
{
    int x;
    int y;
    int w;
    int h;
};

class SharedData
{
public:
    float       level;                              ///< Audio level
    float       wave[WAVE_SIZE];                    ///< Wave scope
    int         mode;                               ///< Window moving mode
    bool        bang_switch;                        ///< Bang switch
    bool        toggles[NUM_VFXWIN][NUM_TOGGLE];    ///< toggles * vfx window number (ID)
    Rectangle   rect[NUM_VFXWIN];                   ///< Windows position and shape
    bool        kill;
};
