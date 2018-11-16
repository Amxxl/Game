//
// Terrain.h
//

#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "TerrainShader.h"

class Terrain
{
    public:
        Terrain();
        ~Terrain();

        void Initialize(ID3D11DeviceContext* deviceContext);
        void SetMatrices(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);
        void Render(ID3D11DeviceContext* deviceContext);

    private:
        int m_terrainWidth, m_terrainHeight;
        int m_vertexCount, m_indexCount;
        VertexBuffer<DirectX::VertexPositionColor> vertexBuffer;
        IndexBuffer<DWORD> indexBuffer;
        TerrainShader shader;
};

