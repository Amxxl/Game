#include "pch.h"
#include "TransformCBuf.h"

namespace Bind
{
    TransformCBuf::TransformCBuf(DX::DeviceResources* deviceResources, Drawable const& parent, UINT slot)
        : parent(parent)
    {
        if (!pVCbuf)
            pVCbuf = std::make_unique<VertexConstantBuffer<Transforms>>(deviceResources, slot);
    }

    void TransformCBuf::Bind(DX::DeviceResources* deviceResources) noexcept
    {
        const Transforms tf =
        {
            DirectX::XMMatrixTranspose(parent.GetTransform() * DirectX::XMMatrixIdentity()),
            DirectX::XMMatrixTranspose(deviceResources->GetCamera()->GetViewMatrix()),
            DirectX::XMMatrixTranspose(deviceResources->GetCamera()->GetProjectionMatrix())
        };

        pVCbuf->Update(deviceResources, tf);
        pVCbuf->Bind(deviceResources);
    }

    std::unique_ptr<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::pVCbuf;
}