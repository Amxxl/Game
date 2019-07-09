#include "pch.h"
#include "Topology.h"

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
}