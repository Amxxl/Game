#include "pch.h"
#include "Sprite.h"

Sprite::Sprite(DX::DeviceResources* deviceResources, std::string const& texturePath)
{
    using namespace Bind;
    dvt::VertexBuffer vbuf(
        dvt::VertexLayout{}
        << dvt::VertexLayout::Position3D
        << dvt::VertexLayout::Texture2D
    );

    vbuf.EmplaceBack(
        DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f),
        DirectX::XMFLOAT2(0.0f, 1.0f)
    );

    vbuf.EmplaceBack(
        DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),
        DirectX::XMFLOAT2(0.0f, 0.0f)
    );

    vbuf.EmplaceBack(
        DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f),
        DirectX::XMFLOAT2(1.0f, 0.0f)
    );

    vbuf.EmplaceBack(
        DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f),
        DirectX::XMFLOAT2(1.0f, 1.0f)
    );

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    AddBind(Topology::Resolve(deviceResources, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    AddBind(Texture::Resolve(deviceResources, texturePath));
    AddBind(Sampler::Resolve(deviceResources, Sampler::State::ANISOTROPIC_WRAP));

    AddBind(std::make_unique<VertexBuffer<dvt::VertexBuffer>>(deviceResources, vbuf));
    AddBind(std::make_unique<IndexBuffer<unsigned int>>(deviceResources, indices));

    auto pvs = VertexShader::Resolve(deviceResources, "Data/Shaders/color.vs");
    auto pvsbc = pvs->GetBytecode();

    AddBind(std::move(pvs));
    AddBind(InputLayout::Resolve(deviceResources, vbuf.GetLayout(), pvsbc));

    AddBind(PixelShader::Resolve(deviceResources, "Data/Shaders/color.ps"));
    
    AddBind(std::make_unique<Transform2D>(deviceResources, *this));
}

void Sprite::Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX accumulatedTransform) const
{
    DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
    Drawable::Draw(deviceResources);
}

DirectX::XMMATRIX Sprite::GetTransform() const noexcept
{
    return DirectX::XMLoadFloat4x4(&transform);
}
