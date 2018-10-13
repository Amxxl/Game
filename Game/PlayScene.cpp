//
// PlayScene.cpp
//

#include "pch.h"
#include "PlayScene.h"

PlayScene PlayScene::s_instance;

PlayScene::PlayScene()
{
    paused = false;
}

PlayScene::~PlayScene()
{
}

bool PlayScene::Load(ID3D11DeviceContext1* deviceContext)
{
    // @todo: Load resources here.
    shape = DirectX::GeometricPrimitive::CreateCube(deviceContext, 2.0f);
    camera.SetProjectionValues(90.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    camera.SetPosition(2.0f, 2.0f, 4.0f);
    camera.SetLookAtPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    mouse_x = Mouse::Get().GetState().x;
    mouse_y = Mouse::Get().GetState().y;
    return true;
}

void PlayScene::Unload()
{
    // @todo: Unload resources here.
}

void PlayScene::Update(DX::StepTimer const& timer)
{
    if (paused)
        return;

    float deltaTime = static_cast<float>(timer.GetElapsedSeconds());
    fps = timer.GetFramesPerSecond();

    auto keyboard = Keyboard::Get().GetState();

    if (keyboard.W)
        camera.AdjustPosition(camera.GetForwardVector() * deltaTime);
    else if (keyboard.S)
        camera.AdjustPosition(camera.GetBackwardVector() * deltaTime);

    if (keyboard.A)
        camera.AdjustPosition(camera.GetLeftVector() * deltaTime);
    else if (keyboard.D)
        camera.AdjustPosition(camera.GetRightVector() * deltaTime);

    if (keyboard.Space)
        camera.AdjustPosition(0.0f, deltaTime, 0.0f);
    else if (keyboard.Z)
        camera.AdjustPosition(0.0f, -deltaTime, 0.0f);

    auto mouse = Mouse::Get().GetState();
    mouse_x = mouse.x;
    mouse_y = mouse.y;
}

void PlayScene::Render()
{
    m_world = XMMatrixIdentity();
    shape->Draw(m_world, camera.GetViewMatrix(), camera.GetProjectionMatrix());

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Info");

    std::ostringstream ss;
    ss << "Frames per second: " << fps;

    ImGui::Text(ss.str().c_str());

    ss.str("");
    ss << "Mouse X: " << mouse_x;
    ImGui::Text(ss.str().c_str());

    ss.str("");
    ss << "Mouse Y: " << mouse_y;
    ImGui::Text(ss.str().c_str());

    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
