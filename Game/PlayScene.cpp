//
// PlayScene.cpp
//

#include "pch.h"
#include "PlayScene.h"
#include "MouseData.h"

PlayScene PlayScene::s_instance;

PlayScene::PlayScene()
{
    paused = false;
}

PlayScene::~PlayScene()
{
    if (root != nullptr)
    {
        delete root;
        root = nullptr;
    }
}

bool PlayScene::Load(ID3D11DeviceContext1* deviceContext)
{
    root = new SceneNode();
    // @todo: Load resources here.
    shape = DirectX::GeometricPrimitive::CreateCube(deviceContext, 2.0f);
    camera.SetProjectionValues(90.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    camera.SetPosition(2.0f, 2.0f, 4.0f);
    camera.SetLookAtPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
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

    static float cameraSpeed = 2.0f;

    if (keyboard.W)
        camera.AdjustPosition(camera.GetForwardVector() * deltaTime * cameraSpeed);
    else if (keyboard.S)
        camera.AdjustPosition(camera.GetBackwardVector() * deltaTime * cameraSpeed);

    if (keyboard.A)
        camera.AdjustPosition(camera.GetLeftVector() * deltaTime * cameraSpeed);
    else if (keyboard.D)
        camera.AdjustPosition(camera.GetRightVector() * deltaTime * cameraSpeed);

    if (keyboard.Space)
        camera.AdjustPosition(0.0f, deltaTime * cameraSpeed, 0.0f);
    else if (keyboard.Z)
        camera.AdjustPosition(0.0f, -deltaTime * cameraSpeed, 0.0f);

    auto mouse = Mouse::Get().GetState();

    if (mouse.rightButton)
    {
        camera.AdjustRotation(MouseData::GetRelativeY() * 0.01f, -MouseData::GetRelativeX() * 0.01f, 0.0f);
    }

    if (keyboard.LeftShift)
        cameraSpeed = 10.0f;
    else
        cameraSpeed = 2.0f;

    MouseData::SetRelativePos(0, 0);

    root->Update(timer);
}

void PlayScene::Render()
{
    m_world = XMMatrixIdentity();
    //shape->Draw(m_world, camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::Aqua);
    //shape->Draw(m_world * XMMatrixTranslation(0.0f, 4.0f, 0.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::AliceBlue);

    for (int y = 0; y <= 40; ++y)
    {
        for (int x = 0; x <= 40; ++x)
        {
            shape->Draw(m_world * XMMatrixTranslation(x * 2.0f, 0.0f, y * 2.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::SpringGreen);
        }
    }

    for (int y = 0; y <= 20; ++y)
    {
        for (int x = 0; x <= 20; ++x)
        {
            shape->Draw(m_world * XMMatrixTranslation(x * 4.0f, 2.0f, y * 4.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::LawnGreen);
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Info");

    std::ostringstream ss;
    ss << "Frames per second: " << fps;
    ImGui::Text(ss.str().c_str());

    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    root->Render();
}
