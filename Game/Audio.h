#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"

#include "Vector3.h"


class Audio;

class Sound
{
    public:
        Sound() = default;
        bool Load(FMOD::System* pSystem, std::string const& fileName);
        bool Play(FMOD::System* pSystem, Vector3f const& vPosition, float volume);
        bool Stop();
        bool Pause();
        bool Resume();

        bool IsPlaying() const;

        bool SetVolume(float volume = 1.0f);

    private:
        FMOD::Sound* pSound = nullptr;
        FMOD::Channel* pChannel = nullptr;
};

class Audio
{
    public:
        Audio();
        ~Audio();

        void Update();

        bool Load(std::string const& fileName);
        bool Play(std::string const& fileName, Vector3f const& vPosition = { 0.0f, 0.0f, 0.0f }, float volume = 0.7f);
        void Stop(std::string const& fileName);
        void Pause(std::string const& fileName);
        void Resume(std::string const& fileName);

        /* Must be called each frame */
        void SetListener(Vector3f const& position, DirectX::XMVECTOR const& forward);

        FMOD::System* GetSystem() const { return pSystem; }

        void SpawnControlWindow();

    public:
        static bool ErrorCheck(FMOD_RESULT result);

    private:
        FMOD::System* pSystem = nullptr;
        std::unordered_map<std::string, std::unique_ptr<Sound>> sounds;
};