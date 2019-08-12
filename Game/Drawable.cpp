#include "pch.h"
#include "Drawable.h"
#include "IndexBuffer.h"

using namespace Bind;

void Drawable::Draw(DX::DeviceResources* deviceResources) const
{
    for (auto& b : binds)
        b->Bind(deviceResources);

    deviceResources->GetDeviceContext()->DrawIndexed(pIndexBuffer->IndexCount(), 0, 0);
}

void Drawable::AddBind(std::shared_ptr<Bind::Bindable> bind)
{
    // special case for index buffer.
    if (typeid(*bind) == typeid(IndexBuffer<unsigned int>))
    {
        assert("Binding multiple index buffers are not allowed.", pIndexBuffer == nullptr);
        pIndexBuffer = &static_cast<IndexBuffer<unsigned int>&>(*bind);
    }
    binds.push_back(std::move(bind));
}