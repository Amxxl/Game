//
// SceneManager.h - @todo: add input devices event handling.
//

#pragma once

#include "Scene.h"
#include "Window.h"

class SceneManager
{
    public:
        SceneManager() = delete;
        SceneManager(SceneManager const&) = delete;
        SceneManager& operator=(SceneManager const&) = delete;

        explicit SceneManager(Window& window);
        ~SceneManager();

        // Scene management
        void SwapScene(Scene* scene);
        void PushScene(Scene* scene);
        void PopScene();

        void OnKeyChar(size_t key);
        void OnKeyPressed(size_t key);
        void OnKeyReleased(size_t key);

        void OnMouseMoved(Vector2i const& position);
        void OnMouseMovedRaw(Vector2i const& position);
        void OnMouseWheelScrolled(Vector2i const& position, float const delta);
        void OnMouseButtonPressed(Vector2i const& position, Input::MouseButton const button);
        void OnMouseButtonReleased(Vector2i const& position, Input::MouseButton const button);
        void OnMouseButtonDoubleClicked(Vector2i const& position, Input::MouseButton const button);

        void Update(DX::StepTimer const& timer);
        void Render();

    private:
        Window& m_window;
        std::vector<Scene*> scenes;
};