//
// Window.h
//

#pragma once

class Window
{
    class WindowClass
    {
        private:
            WindowClass() noexcept;
            ~WindowClass();

            WindowClass(WindowClass const&) = delete;
            WindowClass& operator=(WindowClass const&) = delete;

        public:
            static wchar_t const* GetName() noexcept;
            static HINSTANCE GetInstance() noexcept;

        private:
            static constexpr wchar_t const* wndClassName = L"WindowClass";
            static WindowClass s_instance;
            HINSTANCE hInstance;
    };
    public:
        Window() noexcept;
        Window(wchar_t const* name, int32 width, int32 height) noexcept;
        ~Window();

        Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;

    private:
        static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        int32 width;
        int32 height;
        HWND hWnd;
};