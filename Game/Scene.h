//
// Scene.h
//

#pragma once

#include "StepTimer.h"

class Scene
{
    protected:
        Scene() = default;

    public:
        virtual bool Load() = 0;
        virtual void Unload() = 0;

        virtual void Update(DX::StepTimer const& timer) = 0;
        virtual void Render() = 0;

        virtual void Pause() = 0;
        virtual void Resume() = 0;
};