//
// PlayScene.cpp
//

#include "pch.h"
#include "PlayScene.h"
#include "MouseData.h"

PlayScene PlayScene::s_instance;

using namespace DirectX;

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

    camera.SetProjectionValues(90.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    camera.SetPosition(2.0f, 10.0f, 4.0f);
    camera.SetLookAtPos(DirectX::XMFLOAT3(5.0f, 0.0f, 5.0f));

    sky = DirectX::GeometricPrimitive::CreateSphere(deviceContext, 2000.0f, 16, true, true);
    water = DirectX::GeometricPrimitive::CreateBox(deviceContext, DirectX::XMFLOAT3(512.0f, 50.0f, 512.0f));

    ID3D11Device* device = nullptr;
    deviceContext->GetDevice(&device);

    DirectX::CreateWICTextureFromFile(device, L"Data/sky.jpg", nullptr, skyTexture.ReleaseAndGetAddressOf());
    DirectX::CreateWICTextureFromFile(device, L"Data/water.jpg", nullptr, waterTexture.ReleaseAndGetAddressOf());

    terrain.Initialize(deviceContext);

    quadTree.Initialize(&terrain, deviceContext);

    m_deviceContext = deviceContext;

    player.Initialize(deviceContext);
    player.SetPosition(30.0f, 3.0f, 19.0f);

    effect = std::make_unique<DirectX::BasicEffect>(device);
    effect->SetAmbientLightColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
    effect->SetTextureEnabled(true);
    effect->SetPerPixelLighting(true);
    effect->SetLightingEnabled(true);
    effect->SetLightEnabled(0, true);
    effect->SetLightDiffuseColor(0, Colors::White);
    effect->SetLightDirection(0, SimpleMath::Vector3::UnitY);
    effect->SetTexture(skyTexture.Get());

    sky->CreateInputLayout(effect.get(), inputLayout.ReleaseAndGetAddressOf());

    model.Initialize("Data/10446_Palm_Tree_v1_max2010_iteration-2.obj", device, deviceContext);
    mdl.Initialize("Data/WoodenCabinObj.obj", device, deviceContext);

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

    static float cameraSpeed = 15.0f;

    if (keyboard.W)
    {
        camera.AdjustPosition(camera.GetForwardVector() * deltaTime * cameraSpeed);
    }
    else if (keyboard.S)
    {
        camera.AdjustPosition(camera.GetBackwardVector() * deltaTime * cameraSpeed);
    }

    
    if (keyboard.A)
    {
        camera.AdjustPosition(camera.GetLeftVector() * deltaTime * cameraSpeed);
    }
    else if (keyboard.D)
    {
        camera.AdjustPosition(camera.GetRightVector() * deltaTime * cameraSpeed);
    }

    if (keyboard.Space)
        camera.AdjustPosition(0.0f, deltaTime * cameraSpeed, 0.0f);
    else if (keyboard.Z)
        camera.AdjustPosition(0.0f, -deltaTime * cameraSpeed, 0.0f);

    auto mouse = Mouse::Get().GetState();

    camera.AdjustRotation(MouseData::GetRelativeY() * 0.01f, MouseData::GetRelativeX() * 0.01f, 0.0f);

    if (keyboard.LeftShift)
        cameraSpeed = 40.0f;
    else
        cameraSpeed = 15.0f;

    static int anim_index = 0;


    if (keyboard.NumPad0)
        anim_index = 0;

    if (keyboard.NumPad1)
        anim_index = 1;

    if (keyboard.NumPad2)
        anim_index = 2;

    if (keyboard.NumPad3)
        anim_index = 3;

    if (keyboard.NumPad4)
        anim_index = 4;

    if (keyboard.NumPad5)
        anim_index = 5;

    player.Update(m_deviceContext, deltaTime, anim_index);

    MouseData::SetRelativePos(0, 0);
    sceneGraph->Update(timer);
    m_fps = static_cast<float>(timer.GetFramesPerSecond());
}

void PlayScene::Render()
{
    m_world = XMMatrixIdentity();

    //sky->Draw(m_world * XMMatrixTranslation(camera.GetPositionFloat3().x, camera.GetPositionFloat3().y, camera.GetPositionFloat3().z), camera.GetViewMatrix(), camera.GetProjectionMatrix(), Colors::White, skyTexture.Get());
    effect->SetWorld(m_world * XMMatrixTranslation(camera.GetPositionFloat3().x, camera.GetPositionFloat3().y, camera.GetPositionFloat3().z));
    effect->SetView(camera.GetViewMatrix());
    effect->SetProjection(camera.GetProjectionMatrix());

    sky->Draw(effect.get(), inputLayout.Get());

    frustum.ConstructFrustum(1000.0f, camera.GetProjectionMatrix(), camera.GetViewMatrix(), m_world);
    quadTree.Draw(m_deviceContext, &frustum, m_world, camera.GetViewMatrix(), camera.GetProjectionMatrix());

    //model.Draw(m_deviceContext, m_world * DirectX::XMMatrixScaling(0.04f, 0.04f, 0.04f) * DirectX::XMMatrixTranslation(5.0f, 0.0f, 5.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix());
    //model.Draw(m_world * DirectX::XMMatrixScaling(0.08f, 0.08f, 0.08f) * DirectX::XMMatrixRotationX(3.1415f / 2.0f) * DirectX::XMMatrixTranslation(100.0f, 0.0f, 100.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix());
    
    for (int x = 0; x < 5; ++x)
    {
        for (int z = 0; z < 5; ++z)
        {
            model.Draw(m_world * DirectX::XMMatrixScaling(0.08f, 0.08f, 0.08f) * DirectX::XMMatrixRotationX(3.1415f / 2.0f) * DirectX::XMMatrixTranslation(100.0f * x, 0.0f, 100.0f * z), camera.GetViewMatrix(), camera.GetProjectionMatrix());
        }
    }
    
    player.Draw(m_deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    //water->Draw(m_world * XMMatrixTranslation(256.0f, -15.0f, 256.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix(), XMVectorSet(0.0f, 0.0f, 1.0f, 0.9f), waterTexture.Get());


    mdl.Draw(m_world * DirectX::XMMatrixTranslation(50.0f, 0.0f, 50.0f) * XMMatrixScaling(0.5f, 0.5f, 0.5f), camera.GetViewMatrix(), camera.GetProjectionMatrix());

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    std::ostringstream ss("");
    ss << "Frames per second: " << m_fps;
    ImGui::Text(ss.str().c_str());

    ss.str("");
    ss << "Draw count: " << quadTree.GetDrawCount();
    ImGui::Text(ss.str().c_str());
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    sceneGraph->Render();
}