#pragma once

#include "Bindable.h"

namespace Bind
{
    class Sampler : public Bindable
    {
        public:
            Sampler(DX::DeviceResources* deviceResources);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

        protected:
            Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
    };
}
