//
// Terrain.h
//

#pragma once

#include "VertexBuffer.h"
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
        };

        void Initialize(ID3D11DeviceContext* deviceContext);
        void SetMatrices(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);
        void Render(ID3D11DeviceContext* deviceContext);

    private:
        bool LoadRawHeightMap(char const* fileName);
        void SetTerrainCoordinates();

    private:
        std::unique_ptr<DirectX::CommonStates> states;
        int m_terrainWidth, m_terrainHeight;
        int m_vertexCount, m_indexCount;
        float m_heightScale;
        HeightMapType* m_heightMap;
        VertexBuffer<DirectX::VertexPositionColorTexture> vertexBuffer;
        IndexBuffer<DWORD> indexBuffer;
        TerrainShader shader;
};

