#pragma once

#include "Bindable.h"

namespace Bind
{
    class PixelShader : public Bindable
    {
        public:
            PixelShader(DX::DeviceResources* deviceResources, std::string const& path);
            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

            static std::shared_ptr<PixelShader> Resolve(DX::DeviceResources* deviceResources, std::string const& path);
            static std::string GenerateUID(std::string const& path);
            virtual std::string const& GetUID() const noexcept override;

        protected:
            std::string path;
            Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    };
}