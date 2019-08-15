#include "pch.h"
#include "InputLayout.h"
#include "BindableCache.h"

namespace Bind
{
    InputLayout::InputLayout(DX::DeviceResources* deviceResources,
        VertexLayout layout, ID3DBlob* pVertexShaderByteCode)
        : layout(std::move(layout))
    {
        auto const d3dLayout = this->layout.GetD3DLayout();

        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreateInputLayout(
                d3dLayout.data(), static_cast<UINT>(d3dLayout.size()),
                pVertexShaderByteCode->GetBufferPointer(),
                pVertexShaderByteCode->GetBufferSize(),
                &pInputLayout)
        );
    }

    void InputLayout::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->IASetInputLayout(pInputLayout.Get());
    }
    std::shared_ptr<InputLayout> InputLayout::Resolve(DX::DeviceResources* deviceResources, VertexLayout const& layout, ID3DBlob* pVertexShaderBytecode)
    {
        return BindableCache::Resolve<InputLayout>(deviceResources, layout, pVertexShaderBytecode);
    }
    std::string InputLayout::GenerateUID(VertexLayout const& layout, ID3DBlob* pVertexShaderBytecode)
    {
        using namespace std::string_literals;
        return typeid(InputLayout).name() + "#"s + layout.GetCode();
    }
    std::string const& InputLayout::GetUID() const noexcept
    {
        return GenerateUID(layout);
    }
}