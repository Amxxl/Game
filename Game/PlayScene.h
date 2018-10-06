//
// PlayScene.h
//

#pragma once

#include "Scene.h"

class PlayScene : public Scene
{
    public:
        PlayScene();
        ~PlayScene();

        bool Load() override;
        void Unload() override;
        
        void Update(DX::StepTimer const& timer) override;
        void Render() override;

        void Pause() override { paused = true; }
        void Resume() override { paused = false; }

        bool const IsPaused() const { return paused; }

        static PlayScene* GetInstance() { return &s_instance; }

    private:
        // Instance of our play scene.
        static PlayScene s_instance;

    private:
        bool paused;
};

