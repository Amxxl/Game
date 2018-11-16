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
    ID3D11Device* device = nullptr;
    deviceContext->GetDevice(&device);

    if (device == nullptr)
        return;

    DX::ThrowIfFailed(device->CreateVertexShader(TerrainShaders::TerrainVertexShaderBytecode, sizeof(TerrainShaders::TerrainVertexShaderBytecode), nullptr, vertexShader.GetAddressOf()));
    DX::ThrowIfFailed(device->CreatePixelShader(TerrainShaders::TerrainPixelShaderBytecode, sizeof(TerrainShaders::TerrainPixelShaderBytecode), nullptr, pixelShader.GetAddressOf()));
    DX::ThrowIfFailed(device->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount, TerrainShaders::TerrainVertexShaderBytecode, sizeof(TerrainShaders::TerrainVertexShaderBytecode), inputLayout.GetAddressOf()));
}

void TerrainShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
    // Helper to get device.
    ID3D11Device* device = nullptr;
    deviceContext->GetDevice(&device);

    if (device == nullptr)
        return;

    constantBuffer.Create(device);

    MatrixBufferType matrixBuffer;
    matrixBuffer.world = DirectX::XMMatrixTranspose(worldMatrix);
    matrixBuffer.view = DirectX::XMMatrixTranspose(viewMatrix);
    matrixBuffer.projection = DirectX::XMMatrixTranspose(projectionMatrix);
    constantBuffer.SetData(deviceContext, matrixBuffer);


    deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}

void TerrainShader::RenderShader(ID3D11DeviceContext* deviceContext, int numIndices)
{
    deviceContext->IASetInputLayout(inputLayout.Get());
    deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
    deviceContext->DrawIndexed(numIndices, 0, 0);
}
