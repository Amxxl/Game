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

    Application::Get().GetAudio()->Load("Data/Sounds/Music001.mp3");
    Application::Get().GetAudio()->Load("Data/Sounds/Music002.mp3");
    Application::Get().GetAudio()->Load("Data/Sounds/Music003.mp3");

    camera.SetProjectionValues(90.0f, 800.0f / 600.0f, 0.1f, 1500.0f);
    camera.SetPosition(0.0f, 20.0f, 0.0f);
    camera.SetLookAtPos(Vector3f(5.0f, 0.0f, 5.0f));
    camera.SetRotation(0.0f, 0.0f, 0.0f);

    camera2d.SetProjectionValues(800.0f, 600.0f, 0.0f, 1.0f);
    camera2d.SetPosition(0.0f, 0.0f, 0.0f);
    camera2d.SetRotation(0.0f, 0.0f, 0.0f);

    light = std::make_unique<PointLight>(m_pDeviceResources);

    sky = DirectX::GeometricPrimitive::CreateSphere(m_deviceContext, 2000.0f, 16, true, true);
    water = DirectX::GeometricPrimitive::CreateBox(m_deviceContext, Vector3f(512.0f, 22.0f, 512.0f), false, false);

    ID3D11Device* device = DX::GetDevice(m_deviceContext);

    DirectX::CreateWICTextureFromFile(device, L"Data/sky.jpg", nullptr, skyTexture.ReleaseAndGetAddressOf());

    terrain.Initialize(m_deviceContext);
    octree.Initialize(&terrain, m_deviceContext);

    m_pDeviceResources->SetCamera(&camera);

    player.Initialize(m_deviceContext);
    player.SetPosition(205.0f, 19.5f, 215.0f);
    player.LoadMesh(m_deviceContext, L"Data/Models/Reptile/reptile.md5mesh");
    player.LoadAnim(L"Data/Models/Reptile/idle.md5anim"); // 0
    player.LoadAnim(L"Data/Models/Reptile/walk.md5anim"); // 1
    player.LoadAnim(L"Data/Models/Reptile/jump.md5anim"); // 2
    player.LoadAnim(L"Data/Models/Reptile/run.md5anim"); // 3
    player.LoadAnim(L"Data/Models/Reptile/attack1.md5anim"); // 4
    player.LoadAnim(L"Data/Models/Reptile/attack2.md5anim"); // 5
    player.LoadAnim(L"Data/Models/Reptile/attack3.md5anim"); // 6
    player.LoadAnim(L"Data/Models/Reptile/dead.md5anim"); // 7

    npc.Initialize(m_deviceContext);
    npc.SetPosition(188.0f, 16.5f, 220.0f);
    npc.SetRotation(0.0f, 90.0f, 0.0f);
    npc.LoadMesh(m_deviceContext, L"Data/guard.md5mesh");
    npc.LoadAnim(L"Data/guard.md5anim");

    reptile.Initialize(m_deviceContext);
    reptile.SetPosition(200.0f, 19.0f, 220.0f);
    reptile.SetRotation(0.0f, 90.0f, 0.0f);
    reptile.LoadMesh(m_deviceContext, L"Data/Models/Reptile/reptile.md5mesh");
    reptile.LoadAnim(L"Data/Models/Reptile/idle.md5anim");
    reptile.LoadAnim(L"Data/Models/Reptile/walk.md5anim");
    reptile.LoadAnim(L"Data/Models/Reptile/jump.md5anim");

    jugger.Initialize(m_deviceContext);
    jugger.SetPosition(200.0f, 19.0f, 240.0f);
    jugger.SetRotation(0.0f, 90.0f, 0.0f);
    jugger.LoadMesh(m_deviceContext, L"Data/Models/Juggernaut/Juggernaut.md5mesh");
    jugger.LoadAnim(L"Data/Models/Juggernaut/Juggernaut_idle.md5anim");

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

    tree = std::make_unique<Model>(window.GetDeviceResources(), "Data/10446_Palm_Tree_v1_max2010_iteration-2.obj");
    house = std::make_unique<Model>(window.GetDeviceResources(), "Data/WoodCabin.dae");
    bridge = std::make_unique<Model>(window.GetDeviceResources(), "Data/bridge.dae");
    spruce = std::make_unique<Model>(window.GetDeviceResources(), "Data/spruce.obj");
    well = std::make_unique<Model>(window.GetDeviceResources(), "Data/Models/Well/well.dae");
    //sponza = std::make_unique<Model>(window.GetDeviceResources(), "Data/Models/Sponza/sponza.obj");

    spr = std::make_unique<Sprite>(m_pDeviceResources, "Data/spellbar.jpg");

    spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext);
    font = std::make_unique<DirectX::SpriteFont>(device, L"Data/Fonts/Consolas14BI.spritefont");


    state = std::make_unique<DirectX::CommonStates>(DX::GetDevice(m_deviceContext));
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
    static float speed;

    static int anim_index = 0;

    Vector3f add;
    DirectX::XMStoreFloat3(&add, camera.GetForwardVector());

    if (Input::IsMouseButtonDown(Input::MouseButton::Right))
        player.SetRotation(0.0f, atan2(add.x, add.z), 0.0f);


    if (Input::IsKeyDown(Input::Key::W) || Input::IsMouseButtonDown(Input::MouseButton::Left) && Input::IsMouseButtonDown(Input::MouseButton::Right))
    {
        if (Input::IsKeyDown(VK_SHIFT))
        {
            speed = 12.0f;
            anim_index = 3;
        }
        else
        {
            speed = 6.0f;
            anim_index = 1;
        }
        XMFLOAT3 forward;
        DirectX::XMStoreFloat3(&forward, camera.GetForwardVector());

        player.SetRotation(0.0f, atan2(forward.x, forward.z), 0.0f);
        player.AdjustPosition(player.GetForwardVector(true) * deltaTime * speed);
    }
    else if (Input::IsKeyDown(Input::Key::S))
    {
        if (Input::IsKeyDown(VK_SHIFT))
        {
            speed = 40.0f;
            anim_index = 3;
        }
        else
        {
            speed = 20.0f;
            anim_index = 1;
        }
        Vector3f forward;
        DirectX::XMStoreFloat3(&forward, camera.GetBackwardVector());

        player.SetRotation(0.0f, atan2(forward.x, forward.z), 0.0f);
        player.AdjustPosition(player.GetForwardVector(true) * deltaTime * speed);
    }
    else if (Input::IsKeyDown(Input::Key::A))
    {
        if (Input::IsKeyDown(VK_SHIFT))
        {
            speed = 40.0f;
            anim_index = 3;
        }
        else
        {
            speed = 20.0f;
            anim_index = 1;
        }
        Vector3f forward;
        DirectX::XMStoreFloat3(&forward, camera.GetLeftVector());

        player.SetRotation(0.0f, atan2(forward.x, forward.z), 0.0f);
        player.AdjustPosition(player.GetForwardVector(true) * deltaTime * speed);
    }
    else if (Input::IsKeyDown(Input::Key::D))
    {
        if (Input::IsKeyDown(VK_SHIFT))
        {
            speed = 40.0f;
            anim_index = 3;
        }
        else
        {
            speed = 20.0f;
            anim_index = 1;
        }
        Vector3f forward;
        DirectX::XMStoreFloat3(&forward, camera.GetRightVector());

        player.SetRotation(0.0f, atan2(forward.x, forward.z), 0.0f);
        player.AdjustPosition(player.GetForwardVector(true) * deltaTime * speed);
    }
    else if (in_jump)
        anim_index = 2;
    else
        anim_index = 0;

    if (Input::IsKeyDown(Input::Key::D1))
        anim_index = 4;

    if (Input::IsKeyDown(Input::Key::D2))
        anim_index = 5;

    if (Input::IsKeyDown(Input::Key::D3))
        anim_index = 6;

    if (Input::IsKeyDown(Input::Key::D4))
        anim_index = 7;

    if (player.GetPositionFloat3().y > 19.5f)
    {
        acceleration += velocity * deltaTime;
        player.AdjustPosition(0.0f, -acceleration, 0.0f);
        velocity += 10.0f * deltaTime;
        in_jump = true;
        anim_index = 2;
    }

    if (player.GetPositionFloat3().y <= 19.5f)
    {
        acceleration = 0.0f;
        velocity = 1.0f;
        in_jump = false;
        player.SetPosition(player.GetPositionFloat3().x, 19.5f, player.GetPositionFloat3().z);
    }

    player.Update(m_deviceContext, deltaTime, anim_index);
    npc.Update(m_deviceContext, deltaTime, 0);

    if (Input::IsKeyDown(Input::Key::G))
        reptile.Update(m_deviceContext, deltaTime, 2);
    else
        reptile.Update(m_deviceContext, deltaTime, 0);

    jugger.Update(m_deviceContext, deltaTime, 0);

    camera.SetOrigin(player.GetPositionFloat3());
    camera.UpdateMatrix();

    m_pDeviceResources->SetCamera(&camera);
    m_pDeviceResources->SetCamera2D(&camera2d);

    Application::Get().GetAudio()->SetListener(player.GetPositionFloat3(), player.GetForwardVector());
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

    player.Draw(m_deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    npc.Draw(m_deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    reptile.Draw(m_deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());
    jugger.Draw(m_deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());

    ID3D11RasterizerState* cullNone = state->CullNone();
    m_deviceContext->RSSetState(cullNone);

    light->Bind(m_pDeviceResources, camera.GetViewMatrix());

    spruce->Draw(m_pDeviceResources, m_world * DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) * DirectX::XMMatrixRotationX(AI_MATH_PI / 2) * DirectX::XMMatrixTranslation(250.0f, 16.5f, 200.0f));
    bridge->Draw(m_pDeviceResources, m_world * DirectX::XMMatrixRotationY(-77.0f * (3.1415f / 180.0f)) * DirectX::XMMatrixTranslation(257.0f, 58.0f, 381.0f));
    tree->Draw(m_pDeviceResources, m_world * DirectX::XMMatrixScaling(0.08f, 0.08f, 0.08f) * DirectX::XMMatrixRotationX(3.1415f / 2.0f) * DirectX::XMMatrixTranslation(200.0f, 16.0f, 200.0f));
    well->Draw(m_pDeviceResources, m_world * DirectX::XMMatrixScaling(3.0f, 3.0f, 3.0f) * DirectX::XMMatrixTranslation(168.0f, 16.5f, 220));
    house->Draw(m_pDeviceResources, m_world * DirectX::XMMatrixTranslation(465.0f, 32.5f, 485.0f) * XMMatrixScaling(0.5f, 0.5f, 0.5f));
    house->Draw(m_pDeviceResources, m_world * DirectX::XMMatrixScaling(0.7f, 0.7f, 0.7f)* DirectX::XMMatrixTranslation(365.0f, 32.5f, 485.0f) * XMMatrixScaling(0.5f, 0.5f, 0.5f));
   
    //sponza->Draw(m_pDeviceResources, m_world * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));

    spr->Draw(m_pDeviceResources, m_world * XMMatrixScaling(336.0f, 135.0f, 1.0f) * XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) * XMMatrixTranslation(pWindow->GetSize().x / 2.0f - 336.0f / 2.0f + 336.0f / 2.0f, pWindow->GetSize().y - (135.0f / 2.0f) / 2.0f, 0.0f));

    water->Draw(m_world * XMMatrixTranslation(256.0f, 0.0f, 256.0f), camera.GetViewMatrix(), camera.GetProjectionMatrix(), XMVectorSet(0.0f, 0.0f, 1.0f, 0.7f));


    std::ostringstream ss("");
    ss << "Rezolution: " << static_cast<int>(pWindow->GetSize().x) << "x" << static_cast<int>(pWindow->GetSize().y) << "\nFPS: " << m_fps;


    spriteBatch->Begin();
    font->DrawString(spriteBatch.get(), StringHelper::NarrowToWide(ss.str()).c_str(), DirectX::XMVectorSet(10.0f, 10.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
    spriteBatch->End();


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

    light->SpawnControlWindow();
    Application::Get().GetAudio()->SpawnControlWindow();
}

void PlayScene::OnKeyChar(size_t key)
{
    Logger::Get()->info("EventKeyChar: {}", static_cast<char>(key));
}

void PlayScene::OnKeyPressed(size_t key)
{
    if (!in_jump && key == Input::Key::Space)
    {
        in_jump = true;

        player.AdjustPosition(0.0f, 10.0f, 0.0f);
    }

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