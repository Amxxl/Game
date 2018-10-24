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
    if (sceneGraph != nullptr)
    {
        delete sceneGraph;
        sceneGraph = nullptr;
    }
}

void PlayScene::CreateDeviceDependentResources()
{
}

void PlayScene::CreateWindowSizeDependentResources()
{
}

void PlayScene::OnDeviceLost()
{
}

void PlayScene::OnDeviceRestored()
{
}

void PlayScene::OnActivated()
{
}

void PlayScene::OnDeactivated()
{
}

void PlayScene::OnSuspending()
{
}

void PlayScene::OnResuming()
{
}

void PlayScene::OnWindowMoved()
{
}

void PlayScene::OnWindowSizeChanged(int width, int height)
{
}

bool PlayScene::Load(ID3D11DeviceContext1* deviceContext)
{
    sceneGraph = new SceneNode();

    sceneGraph->AddNode(new NodeDisplayFPS());
    // @todo: Load resources here.
    shape = DirectX::GeometricPrimitive::CreateCube(deviceContext, 2.0f);
    camera.SetProjectionValues(90.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    camera.SetPosition(2.0f, 2.0f, 4.0f);
    camera.SetLookAtPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    sky = DirectX::GeometricPrimitive::CreateSphere(deviceContext, 1200.0f, 32, false, true);

    ID3D11Device* device = nullptr;
    deviceContext->GetDevice(&device);

    DirectX::CreateWICTextureFromFile(device, L"sky.jpg", nullptr, skyTexture.ReleaseAndGetAddressOf());
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

   // if (mouse.rightButton)
    //{
        camera.AdjustRotation(MouseData::GetRelativeY() * 0.01f, -MouseData::GetRelativeX() * 0.01f, 0.0f);
   // }

    if (keyboard.LeftShift)
        cameraSpeed = 30.0f;
    else
        cameraSpeed = 10.0f;

    MouseData::SetRelativePos(0, 0);
    sceneGraph->Update(timer);
}

void PlayScene::Render()
{
    m_world = XMMatrixIdentity();
    sky->Draw(m_world * XMMatrixTranslation(camera.GetPositionFloat3().x, camera.GetPositionFloat3().y, camera.GetPositionFloat3().z), camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::White, skyTexture.Get());
    
    for (int y = 0; y <= 60; ++y)
    {
        for (int x = 0; x <= 60; ++x)
        {
            shape->Draw(m_world * XMMatrixTranslation(x * 2.0f, 0.0f, y * 2.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::SpringGreen);
        }
    }

    for (int y = 0; y <= 30; ++y)
    {
        for (int x = 0; x <= 30; ++x)
        {
            shape->Draw(m_world * XMMatrixTranslation(x * 4.0f, 2.0f, y * 4.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::LawnGreen);
        }
    }

    sceneGraph->Render();
}
