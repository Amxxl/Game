//
// PlayScene.cpp
//

#include "pch.h"
#include "PlayScene.h"
#include "Mouse.h"

#include "StringHelper.h"
#include "SceneManager.h"

#include "Application.h"
#include "WindowEvents.h"
#include "Application.h"

PlayScene PlayScene::s_instance;

using namespace DirectX;

PlayScene::PlayScene()
{
    paused = false;
    in_jump = false;
}

PlayScene::~PlayScene()
{
}

bool PlayScene::Load(SceneManager* sceneManager, Window& window)
{
    m_pDeviceResources = window.GetDeviceResources();
    m_deviceContext = window.GetDeviceResources()->GetDeviceContext();
    pWindow = &window;

    Application::Get().GetAudio()->LoadSound("Data/file.mp3", true, true);

    camera.SetProjectionValues(60.0f, 800.0f / 600.0f, 0.1f, 1500.0f);
    camera.SetPosition(0.0f, 20.0f, 0.0f);
    camera.SetLookAtPos(Vector3f(5.0f, 0.0f, 5.0f));
    camera.SetRotation(0.0f, 0.0f, 0.0f);

    sky = DirectX::GeometricPrimitive::CreateSphere(m_deviceContext, 2000.0f, 16, true, true);
    water = DirectX::GeometricPrimitive::CreateBox(m_deviceContext, Vector3f(512.0f, 22.0f, 512.0f), false, false);

    ID3D11Device* device = DX::GetDevice(m_deviceContext);

    DirectX::CreateWICTextureFromFile(device, L"Data/sky.jpg", nullptr, skyTexture.ReleaseAndGetAddressOf());

    terrain.Initialize(m_deviceContext);
    octree.Initialize(&terrain, m_deviceContext);

    m_pDeviceResources->SetCamera(&camera);

    player.Initialize(m_deviceContext);
    player.SetPosition(205.0f, 16.5f, 215.0f);
    player.LoadMesh(m_deviceContext, L"Data/wraith.md5mesh");
    player.LoadAnim(L"Data/idle.md5anim"); // 0
    player.LoadAnim(L"Data/walk.md5anim"); // 1
    player.LoadAnim(L"Data/attack1.md5anim"); // 2
    player.LoadAnim(L"Data/attack2.md5anim"); // 3
    player.LoadAnim(L"Data/attack3.md5anim"); // 4

    player.LoadAnim(L"Data/ceiling_attack_128.md5anim"); // 5
    player.LoadAnim(L"Data/ceiling_attack_192.md5anim"); // 6
    player.LoadAnim(L"Data/ceiling_attack_256.md5anim"); // 7 // error
    player.LoadAnim(L"Data/ceiling_idle_128.md5anim"); // 8
    player.LoadAnim(L"Data/ceiling_idle_192.md5anim"); // 9
    player.LoadAnim(L"Data/ceiling_idle_256.md5anim"); // 10
    player.LoadAnim(L"Data/evade_left.md5anim"); // 11
    player.LoadAnim(L"Data/evade_right.md5anim"); // 12
    player.LoadAnim(L"Data/af_pose.md5anim"); // 13
    player.LoadAnim(L"Data/sight.md5anim"); // 14
    player.LoadAnim(L"Data/teleport.md5anim"); // 15
    player.LoadAnim(L"Data/run.md5anim"); // 16

    npc.Initialize(m_deviceContext);
    npc.SetPosition(188.0f, 16.5f, 220.0f);
    npc.SetRotation(0.0f, 90.0f, 0.0f);
    npc.LoadMesh(m_deviceContext, L"Data/guard.md5mesh");
    npc.LoadAnim(L"Data/guard.md5anim");

    effect = std::make_unique<DirectX::BasicEffect>(device);
    effect->SetAmbientLightColor(XMVectorSet(1.0f, 1.0f, 1.0f, 0.5f));
    effect->SetTextureEnabled(true);
    effect->SetPerPixelLighting(true);
    effect->SetLightingEnabled(true);
    effect->SetLightEnabled(0, true);
    effect->SetLightDiffuseColor(0, XMVectorSet(1.0f, 1.0f, 1.0f, 0.5f));
    effect->SetLightDirection(0, SimpleMath::Vector3::UnitZ);
    effect->SetTexture(skyTexture.Get());

    sky->CreateInputLayout(effect.get(), inputLayout.ReleaseAndGetAddressOf());

    model.Initialize("Data/10446_Palm_Tree_v1_max2010_iteration-2.obj", device, m_deviceContext);
    mdl.Initialize("Data/WoodCabin.dae", device, m_deviceContext);
    bridge.Initialize("Data/bridge.dae", device, m_deviceContext);

    spruce.Initialize("Data/spruce.obj", device, m_deviceContext);

    testModel = std::make_unique<expr::Model>(window.GetDeviceResources(), "Data/10446_Palm_Tree_v1_max2010_iteration-2.obj");

    spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext);
    font = std::make_unique<DirectX::SpriteFont>(device, L"Data/Fonts/Consolas14BI.spritefont");

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
    m_fps = static_cast<float>(timer.GetFramesPerSecond());

    static float cameraSpeed = 40.0f;
    static float speed = 20.0f;

    static int anim_index = 0;

    Vector3f add;
    DirectX::XMStoreFloat3(&add, camera.GetForwardVector());

    if (Input::IsMouseButtonDown(Input::MouseButton::Right))
        player.SetRotation(0.0f, atan2(add.x, add.z), 0.0f);


    if (Input::IsKeyDown(Input::Key::W) || Input::IsMouseButtonDown(Input::MouseButton::Left) && Input::IsMouseButtonDown(Input::MouseButton::Right))
    {
        XMFLOAT3 forward;
        DirectX::XMStoreFloat3(&forward, camera.GetForwardVector());

        
        player.AdjustPosition(player.GetForwardVector() * timer.GetElapsedSeconds() * speed);
        player.SetRotation(0.0f, atan2(forward.x, forward.z), 0.0f);
        anim_index = 1;
    }
    else if (Input::IsKeyDown(Input::Key::S))
    {
        Vector3f forward;
        DirectX::XMStoreFloat3(&forward, camera.GetBackwardVector());

        player.AdjustPosition(forward.x, 0.0f, forward.z);
        //player.AdjustPosition(player.GetBackwardVector() * timer.GetElapsedSeconds() * speed);
        player.SetRotation(0.0f, atan2(forward.x, forward.z), 0.0f);
        anim_index = 1;
    }
    else if (Input::IsKeyDown(Input::Key::A))
    {
        Vector3f forward;
        DirectX::XMStoreFloat3(&forward, camera.GetLeftVector());

        player.AdjustPosition(forward.x, 0.0f, forward.z);
        player.SetRotation(0.0f, atan2(forward.x, forward.z), 0.0f);
        anim_index = 1;

    }
    else if (Input::IsKeyDown(Input::Key::D))
    {
        Vector3f forward;
        DirectX::XMStoreFloat3(&forward, camera.GetRightVector());

        player.AdjustPosition(forward.x, 0.0f, forward.z);
        player.SetRotation(0.0f, atan2(forward.x, forward.z), 0.0f);
        anim_index = 1;
    }

    else anim_index = 0;

    if (Input::IsKeyDown(Input::Key::NumPad0))
        anim_index = 11;
    else if (Input::IsKeyDown(Input::Key::NumPad1))
        anim_index = 12;
    else if (Input::IsKeyDown(Input::Key::NumPad2))
        anim_index = 2;
    else if (Input::IsKeyDown(Input::Key::NumPad3))
        anim_index = 3;
    else if (Input::IsKeyDown(Input::Key::NumPad4))
        anim_index = 4;
    else if (Input::IsKeyDown(Input::Key::NumPad5))
        anim_index = 5;
    else if (Input::IsKeyDown(Input::Key::NumPad6))
        anim_index = 6;
    else if (Input::IsKeyDown(Input::Key::NumPad7))
        anim_index = 7;
    else if (Input::IsKeyDown(Input::Key::NumPad8))
        anim_index = 8;
    else if (Input::IsKeyDown(Input::Key::NumPad9))
        anim_index = 9;

    if (Input::IsKeyDown(Input::Key::D1))
        anim_index = 3;

    if (Input::IsKeyDown(Input::Key::D2))
        anim_index = 4;

    if (Input::IsKeyDown(Input::Key::D3))
        anim_index = 5;

    //static float velocity = 15.0f;

    if (player.GetPositionFloat3().y > 16.5f)
    {
        acceleration += velocity * deltaTime;
        //player.AdjustPosition(0.0f, -acceleration, 0.0f);
        //velocity += 15.0f * deltaTime;
        in_jump = false;
    }

    if (player.GetPositionFloat3().y <= 16.5f)
    {
        acceleration = 0.0f;
        velocity = 1.0f;
        in_jump = false;
        player.SetPosition(player.GetPositionFloat3().x, 16.5f, player.GetPositionFloat3().z);
    }

    player.Update(m_deviceContext, deltaTime, anim_index);
    npc.Update(m_deviceContext, deltaTime, 0);


    camera.SetOrigin(player.GetPositionFloat3());
    camera.UpdateMatrix();

    m_pDeviceResources->SetCamera(&camera);

    FMOD_VECTOR audioPos{ player.GetPositionFloat3().x, player.GetPositionFloat3().y, player.GetPositionFloat3().z };
    FMOD_VECTOR audioUp{ 0.0f, 1.0f, 0.0 };

    XMFLOAT3 testas;
    DirectX::XMStoreFloat3(&testas, player.GetForwardVector());

    FMOD_VECTOR audioFor{ testas.x, testas.y, testas.z };

    Application::Get().GetAudio()->GetSystem()->set3DListenerAttributes(0, &audioPos, nullptr, &audioFor, &audioUp);
}

void PlayScene::Render()
{
    m_world = XMMatrixIdentity();

    effect->SetWorld(m_world * XMMatrixTranslation(player.GetPositionFloat3().x, player.GetPositionFloat3().y, player.GetPositionFloat3().z));
    effect->SetView(camera.GetViewMatrix());
    effect->SetProjection(camera.GetProjectionMatrix());

    sky->Draw(effect.get(), inputLayout.Get());

    // Terrain drawing.
    frustum.Construct(500.0f, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    octree.Draw(m_deviceContext, &frustum, m_world, camera.GetViewMatrix(), camera.GetProjectionMatrix());


    model.Draw(m_world * DirectX::XMMatrixScaling(0.08f, 0.08f, 0.08f) * DirectX::XMMatrixRotationX(3.1415f / 2.0f) * DirectX::XMMatrixTranslation(200.0f, 16.0f, 200.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix());

    player.Draw(m_deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    npc.Draw(m_deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());

    mdl.Draw(m_world * DirectX::XMMatrixTranslation(465.0f, 32.5f, 485.0f) * XMMatrixScaling(0.5f, 0.5f, 0.5f), camera.GetViewMatrix(), camera.GetProjectionMatrix());
    mdl.Draw(m_world * DirectX::XMMatrixScaling(0.7f, 0.7f, 0.7f) * DirectX::XMMatrixTranslation(365.0f, 32.5f, 485.0f) * XMMatrixScaling(0.5f, 0.5f, 0.5f), camera.GetViewMatrix(), camera.GetProjectionMatrix());

    spruce.Draw(m_world * DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) * DirectX::XMMatrixRotationX(AI_MATH_PI / 2) * DirectX::XMMatrixTranslation(250.0f, 16.5f, 200.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix());

    bridge.Draw(m_world * DirectX::XMMatrixRotationY(-77.0f * (3.1415f / 180.0f)) * DirectX::XMMatrixTranslation(257.0f, 58.0f, 381.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix());
    
    testModel->Draw(m_pDeviceResources, DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
    
    water->Draw(m_world * XMMatrixTranslation(256.0f, 0.0f, 256.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix(), XMVectorSet(0.0f, 0.0f, 1.0f, 0.7f));


    std::ostringstream ss("");
    ss << "FPS: " << m_fps;

    spriteBatch->Begin();
    font->DrawString(spriteBatch.get(), StringHelper::StringToWide(ss.str()).c_str(), DirectX::XMVectorSet(10.0f, 10.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
    spriteBatch->End();

    // ImGui Window.
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::Text("Position");
    ImGui::SliderFloat("R", &camera.r, 0.0f, 40.0f, "%.1f");
    ImGui::SliderAngle("Theta", &camera.theta, -180.0f, 180.0f);
    ImGui::SliderAngle("Phi", &camera.phi, -89.0f, 89.0f);
    
    ImGui::Text("Orientation");
    ImGui::SliderAngle("Roll", &camera.roll, -180.0f, 180.0f);
    ImGui::SliderAngle("Pitch", &camera.pitch, -180.0f, 180.0f);
    ImGui::SliderAngle("Yaw", &camera.yaw, -180.0f, 180.0f);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void PlayScene::OnKeyPressed(size_t key)
{
    if (!in_jump && key == Input::Key::Space)
    {
        in_jump = true;

        player.AdjustPosition(0.0f, 10.0f, 0.0f);
    }

    if (key == Input::Key::F)
    {
        Vector3 poza = npc.GetPositionFloat3();
        Application::Get().GetAudio()->PlaySound("Data/file.mp3", npc.GetPositionFloat3());
    }

    if (key == Input::Key::C)
        Application::Get().GetAudio()->StopAllChannels();

    Logger::Get()->info("Key was pressed: {}", key);
}

void PlayScene::OnKeyReleased(size_t key)
{
    Logger::Get()->info("Key was released: {}", key);
}

void PlayScene::OnMouseMoved(Vector2i const& position)
{
}

void PlayScene::OnMouseMovedRaw(Vector2i const& position)
{
    if (Input::IsMouseButtonDown(Input::MouseButton::Right) || Input::IsMouseButtonDown(Input::MouseButton::Left))
    {
        if (camera.theta < 0.0f)
            camera.theta = DirectX::XM_2PI;
        else if (camera.theta > DirectX::XM_2PI)
            camera.theta = 0.0f;

        camera.theta -= position.x * 0.01f;
        camera.phi += position.y * 0.01f;


        if (camera.phi <= 0.0f)
            camera.phi = 0.0f;

        if (camera.phi >= DirectX::XMConvertToRadians(89.0f))
            camera.phi = DirectX::XMConvertToRadians(89.0f);
    }
    //camera.AdjustRotation(position.y * 0.01f, position.x * 0.01f, 0.0f);
}

void PlayScene::OnMouseWheelScrolled(Vector2i const& position, float const delta)
{
    camera.r -= delta;

    if (camera.r > 40.0f)
        camera.r = 40.0f;
    else if (camera.r < 1.0f)
        camera.r = 1.0f;
}

void PlayScene::OnMouseButtonPressed(Vector2i const& position, Input::MouseButton const button)
{
    if (button == Input::MouseButton::Left)
    {
        pWindow->DisableCursor();
    }
    else if (button == Input::MouseButton::Right)
    {
        pWindow->DisableCursor();
    }

    Logger::Get()->info("Mouse button pressed: {}", static_cast<int>(button));
}

void PlayScene::OnMouseButtonReleased(Vector2i const& position, Input::MouseButton const button)
{
    if (button == Input::MouseButton::Left)
    {
        pWindow->EnableCursor();
    }
    else if (button == Input::MouseButton::Right)
    {
        pWindow->EnableCursor();
    }

    Logger::Get()->info("Mouse button released: {}", static_cast<int>(button));
}

void PlayScene::OnMouseButtonDoubleClicked(Vector2i const& position, Input::MouseButton const button)
{
    Logger::Get()->info("Mouse button double clicked: {}", static_cast<int>(button));
}