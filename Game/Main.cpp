//
// main.cpp
//

#include "pch.h"
#include "Application.h"


constexpr static uint16 HID_USAGE_PAGE_GENERIC = static_cast<uint16>(0x01);
constexpr static uint16 HID_USAGE_GENERIC_MOUSE = static_cast<uint16>(0x02);

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!DirectX::XMVerifyCPUSupport())
        return 1;

    if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED)))
        return 1;

    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->Initialize(800, 600);

    RAWINPUTDEVICE rid;
    rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
    rid.usUsage = HID_USAGE_GENERIC_MOUSE;
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;
    
    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    {
        MessageBox(nullptr, L"Failed to register RAWINPUTDEVICE.", L"Error", MB_OK);
        return 1;
    }

    // Main message loop
    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            app->Tick();
        }
    }

    app.reset();
    CoUninitialize();
    return static_cast<int>(msg.wParam);
}