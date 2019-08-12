#include "pch.h"
#include "Mesh.h"

namespace expr
{
    Mesh::Mesh(DX::DeviceResources* deviceResources, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
    {
        AddBind(std::make_unique<Bind::Topology>(deviceResources, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

        for (auto& pb : bindPtrs)
            AddBind(std::move(pb));

        AddBind(std::make_unique<Bind::TransformCBuf>(deviceResources, *this));
    }

    void Mesh::Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX accumulatedTransform) const
    {
        DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
        Drawable::Draw(deviceResources);
    }

    DirectX::XMMATRIX Mesh::GetTransform() const noexcept
    {
        return DirectX::XMLoadFloat4x4(&transform);
    }
}