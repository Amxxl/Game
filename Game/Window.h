//
// Window.h
//

#pragma once

#include "DeviceResources.h"
#include "Input.h"

class Window : public DX::IDeviceNotify, public EventDispatcher
{
    public:
        Window() = delete;
        explicit Window(std::wstring const& name, int width, int height, bool fullScreen = false) noexcept;
        ~Window();
        Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;
        Window(Window&&) = delete;

        void SetTitle(std::wstring const& title);
        HWND GetHandle() const { return m_hWindow; }

        virtual void OnDeviceLost() final override;
        virtual void OnDeviceRestored() final override;

        DX::DeviceResources* GetDeviceResources() const { return m_deviceResources.get(); }

        Vector2i const GetSize() const { return { m_iWidth, m_iHeight }; }
        Input& GetInput() { return input; }
        Input const& GetInput() const { return input; }

        void ShowCursor();
        void HideCursor();

    private:
        Input input;

    private:
        static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        
    private:
        LRESULT CALLBACK HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        // Direct3D Device Resources.
        std::unique_ptr<DX::DeviceResources> m_deviceResources;

        HINSTANCE m_hInstance;
        std::wstring m_wsTitle;
        int m_iWidth, m_iHeight;
        bool m_bFullScreen;
        bool m_bCursorVisible;
        HWND m_hWindow;
};