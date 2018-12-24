//
// Terrain.h
//

#pragma once

#include "VertexBuffer.h"
#include "VertexBufferTypes.h"
#include "IndexBuffer.h"
#include "TerrainShader.h"
#include "CommonStates.h"

class Terrain
{
    public:
        Terrain();
        ~Terrain();

        struct HeightMapType
        {
            float x, y, z;
            float nx, ny, nz;
            float u, v;
            float u1, v1;
        };
        
        struct VectorType
        {
            float x, y, z;
        };

        void Initialize(ID3D11DeviceContext* deviceContext);
        void CopyVertexArray(void* vertexList);

        int GetVertexCount() const { return m_vertexCount; }

    private:
        bool LoadRawHeightMap(char const* fileName);

        void SetTerrainCoordinates();
        void SetTextureCoordinates();
        void SetTextureCoordinates1();
        void CalculateNormals();

    private:
        static constexpr int const TEXTURE_REPEAT = 16;

        int m_terrainWidth, m_terrainHeight;
        int m_vertexCount;
        float m_heightScale;
        HeightMapType* m_heightMap;
        DirectX::VertexPositionNormalColorDualTexture* m_vertices;
};