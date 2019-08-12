//
// Octree.h
//

#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Terrain.h"
#include "TerrainShader.h"
#include "Frustum.h"

class Octree
{
    public:
        Octree();
        ~Octree();

        bool Initialize(Terrain* terrain, ID3D11DeviceContext* deviceContext);
        void Draw(ID3D11DeviceContext* deviceContext, Frustum* frustum, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
        void Shutdown();

        int GetDrawCount() const { return m_drawCount; }

    private:
        struct OctreeNode
        {
            float x, y, z, width;
            int triangleCount;
            DX::VertexBuffer<DirectX::VertexPositionNormalColorDualTexture> vertexBuffer;
            DX::IndexBuffer<unsigned long> indexBuffer;
            OctreeNode* nodes[8];
        };

    private:
        void CalculateMeshDimensions(int vertexCount, float& centerX, float& centerY, float& centerZ, float& meshWidth);
        void CreateTreeNode(OctreeNode* node, float positionX, float positionY, float positionZ, float width, ID3D11DeviceContext* deviceContext);
        int CountTriangles(float positionX, float positionY, float positionZ, float width);
        bool IsTriangleContained(int index, float positionX, float positionY, float positionZ, float width);
        void ReleaseNode(OctreeNode* node);
        void DrawNode(ID3D11DeviceContext* deviceContext, Frustum* frustum, OctreeNode* node, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);

    private:
        static constexpr int const MAX_TRIANGLES = 10000;

        int m_triangleCount, m_drawCount;
        DirectX::VertexPositionNormalColorDualTexture* m_vertexList;
        OctreeNode* m_pRootNode;
        TerrainShader shader;
};