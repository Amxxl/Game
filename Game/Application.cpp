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
    m_window = std::make_unique<Window>(L"Game", width, height, false);

    if (!m_window)
        Logger::Get()->error("Window creation process failed!");

    auto deviceResources = m_window->GetDeviceResources();

    deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplWin32_Init(m_window->GetHandle());
    ImGui_ImplDX11_Init(deviceResources->GetDevice(), deviceResources->GetDeviceContext());
    ImGui::StyleColorsDark();

    m_audio = std::make_unique<Audio>();

    // Scenes manager.
    m_sceneManager = std::make_unique<SceneManager>(*m_window.get());
    m_sceneManager->PushScene(&PlayScene::Get());

    // TIP: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    m_window->GetInput().AddEventListener<EventKeyChar>(*this);
    m_window->GetInput().AddEventListener<EventKeyPressed>(*this);
    m_window->GetInput().AddEventListener<EventKeyReleased>(*this);
    m_window->GetInput().AddEventListener<EventMouseMoved>(*this);
    m_window->GetInput().AddEventListener<EventMouseMovedRaw>(*this);
    m_window->GetInput().AddEventListener<EventMouseWheelScrolled>(*this);
    m_window->GetInput().AddEventListener<EventMouseButtonPressed>(*this);
    m_window->GetInput().AddEventListener<EventMouseButtonReleased>(*this);
    m_window->GetInput().AddEventListener<EventMouseButtonDoubleClicked>(*this);
}

void Application::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    m_audio->Update();
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
    auto context = deviceResources->GetDeviceContext();

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
    auto context = deviceResources->GetDeviceContext();
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

void Application::OnEvent(EventKeyChar const& event)
{
    m_sceneManager->OnKeyChar(event.key);
}

void Application::OnEvent(EventKeyPressed const& event)
{
    m_sceneManager->OnKeyPressed(event.key);
}

void Application::OnEvent(EventKeyReleased const& event)
{
    m_sceneManager->OnKeyReleased(event.key);
}

void Application::OnEvent(EventMouseMoved const& event)
{
    m_sceneManager->OnMouseMoved(event.position);
}

void Application::OnEvent(EventMouseMovedRaw const& event)
{
    m_sceneManager->OnMouseMovedRaw(event.position);
}

void Application::OnEvent(EventMouseWheelScrolled const& event)
{
    m_sceneManager->OnMouseWheelScrolled(event.position, event.delta);
}

void Application::OnEvent(EventMouseButtonPressed const& event)
{
    m_sceneManager->OnMouseButtonPressed(event.position, event.button);
}

void Application::OnEvent(EventMouseButtonReleased const& event)
{
    m_sceneManager->OnMouseButtonReleased(event.position, event.button);
}

void Application::OnEvent(EventMouseButtonDoubleClicked const& event)
{
    m_sceneManager->OnMouseButtonDoubleClicked(event.position, event.button);
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

    auto device = deviceResources->GetDevice();
    auto context = deviceResources->GetDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
    context;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Application::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}