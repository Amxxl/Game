#pragma once
#include "ConditionalNoexcept.h"

class Job
{
    public:
        Job(const class Step* pStep, const class Drawable* pDrawable);
        void Execute(class DX::DeviceResources* deviceResources) const noxnd;

    private:
        const class Drawable* pDrawable;
        const class Step* pStep;
};

