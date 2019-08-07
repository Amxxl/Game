#include "pch.h"
#include "SceneManager.h"


SceneManager::SceneManager(Window& window)
    : m_window(window)
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
    scenes.emplace_back(scene);
    scenes.back()->Load(this, m_window);
}

void SceneManager::PushScene(Scene* scene)
{
    // Pause current scene
    if (!scenes.empty())
        scenes.back()->Pause();

    // Store and load new scene
    scenes.emplace_back(scene);
    scenes.back()->Load(this, m_window);
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

void SceneManager::OnKeyChar(size_t key)
{
    if (!scenes.empty())
        scenes.back()->OnKeyChar(key);
}

void SceneManager::OnKeyPressed(size_t key)
{
    if (!scenes.empty())
        scenes.back()->OnKeyPressed(key);
}

void SceneManager::OnKeyReleased(size_t key)
{
    if (!scenes.empty())
        scenes.back()->OnKeyReleased(key);
}

void SceneManager::OnMouseMoved(Vector2i const& position)
{
    if (!scenes.empty())
        scenes.back()->OnMouseMoved(position);
}

void SceneManager::OnMouseMovedRaw(Vector2i const& position)
{
    if (!scenes.empty())
        scenes.back()->OnMouseMovedRaw(position);
}

void SceneManager::OnMouseWheelScrolled(Vector2i const& position, float const delta)
{
    if (!scenes.empty())
        scenes.back()->OnMouseWheelScrolled(position, delta);
}

void SceneManager::OnMouseButtonPressed(Vector2i const& position, Input::MouseButton const button)
{
    if (!scenes.empty())
        scenes.back()->OnMouseButtonPressed(position, button);
}

void SceneManager::OnMouseButtonReleased(Vector2i const& position, Input::MouseButton const button)
{
    if (!scenes.empty())
        scenes.back()->OnMouseButtonReleased(position, button);
}

void SceneManager::OnMouseButtonDoubleClicked(Vector2i const& position, Input::MouseButton const button)
{
    if (!scenes.empty())
        scenes.back()->OnMouseButtonDoubleClicked(position, button);
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