#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<MD5Vertex>& vertices, std::vector<DWORD>& indices)
{
    this->deviceContext = deviceContext;

    vertexBuffer.Create(device, vertices.data(), static_cast<uint32>(vertices.size()));
    indexBuffer.Create(device, indices.data(), static_cast<uint32>(indices.size()));
}

Mesh::Mesh(Mesh const& mesh)
{
    this->deviceContext = mesh.deviceContext;
    this->indexBuffer = mesh.indexBuffer;
    this->vertexBuffer = mesh.vertexBuffer;
}

void Mesh::Draw()
{
    uint32 offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
    deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    //deviceContext->DrawIndexed(indexBuffer.IndexCount(), 0, 0);
}