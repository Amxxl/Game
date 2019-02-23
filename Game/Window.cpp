//
// Window.cpp
//

#include "pch.h"
#include "Window.h"
#include "Application.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
    : hInstance(GetModuleHandle(nullptr))
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = HandleMessage;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, L"IDI_ICON");
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 3);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = wndClassName;
    wcex.hIconSm = LoadIcon(hInstance, L"IDI_ICON");

    if (!RegisterClassEx(&wcex))
        PostQuitMessage(1);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClass.wndClassName, hInstance);
}

wchar_t const* Window::WindowClass::GetName() noexcept
{
    return wndClass.wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInstance;
}

Window::Window(std::wstring const& name, int width, int height) noexcept
{
    RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

    hWnd = CreateWindowEx(0, WindowClass::GetName(), name.c_str(), WS_OVERLAPPEDWINDOW,
        x, y, width, height, nullptr, nullptr, WindowClass::GetInstance(), this);

    // TIP: Change to CreateWindowEx(WS_EX_TOPMOST, WindowClass::GetName(), name.c_str(), WS_POPUP,
    // to default to fullscreen.

    if (!hWnd)
        PostQuitMessage(2);

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    // TIP: Change SW_SHOWDEFAULT to SW_SHOWMAXIMIZED to default to fullscreen.
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

void Window::SetTitle(std::wstring const& title)
{
    SetWindowTextW(hWnd, title.c_str());
}

void Window::RegisterUserData(LONG_PTR userData)
{
    assert(userData == nullptr);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, userData);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    PAINTSTRUCT ps;
    HDC hdc;

    static bool s_in_sizemove = false;
    static bool s_in_suspend = false;
    static bool s_minimized = false;
    // TIP: Set s_fullscreen to true if defaulting to fullscreen.
    static bool s_fullscreen = false;

    auto app = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
        case WM_PAINT:
            if (s_in_sizemove && app)
            {
                app->Tick();
            }
            else
            {
                hdc = BeginPaint(hWnd, &ps);
                EndPaint(hWnd, &ps);
            }
            break;

        case WM_MOVE:
            if (app)
            {
                app->OnWindowMoved();
            }
            break;

        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
            {
                if (!s_minimized)
                {
                    s_minimized = true;
                    if (!s_in_suspend && app)
                        app->OnSuspending();
                    s_in_suspend = true;
                }
            }
            else if (s_minimized)
            {
                s_minimized = false;
                if (s_in_suspend && app)
                    app->OnResuming();
                s_in_suspend = false;
            }
            else if (!s_in_sizemove && app)
            {
                app->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
            }
            break;

        case WM_ENTERSIZEMOVE:
            s_in_sizemove = true;
            break;

        case WM_EXITSIZEMOVE:
            s_in_sizemove = false;
            if (app)
            {
                RECT rc;
                GetClientRect(hWnd, &rc);

                app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
            }
            break;

        case WM_GETMINMAXINFO:
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;

        case WM_ACTIVATEAPP:
            if (app)
            {
                if (wParam)
                    app->OnActivated();
                else
                    app->OnDeactivated();
            }
            DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
            //DirectX::Mouse::ProcessMessage(message, wParam, lParam);
            break;

        case WM_INPUT:
        {
            UINT dataSize;
            GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

            if (dataSize > 0)
            {
                std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
                {
                    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
                    
                    if (raw->header.dwType == RIM_TYPEMOUSE)
                    {
                        app->GetMouse()->OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                    }
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        case WM_MOUSEMOVE:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            app->GetMouse()->OnMouseMove(x, y);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            app->GetMouse()->OnLeftPressed(x, y);
            break;
        }
        case WM_LBUTTONUP:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            app->GetMouse()->OnLeftReleased(x, y);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            app->GetMouse()->OnRightPressed(x, y);
            break;
        }
        case WM_RBUTTONUP:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            app->GetMouse()->OnRightReleased(x, y);
            break;
        }
        case WM_MBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            app->GetMouse()->OnMiddlePressed(x, y);
            break;
        }
        case WM_MBUTTONUP:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            app->GetMouse()->OnMiddleReleased(x, y);
            break;
        }
        case WM_MOUSEWHEEL:
        case WM_XBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
                app->GetMouse()->OnXButton1Pressed(x, y);
            else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
                app->GetMouse()->OnXButton2Pressed(x, y);
            break;
        }
        case WM_XBUTTONUP:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
                app->GetMouse()->OnXButton1Released(x, y);
            else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
                app->GetMouse()->OnXButton2Released(x, y);
            break;
        }
        case WM_MOUSEHOVER:
            //DirectX::Mouse::ProcessMessage(message, wParam, lParam);
            break;

        case WM_POWERBROADCAST:
            switch (wParam)
            {
            case PBT_APMQUERYSUSPEND:
                if (!s_in_suspend && app)
                    app->OnSuspending();
                s_in_suspend = true;
                return TRUE;

            case PBT_APMRESUMESUSPEND:
                if (!s_minimized)
                {
                    if (s_in_suspend && app)
                        app->OnResuming();
                    s_in_suspend = false;
                }
                return TRUE;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (message == WM_SYSKEYDOWN && wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
            {
                // Implements the classic ALT+ENTER fullscreen toggle
                if (s_fullscreen)
                {
                    SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                    SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                    int width = 800;
                    int height = 600;
                    if (app)
                        app->GetDefaultSize(width, height);

                    ShowWindow(hWnd, SW_SHOWNORMAL);

                    SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
                }
                else
                {
                    SetWindowLongPtr(hWnd, GWL_STYLE, 0);
                    SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                    SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

                    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
                }

                s_fullscreen = !s_fullscreen;
            }
            DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
            break;

        case WM_MENUCHAR:
            // A menu is active and the user presses a key that does not correspond
            // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
            return MAKELRESULT(0, MNC_CLOSE);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}