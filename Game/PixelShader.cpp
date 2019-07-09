#include "pch.h"
#include "PixelShader.h"

namespace Bind
{
    PixelShader::PixelShader(DX::DeviceResources* deviceResources, std::wstring const& path)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBlob));

        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreatePixelShader(pBlob->GetBufferPointer(),
                pBlob->GetBufferSize(), nullptr, &pPixelShader)
        );
    }

    void PixelShader::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
    }
}