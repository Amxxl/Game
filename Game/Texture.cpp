//
// Texture.cpp
//

#include "pch.h"
#include "Texture.h"

using namespace axec;

Texture::Texture()
{
}


Texture::~Texture()
{
}

bool Texture::Create(ID3D11Device* device, uint16 width, uint16 height, Color const* color)
{
    CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

    D3D11_SUBRESOURCE_DATA initialData = { };
    initialData.pSysMem = color;
    initialData.SysMemSlicePitch = sizeof(Color);

    ID3D11Texture2D* texture2D = nullptr;
    DX::ThrowIfFailed(
        device->CreateTexture2D(&textureDesc, &initialData, &texture2D)
    );

    texture = static_cast<ID3D11Texture2D*>(texture2D);

    CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
    DX::ThrowIfFailed(
        device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf())
    );

    return false;
}
