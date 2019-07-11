#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<MD5Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, DirectX::XMMATRIX const& transformMatrix)
{
    this->deviceContext = deviceContext;
    this->textures = textures;
    this->transformMatrix = transformMatrix;

    vertexBuffer.Create(device, vertices.data(), static_cast<uint32>(vertices.size()));
    indexBuffer.Create(device, indices.data(), static_cast<uint32>(indices.size()));
}

void Mesh::Draw()
{
    uint32 offset = 0;

    for (uint32 i = 0; i < textures.size(); ++i)
    {
        if (textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
        {
            deviceContext->PSSetShaderResources(0, 1, textures[i].GetAddressOf());
            break;
        }
    }

    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
    deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

expr::Mesh::Mesh(DX::DeviceResources* deviceResources, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs)
{
    if (!IsStaticInitialized())
    {
        AddStaticBind(std::make_unique<Bind::Topology>(deviceResources, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }

    for (auto& pb : bindPtrs)
    {
        if (auto pi = dynamic_cast<Bind::IndexBuffer<unsigned int>*>(pb.get()))
        {
            AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer<unsigned int>>{ pi });
            pb.release();
        }
        else
            AddBind(std::move(pb));
    }

    AddBind(std::make_unique<Bind::TransformCBuf>(deviceResources, *this));
}

void expr::Mesh::Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX accumulatedTransform) const
{
    DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
    Drawable::Draw(deviceResources);
}

DirectX::XMMATRIX expr::Mesh::GetTransform() const noexcept
{
    return DirectX::XMLoadFloat4x4(&transform);
}
