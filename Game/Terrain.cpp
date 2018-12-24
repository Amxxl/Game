#include "pch.h"
#include "Terrain.h"
#include <Windows.h>

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
    if (m_vertices != nullptr)
    {
        delete[] m_vertices;
        m_vertices = nullptr;
    }
    if (m_heightMap != nullptr)
    {
        delete[] m_heightMap;
        m_heightMap = nullptr;
    }
}

void Terrain::Initialize(ID3D11DeviceContext* deviceContext)
{
    DX::ExecutionTimer execTime;

    m_terrainWidth = 512;
    m_terrainHeight = 512;
    m_heightScale = 1200.0f;

    this->LoadRawHeightMap("terrain.raw");
    this->SetTerrainCoordinates();
    this->SetTextureCoordinates();
    this->SetTextureCoordinates1();
    this->CalculateNormals();

    int index, index1, index2, index3, index4;
    float u, v;

    // Calculate the number of vertices in the terrain mesh.
    m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

    // Create the vertex array.
    m_vertices = new DirectX::VertexPositionNormalColorDualTexture[m_vertexCount];

    // Initialize the index to the vertex array.
    index = 0;

    for (int j = 0; j < (m_terrainHeight - 1); ++j)
    {
        for (int i = 0; i < (m_terrainWidth - 1); ++i)
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
            m_vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index1].x, m_heightMap[index1].y, m_heightMap[index1].z);
            m_vertices[index].normal = DirectX::XMFLOAT3(m_heightMap[index1].nx, m_heightMap[index1].ny, m_heightMap[index1].nz);
            m_vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            m_vertices[index].textureCoordinate0 = DirectX::XMFLOAT2(u, v);
            m_vertices[index].textureCoordinate1 = DirectX::XMFLOAT2(m_heightMap[index1].u1, m_heightMap[index1].v1);
            index++;


            // Upper right.
            u = m_heightMap[index2].u;
            v = m_heightMap[index2].v;

            // Modify the texture coordinates to cover the top and right edge.
            if (u == 0.0f) { u = 1.0f; }
            if (v == 1.0f) { v = 0.0f; }

            // Triangle 1 - Upper right.
            m_vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
            m_vertices[index].normal = DirectX::XMFLOAT3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
            m_vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            m_vertices[index].textureCoordinate0 = DirectX::XMFLOAT2(u, v);
            m_vertices[index].textureCoordinate1 = DirectX::XMFLOAT2(m_heightMap[index2].u1, m_heightMap[index2].v1);
            index++;


            // Bottom left.
            u = m_heightMap[index3].u;
            v = m_heightMap[index3].v;

            if (u == 1.0f) { u = 0.0f; }
            if (v == 0.0f) { v = 1.0f; }

            // Triangle 1 - Bottom left.
            m_vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
            m_vertices[index].normal = DirectX::XMFLOAT3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
            m_vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            m_vertices[index].textureCoordinate0 = DirectX::XMFLOAT2(u, v);
            m_vertices[index].textureCoordinate1 = DirectX::XMFLOAT2(m_heightMap[index3].u1, m_heightMap[index3].v1);
            index++;

            // Bottom left.
            u = m_heightMap[index3].u;
            v = m_heightMap[index3].v;

            if (u == 1.0f) { u = 0.0f; }
            if (v == 0.0f) { v = 1.0f; }

            // Triangle 2 - Bottom left.
            m_vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index3].x, m_heightMap[index3].y, m_heightMap[index3].z);
            m_vertices[index].normal = DirectX::XMFLOAT3(m_heightMap[index3].nx, m_heightMap[index3].ny, m_heightMap[index3].nz);
            m_vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            m_vertices[index].textureCoordinate0 = DirectX::XMFLOAT2(u, v);
            m_vertices[index].textureCoordinate1 = DirectX::XMFLOAT2(m_heightMap[index3].u1, m_heightMap[index3].v1);
            index++;

            // Upper right.
            u = m_heightMap[index2].u;
            v = m_heightMap[index2].v;

            // Modify the texture coordinates to cover the top and right edge.
            if (u == 0.0f) { u = 1.0f; }
            if (v == 1.0f) { v = 0.0f; }

            // Triangle 2 - Upper right.
            m_vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index2].x, m_heightMap[index2].y, m_heightMap[index2].z);
            m_vertices[index].normal = DirectX::XMFLOAT3(m_heightMap[index2].nx, m_heightMap[index2].ny, m_heightMap[index2].nz);
            m_vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            m_vertices[index].textureCoordinate0 = DirectX::XMFLOAT2(u, v);
            m_vertices[index].textureCoordinate1 = DirectX::XMFLOAT2(m_heightMap[index2].u1, m_heightMap[index2].v1);
            index++;

            // Bottom right.
            u = m_heightMap[index4].u;
            v = m_heightMap[index4].v;

            // Modify the texture coordinates to cover the right edge.
            if (u == 0.0f) { u = 1.0f; }
            if (v == 0.0f) { v = 1.0f; }

            // Triangle 2 - Bottom right.
            m_vertices[index].position = DirectX::XMFLOAT3(m_heightMap[index4].x, m_heightMap[index4].y, m_heightMap[index4].z);
            m_vertices[index].normal = DirectX::XMFLOAT3(m_heightMap[index4].nx, m_heightMap[index4].ny, m_heightMap[index4].nz);
            m_vertices[index].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            m_vertices[index].textureCoordinate0 = DirectX::XMFLOAT2(u, v);
            m_vertices[index].textureCoordinate1 = DirectX::XMFLOAT2(m_heightMap[index4].u1, m_heightMap[index4].v1);
            index++;
        }
    }
}

void Terrain::CopyVertexArray(void* vertexList)
{
    memcpy(vertexList, m_vertices, sizeof(DirectX::VertexPositionNormalColorDualTexture) * m_vertexCount);
}

void Terrain::SetTerrainCoordinates()
{
    int i, j, index;

    // Loop through all the elements in the height map array and adjust their coordinates correctly.
    for (j = 0; j < m_terrainHeight; ++j)
    {
        for (i = 0; i < m_terrainWidth; ++i)
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
    for (j = 0; j < m_terrainHeight; ++j)
    {
        for (i = 0; i < m_terrainWidth; ++i)
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

void Terrain::SetTextureCoordinates1()
{
    int index1, index2, index3, index4;

    for (int j = 0; j < m_terrainHeight - 1; ++j)
    {
        for (int i = 0; i < m_terrainWidth - 1; ++i)
        {
            // Get the indexes to the four points of the quad.
            index1 = (m_terrainHeight * (j + 1)) + i;       // Upper left.
            index2 = (m_terrainHeight * (j + 1)) + (i + 1); // Upper right.
            index3 = (m_terrainHeight * j) + i;             // Bottom left.
            index4 = (m_terrainHeight * j) + (i + 1);       // Bottom right.

            // Store the texture coordinate in the height map.
            m_heightMap[index1].u1 = 1.0f / m_terrainWidth * i;
            m_heightMap[index1].v1 = 1.0f / m_terrainHeight * j;

            m_heightMap[index2].u1 = 1.0f / m_terrainWidth * i;
            m_heightMap[index2].v1 = 1.0f / m_terrainHeight * j;

            m_heightMap[index3].u1 = 1.0f / m_terrainWidth * i;
            m_heightMap[index3].v1 = 1.0f / m_terrainHeight * j;

            m_heightMap[index4].u1 = 1.0f / m_terrainWidth * i;
            m_heightMap[index4].v1 = 1.0f / m_terrainHeight * j;

        }
    }

}

void Terrain::CalculateNormals()
{
    int index1, index2, index3, index, count;
    float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
    VectorType* normals;

    // Create a temporary array to hold the un-normalized normal vectors.
    normals = new VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];

    // Go through all the faces in the mesh and calculate their normals.
    for (int j = 0; j < (m_terrainHeight - 1); ++j)
    {
        for (int i = 0; i < (m_terrainWidth - 1); ++i)
        {
            index1 = (j * m_terrainHeight) + i;
            index2 = (j * m_terrainHeight) + (i + 1);
            index3 = ((j + 1) * m_terrainHeight) + i;

            // Get three vertices from the face.
            vertex1[0] = m_heightMap[index1].x;
            vertex1[1] = m_heightMap[index1].y;
            vertex1[2] = m_heightMap[index1].z;

            vertex2[0] = m_heightMap[index2].x;
            vertex2[1] = m_heightMap[index2].y;
            vertex2[2] = m_heightMap[index2].z;

            vertex3[0] = m_heightMap[index3].x;
            vertex3[1] = m_heightMap[index3].y;
            vertex3[2] = m_heightMap[index3].z;

            // Calculate the two vectors for this face.
            vector1[0] = vertex1[0] - vertex3[0];
            vector1[1] = vertex1[1] - vertex3[1];
            vector1[2] = vertex1[2] - vertex3[2];
            vector2[0] = vertex3[0] - vertex2[0];
            vector2[1] = vertex3[1] - vertex2[1];
            vector2[2] = vertex3[2] - vertex2[2];

            index = (j * (m_terrainHeight - 1)) + i;

            // Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
            normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
            normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
            normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
        }
    }

    // Now go through all the vertices and take an average of each face normal 	
    // that the vertex touches to get the averaged normal for that vertex.
    for (int j = 0; j < m_terrainHeight; ++j)
    {
        for (int i = 0; i < m_terrainWidth; ++i)
        {
            // Initialize the sum.
            sum[0] = 0.0f;
            sum[1] = 0.0f;
            sum[2] = 0.0f;

            // Initialize the count.
            count = 0;

            // Bottom left face.
            if (((i - 1) >= 0) && ((j - 1) >= 0))
            {
                index = ((j - 1) * (m_terrainHeight - 1)) + (i - 1);

                sum[0] += normals[index].x;
                sum[1] += normals[index].y;
                sum[2] += normals[index].z;
                count++;
            }

            // Bottom right face.
            if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
            {
                index = ((j - 1) * (m_terrainHeight - 1)) + i;

                sum[0] += normals[index].x;
                sum[1] += normals[index].y;
                sum[2] += normals[index].z;
                count++;
            }

            // Upper left face.
            if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
            {
                index = (j * (m_terrainHeight - 1)) + (i - 1);

                sum[0] += normals[index].x;
                sum[1] += normals[index].y;
                sum[2] += normals[index].z;
                count++;
            }

            // Upper right face.
            if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
            {
                index = (j * (m_terrainHeight - 1)) + i;

                sum[0] += normals[index].x;
                sum[1] += normals[index].y;
                sum[2] += normals[index].z;
                count++;
            }

            // Take the average of the faces touching this vertex.
            sum[0] = (sum[0] / static_cast<float>(count));
            sum[1] = (sum[1] / static_cast<float>(count));
            sum[2] = (sum[2] / static_cast<float>(count));

            // Calculate the length of this normal.
            length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

            // Get an index to the vertex location in the height map array.
            index = (j * m_terrainHeight) + i;

            // Normalize the final shared normal for this vertex and store it in the height map array.
            m_heightMap[index].nx = (sum[0] / length);
            m_heightMap[index].ny = (sum[1] / length);
            m_heightMap[index].nz = (sum[2] / length);
        }
    }

    // Release the temporary normals.
    delete[] normals;
    normals = nullptr;
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
    rawImage = nullptr;

    return false;
}