//
// QuadTree.h
//

#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Terrain.h"
#include "TerrainShader.h"
#include "FrustumCuller.h"

class QuadTree
{
    public:
        QuadTree();
        ~QuadTree();

        bool Initialize(Terrain* terrain, ID3D11DeviceContext* deviceContext);
        void Draw(ID3D11DeviceContext* deviceContext, Frustum* frustum, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
        void Shutdown();

        int GetDrawCount() const { return m_drawCount; }
        
    private:
        struct NodeType
        {
            float positionX, positionZ, width;
            int triangleCount;
            VertexBuffer<DirectX::VertexPositionNormalColorDualTexture> vertexBuffer;
            IndexBuffer<unsigned long> indexBuffer;
            NodeType* nodes[4];
        };

    private:
        void CalculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth);
        void CreateTreeNode(NodeType* node, float positionX, float positionZ, float width, ID3D11DeviceContext* deviceContext);
        int CountTriangles(float positionX, float positionZ, float width);
        bool IsTriangleContained(int index, float positionX, float positionZ, float width);
        void ReleaseNode(NodeType* node);
        void DrawNode(ID3D11DeviceContext* deviceContext, Frustum* frustum, NodeType* node, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
        
    private:
        static constexpr int const MAX_TRIANGLES = 10000;

        int m_triangleCount, m_drawCount;
        DirectX::VertexPositionNormalColorDualTexture* m_vertexList;
        NodeType* m_parentNode;
        TerrainShader shader;
};