#include "pch.h"
#include "SceneManager.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
    while (!scenes.empty())
    {
        scenes.back()->Unload();
        scenes.pop_back();
    }
}

void SceneManager::SwapScene(Scene* scene)
{
    // Unload current scene
    if (!scenes.empty())
    {
        scenes.back()->Unload();
        scenes.pop_back();
    }

    // Store and load new scene
    scenes.push_back(scene);
    scenes.back()->Load();
}

void SceneManager::PushScene(Scene* scene)
{
    // Pause current scene
    if (!scenes.empty())
        scenes.back()->Pause();

    // Store and load new scene
    scenes.push_back(scene);
    scenes.back()->Load();
}

void SceneManager::PopScene()
{
    // Unload current scene
    if (!scenes.empty())
    {
        scenes.back()->Unload();
        scenes.pop_back();
    }

    // Resume previous scene
    if (!scenes.empty())
        scenes.back()->Resume();
}

void SceneManager::Update(DX::StepTimer const& timer)
{
    if (scenes.empty())
        return;

    scenes.back()->Update(timer);
}

void SceneManager::Render()
{
    if (scenes.empty())
        return;

    scenes.back()->Render();
}