//
// Texture.cpp
//

#include "pch.h"
#include "Texture.h"
#include "StringHelper.h"


Texture::Texture(ID3D11Device* device, Color const& color, aiTextureType type)
{
    Create1x1ColorTexture(device, color, type);
}

Texture::Texture(ID3D11Device* device, uint16 width, uint16 height, Color const* color, aiTextureType type)
{
    CreateColorTexture(device, width, height, color, type);
}

Texture::Texture(ID3D11Device* device, std::string const& filePath, aiTextureType type)
{
    this->type = type;
    if (StringHelper::GetFileExtension(filePath) == ".dds")
    {
        HRESULT hr = DirectX::CreateDDSTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
        
        if (FAILED(hr))
        {
            Create1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
        }
    }
    else if (StringHelper::GetFileExtension(filePath) == ".tga")
    {
        // @todo: Load targa textures here.
    }
    else
    {
        HRESULT hr = DirectX::CreateWICTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
        
        if (FAILED(hr))
        {
            Create1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
        }
    }
}

Texture::Texture(ID3D11Device* device, uint8 const* pData, size_t size, aiTextureType type)
{
    this->type = type;

    DX::ThrowIfFailed(
        DirectX::CreateWICTextureFromMemory(device, pData, size, texture.GetAddressOf(), textureView.GetAddressOf())
    );
}

void Texture::Create1x1ColorTexture(ID3D11Device* device, Color const& color, aiTextureType type)
{
    CreateColorTexture(device, 1, 1, &color, type);
}

void Texture::CreateColorTexture(ID3D11Device* device, uint16 width, uint16 height, Color const* color, aiTextureType type)
{
    this->type = type;

    CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

    D3D11_SUBRESOURCE_DATA initialData = { };
    initialData.pSysMem = color;
    initialData.SysMemPitch = sizeof(Color);

    ID3D11Texture2D* texture2D = nullptr;
    DX::ThrowIfFailed(
        device->CreateTexture2D(&textureDesc, &initialData, &texture2D)
    );

    texture = static_cast<ID3D11Texture2D*>(texture2D);

    CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
    DX::ThrowIfFailed(
        device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf())
    );
}