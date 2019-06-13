#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"

class Audio
{
    public:
        Audio();
        ~Audio();

        void Update();

        FMOD::System* GetSystem() const { return system; }

    private:
        FMOD::System* system = nullptr;
};