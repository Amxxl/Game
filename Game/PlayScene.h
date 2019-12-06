//
// PlayScene.h
//

#pragma once

#include "Scene.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GeometricPrimitive.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "ThirdPersonCamera.h"
#include "Terrain.h"
#include "Octree.h"

#include "Frustum.h"
#include "Model.h"
#include "RenderableGameObject.h"
#include "PointLight.h"
#include "Sprite.h"

#include "MouseEvents.h"
#include "KeyEvents.h"

class PlayScene : public Scene
{
    public:
        PlayScene();
        ~PlayScene();

        virtual bool Load(SceneManager* sceneManager, Window& window) final override;
        virtual void Unload() final override;
        
        virtual void Update(DX::StepTimer const& timer) final override;
        virtual void Render() final override;

        virtual void Pause() final override { paused = true; }
        virtual void Resume() final override { paused = false; }

        bool const IsPaused() const { return paused; }

        virtual void OnKeyChar(size_t key) final override;
        virtual void OnKeyPressed(size_t key) final override;
        virtual void OnKeyReleased(size_t key) final override;

        virtual void OnMouseMoved(Vector2i const& position) final override;
        virtual void OnMouseMovedRaw(Vector2i const& position) final override;
        virtual void OnMouseWheelScrolled(Vector2i const& position, float const delta) final override;
        virtual void OnMouseButtonPressed(Vector2i const& position, Input::MouseButton const button) final override;
        virtual void OnMouseButtonReleased(Vector2i const& position, Input::MouseButton const button) final override;
        virtual void OnMouseButtonDoubleClicked(Vector2i const& position, Input::MouseButton const button) final override;

        static PlayScene& Get() { return s_instance; }

    private:
        // Instance of our play scene.
        static PlayScene s_instance;

    private:
        std::unique_ptr<DirectX::GeometricPrimitive> water;
        std::unique_ptr<DirectX::GeometricPrimitive> sky;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyTexture;
        std::unique_ptr<DirectX::BasicEffect> effect;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

        std::unique_ptr<DirectX::CommonStates> state;

        std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
        std::unique_ptr<DirectX::SpriteFont> font;

        ID3D11DeviceContext* m_deviceContext = nullptr;
        Window* pWindow = nullptr;

        Terrain terrain;
        Octree octree;
        Frustum frustum;

        RenderableGameObject player;
        RenderableGameObject npc;
        RenderableGameObject reptile;

        std::unique_ptr<PointLight> light;

        std::unique_ptr<Model> testModel;
        std::unique_ptr<Model> house;
        std::unique_ptr<Model> tree;
        std::unique_ptr<Model> bridge;
        std::unique_ptr<Model> spruce;
        std::unique_ptr<Model> well;

        std::unique_ptr<Sprite> spr;

        DX::DeviceResources* m_pDeviceResources;

        //Camera camera;
        Camera2D camera2d;
        ThirdPersonCamera camera;
        DirectX::XMMATRIX m_world;

        float velocity = 1.0f;
        float acceleration = 0.0f;
        float m_fps;
        bool in_jump;

    private:
        bool paused;
};

