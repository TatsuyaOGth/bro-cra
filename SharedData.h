#pragma once

static const string SHARED_DATA_KEY = "brocra";
#define WAVE_SIZE 256

struct SharedDataStruct
{
    float   level;              ///< Audio level
    float   wave[WAVE_SIZE];    ///< Wave scope
    int     mode;               ///< Window moving mode
};

static SharedDataStruct SharedData;
