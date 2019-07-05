//
// main.cpp
//

#include "pch.h"
#include "Application.h"

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

    Logger::Init();
    Logger::Get()->info("Logging system initialized.");

    if (!DirectX::XMVerifyCPUSupport())
    {
        Logger::Get()->error("Your CPU doesn't support DirectX Math!");
        return 1;
    }

    if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED)))
        return 1;

    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->Initialize(1366, 768);

    Logger::Get()->info("Application successfully initialized.");
    
    // Main message loop
    MSG msg = { };

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