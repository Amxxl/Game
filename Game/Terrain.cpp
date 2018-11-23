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
    this->SetTextureCoordinates();

    DirectX::VertexPositionColorTexture* vertices;
    unsigned long* indices;
    int i, j, index, index1, index2, index3, index4;
    float u, v;

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
            index1 = (m_terrainHeight * (j + 1)) + i;       // Upper left.
            index2 = (m_terrainHeight * (j + 1)) + (i + 1); // Upper right.
            index3 = (m_terrainHeight * j) + i;             // Bottom left.
            index4 = (m_terrainHeight * j) + (i + 1);       // Bottom right.

            // Upper left.
            u = m_heightMap[index1].u;
            v = m_heightMap[index1].v;

            // Modify the texture coordinates to cover the top edge.
            if (u == 1.0f) { u = 0.0f; }
            if (v == 1.0f) { v = 0.0f; }
            

            // Now create two triangles for that quad.
            // Triangle 1 - Upper left.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(u, v);
            indices[index] = index;
            index++;


            // Upper right.
            u = m_heightMap[index2].u;
            v = m_heightMap[index2].v;

            // Modify the texture coordinates to cover the top and right edge.
            if (u == 0.0f) { u = 1.0f; }
            if (v == 1.0f) { v = 0.0f; }

            // Triangle 1 - Upper right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(u, v);
            indices[index] = index;
            index++;


            // Bottom left.
            u = m_heightMap[index3].u;
            v = m_heightMap[index3].v;

            if (u == 1.0f) { u = 0.0f; }
            if (v == 0.0f) { v = 1.0f; }

            // Triangle 1 - Bottom left.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(u, v);
            indices[index] = index;
            index++;

            // Bottom left.
            u = m_heightMap[index3].u;
            v = m_heightMap[index3].v;

            if (u == 1.0f) { u = 0.0f; }
            if (v == 0.0f) { v = 1.0f; }

            // Triangle 2 - Bottom left.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(u, v);
            indices[index] = index;
            index++;

            // Upper right.
            u = m_heightMap[index2].u;
            v = m_heightMap[index2].v;

            // Modify the texture coordinates to cover the top and right edge.
            if (u == 0.0f) { u = 1.0f; }
            if (v == 1.0f) { v = 0.0f; }

            // Triangle 2 - Upper right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(u, v);
            indices[index] = index;
            index++;

            // Bottom right.
            u = m_heightMap[index4].u;
            v = m_heightMap[index4].v;

            // Modify the texture coordinates to cover the right edge.
            if (u == 0.0f) { u = 1.0f; }
            if (v == 0.0f) { v = 1.0f; }

            // Triangle 2 - Bottom right.
            vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
            vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertices[index].textureCoordinate = DirectX::XMFLOAT2(u, v);
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

    deviceContext->RSSetState(states->CullCounterClockwise());

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

void Terrain::SetTextureCoordinates()
{
    int incrementCount, i, j, uCount, vCount;
    float incrementValue, uCoordinate, vCoordinate;

    // Calculate how much to increment the texture coordinates by.
    incrementValue = static_cast<float>(TEXTURE_REPEAT) / static_cast<float>(m_terrainWidth);

    // Calculate how many times to repeat the texture.
    incrementCount = m_terrainWidth / TEXTURE_REPEAT;

    // Initialize the u and v coordinate values.
    uCoordinate = 0.0f;
    vCoordinate = 1.0f;

    // Initialize the u and v coordinate indexes.
    uCount = 0;
    vCount = 0;

    // Loop through the entire height map and calculate the u and v texture coordinates for each vertex.
    for (j = 0; j < m_terrainHeight; j++)
    {
        for (i = 0; i < m_terrainWidth; i++)
        {
            // Store the texture coordinate in the height map.
            m_heightMap[(m_terrainHeight * j) + i].u = uCoordinate;
            m_heightMap[(m_terrainHeight * j) + i].v = vCoordinate;

            // Increment the u texture coordinate by the increment value and increment the index by one.
            uCoordinate += incrementValue;
            uCount++;

            // Check if at the far right end of the texture and if so then start at the beginning again.
            if (uCount == incrementCount)
            {
                uCoordinate = 0.0f;
                uCount = 0;
            }
        }

        // Increment the v texture coordinate by the increment value and increment the index by one.
        vCoordinate -= incrementValue;
        vCount++;

        // Check if at the top of the texture and if so then start at the bottom again.
        if (vCount == incrementCount)
        {
            vCoordinate = 1.0f;
            vCount = 0;
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