//
// SceneManager.h - @todo: add input devices event handling.
//

#pragma once

#include "Scene.h"

class SceneManager
{
    public:
        SceneManager(ID3D11DeviceContext1* deviceContext);
        ~SceneManager();

        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();

        // IDeviceNotify
        void OnDeviceLost();
        void OnDeviceRestored();

        // Messages
        void OnActivated();
        void OnDeactivated();
        void OnSuspending();
        void OnResuming();
        void OnWindowMoved();
        void OnWindowSizeChanged(int width, int height);

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