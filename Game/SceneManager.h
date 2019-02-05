//
// SceneManager.h - @todo: add input devices event handling.
//

#pragma once

#include "Scene.h"

class SceneManager // @todo: Rename it to SceneController.
{
    public:
        SceneManager(ID3D11DeviceContext1* deviceContext);
        ~SceneManager();

        // Scene management
        void SwapScene(Scene* scene);
        void PushScene(Scene* scene);
        void PopScene();

        void Update(DX::StepTimer const& timer);
        void Render();

    private:
        ID3D11DeviceContext1* m_deviceContext;
        std::vector<Scene*> scenes;
};