#pragma once

#include "MD5Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class Mesh
{
    public:
        Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<MD5Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, DirectX::XMMATRIX const& transformMatrix);
        Mesh(Mesh const&) = default;

        void Draw();

        DirectX::XMMATRIX const& GetTransformMatrix() { return transformMatrix; }

        uint32 GetIndexCount() const { return indexBuffer.IndexCount(); }

    private:
        VertexBuffer<MD5Vertex> vertexBuffer;
        IndexBuffer<DWORD> indexBuffer;
        std::vector<Texture> textures;
        ID3D11DeviceContext* deviceContext;
        DirectX::XMMATRIX transformMatrix;
};