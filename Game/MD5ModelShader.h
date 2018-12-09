//
// MD5ModelShader.h
//

#pragma once

#include "ConstantBuffer.h"
#include "CommonStates.h"

class MD5ModelShader
{
    public:
        MD5ModelShader();
        ~MD5ModelShader();

        struct MatrixBufferType
        {
            DirectX::XMMATRIX world;
            DirectX::XMMATRIX view;
            DirectX::XMMATRIX proj;
        };

        struct LightBufferType
        {
            DirectX::XMFLOAT4 ambientColor;
            DirectX::XMFLOAT4 diffuseColor;
            DirectX::XMFLOAT3 lightDirection;
            float padding;
        };

        void InitializeShaders(ID3D11DeviceContext* deviceContext);
        void SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
        void SetTexture(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
        void RenderShader(ID3D11DeviceContext* deviceContext, UINT numIndices);

    private:
        std::unique_ptr<DirectX::CommonStates> states;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
        ConstantBuffer<MatrixBufferType> constantBuffer;
        ConstantBuffer<LightBufferType> lightBuffer;
};

