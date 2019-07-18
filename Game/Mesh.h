#pragma once

#include "MD5Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "BindableCommon.h"
#include "DrawableBase.h"

class Mesh
{
    public:
        Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<MD5Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, DirectX::XMMATRIX const& transformMatrix);
        Mesh(Mesh const&) = default;

        void Draw();

        DirectX::XMMATRIX const& GetTransformMatrix() { return transformMatrix; }

        uint32 GetIndexCount() const { return indexBuffer.IndexCount(); }

    private:
        Bind::VertexBuffer<MD5Vertex> vertexBuffer;
        Bind::IndexBuffer<DWORD> indexBuffer;
        std::vector<Texture> textures;
        ID3D11DeviceContext* deviceContext;
        DirectX::XMMATRIX transformMatrix;
};

namespace expr
{
    class Mesh : public DrawableBase<Mesh>
    {
        public:
            Mesh(DX::DeviceResources* deviceResources, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs);
            void Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX accumulatedTransform) const;
            DirectX::XMMATRIX GetTransform() const noexcept override;
        private:
            mutable DirectX::XMFLOAT4X4 transform;
    };
}