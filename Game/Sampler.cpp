#include "pch.h"
#include "Sampler.h"
#include "BindableCache.h"

namespace Bind
{
    Sampler::Sampler(DX::DeviceResources* deviceResources, State state)
        : state(state)
    {
        switch (state)
        {
            case State::POINT_WRAP:
                Create(deviceResources, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);
                break;
            case State::POINT_CLAMP:
                Create(deviceResources, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);
                break;
            case State::LINEAR_WRAP:
                Create(deviceResources, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
                break;
            case State::LINEAR_CLAMP:
                Create(deviceResources, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
                break;
            case State::ANISOTROPIC_WRAP:
                Create(deviceResources, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP);
                break;
            case State::ANISOTROPIC_CLAMP:
                Create(deviceResources, D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP);
                break;
        }
    }

    void Sampler::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
    }

    std::shared_ptr<Sampler> Sampler::Resolve(DX::DeviceResources* deviceResources, State state)
    {
        return BindableCache::Resolve<Sampler>(deviceResources, state);
    }

    std::string Sampler::GenerateUID(State state)
    {
        using namespace std::string_literals;
        return typeid(Sampler).name() + "#"s + std::to_string(static_cast<int>(state));
    }

    std::string const& Sampler::GetUID() const noexcept
    {
        return GenerateUID(state);
    }

    void Sampler::Create(DX::DeviceResources* deviceResources, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode)
    {
        D3D11_SAMPLER_DESC desc = {};

        desc.Filter = filter;
        desc.AddressU = addressMode;
        desc.AddressV = addressMode;
        desc.AddressW = addressMode;
        
        desc.MipLODBias = 0;

        desc.MaxAnisotropy = (deviceResources->GetDeviceFeatureLevel() > D3D10_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2u;
        
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

        desc.BorderColor[0] = 1.0f;
        desc.BorderColor[1] = 1.0f;
        desc.BorderColor[2] = 1.0f;
        desc.BorderColor[3] = 1.0f;

        desc.MinLOD = -FLT_MAX;
        desc.MaxLOD = FLT_MAX;

        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreateSamplerState(&desc, pSampler.GetAddressOf())
        );
    }
}