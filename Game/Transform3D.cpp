#include "pch.h"
#include "Transform3D.h"

namespace Bind
{
    Transform3D::Transform3D(DX::DeviceResources* deviceResources, Drawable const& parent, UINT slot)
        : parent(parent)
    {
        if (!pVertexConstantBuffer)
            pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(deviceResources, slot);
    }

    void Transform3D::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        Transforms const tf = {
            DirectX::XMMatrixTranspose(parent.GetTransform() * DirectX::XMMatrixIdentity()),
            DirectX::XMMatrixTranspose(deviceResources->GetCamera()->GetViewMatrix()),
            DirectX::XMMatrixTranspose(deviceResources->GetCamera()->GetProjectionMatrix())
        };

        pVertexConstantBuffer->Update(deviceResources, tf);
        pVertexConstantBuffer->Bind(deviceResources);
    }

    std::unique_ptr<VertexConstantBuffer<Transform3D::Transforms>> Transform3D::pVertexConstantBuffer;
}