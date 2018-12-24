//
// MD5ModelShader.cpp
//

#include "pch.h"
#include "MD5ModelShader.h"
#include "MD5Vertex.h"

namespace MD5ModelShaders
{
#include "MD5ModelVertexShader.shh"
#include "MD5ModelPixelShader.shh"
}

MD5ModelShader::MD5ModelShader()
{
}


MD5ModelShader::~MD5ModelShader()
{
}

void MD5ModelShader::InitializeShaders(ID3D11DeviceContext* deviceContext)
{
    ID3D11Device* device = DX::GetDevice(deviceContext);
    
    if (device == nullptr)
        return;

    DX::ThrowIfFailed(device->CreateVertexShader(MD5ModelShaders::MD5ModelVertexShaderBytecode, sizeof(MD5ModelShaders::MD5ModelVertexShaderBytecode), nullptr, vertexShader.GetAddressOf()));
    DX::ThrowIfFailed(device->CreatePixelShader(MD5ModelShaders::MD5ModelPixelShaderBytecode, sizeof(MD5ModelShaders::MD5ModelPixelShaderBytecode), nullptr, pixelShader.GetAddressOf()));
    DX::ThrowIfFailed(device->CreateInputLayout(MD5Vertex::InputElements, MD5Vertex::InputElementCount, MD5ModelShaders::MD5ModelVertexShaderBytecode, sizeof(MD5ModelShaders::MD5ModelVertexShaderBytecode), inputLayout.GetAddressOf()));

    states = std::make_unique<DirectX::CommonStates>(device);

    constantBuffer.Create(device);
    lightBuffer.Create(device);
}

void MD5ModelShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
    ID3D11Device* device = DX::GetDevice(deviceContext);
    if (device == nullptr)
        return;

    MatrixBufferType matrixBuffer;
    matrixBuffer.world = DirectX::XMMatrixTranspose(world);
    matrixBuffer.view = DirectX::XMMatrixTranspose(view);
    matrixBuffer.proj = DirectX::XMMatrixTranspose(proj);
    constantBuffer.SetData(deviceContext, matrixBuffer);

    deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    LightBufferType light;
    light.ambientColor = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    light.diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    light.lightDirection = DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f);
    light.padding = 0.0f;
    lightBuffer.SetData(deviceContext, light);

    deviceContext->PSSetConstantBuffers(0, 1, lightBuffer.GetAddressOf());
}

void MD5ModelShader::SetTexture(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture)
{
    ID3D11Device* device = DX::GetDevice(deviceContext);
    if (device == nullptr)
        return;

    deviceContext->PSSetShaderResources(0, 1, &texture);
}

void MD5ModelShader::RenderShader(ID3D11DeviceContext* deviceContext, UINT numIndices)
{
    deviceContext->IASetInputLayout(inputLayout.Get());
    deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
    
    ID3D11RasterizerState* cullNone = states->CullNone();
    deviceContext->RSSetState(cullNone);

    ID3D11SamplerState* sampler = states->LinearClamp();
    deviceContext->PSSetSamplers(0, 1, &sampler);

    deviceContext->DrawIndexed(numIndices, 0, 0);
}
