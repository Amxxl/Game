o//
// Window.cpp
//

#include "pch.h"
#include "Window.h"
#include "Application.h"

#include "WindowEvents.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(std::wstring const& name, int width, int height, bool fullScreen) noexcept
    : m_deviceResources(std::make_unique<DX::DeviceResources>())
    , m_hInstance(GetModuleHandle(nullptr))
    , m_wsTitle(name)
    , m_iWidth(width)
    , m_iHeight(height)
    , m_bFullScreen(fullScreen)
{
    m_deviceResources->RegisterDeviceNotify(this);

    WNDCLASSEXW wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = /*CS_VREDRAW | CS_HREDRAW |*/ CS_OWNDC;// | CS_DBLCLKS;
    wcex.lpfnWndProc = HandleMessageSetup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, L"IDI_ICON");
    wcex.hCursor = nullptr;//LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 3);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = m_wsTitle.c_str();
    wcex.hIconSm = wcex.hIcon;

    if (!RegisterClassExW(&wcex))
    {
        Logger::Get()->error("Failed to register window class!");
        PostQuitMessage(1);
    }

    RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

    if (fullScreen)
    {
        m_hWindow = CreateWindowEx(WS_EX_TOPMOST, wcex.lpszClassName, m_wsTitle.c_str(),
            WS_POPUP, x, y, width, height, nullptr, nullptr, m_hInstance, this);
    }
    else
    {
        m_hWindow = CreateWindowEx(WS_EX_APPWINDOW, wcex.lpszClassName, m_wsTitle.c_str(),
            WS_OVERLAPPEDWINDOW, x, y, width, height, nullptr, nullptr, m_hInstance, this);
    }

    if (!m_hWindow)
    {
        Logger::Get()->error("Failed to create window!");
        PostQuitMessage(2);
    }

    m_deviceResources->SetWindow(m_hWindow, width, height);

    ShowWindow(m_hWindow, (fullScreen ? SW_SHOWMAXIMIZED : SW_SHOWDEFAULT));
}

Window::~Window()
{
    UnregisterClassW(m_wsTitle.c_str(), m_hInstance);
    DestroyWindow(m_hWindow);
}

void Window::SetTitle(std::wstring const& title)
{
    SetWindowTextW(m_hWindow, title.c_str());
}

void Window::EnableCursor()
{
    if (m_bCursorVisible == true)
        return;

    m_bCursorVisible = true;
    SetCursorPos(m_mousePoint.x, m_mousePoint.y);
    ShowCursor();
    EnableImGuiMouse();
    FreeCursor();
}

void Window::DisableCursor()
{
    if (m_bCursorVisible == false)
        return;

    m_bCursorVisible = false;
    GetCursorPos(&m_mousePoint);
    HideCursor();
    DisableImGuiMouse();
    ConfineCursor();
}

void Window::OnDeviceLost()
{
}

void Window::OnDeviceRestored()
{
    Application::Get().CreateDeviceDependentResources();
    Application::Get().CreateWindowSizeDependentResources();
}

void Window::FreeCursor() noexcept
{
    ClipCursor(nullptr);
}

void Window::ConfineCursor() noexcept
{
    RECT rect = {};
    GetClientRect(m_hWindow, &rect);
    MapWindowPoints(m_hWindow, nullptr, reinterpret_cast<POINT*>(&rect), 2);
    ClipCursor(&rect);
}

void Window::ShowCursor() noexcept
{
    while (::ShowCursor(TRUE) < 0);
}

void Window::HideCursor() noexcept
{
    while (::ShowCursor(FALSE) >= 0);
}

void Window::EnableImGuiMouse() noexcept
{
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse() noexcept
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

LRESULT Window::HandleMessageSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCCREATE)
    {
        CREATESTRUCTW const* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

        Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&HandleMessageThunk));

        return pWindow->HandleMessage(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Window::HandleMessageThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWindow->HandleMessage(hWnd, message, wParam, lParam);
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
    static bool s_fullscreen = m_bFullScreen;//false;

    // @todo: Find a better way to handle this.
    Application* app = &Application::Get();

    ImGuiIO& io = ImGui::GetIO();

    switch (message)
    {
        case WM_PAINT:
        {
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
        }
        case WM_MOVE:
        {
            if (app)
                app->OnWindowMoved();

            break;
        }
        case WM_SIZE:
        {
            m_iWidth = LOWORD(lParam);
            m_iHeight = HIWORD(lParam);

            DispatchEvent<EventWindowResized>(m_iWidth, m_iHeight);

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
        }
        case WM_ENTERSIZEMOVE:
        {
            s_in_sizemove = true;
            break;
        }
        case WM_EXITSIZEMOVE:
        {
            s_in_sizemove = false;
            if (app)
            {
                RECT rc;
                GetClientRect(hWnd, &rc);

                app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
                //m_deviceResources->WindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
            }
            break;
        }
        case WM_GETMINMAXINFO:
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
 
            break;
        }
        case WM_ACTIVATEAPP:
        {
            if (app)
            {
                if (wParam)
                    app->OnActivated();
                else
                    app->OnDeactivated();
            }
            break;
        }
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
                        input.OnMouseMovedRaw(Vector2i(raw->data.mouse.lLastX, raw->data.mouse.lLastY));
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        case WM_MOUSEHOVER:
            input.OnMouseEnter();
            break;
        case WM_MOUSELEAVE:
            input.OnMouseLeave();
            break;

        case WM_MOUSEMOVE:
        {
            if (io.WantCaptureMouse)
                break;

            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseMoved(position);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            if (io.WantCaptureMouse)
                break;

            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            float delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
            input.OnMouseWheelScrolled(position, delta);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            if (io.WantCaptureMouse)
                return true;

            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonPressed(position, Input::MouseButton::Left);
            break;
        }
        case WM_LBUTTONUP:
        {
            if (io.WantCaptureMouse)
                return true;

            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonReleased(position, Input::MouseButton::Left);
            break;
        }
        case WM_LBUTTONDBLCLK:
        {
            if (io.WantCaptureMouse)
                return true;

            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonDoubleClicked(position, Input::MouseButton::Left);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            if (io.WantCaptureMouse)
                return true;

            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonPressed(position, Input::MouseButton::Right);
            break;
        }
        case WM_RBUTTONUP:
        {
            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonReleased(position, Input::MouseButton::Right);
            break;
        }
        case WM_RBUTTONDBLCLK:
        {
            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonDoubleClicked(position, Input::MouseButton::Right);
            break;
        }
        case WM_MBUTTONDOWN:
        {
            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonPressed(position, Input::MouseButton::Middle);
            break;
        }
        case WM_MBUTTONUP:
        {
            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonReleased(position, Input::MouseButton::Middle);
            break;
        }
        case WM_MBUTTONDBLCLK:
        {
            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            input.OnMouseButtonDoubleClicked(position, Input::MouseButton::Middle);
            break;
        }
        case WM_XBUTTONDOWN:
        {
            Vector2i position(LOWORD(lParam), HIWORD(lParam));
            
            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
                input.OnMouseButtonPressed(position, Input::MouseButton::X1);
            else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
                input.OnMouseButtonPressed(position, Input::MouseButton::X2);
            break;
        }
        case WM_XBUTTONUP:
        {
            Vector2i position(LOWORD(lParam), HIWORD(lParam));

            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
                input.OnMouseButtonReleased(position, Input::MouseButton::X1);
            else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
                input.OnMouseButtonReleased(position, Input::MouseButton::X2);
            break;
        }
        case WM_XBUTTONDBLCLK:
        {
            Vector2i position(LOWORD(lParam), HIWORD(lParam));

            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
                input.OnMouseButtonDoubleClicked(position, Input::MouseButton::X1);
            else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
                input.OnMouseButtonDoubleClicked(position, Input::MouseButton::X2);
            break;
        }
        case WM_POWERBROADCAST:
        {
            switch (wParam)
            {
                case PBT_APMQUERYSUSPEND:
                {
                    if (!s_in_suspend && app)
                        app->OnSuspending();
                    s_in_suspend = true;
                    return TRUE;
                }
                case PBT_APMRESUMESUSPEND:
                {
                    if (!s_minimized)
                    {
                        if (s_in_suspend && app)
                            app->OnResuming();
                        s_in_suspend = false;
                    }
                    return TRUE;
                }
            }
            break;
        }
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_KILLFOCUS:
            input.Reset();
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
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

                    RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
                    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

                    width = rc.right - rc.left;
                    height = rc.bottom - rc.top;
                    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
                    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

                    ShowWindow(hWnd, SW_SHOWNORMAL);

                    SetWindowPos(hWnd, HWND_TOP, x, y, width, height, /*SWP_NOMOVE | */SWP_NOZORDER | SWP_FRAMECHANGED);
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

            input.OnKeyPressed(static_cast<size_t>(wParam), lParam & 0x40000000);
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            input.OnKeyReleased(static_cast<size_t>(wParam));
            break;
        }
        case WM_CHAR:
        {
            input.OnKeyChar(static_cast<size_t>(wParam), lParam & 0x40000000);
            break;
        }
        case WM_MENUCHAR:
            // A menu is active and the user presses a key that does not correspond
            // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
            return MAKELRESULT(0, MNC_CLOSE);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}