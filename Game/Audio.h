#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"

#include "Vector3.h"

class Audio
{
    public:
        Audio();
        ~Audio();

        void Update();

        bool LoadSound(std::string const& fileName, bool in_3d = true, bool loop = false, bool stream = false);
        int PlaySound(std::string const& fileName, Vector3 const& vPosition = { 0.0f, 0.0f, 0.0f }, float volume = 0.5f);


        void StopChannel(unsigned int channelId);

        void StopAllChannels();

        FMOD::System* GetSystem() const { return pSystem; }

    private:
        bool ErrorCheck(FMOD_RESULT result);

    private:
        using SoundMap      = std::unordered_map<std::string, FMOD::Sound*>;
        using ChannelMap    = std::unordered_map<unsigned int, FMOD::Channel*>;

    private:
        FMOD::System*   pSystem = nullptr;

        SoundMap        m_sounds;
        ChannelMap      m_channels;
        unsigned int    m_iChannelIndex;
};