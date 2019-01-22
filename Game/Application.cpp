//
// Application.cpp
//

#include "pch.h"
#include "Application.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Application::Application() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Application::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);
    m_deviceResources->CreateDeviceResources();

    // Scenes manager.
    m_sceneManager = std::make_unique<SceneManager>(m_deviceResources->GetD3DDeviceContext());
    m_sceneManager->PushScene(PlayScene::Instance());

    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext());
    ImGui::StyleColorsDark();

    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);
}

#pragma region Frame Update
// Executes the basic game loop.
void Application::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Application::Update(DX::StepTimer const& timer)
{
    // TODO: Add your game logic here.
    auto kb = m_keyboard->GetState();
    if (kb.Escape)
        PostQuitMessage(0);

    auto mouse = m_mouse->GetState();

    m_sceneManager->Update(timer);
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Application::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
        return;

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    m_sceneManager->Render();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Application::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Application::OnActivated()
{
    // TODO: Game is becoming active window.
    m_sceneManager->OnActivated();
}

void Application::OnDeactivated()
{
    // TODO: Game is becoming background window.
    m_sceneManager->OnDeactivated();
}

void Application::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
    m_sceneManager->OnSuspending();
}

void Application::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
    m_sceneManager->OnResuming();
}

void Application::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
    m_sceneManager->OnWindowMoved();
}

void Application::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
    m_sceneManager->OnWindowSizeChanged(width, height);

}

// Properties
void Application::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Application::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
    context;

    m_sceneManager->CreateDeviceDependentResources();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Application::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    m_sceneManager->CreateWindowSizeDependentResources();
}

void Application::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_sceneManager->OnDeviceLost();
}

void Application::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
    m_sceneManager->OnDeviceRestored();
}
#pragma endregion
