#include "pch.h"
#include "PixelShader.h"
#include "BindableCache.h"
#include "StringHelper.h"

namespace Bind
{
    PixelShader::PixelShader(DX::DeviceResources* deviceResources, std::string const& path)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        DX::ThrowIfFailed(D3DReadFileToBlob(StringHelper::StringToWide(path).c_str(), &pBlob));

        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreatePixelShader(pBlob->GetBufferPointer(),
                pBlob->GetBufferSize(), nullptr, &pPixelShader)
        );
    }

    void PixelShader::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
    }

    std::shared_ptr<PixelShader> PixelShader::Resolve(DX::DeviceResources* deviceResources, std::string const& path)
    {
        return BindableCache::Resolve<PixelShader>(deviceResources, path);
    }

    std::string PixelShader::GenerateUID(std::string const& path)
    {
        using namespace std::string_literals;
        return typeid(PixelShader).name() + "#"s + path;
    }

    std::string const& PixelShader::GetUID() const noexcept
    {
        return GenerateUID(path);
    }
}