//
// VertexBufferTypes.h
//

#pragma once

namespace DirectX
{
    // Vertex struct holding position, normal vector, color, and dual texture mapping information.
    struct VertexPositionNormalColorDualTexture
    {
        VertexPositionNormalColorDualTexture() = default;

        VertexPositionNormalColorDualTexture(VertexPositionNormalColorDualTexture const&) = default;
        VertexPositionNormalColorDualTexture& operator=(VertexPositionNormalColorDualTexture const&) = default;

        VertexPositionNormalColorDualTexture(VertexPositionNormalColorDualTexture&&) = default;
        VertexPositionNormalColorDualTexture& operator=(VertexPositionNormalColorDualTexture&&) = default;

        VertexPositionNormalColorDualTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate0, XMFLOAT2 const& textureCoordinate1)
            : position(position),
            normal(normal),
            color(color),
            textureCoordinate0(textureCoordinate0),
            textureCoordinate1(textureCoordinate1)
        { }

        VertexPositionNormalColorDualTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR color, FXMVECTOR textureCoordinate0, FXMVECTOR textureCoordinate1)
        {
            XMStoreFloat3(&this->position, position);
            XMStoreFloat3(&this->normal, normal);
            XMStoreFloat4(&this->color, color);
            XMStoreFloat2(&this->textureCoordinate0, textureCoordinate0);
            XMStoreFloat2(&this->textureCoordinate1, textureCoordinate1);
        }

        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT4 color;
        XMFLOAT2 textureCoordinate0;
        XMFLOAT2 textureCoordinate1;

        static const int InputElementCount = 5;
        static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
    };
}