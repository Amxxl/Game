//
// MD5Vertex.h
//

#pragma once

struct MD5Vertex
{
    MD5Vertex() = default;

    MD5Vertex(MD5Vertex const&) = default;
    MD5Vertex& operator=(MD5Vertex const&) = default;

    MD5Vertex(MD5Vertex&&) = default;
    MD5Vertex& operator=(MD5Vertex&&) = default;

    MD5Vertex(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT2 const& textureCoordinate)
        : position(position),
        normal(normal),
        textureCoordinate(textureCoordinate)
    {
    }

    MD5Vertex(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR textureCoordinate)
    {
        DirectX::XMStoreFloat3(&this->position, position);
        DirectX::XMStoreFloat3(&this->normal, normal);
        DirectX::XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
    }

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 textureCoordinate;

    // Won't be send to the shader.
    int StartWeight = 0;
    int WeightCount = 0;

    static const int InputElementCount = 3;
    static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};