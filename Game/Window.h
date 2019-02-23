//
// Window.h
//

#pragma once

class Window
{
    private:
        class WindowClass
        {
            public:
                static wchar_t const* GetName() noexcept;
                static HINSTANCE GetInstance() noexcept;

            private:
                WindowClass() noexcept;
                ~WindowClass();
                WindowClass(WindowClass const&) = delete;
                WindowClass& operator=(WindowClass const&) = delete;

            private:
                static constexpr wchar_t const* wndClassName = L"WindowClass";
                static WindowClass wndClass;

            private:
                HINSTANCE hInstance;
        };

    public:
        Window() = default;
        Window(std::wstring const& name, int width, int height) noexcept;
        ~Window();
        Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;

        void SetTitle(std::wstring const& title);
        void RegisterUserData(LONG_PTR userData);
        HWND GetHandle() const { return hWnd; }

    private:
        static LRESULT CALLBACK HandleMessage(HWND HWnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        int width;
        int height;
        HWND hWnd;
};