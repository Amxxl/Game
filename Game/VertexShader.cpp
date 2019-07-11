#include "pch.h"
#include "VertexShader.h"

namespace Bind
{
    VertexShader::VertexShader(DX::DeviceResources* deviceResources, std::wstring const& path)
    {
        DX::ThrowIfFailed(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));

        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(),
                pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader)
        );
    }

    void VertexShader::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
    }
}