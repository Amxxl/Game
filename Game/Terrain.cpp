#include "pch.h"
#include "Terrain.h"
#include <Windows.h>

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

    m_terrainWidth = 256;
    m_terrainHeight = 256;
    m_heightScale = 1200.0f;

    this->LoadRawHeightMap("terrain.raw");
    this->SetTerrainCoordinates();

    DirectX::VertexPositionColorTexture* vertices;
    unsigned long* indices;
    int i, j, index, index1, index2, index3, index4;

    // Calculate the number of vertices in the terrain mesh.
    m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;
    
    // Set index count to be same as vertex count.
    m_indexCount = m_vertexCount;

    // Create the vertex array.
    vertices = new DirectX::VertexPositionColorTexture[m_vertexCount];
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
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(0.0f, 0.0f);
            indices[index] = index;
            index++;

            // Triangle 1 - Upper right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(1.0f, 0.0f);
            indices[index] = index;
            index++;

            // Triangle 1 - Bottom left.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(0.0f, 1.0f);
            indices[index] = index;
            index++;

            // Triangle 2 - Bottom left.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(0.0f, 1.0f);
            indices[index] = index;
            index++;

            // Triangle 2 - Upper right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(1.0f, 0.0f);
            indices[index] = index;
            index++;

            // Triangle 2 - Bottom right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(1.0f, 1.0f);
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

void Terrain::Render(ID3D11DeviceContext* deviceContext)
{
    unsigned int offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
    deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    deviceContext->RSSetState(states->CullClockwise());

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
            m_heightMap[index].z = -static_cast<float>(j);

            // Move the terrain depth into the positive range. for example from (0, -256) to (256, 0).
            m_heightMap[index].z += static_cast<float>(m_terrainHeight - 1);

            // Scale the height.
            m_heightMap[index].y /= m_heightScale;
        }
    }
}

bool Terrain::LoadRawHeightMap(char const* fileName)
{
    int i, j, index;
    unsigned int imageSize;
    unsigned short* rawImage;

    // Calculate the size of the raw image data.
    imageSize = m_terrainWidth * m_terrainHeight;

    // Create the float array to hold the height map data.
    m_heightMap = new HeightMapType[imageSize];
    if (!m_heightMap)
        return false;

    // Open raw file to read.
    std::ifstream File(fileName, std::ios::binary);
    if (!File.is_open())
        return false;

    // Read raw data from file.
    rawImage = new unsigned short[imageSize];
    File.seekg(0, std::ios::beg);
    File.read((char*)rawImage, imageSize * sizeof(unsigned short));
    File.close();

    // Copy the image data into the height map array.
    for (j = 0; j < m_terrainHeight; j++)
    {
        for (i = 0; i < m_terrainWidth; i++)
        {
            index = (m_terrainWidth * j) + i;

            // Store the height at this point in the height map array.
            m_heightMap[index].y = static_cast<float>(rawImage[index]);
        }
    }

    // Release the bitmap image data.
    delete[] rawImage;
    rawImage = 0;

    return false;
}