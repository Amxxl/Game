#pragma once
#include <array>
#include "BindableCommon.h"
#include "DeviceResources.h"
#include "Job.h"
#include "Pass.h"
#include "NullPixelShader.h"
#include "Stencil.h"

class FrameCommander
{
    public:
        void Accept(Job job, size_t target) noexcept
        {
            passes[target].Accept(job);
        }

        void Execute(DX::DeviceResources* deviceResources) const noxnd
        {
            using namespace Bind;
            // normally this would be a loop with each pass defining it setop / etc.
            // and later on it would be a complex graph with parallel execution contingent
            // on input / output requirements.

            // main phong lighting pass
            Stencil::Resolve(deviceResources, Stencil::Mode::Off)->Bind(deviceResources);
            passes[0].Execute(deviceResources);
            // outline masking pass
            Stencil::Resolve(deviceResources, Stencil::Mode::Write)->Bind(deviceResources);
            NullPixelShader::Resolve(deviceResources)->Bind(deviceResources);
            passes[1].Execute(deviceResources);
            // outline drawing pass
            Stencil::Resolve(deviceResources, Stencil::Mode::Mask)->Bind(deviceResources);
            passes[2].Execute(deviceResources);
        }

        void Reset() noexcept
        {
            for (auto& p : passes)
            {
                p.Reset();
            }
        }

    private:
        std::array<Pass, 3> passes;
};

