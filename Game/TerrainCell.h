#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferTypes.h"

class TerrainCell
{
    struct ModelType
    {
        float x, y, z;
        //float tu, tv;
        float nx, ny, nz;
        //float tx, ty, tz;
        //float bx, by, bz;
        //float r, g, b;
        //float tu2, tv2;
        float u, v;
        float u1, v1;
    };

    //struct VertexType
    //{
    //    DirectX::XMFLOAT3 position;
    //    DirectX::XMFLOAT3 normal;
    //    //DirectX::XMFLOAT3 tangent;
    //    //DirectX::XMFLOAT3 binormal;
    //    //DirectX::XMFLOAT3 color;
    //    DirectX::XMFLOAT2 texture;
    //    DirectX::XMFLOAT2 texture2;
    //};

    struct VectorType
    {
        float x, y, z;
    };

    struct ColorVertexType
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
    };

    public:
        TerrainCell();
        TerrainCell(TerrainCell const&) = default;
        ~TerrainCell();

        bool Initialize(ID3D11Device* device, void* terrainModelPtr, int nodeIndexX,
            int nodeIndexY, int cellWidth, int cellHeight, int terrainWidth);

        void Shutdown();

        void Draw(ID3D11DeviceContext* deviceContext);
        void DrawLineBuffers(ID3D11DeviceContext* deviceContext);

        int GetVertexCount();
        int GetIndexCount();
        int GetLineBuffersIndexCount();

        void GetCellDimensions(float& minWidth, float& minHeight, float& minDepth,
            float& maxWidth, float& maxHeight, float& maxDepth);

    private:
        bool InitializeBuffers(ID3D11Device* device, int nodeIndexX, int nodeIndexY,
            int cellWidth, int cellHeight, int terrainWidth, ModelType* terrainModel);

        void ShutdownBuffers();
        void RenderBuffers(ID3D11DeviceContext* deviceContext);
        void CalculateCellDimensions();
        bool BuildLineBuffers(ID3D11Device* device);
        void ShutdownLineBuffers();

    public:
        VectorType* pVertexList = nullptr;

    private:
        int m_iVertexCount, m_iIndexCount, m_iLineIndexCount;

        DX::VertexBuffer<DirectX::VertexPositionNormalColorDualTexture> pVertexBuffer;
        DX::IndexBuffer<unsigned int> pIndexBuffer;
        DX::VertexBuffer<ColorVertexType> pLineVertexBuffer;
        DX::IndexBuffer<unsigned int> pLineIndexBuffer;

        float m_fMinWidth, m_fMinHeight, m_fMinDepth;
        float m_fMaxWidth, m_fMaxHeight, m_fMaxDepth;
        float m_fPositionX, m_fPositionY, m_fPositionZ;
};