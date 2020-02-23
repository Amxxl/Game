#pragma once
#include "Bindable.h"

namespace Bind
{
    class NullPixelShader : public Bindable
    {
        public:
            NullPixelShader(DX::DeviceResources* deviceResources);
            void Bind(DX::DeviceResources* deviceResources) noexcept override;
            static std::shared_ptr<NullPixelShader> Resolve(DX::DeviceResources* deviceResources);
            static std::string GenerateUID();
            virtual std::string const& GetUID() const noexcept override;
    };
}