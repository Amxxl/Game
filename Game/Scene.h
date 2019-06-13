//
// Scene.h
//

#pragma once

#include "StepTimer.h"
#include "Window.h"

class SceneManager;

class Scene
{
    protected:
        Scene() = default;
        Scene(Scene const&) = delete;
        Scene& operator=(Scene const&) = delete;
        virtual ~Scene() = default;

    public:
        virtual bool Load(SceneManager* sceneManager, Window& window) = 0;
        virtual void Unload() = 0;

        virtual void Update(DX::StepTimer const& timer) = 0;
        virtual void Render() = 0;

        virtual void Pause() = 0;
        virtual void Resume() = 0;

    public:
        virtual void OnKeyPressed(size_t key) {}
        virtual void OnKeyReleased(size_t key) {}

    public:
        virtual void OnMouseMoved(Vector2i const& position) {}
        virtual void OnMouseMovedRaw(Vector2i const& position) {}
        virtual void OnMouseWheelScrolled(Vector2i const& position, float const delta) {}
        virtual void OnMouseButtonPressed(Vector2i const& position, Input::MouseButton const button) {}
        virtual void OnMouseButtonReleased(Vector2i const& position, Input::MouseButton const button) {}
        virtual void OnMouseButtonDoubleClicked(Vector2i const& position, Input::MouseButton const button) {}
};