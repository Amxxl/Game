#include "pch.h"
#include "VertexShader.h"
#include "BindableCache.h"
#include "StringHelper.h"

namespace Bind
{
    VertexShader::VertexShader(DX::DeviceResources* deviceResources, std::string const& path)
        : path(path)
    {
        DX::ThrowIfFailed(D3DReadFileToBlob(StringHelper::NarrowToWide(path).c_str(), &pBytecodeBlob));

        DX::ThrowIfFailed(
            GetDevice(deviceResources)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(),
                pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader)
        );
    }

    void VertexShader::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
    }

    std::shared_ptr<VertexShader> VertexShader::Resolve(DX::DeviceResources* deviceResources, std::string const& path)
    {
        return BindableCache::Resolve<VertexShader>(deviceResources, path);
    }

    std::string VertexShader::GenerateUID(std::string const& path)
    {
        using namespace std::string_literals;
        return typeid(VertexShader).name() + "#"s + path;
    }

    std::string const& VertexShader::GetUID() const noexcept
    {
        return GenerateUID(path);
    }
}