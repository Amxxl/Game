//
// Window.h
//

#pragma once

#include "DeviceResources.h"
#include "Input.h"

class Window : public DX::IDeviceNotify
{
    public:
        Window() = default;
        Window(std::wstring const& name, int width, int height) noexcept;
        ~Window();
        Window(Window const&) = default;
        Window& operator=(Window const&) = default;

        void SetTitle(std::wstring const& title);
        HWND GetHandle() const { return m_hWindow; }

        virtual void OnDeviceLost() final override;
        virtual void OnDeviceRestored() final override;

        DX::DeviceResources* GetDeviceResources() const { return m_deviceResources.get(); }

        Vector2i const GetSize() const { return { m_iWidth, m_iHeight }; }
        Input const& GetInput() const { return input; }

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
        HWND m_hWindow;
        std::wstring m_wsTitle;
        int m_iWidth, m_iHeight;
};