#pragma once

#include "MD5Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class Mesh
{
    public:
        Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<MD5Vertex>& vertices, std::vector<DWORD>& indices);
        Mesh(Mesh const& mesh);

        void Draw();

        uint32 GetIndexCount() const { return indexBuffer.IndexCount(); }

    private:
        VertexBuffer<MD5Vertex> vertexBuffer;
        IndexBuffer<DWORD> indexBuffer;
        ID3D11DeviceContext* deviceContext;
};