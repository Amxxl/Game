#include "pch.h"
#include "IndexedTriangleList.h"

IndexedTriangleList::IndexedTriangleList(dvt::VertexBuffer verts_in, std::vector<unsigned short> indices_in)
    : vertices(std::move(verts_in))
    , indices(std::move(indices_in))
{
    assert(vertices.Size() > 2);
    assert(indices.size() % 3 == 0);
}

void IndexedTriangleList::Transform(DirectX::FXMMATRIX matrix)
{
    using Elements = dvt::VertexLayout::ElementType;
    for (int i = 0; i < vertices.Size(); ++i)
    {
        auto& pos = vertices[i].Attr<Elements::Position3D>();
        DirectX::XMStoreFloat3(&pos, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&pos), matrix));
    }
}

void IndexedTriangleList::SetNormalsIndependentFlat()
{
    using namespace DirectX;
    using Type = dvt::VertexLayout::ElementType;

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        auto v0 = vertices[indices[i]];
        auto v1 = vertices[indices[i + 1]];
        auto v2 = vertices[indices[i + 2]];

        auto const p0 = XMLoadFloat3(&v0.Attr<Type::Position3D>());
        auto const p1 = XMLoadFloat3(&v1.Attr<Type::Position3D>());
        auto const p2 = XMLoadFloat3(&v2.Attr<Type::Position3D>());

        auto const n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

        XMStoreFloat3(&v0.Attr<Type::Normal>(), n);
        XMStoreFloat3(&v1.Attr<Type::Normal>(), n);
        XMStoreFloat3(&v2.Attr<Type::Normal>(), n);
    }
}