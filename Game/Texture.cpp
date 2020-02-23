//
// Texture.cpp
//

#include "pch.h"
#include "Texture.h"
#include "StringHelper.h"
#include "BindableCache.h"

namespace Bind
{
    Texture::Texture(DX::DeviceResources* deviceResources, std::wstring const& path, unsigned int slot)
        : path(StringHelper::WideToNarrow(path))
        , slot(slot)
    {
        if (StringHelper::GetFileExtension(path) == L".dds")
        {
            DX::ThrowIfFailed(
                DirectX::CreateDDSTextureFromFile(GetDevice(deviceResources),
                    path.c_str(), nullptr, pTextureView.GetAddressOf())
            );
        }
        else if (StringHelper::GetFileExtension(path) == L".tga")
        {
            assert("[.TGA] extension not supported yet." && false);
        }
        else
        {
            DX::ThrowIfFailed(
                DirectX::CreateWICTextureFromFile(GetDevice(deviceResources),
                    path.c_str(), nullptr, pTextureView.GetAddressOf())
            );
        }

    }

    Texture::Texture(DX::DeviceResources* deviceResources, std::string const& path, unsigned int slot)
        : Texture(deviceResources, StringHelper::NarrowToWide(path), slot)
    {

    }

    Texture::Texture(DX::DeviceResources* deviceResources, uint8 const* data, size_t size, unsigned int slot)
        : path("")
        , slot(slot)
    {
        DX::ThrowIfFailed(
            DirectX::CreateWICTextureFromMemory(GetDevice(deviceResources), data, size,
                pTexture.GetAddressOf(), pTextureView.GetAddressOf())
        );
    }

    Texture::Texture(DX::DeviceResources* deviceResources, uint16 width, uint16 height, Color const* color, unsigned int slot)
        : path("")
        , slot(slot)
    {
        CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

        D3D11_SUBRESOURCE_DATA initialData = {};
        initialData.pSysMem = color;
        initialData.SysMemPitch = sizeof(Color);

        ID3D11Texture2D* pTexture2D = nullptr;
        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreateTexture2D(&textureDesc, &initialData, &pTexture2D)
        );

        pTexture = static_cast<ID3D11Texture2D*>(pTexture2D);

        CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreateShaderResourceView(pTexture.Get(), &srvDesc, pTextureView.GetAddressOf())
        );
    }

    Texture::Texture(DX::DeviceResources* deviceResources, Color color, unsigned int slot)
        : Texture(deviceResources, 1, 1, &color, slot)
    {
    }

    void Texture::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
    }

    std::shared_ptr<Texture> Texture::Resolve(DX::DeviceResources* deviceResources, std::string const& path, unsigned int slot)
    {
        return BindableCache::Resolve<Texture>(deviceResources, path, slot);
    }

    std::string Texture::GenerateUID(std::string const& path, UINT slot)
    {
        using namespace std::string_literals;
        return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
    }

    std::string const& Texture::GetUID() const noexcept
    {
        return GenerateUID(path, slot);
    }
}