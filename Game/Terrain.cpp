#include "pch.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
    if (m_heightMap != nullptr)
    {
        delete[] m_heightMap;
        m_heightMap = nullptr;
    }
}

void Terrain::Initialize(ID3D11DeviceContext * deviceContext)
{
    ID3D11Device* device = nullptr;
    deviceContext->GetDevice(&device);

    if (device == nullptr)
        return;

    states = std::make_unique<DirectX::CommonStates>(device);

    m_terrainWidth = 100;
    m_terrainHeight = 100;
    m_heightScale = 300.0f;

    m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];

    this->SetTerrainCoordinates();

    DirectX::VertexPositionColor* vertices;
    unsigned long* indices;
    int i, j, index, index1, index2, index3, index4;
    //float posX, posZ;

    // Calculate the number of vertices in the terrain mesh.
    m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;
    
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
            // Get the indexes to the four points of the quad.
            index1 = (m_terrainWidth * j) + i; // Upper left.
            index2 = (m_terrainWidth * j) + (i + 1); // Upper right.
            index3 = (m_terrainWidth * (j + 1)) + i; // Bottom left.
            index4 = (m_terrainWidth * (j + 1)) + (i + 1); // Bottom right.

            // Now create two triangles for that quad.
            // Triangle 1 - Upper left.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Triangle 1 - Upper right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Triangle 1 - Bottom left.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Triangle 2 - Bottom left.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Triangle 2 - Upper right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;

            // Triangle 2 - Bottom right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            indices[index] = index;
            index++;
        }
    }

    vertexBuffer.Create(device, vertices, m_vertexCount);
    indexBuffer.Create(device, indices, m_indexCount);

    delete[] vertices;
    vertices = 0;
    delete[] indices;
    indices = 0;

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
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    deviceContext->RSSetState(states->Wireframe());

    shader.RenderShader(deviceContext, m_indexCount);
}

void Terrain::SetTerrainCoordinates()
{
    int i, j, index;

    // Loop through all the elements in the height map array and adjust their coordinates correctly.
    for (j = 0; j < m_terrainHeight; j++)
    {
        for (i = 0; i < m_terrainWidth; i++)
        {
            index = (m_terrainWidth * j) + i;

            // Set the X and Z coordinates.
            m_heightMap[index].x = static_cast<float>(i);
            m_heightMap[index].y = 0.0f;
            m_heightMap[index].z = -static_cast<float>(j);

            // Move the terrain depth into the positive range. for example from (0, -256) to (256, 0).
            m_heightMap[index].z += static_cast<float>(m_terrainHeight - 1);

            // Scale the height.
            m_heightMap[index].y /= m_heightScale;
        }
    }
}
