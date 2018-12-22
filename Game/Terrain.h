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
        void Draw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

    private:
        bool LoadRawHeightMap(char const* fileName);

        void SetTerrainCoordinates();
        void SetTextureCoordinates();
        void SetTextureCoordinates1();
        void CalculateNormals();

    private:
        static constexpr int const TEXTURE_REPEAT = 16;

        std::unique_ptr<DirectX::CommonStates> states;
        int m_terrainWidth, m_terrainHeight;
        int m_vertexCount, m_indexCount;
        float m_heightScale;
        HeightMapType* m_heightMap;
        VertexBuffer<DirectX::VertexPositionNormalColorDualTexture> vertexBuffer;
        IndexBuffer<DWORD> indexBuffer;
        TerrainShader shader;
};

