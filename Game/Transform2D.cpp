#include "pch.h"
#include "Transform2D.h"

namespace Bind
{
    Transform2D::Transform2D(DX::DeviceResources* deviceResources, Drawable const& owner, UINT slot)
        : owner(owner)
    {
        if (!pVertexConstantBuffer)
            pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(deviceResources, slot);
    }

    void Transform2D::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        Transforms const tf = {
            DirectX::XMMatrixTranspose(owner.GetTransform() *
            deviceResources->GetCamera2D()->GetWorldMatrix() *
            deviceResources->GetCamera2D()->GetOrthoMatrix())
        };

        pVertexConstantBuffer->Update(deviceResources, tf);
        pVertexConstantBuffer->Bind(deviceResources);
    }

    std::unique_ptr<VertexConstantBuffer<Transform2D::Transforms>> Transform2D::pVertexConstantBuffer;
}