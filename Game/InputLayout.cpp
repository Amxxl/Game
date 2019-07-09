#include "pch.h"
#include "InputLayout.h"

namespace Bind
{
    InputLayout::InputLayout(DX::DeviceResources* deviceResources,
        std::vector<D3D11_INPUT_ELEMENT_DESC> const& layout,
        ID3DBlob* pVertexShaderByteCode)
    {
        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreateInputLayout(
                layout.data(), static_cast<UINT>(layout.size()),
                pVertexShaderByteCode->GetBufferPointer(),
                pVertexShaderByteCode->GetBufferSize(),
                &pInputLayout)
        );
    }

    void InputLayout::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->IASetInputLayout(pInputLayout.Get());
    }
}