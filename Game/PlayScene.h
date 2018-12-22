//
// PlayScene.h
//

#pragma once

#include <string>
#include <sstream>

#include "Scene.h"

#include "GeometricPrimitive.h"
#include "Camera.h"

#include "NodeDisplayFPS.h"
#include "Terrain.h"

#include "MD5Model.h"

class PlayScene : public Scene
{
    public:
        PlayScene();
        ~PlayScene();

        void CreateDeviceDependentResources() override;
        void CreateWindowSizeDependentResources() override;

        // IDeviceNotify
        void OnDeviceLost() override;
        void OnDeviceRestored() override;

        // Messages
        void OnActivated() override;
        void OnDeactivated() override;
        void OnSuspending() override;
        void OnResuming() override;
        void OnWindowMoved() override;
        void OnWindowSizeChanged(int width, int height) override;

        bool Load(ID3D11DeviceContext1* ) override;
        void Unload() override;
        
        void Update(DX::StepTimer const& timer) override;
        void Render() override;

        void Pause() override { paused = true; }
        void Resume() override { paused = false; }

        bool const IsPaused() const { return paused; }

        static PlayScene* Instance() { return &s_instance; }

    private:
        // Instance of our play scene.
        static PlayScene s_instance;

    private:
        std::unique_ptr<DirectX::GeometricPrimitive> sky;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyTexture;

        std::unique_ptr<DirectX::GeometricPrimitive> water;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> waterTexture;
        std::unique_ptr<DirectX::CommonStates> state;

        ID3D11DeviceContext* m_deviceContext = nullptr;

        Terrain terrain;
        MD5Model model;
        Camera camera;
        DirectX::XMMATRIX m_world;

        int mouse_x;
        int mouse_y;

    private:
        bool paused;
};

