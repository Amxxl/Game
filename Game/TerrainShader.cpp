//
// TerrainShader.cpp
//

#include "pch.h"
#include "TerrainShader.h"

namespace TerrainShaders
{
#include "TerrainVertexShader.shh"
#include "TerrainPixelShader.shh"
}

TerrainShader::TerrainShader()
{
}

TerrainShader::~TerrainShader()
{
}

void TerrainShader::InitializeShaders(ID3D11DeviceContext* deviceContext)
{
    // Helper to get device.
    ID3D11Device* device = DX::GetDevice(deviceContext);

    DX::ThrowIfFailed(device->CreateVertexShader(TerrainShaders::TerrainVertexShaderBytecode, sizeof(TerrainShaders::TerrainVertexShaderBytecode), nullptr, vertexShader.GetAddressOf()));
    DX::ThrowIfFailed(device->CreatePixelShader(TerrainShaders::TerrainPixelShaderBytecode, sizeof(TerrainShaders::TerrainPixelShaderBytecode), nullptr, pixelShader.GetAddressOf()));
    DX::ThrowIfFailed(device->CreateInputLayout(DirectX::VertexPositionNormalColorTexture::InputElements, DirectX::VertexPositionNormalColorTexture::InputElementCount, TerrainShaders::TerrainVertexShaderBytecode, sizeof(TerrainShaders::TerrainVertexShaderBytecode), inputLayout.GetAddressOf()));
    
    states = std::make_unique<DirectX::CommonStates>(device);
}

void TerrainShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
    // Helper to get device.
    ID3D11Device* device = DX::GetDevice(deviceContext);

    // Load texture
    DirectX::CreateWICTextureFromFile(device, L"terrain.png", nullptr, texture.ReleaseAndGetAddressOf());

    constantBuffer.Create(device);

    MatrixBufferType matrixBuffer;
    matrixBuffer.world = DirectX::XMMatrixTranspose(worldMatrix);
    matrixBuffer.view = DirectX::XMMatrixTranspose(viewMatrix);
    matrixBuffer.projection = DirectX::XMMatrixTranspose(projectionMatrix);
    constantBuffer.SetData(deviceContext, matrixBuffer);


    deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    lightBuffer.Create(device);

    LightBufferType light;
    light.ambientColor = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    light.diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    light.lightDirection = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    light.padding = 0.0f;
    lightBuffer.SetData(deviceContext, light);

    deviceContext->PSSetConstantBuffers(0, 1, lightBuffer.GetAddressOf());
    deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
}

void TerrainShader::RenderShader(ID3D11DeviceContext* deviceContext, int numIndices)
{
    deviceContext->IASetInputLayout(inputLayout.Get());
    deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

    ID3D11SamplerState* samplerState = states->LinearClamp();
    deviceContext->PSSetSamplers(0, 1, &samplerState);

    ID3D11RasterizerState* raster = states->CullCounterClockwise();
    deviceContext->RSSetState(raster);

    deviceContext->DrawIndexed(numIndices, 0, 0);
}
