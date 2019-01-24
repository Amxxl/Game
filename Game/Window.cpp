//
// Window.cpp - It is not used now.
//

#include "pch.h"
#include "Window.h"


Window::WindowClass Window::WindowClass::s_instance;

Window::WindowClass::WindowClass() noexcept
    : hInstance(GetModuleHandle(nullptr))
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = HandleMsgSetup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, L"IDI_ICON");
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 3);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = wndClassName;
    wcex.hIconSm = LoadIcon(hInstance, L"IDI_ICON");
    RegisterClassEx(&wcex);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, hInstance);
}

wchar_t const* Window::WindowClass::GetName() noexcept
{
    return s_instance.wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return s_instance.hInstance;
}

Window::Window() noexcept
{
}

Window::Window(wchar_t const* name, int32 width, int32 height) noexcept
{
    RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
    int32 x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    int32 y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

    hWnd = CreateWindowEx(0, WindowClass::GetName(), name, WS_OVERLAPPEDWINDOW,
        x, y, width, height, nullptr, nullptr, WindowClass::GetInstance(), this);

    ShowWindow(hWnd, SW_SHOWDEFAULT);
}


Window::~Window()
{
    DestroyWindow(hWnd);
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCTW const* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}