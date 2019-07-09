#pragma once

#include "Bindable.h"

namespace Bind
{
    class PixelShader : public Bindable
    {
        public:
            PixelShader(DX::DeviceResources* deviceResources, std::wstring const& path);
            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

        protected:
            Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    };
}