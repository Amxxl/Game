//
// TerrainShader.h
//

#pragma once

#include "ConstantBuffer.h"

class TerrainShader
{
    public:
        struct MatrixBufferType
        {
            DirectX::XMMATRIX world;
            DirectX::XMMATRIX view;
            DirectX::XMMATRIX projection;
        };

        struct LightBufferType
        {
            DirectX::XMFLOAT4 ambientColor;
            DirectX::XMFLOAT4 diffuseColor;
            DirectX::XMFLOAT3 lightDirection;
            float padding;
        };

        TerrainShader();
        ~TerrainShader();

        void InitializeShaders(ID3D11DeviceContext* deviceContext);
        void SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
        void RenderShader(ID3D11DeviceContext* deviceContext, int numIndices);

    private:
        std::unique_ptr<DirectX::CommonStates> states;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture0;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture1;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture2;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture3;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture4;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
        DX::ConstantBuffer<MatrixBufferType> constantBuffer;
        DX::ConstantBuffer<LightBufferType> lightBuffer;
};