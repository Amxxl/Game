#include "pch.h"
#include "Audio.h"


Audio::Audio()
{
    DX::ExecutionTimer timer;

    FMOD_RESULT result;
    result = FMOD::System_Create(&system);

    if (result != FMOD_OK)
    {
        Logger::Get()->error("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        PostQuitMessage(1);
    }
    else
    {
        Logger::Get()->info("FMOD system created!");
    }

    result = system->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        Logger::Get()->error("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        PostQuitMessage(1);
    }
    else
    {
        Logger::Get()->info("FMOD system initialized!");
    }
}


Audio::~Audio()
{
}

void Audio::Update()
{
    system->update();
}
