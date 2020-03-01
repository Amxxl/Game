#include "pch.h"
#include "Audio.h"
#include "imgui.h"
#include <assert.h>

Audio::Audio()
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
    Audio::ErrorCheck(pSystem->update());
}

bool Audio::Load(std::string const& fileName)
{
    std::unique_ptr<Sound> pSound = std::make_unique<Sound>();

    if (!pSound->Load(pSystem, fileName))
        return false;

    sounds[fileName] = std::move(pSound);
    return true;
}

bool Audio::Play(std::string const& fileName, Vector3f const& vPosition, float volume)
{
    if (sounds[fileName] == nullptr)
    {
        if (!Load(fileName))
            return false;
    }

    if (sounds[fileName]->IsPlaying())
        return false;

    return sounds[fileName]->Play(pSystem, vPosition, volume);
}

void Audio::Stop(std::string const& fileName)
{
    if (sounds[fileName] == nullptr)
        return;

    sounds[fileName]->Stop();
}

void Audio::Pause(std::string const& fileName)
{
    if (sounds[fileName] == nullptr)
        return;

    sounds[fileName]->Pause();
}

void Audio::Resume(std::string const& fileName)
{
    if (sounds[fileName] == nullptr)
        return;

    sounds[fileName]->Resume();
}

void Audio::SetListener(Vector3f const& position, DirectX::XMVECTOR const& forward)
{
    assert(pSystem == nullptr);
    FMOD_VECTOR pos{ position.x, position.y, position.z };
    FMOD_VECTOR up{ 0.0f, 1.0f, 0.0f };

    Vector3f fwd;
    DirectX::XMStoreFloat3(&fwd, forward);
    FMOD_VECTOR vecFwd{ fwd.x, fwd.y, fwd.z };

    pSystem->set3DListenerAttributes(0, &pos, nullptr, &vecFwd, &up);
}

void Audio::SpawnControlWindow()
{
    for (auto&& i : sounds)
    {
        ImGui::Begin(i.first.c_str());

        ImGui::Text("File: %s", i.first.c_str());

        if (ImGui::Button("Play"))
        {
            Play(i.first.c_str());
        }

        ImGui::SameLine();
        if (ImGui::Button("Pause"))
        {
            Pause(i.first.c_str());
        }

        ImGui::SameLine();
        if (ImGui::Button("Resume"))
        {
            Resume(i.first.c_str());
        }

        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            Stop(i.first.c_str());
        }

        static float volume = 0.5f;
        ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
        i.second->SetVolume(volume);

        ImGui::End();
    }
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

bool Sound::Load(FMOD::System* pSystem, std::string const& fileName)
{
    if (!Audio::ErrorCheck(pSystem->createSound(fileName.c_str(), FMOD_3D, nullptr, &pSound)))
        return false;

    if (pSound == nullptr)
        return false;

    // Temporary.
    if (!Audio::ErrorCheck(pSound->set3DMinMaxDistance(1.0f, 512.0f)))
        return false;

    // Temporary.
    if (!Audio::ErrorCheck(pSound->setMode(FMOD_3D | FMOD_3D_INVERSEROLLOFF)))
        return false;

    return true;
}

bool Sound::Play(FMOD::System* pSystem, Vector3f const& vPosition, float volume)
{
    if (pSystem == nullptr || pSound == nullptr)
        return false;

    if (!Audio::ErrorCheck(pSystem->playSound(pSound, nullptr, true, &pChannel)))
        return false;

    if (pChannel == nullptr)
        return false;

    FMOD_VECTOR position = { vPosition.x, vPosition.y, vPosition.z };
    FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

    if (!Audio::ErrorCheck(pChannel->set3DAttributes(&position, &velocity)))
        return false;

    if (!Audio::ErrorCheck(pChannel->set3DMinMaxDistance(1.0f, 512.0f)))
        return false;

    if (!Audio::ErrorCheck(pChannel->setVolume(volume)))
        return false;

    if (!Audio::ErrorCheck(pChannel->setPaused(false)))
        return false;

    return true;
}

bool Sound::Stop()
{
    if (pChannel == nullptr)
        return false;

    return Audio::ErrorCheck(pChannel->stop());
}

bool Sound::Pause()
{
    if (pChannel == nullptr)
        return false;

    return Audio::ErrorCheck(pChannel->setPaused(true));
}

bool Sound::Resume()
{
    if (pChannel == nullptr)
        return false;

    return Audio::ErrorCheck(pChannel->setPaused(false));
}

bool Sound::IsPlaying() const
{
    if (pChannel == nullptr)
        return false;

    int index = 0;
    assert(!Audio::ErrorCheck(pChannel->getIndex(&index)));
    return index > 0;
}

bool Sound::SetVolume(float volume)
{
    if (pChannel == nullptr)
        return false;

    return Audio::ErrorCheck(pChannel->setVolume(volume));
}