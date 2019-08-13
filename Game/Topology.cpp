#include "pch.h"
#include "Topology.h"
#include "BindableCache.h"

namespace Bind
{
    Topology::Topology(DX::DeviceResources* deviceResources, D3D11_PRIMITIVE_TOPOLOGY type)
        : topology(type)
    {
    }

    void Topology::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        GetContext(deviceResources)->IASetPrimitiveTopology(topology);
    }

    std::shared_ptr<Topology> Topology::Resolve(DX::DeviceResources* deviceResources, D3D11_PRIMITIVE_TOPOLOGY type)
    {
        return BindableCache::Resolve<Topology>(deviceResources, type);
    }

    std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type)
    {
        using namespace std::string_literals;
        return typeid(Topology).name() + "#"s + std::to_string(type);
    }

    std::string const& Topology::GetUID() const noexcept
    {
        return GenerateUID(topology);
    }
}