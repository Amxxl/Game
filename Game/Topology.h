#pragma once

#include "Bindable.h"

namespace Bind
{
    class Topology : public Bindable
    {
        public:
            Topology(DX::DeviceResources* deviceResources, D3D11_PRIMITIVE_TOPOLOGY type);
            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

            static std::shared_ptr<Topology> Resolve(DX::DeviceResources* deviceResources, D3D11_PRIMITIVE_TOPOLOGY type);
            static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
            virtual std::string const& GetUID() const noexcept override;

        protected:
            D3D11_PRIMITIVE_TOPOLOGY topology;
    };
}
