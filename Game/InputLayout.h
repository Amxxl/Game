#pragma once

#include "Bindable.h"

namespace Bind
{
    class InputLayout : public Bindable
    {
        public:
            InputLayout(DX::DeviceResources* deviceResources,
                std::vector<D3D11_INPUT_ELEMENT_DESC> const& layout,
                ID3DBlob* pVertexShaderByteCode);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

        protected:
            Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    };
}