#include "pch.h"
#include "Drawable.h"
#include "IndexBuffer.h"

using namespace Bind;

void Drawable::Draw(DX::DeviceResources* deviceResources) const
{
    for (auto& b : binds)
        b->Bind(deviceResources);

    for (auto& b : GetStaticBinds())
        b->Bind(deviceResources);

    deviceResources->GetDeviceContext()->DrawIndexed(pIndexBuffer->IndexCount(), 0, 0);
}

void Drawable::AddBind(std::unique_ptr<Bind::Bindable> bind)
{
    assert("Must use AddIndexBuffer to bind index buffer." && typeid(*bind) != typeid(IndexBuffer));
    binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer<unsigned int>> indexBuffer)
{
    pIndexBuffer = indexBuffer.get();
    binds.push_back(std::move(indexBuffer));
}
