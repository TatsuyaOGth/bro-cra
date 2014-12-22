#pragma once

static const string SHARED_DATA_KEY = "brocra";

#define WAVE_SIZE 256
#define NUM_VFXWIN 18
#define NUM_TOGGLE 5

class SharedData
{
public:
    float   level;                              ///< Audio level
    float   wave[WAVE_SIZE];                    ///< Wave scope
    int     mode;                               ///< Window moving mode
    bool    bang_switch;                        ///< Bang switch
    bool    toggles[NUM_VFXWIN][NUM_TOGGLE];    ///< toggles * vfx window number (ID)
};
