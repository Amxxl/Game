#pragma once

#include "Bindable.h"

namespace Bind
{
    class Sampler : public Bindable
    {
        public:
            enum class State
            {
                POINT_WRAP,
                POINT_CLAMP,
                LINEAR_WRAP,
                LINEAR_CLAMP,
                ANISOTROPIC_WRAP,
                ANISOTROPIC_CLAMP
            };

            Sampler(DX::DeviceResources* deviceResources, State state);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

        private:
            void Create(DX::DeviceResources* deviceResources, D3D11_FILTER fileter, D3D11_TEXTURE_ADDRESS_MODE addressMode);

        protected:
            Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
    };
}
