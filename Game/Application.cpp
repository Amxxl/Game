//
// Application.cpp
//

#include "pch.h"
#include "Application.h"
#include "Color.h"


Application* Application::s_instance;

Application::Application() noexcept(false)
{
    s_instance = this;
}

Application::~Application()
{
    if (ImGui::GetCurrentContext() != nullptr)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
}

// Initialize the Direct3D resources required to run.
void Application::Initialize(int width, int height)
{
    m_window = std::make_unique<Window>(L"Game", width, height);

    if (!m_window)
        Logger::Get()->error("Window creation process failed!");


    m_audio = std::make_unique<Audio>();

    auto deviceResources = m_window->GetDeviceResources();

    deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplWin32_Init(m_window->GetHandle());
    ImGui_ImplDX11_Init(deviceResources->GetD3DDevice(), deviceResources->GetD3DDeviceContext());
    ImGui::StyleColorsDark();

    // Scenes manager.
    m_sceneManager = std::make_unique<SceneManager>(*m_window.get());
    m_sceneManager->PushScene(&PlayScene::Get());

    // TIP: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    // Event handlers @todo: move it somewhere else.
    ON_EVENT_DISPATCHED([&](EventKeyPressed const* e) {
        m_sceneManager->OnKeyPressed(e->GetKey());
    });
    
    ON_EVENT_DISPATCHED([&](EventKeyReleased const* e) {
        m_sceneManager->OnKeyReleased(e->GetKey());
    });

    ON_EVENT_DISPATCHED([&](EventMouseMoved const* e) {
        m_sceneManager->OnMouseMoved(e->GetPosition());
    });

    ON_EVENT_DISPATCHED([&](EventMouseMovedRaw const* e) {
        m_sceneManager->OnMouseMovedRaw(e->GetPosition());
    });

    ON_EVENT_DISPATCHED([&](EventMouseWheelScrolled const* e) {
        m_sceneManager->OnMouseWheelScrolled(e->GetPosition(), e->GetDelta());
    });

    ON_EVENT_DISPATCHED([&](EventMouseButtonPressed const* e) {
        m_sceneManager->OnMouseButtonPressed(e->GetPosition(), e->GetButton());
    });

    ON_EVENT_DISPATCHED([&](EventMouseButtonReleased const* e) {
        m_sceneManager->OnMouseButtonReleased(e->GetPosition(), e->GetButton());
    });

    ON_EVENT_DISPATCHED([&](EventMouseButtonDoubleClicked const* e) {
        m_sceneManager->OnMouseButtonDoubleClicked(e->GetPosition(), e->GetButton());
    });
}

void Application::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

void Application::Update(DX::StepTimer const& timer)
{
    if (m_window->GetInput().IsKeyDown(Input::Key::Escape))
        PostQuitMessage(0);

    m_sceneManager->Update(timer);
}

void Application::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
        return;

    Clear();

    auto deviceResources = m_window->GetDeviceResources();

    deviceResources->PIXBeginEvent(L"Render");
    auto context = deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    m_sceneManager->Render();

    deviceResources->PIXEndEvent();

    // Show the new frame.
    deviceResources->Present();
}

// Helper method to clear the back buffers.
void Application::Clear()
{
    auto deviceResources = m_window->GetDeviceResources();

    deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = deviceResources->GetD3DDeviceContext();
    auto renderTarget = deviceResources->GetRenderTargetView();
    auto depthStencil = deviceResources->GetDepthStencilView();

    float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    context->ClearRenderTargetView(renderTarget, color);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    deviceResources->PIXEndEvent();
}

// Message handlers
void Application::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Application::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Application::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Application::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Application::OnWindowMoved()
{
    if (!m_window)
        return;

    auto deviceResources = m_window->GetDeviceResources();

    auto r = deviceResources->GetOutputSize();
    deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Application::OnWindowSizeChanged(int width, int height)
{
    if (!m_window)
        return;

    auto deviceResources = m_window->GetDeviceResources();

    if (deviceResources && !deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.

}

// Properties
void Application::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Application::CreateDeviceDependentResources()
{
    auto deviceResources = m_window->GetDeviceResources();

    auto device = deviceResources->GetD3DDevice();
    auto context = deviceResources->GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
    context;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Application::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}