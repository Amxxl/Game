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

            
            static std::shared_ptr<Sampler> Resolve(DX::DeviceResources* deviceResources, State state);
            static std::string GenerateUID(State state);
            virtual std::string const& GetUID() const noexcept override;

        private:
            void Create(DX::DeviceResources* deviceResources, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode);

        protected:
            State state;
            Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
    };
}
