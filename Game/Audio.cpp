#include "pch.h"
#include "Audio.h"


Audio::Audio() : m_iChannelIndex(0)
{
    Audio::ErrorCheck(FMOD::System_Create(&pSystem));
    Audio::ErrorCheck(pSystem->init(512, FMOD_INIT_NORMAL, nullptr));
}


Audio::~Audio()
{
    Audio::ErrorCheck(pSystem->release());
}

void Audio::Update()
{
    pSystem->update();
}

bool Audio::LoadSound(std::string const& fileName, bool in_3d, bool loop, bool stream)
{
    FMOD::Sound* pSound = nullptr;
    if (!Audio::ErrorCheck(pSystem->createSound(fileName.c_str(), FMOD_3D, nullptr, &pSound)))
        return false;

    if (pSound == nullptr)
        return false;

    Audio::ErrorCheck(pSound->set3DMinMaxDistance(1.0f, 512.0f));
    Audio::ErrorCheck(pSound->setMode(FMOD_3D | FMOD_3D_INVERSEROLLOFF));
    m_sounds[fileName] = pSound;

    return true;
}

int Audio::PlaySound(std::string const& fileName, Vector3 const& vPosition, float volume)
{
    int channelId = m_iChannelIndex++;

    if (m_sounds.find(fileName) == m_sounds.end())
    {
        LoadSound(fileName);

        if (m_sounds.find(fileName) == m_sounds.end())
            return channelId;
    }

    FMOD::Channel* pChannel = nullptr;
    Audio::ErrorCheck(pSystem->playSound(m_sounds[fileName], nullptr, true, &pChannel));

    if (pChannel != nullptr)
    {
        FMOD_VECTOR position = { vPosition.x, vPosition.y, vPosition.z };
        FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

        Audio::ErrorCheck(pChannel->set3DAttributes(&position, &velocity));
        Audio::ErrorCheck(pChannel->set3DMinMaxDistance(1.0f, 512.0f));
        Audio::ErrorCheck(pChannel->setVolume(volume));
        Audio::ErrorCheck(pChannel->setPaused(false));

        m_channels[channelId] = pChannel;
    }

    Logger::Get()->info("FMOD: Playing on channel id: {}", channelId);

    return channelId;
}

void Audio::StopChannel(unsigned int channelId)
{
    m_channels[channelId]->stop();
}

void Audio::StopAllChannels()
{
    for (auto i : m_channels)
        i.second->stop();
}

bool Audio::ErrorCheck(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        Logger::Get()->error("FMOD: {}", FMOD_ErrorString(result));
        return false;
    }
    return true;
}