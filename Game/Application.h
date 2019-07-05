//
// Application.h
//

#pragma once

#include "Window.h"
#include "StepTimer.h"

#include "Mouse.h"

#include "SceneManager.h"
#include "PlayScene.h"

#include "Audio.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


class Application
{
    public:
        Application() noexcept(false);
        ~Application();

        // Initialization and management
        void Initialize(int width, int height);

        // Basic application loop
        void Tick();

        // Messages
        void OnActivated();
        void OnDeactivated();
        void OnSuspending();
        void OnResuming();
        void OnWindowMoved();
        void OnWindowSizeChanged(int width, int height);

        void OnEvent(EventKeyPressed const& event);
        void OnEvent(EventKeyReleased const& event);
        void OnEvent(EventMouseMoved const& event);
        void OnEvent(EventMouseMovedRaw const& event);
        void OnEvent(EventMouseWheelScrolled const& event);
        void OnEvent(EventMouseButtonPressed const& event);
        void OnEvent(EventMouseButtonReleased const& event);
        void OnEvent(EventMouseButtonDoubleClicked const& event);

        // Properties
        void GetDefaultSize(int& width, int& height) const;

        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();

        static Application& Get() { return *s_instance; }

    private:
        void Update(DX::StepTimer const& timer);
        void Render();

        void Clear();

    private:
        // Basic Win32 Window.
        std::unique_ptr<Window>                 m_window;

        // Rendering loop timer.
        DX::StepTimer                           m_timer;

        // Scene rendering management.
        std::unique_ptr<SceneManager>           m_sceneManager;

        std::unique_ptr<Audio>                  m_audio;

    private:
        static Application* s_instance;
};