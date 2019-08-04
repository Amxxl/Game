#include "pch.h"
#include "Sampler.h"

namespace Bind
{
    Sampler::Sampler(DX::DeviceResources* deviceResources)
    {
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        GetDevice(deviceResources)->CreateSamplerState(&samplerDesc, &pSampler);
    }

    void Sampler::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
    }
}