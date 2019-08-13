#include "pch.h"
#include "Mesh.h"
#include "BindableCache.h"

Mesh::Mesh(DX::DeviceResources* deviceResources, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
{
    AddBind(Bind::Topology::Resolve(deviceResources, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    for (auto& pb : bindPtrs)
        AddBind(std::move(pb));

    AddBind(std::make_shared<Bind::TransformCBuf>(deviceResources, *this));
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