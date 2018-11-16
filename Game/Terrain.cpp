#include "pch.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::Initialize(ID3D11DeviceContext * deviceContext)
{
    ID3D11Device* device = nullptr;
    deviceContext->GetDevice(&device);

    if (device == nullptr)
        return;

    m_terrainWidth = 100;
    m_terrainHeight = 100;

    DirectX::VertexPositionColor* vertices;
    unsigned long* indices;
    int index, i, j;
    float posX, posZ;

    // Calculate the number of vertices in the terrain mesh.
    m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 8;
    
    // Set index count to be same as vertex count.
    m_indexCount = m_vertexCount;

    // Create the vertex array.
    vertices = new DirectX::VertexPositionColor[m_vertexCount];
    indices = new unsigned long[m_indexCount];

    // Initialize the index to the vertex array.
    index = 0;

    for (j = 0; j < (m_terrainHeight - 1); j++)
    {
        for (i = 0; i < (m_terrainWidth - 1); i++)
        {
            // LINE 1
            // Upper left.
            posX = (float)i;
            posZ = (float)(j + 1);

            vertices[index].position = DirectX::XMFLOAT3(posX, 0.0f, posZ);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Upper right.
            posX = (float)(i + 1);
            posZ = (float)(j + 1);

            vertices[index].position = DirectX::XMFLOAT3(posX, 0.0f, posZ);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // LINE 2
            // Upper right.
            posX = (float)(i + 1);
            posZ = (float)(j + 1);

            vertices[index].position = DirectX::XMFLOAT3(posX, 0.0f, posZ);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Bottom right.
            posX = (float)(i + 1);
            posZ = (float)j;

            vertices[index].position = DirectX::XMFLOAT3(posX, 0.0f, posZ);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // LINE 3
            // Bottom right.
            posX = (float)(i + 1);
            posZ = (float)j;

            vertices[index].position = DirectX::XMFLOAT3(posX, 0.0f, posZ);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Bottom left.
            posX = (float)i;
            posZ = (float)j;

            vertices[index].position = DirectX::XMFLOAT3(posX, 0.0f, posZ);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // LINE 4
            // Bottom left.
            posX = (float)i;
            posZ = (float)j;

            vertices[index].position = DirectX::XMFLOAT3(posX, 0.0f, posZ);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Upper left.
            posX = (float)i;
            posZ = (float)(j + 1);

            vertices[index].position = DirectX::XMFLOAT3(posX, 0.0f, posZ);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;
        }
    }

    vertexBuffer.Create(device, vertices, m_vertexCount);
    indexBuffer.Create(device, indices, m_indexCount);

    //delete[] vertices;
    //vertices = 0;
    //delete[] indices;
    //indices = 0;

    shader.InitializeShaders(deviceContext);
}

void Terrain::SetMatrices(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
    shader.SetShaderParameters(deviceContext, world, view, projection);
}

void Terrain::Render(ID3D11DeviceContext * deviceContext)
{
    unsigned int offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
    deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    shader.RenderShader(deviceContext, m_indexCount);
}
