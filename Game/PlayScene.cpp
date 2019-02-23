//
// PlayScene.cpp
//

#include "pch.h"
#include "PlayScene.h"
#include "Mouse.h"

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

bool PlayScene::Load(ID3D11DeviceContext1* deviceContext)
{
    sceneGraph = new SceneNode();

    camera.SetProjectionValues(90.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    camera.SetPosition(2.0f, 10.0f, 4.0f);
    camera.SetLookAtPos(DirectX::XMFLOAT3(5.0f, 0.0f, 5.0f));

    sky = DirectX::GeometricPrimitive::CreateSphere(deviceContext, 2000.0f, 16, true, true);

    ID3D11Device* device = nullptr;
    deviceContext->GetDevice(&device);

    DirectX::CreateWICTextureFromFile(device, L"Data/sky.jpg", nullptr, skyTexture.ReleaseAndGetAddressOf());

    terrain.Initialize(deviceContext);

    quadTree.Initialize(&terrain, deviceContext);

    m_deviceContext = deviceContext;

    player.Initialize(deviceContext);
    player.SetPosition(130.0f, 3.0f, 119.0f);

    effect = std::make_unique<DirectX::BasicEffect>(device);
    effect->SetAmbientLightColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
    effect->SetTextureEnabled(true);
    effect->SetPerPixelLighting(true);
    effect->SetLightingEnabled(true);
    effect->SetLightEnabled(0, true);
    effect->SetLightDiffuseColor(0, XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
    effect->SetLightDirection(0, SimpleMath::Vector3::UnitY);
    effect->SetTexture(skyTexture.Get());

    sky->CreateInputLayout(effect.get(), inputLayout.ReleaseAndGetAddressOf());

    model.Initialize("Data/10446_Palm_Tree_v1_max2010_iteration-2.obj", device, deviceContext);
    //mdl.Initialize("Data/WoodenCabinObj.obj", device, deviceContext);
    mdl.Initialize("Data/WoodCabin.dae", device, deviceContext);
    bridge.Initialize("Data/person_embeddedindexed.blend", device, deviceContext);
    bx = by = bz = 0.0f;
    //sbx = sby = sbz = 1.0f;
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

    //auto mouse = Mouse::Get().GetState();

    //if (mouse.rightButton)
       // camera.AdjustRotation(Mouse::GetRelativeY() * 0.01f, Mouse::GetRelativeX() * 0.01f, 0.0f);

    Mouse* mouse = &Mouse::Get();

    while (!mouse->EventBufferIsEmpty())
    {
        Mouse::Event mouseEvent = mouse->ReadEvent();

        if (mouseEvent.GetType() == Mouse::Event::MoveRaw && mouse->IsRightDown())
        {
            camera.AdjustRotation(mouseEvent.GetPositionY() * deltaTime * 0.5f, mouseEvent.GetPositionX() * deltaTime * 0.5f, 0.0f);
        }

    }

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

    //Mouse::SetRelativePos(0, 0);
    sceneGraph->Update(timer);
    m_fps = static_cast<float>(timer.GetFramesPerSecond());
}

void PlayScene::Render()
{
    m_world = XMMatrixIdentity();

    effect->SetWorld(m_world * XMMatrixTranslation(camera.GetPositionFloat3().x, camera.GetPositionFloat3().y, camera.GetPositionFloat3().z));
    effect->SetView(camera.GetViewMatrix());
    effect->SetProjection(camera.GetProjectionMatrix());

    sky->Draw(effect.get(), inputLayout.Get());

    frustum.Construct(1000.0f, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    quadTree.Draw(m_deviceContext, &frustum, m_world, camera.GetViewMatrix(), camera.GetProjectionMatrix());

    for (int x = 0; x < 3; ++x)
    {
        for (int z = 0; z < 3; ++z)
        {
            model.Draw(m_world * DirectX::XMMatrixScaling(0.08f, 0.08f, 0.08f) * DirectX::XMMatrixRotationX(3.1415f / 2.0f) * DirectX::XMMatrixTranslation(100.0f * x, 0.0f, 100.0f * z), camera.GetViewMatrix(), camera.GetProjectionMatrix());
        }
    }
    
    player.Draw(m_deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    
    mdl.Draw(m_world * DirectX::XMMatrixTranslation(125.0f, 45.0f, 150.0f) * XMMatrixScaling(0.5f, 0.5f, 0.5f), camera.GetViewMatrix(), camera.GetProjectionMatrix());
    bridge.Draw(m_world * DirectX::XMMatrixTranslation(bx, by, bz), camera.GetViewMatrix(), camera.GetProjectionMatrix());

    // ImGui Window.
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    std::ostringstream ss("");
    ss << "Frames per second: " << m_fps;
    ImGui::Text(ss.str().c_str());

    ImGui::DragFloat("Position X: ", &bx);
    ImGui::DragFloat("Position Y: ", &by);
    ImGui::DragFloat("Position Z: ", &bz);

    ImGui::DragFloat("Scale X: ", &sbx);
    ImGui::DragFloat("Scale Y: ", &sby);
    ImGui::DragFloat("Scale Z: ", &sbz);


    ss.str("");
    ss << "Draw count: " << quadTree.GetDrawCount();
    ImGui::Text(ss.str().c_str());
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    sceneGraph->Render();
}