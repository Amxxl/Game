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