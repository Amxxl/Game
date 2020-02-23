#include "pch.h"
#include "NullPixelShader.h"
#include "BindableCache.h"

namespace Bind
{
    NullPixelShader::NullPixelShader(DX::DeviceResources* deviceResources)
    {
    }
    void NullPixelShader::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->PSSetShader(nullptr, nullptr, 0u);
    }
    std::shared_ptr<NullPixelShader> NullPixelShader::Resolve(DX::DeviceResources* deviceResources)
    {
        return BindableCache::Resolve<NullPixelShader>(deviceResources);
    }
    std::string NullPixelShader::GenerateUID()
    {
        return typeid(NullPixelShader).name();
    }
    std::string const& NullPixelShader::GetUID() const noexcept
    {
        return GenerateUID();
    }
}