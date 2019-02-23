//
// Application.h
//

#pragma once

#include "Window.h"
#include "DeviceResources.h"
#include "StepTimer.h"

#include "Mouse.h"

#include "SceneManager.h"
#include "PlayScene.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


class Application : public DX::IDeviceNotify
{
    public:
        Application() noexcept(false);

        // Initialization and management
        void Initialize(int width, int height);

        // Basic application loop
        void Tick();

        // IDeviceNotify
        virtual void OnDeviceLost() override;
        virtual void OnDeviceRestored() override;

        // Messages
        void OnActivated();
        void OnDeactivated();
        void OnSuspending();
        void OnResuming();
        void OnWindowMoved();
        void OnWindowSizeChanged(int width, int height);

        // Properties
        void GetDefaultSize(int& width, int& height) const;

        Mouse* GetMouse() const { return m_mouse.get(); }

    private:
        void Update(DX::StepTimer const& timer);
        void Render();

        void Clear();

        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();

        // Basic Win32 Window.
        std::unique_ptr<Window>                 m_window;

        // Direct3D Device Resources.
        std::unique_ptr<DX::DeviceResources>    m_deviceResources;

        // Rendering loop timer.
        DX::StepTimer                           m_timer;

        // Keyboard and Mouse input.
        std::unique_ptr<DirectX::Keyboard>      m_keyboard;
        std::unique_ptr<Mouse>                  m_mouse;

        std::unique_ptr<SceneManager>           m_sceneManager;
};