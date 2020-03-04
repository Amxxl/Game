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
        void Play(FMOD::System* pSystem, Vector3f const& vPosition, float volume);
        void Stop();
        void Pause();
        void Resume();

        bool IsPlaying() const;

        void SetVolume(float volume);

        float GetVolume() const noexcept { return m_fVolume; }

    private:
        float m_fVolume = 0.5f;

    private:
        FMOD::Sound* pSound = nullptr;
        FMOD::Channel* pChannel = nullptr;
};

class Audio
{
    class AudioException : public std::exception
    {
        public:
            AudioException(FMOD_RESULT result)
                : result(result)
            {
            }

            char const* GetType() const noexcept
            {
                return "Audio Exception";
            }

            char const* what() const noexcept override
            {
                std::ostringstream oss("");
                oss << GetType() << ": " << FMOD_ErrorString(result) << "\n";
                return oss.str().c_str();
            }

        private:
            FMOD_RESULT result;
    };

    public:
        Audio();
        ~Audio();

        void Update();

        bool Load(std::string const& fileName);
        bool Play(std::string const& fileName, Vector3f const& vPosition = { 0.0f, 0.0f, 0.0f }, float volume = 0.5f);
        void Stop(std::string const& fileName);
        void Pause(std::string const& fileName);
        void Resume(std::string const& fileName);

        /* Must be called each frame */
        void SetListener(Vector3f const& position, DirectX::XMVECTOR const& forward);

        FMOD::System* GetSystem() const { return pSystem; }

        void SpawnControlWindow();

    public:
        static void ErrorCheck(FMOD_RESULT result);

    private:
        FMOD::System* pSystem = nullptr;
        FMOD::ChannelGroup* pMasterChannel = nullptr;
        std::unordered_map<std::string, std::unique_ptr<Sound>> sounds;
};