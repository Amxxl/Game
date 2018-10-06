//
// SceneManager.h - @todo: add input devices event handling.
//

#pragma once

#include "Scene.h"

class SceneManager
{
    public:
        SceneManager();
        ~SceneManager();

        void SwapScene(Scene* scene);
        void PushScene(Scene* scene);
        void PopScene();

        void Update(DX::StepTimer const& timer);
        void Render();

    private:
        std::vector<Scene*> scenes;
};