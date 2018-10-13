//
// PlayScene.h
//

#pragma once

#include <string>
#include <sstream>

#include "Scene.h"

#include "GeometricPrimitive.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class PlayScene : public Scene
{
    public:
        PlayScene();
        ~PlayScene();

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
        std::unique_ptr<DirectX::GeometricPrimitive> shape;
        Camera camera;
        XMMATRIX m_world;

        unsigned int fps;

        int mouse_x;
        int mouse_y;
    private:
        bool paused;
};

