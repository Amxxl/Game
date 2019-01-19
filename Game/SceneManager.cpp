#include "pch.h"
#include "SceneManager.h"


SceneManager::SceneManager(ID3D11DeviceContext1* deviceContext)
{
    m_deviceContext = deviceContext;
}


SceneManager::~SceneManager()
{
    while (!scenes.empty())
    {
        scenes.back()->Unload();
        scenes.pop_back();
    }
}

void SceneManager::CreateDeviceDependentResources()
{
    if (scenes.empty())
        return;

    scenes.back()->CreateDeviceDependentResources();
}

void SceneManager::CreateWindowSizeDependentResources()
{
    if (scenes.empty())
        return;

    scenes.back()->CreateWindowSizeDependentResources();
}

void SceneManager::OnDeviceLost()
{
    if (scenes.empty())
        return;

    scenes.back()->OnDeviceLost();
}

void SceneManager::OnDeviceRestored()
{
    if (scenes.empty())
        return;

    scenes.back()->OnDeviceRestored();
}

void SceneManager::OnActivated()
{
    if (scenes.empty())
        return;

    scenes.back()->OnActivated();
}

void SceneManager::OnDeactivated()
{
    if (scenes.empty())
        return;

    scenes.back()->OnDeactivated();
}

void SceneManager::OnSuspending()
{
    if (scenes.empty())
        return;

    scenes.back()->OnSuspending();
}

void SceneManager::OnResuming()
{
    if (scenes.empty())
        return;

    scenes.back()->OnResuming();
}

void SceneManager::OnWindowMoved()
{
    if (scenes.empty())
        return;

    scenes.back()->OnWindowMoved();
}

void SceneManager::OnWindowSizeChanged(int width, int height)
{
    if (scenes.empty())
        return;

    scenes.back()->OnWindowSizeChanged(width, height);
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
    scenes.emplace_back(scene);
    scenes.back()->Load(m_deviceContext);
}

void SceneManager::PushScene(Scene* scene)
{
    // Pause current scene
    if (!scenes.empty())
        scenes.back()->Pause();

    // Store and load new scene
    scenes.emplace_back(scene);
    scenes.back()->Load(m_deviceContext);
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