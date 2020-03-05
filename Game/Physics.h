#pragma once

#include "StepTimer.h"

struct PhysicsMaterial
{
    float static_friction;
    float dynamic_friction;
    float restitution;
};

class Physics
{
    public:
        Physics();

        void Initialize();
        void Shutdown();

    public:
        static constexpr PhysicsMaterial DEFAULT_MATERIAL = { 0.5f, 0.5f, 0.5f };
};