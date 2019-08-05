#include "pch.h"
#include "Vertex.h"

VertexLayout::Element::Element(ElementType type, size_t offset, UINT semanticIndex)
    : type(type)
    , offset(offset)
    , semanticIndex(semanticIndex)
{
}

size_t VertexLayout::Element::GetOffsetAfter() const
{
    return offset + Size();
}

size_t VertexLayout::Element::GetOffset() const
{
    return offset;
}

size_t VertexLayout::Element::Size() const
{
    return SizeOf(type);
}

UINT VertexLayout::Element::GetSemanticIndex() const
{
    return semanticIndex;
}

constexpr size_t VertexLayout::Element::SizeOf(ElementType type)
{
    switch (type)
    {
        case Position2D:
            return sizeof(Map<Position2D>::SysType);
        case Position3D:
            return sizeof(Map<Position3D>::SysType);
        case Texture2D:
            return sizeof(Map<Texture2D>::SysType);
        case Normal:
            return sizeof(Map<Normal>::SysType);
        case Float3Color:
            return sizeof(Map<Float3Color>::SysType);
        case Float4Color:
            return sizeof(Map<Float4Color>::SysType);
        case RGBAColor:
            return sizeof(Map<RGBAColor>::SysType);
    }
    assert("Invalid element type" && false);
    return 0u;
}

VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
{
    return type;
}

D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const
{
    switch (type)
    {
        case Position2D:
            return GenerateDesc<Position2D>(GetOffset(), GetSemanticIndex());
        case Position3D:
            return GenerateDesc<Position3D>(GetOffset(), GetSemanticIndex());
        case Texture2D:
            return GenerateDesc<Texture2D>(GetOffset(), GetSemanticIndex());
        case Normal:
            return GenerateDesc<Normal>(GetOffset(), GetSemanticIndex());
        case Float3Color:
            return GenerateDesc<Float3Color>(GetOffset(), GetSemanticIndex());
        case Float4Color:
            return GenerateDesc<Float4Color>(GetOffset(), GetSemanticIndex());
        case RGBAColor:
            return GenerateDesc<RGBAColor>(GetOffset(), GetSemanticIndex());
    }
    assert("Invalid element type." && false);
    return { "INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
}

VertexLayout::Element const& VertexLayout::ResolveByIndex(size_t index) const
{
    return elements[index];
}

VertexLayout& VertexLayout::Append(ElementType type)
{
    UINT semanticIndex = 0;
    for (auto& i : elements)
    {
        if (i.GetType() == type)
            semanticIndex++;
    }
    elements.emplace_back(type, Size(), semanticIndex);
    return *this;
}

VertexLayout& VertexLayout::operator<<(ElementType type)
{
    Append(type);
    return *this;
}

size_t VertexLayout::Size() const
{
    return elements.empty() ? 0u : elements.back().GetOffsetAfter();
}

size_t VertexLayout::GetElementCount() const noexcept
{
    return elements.size();
}

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
    desc.reserve(GetElementCount());
    for (auto const& element : elements)
    {
        desc.push_back(element.GetDesc());
    }
    return desc;
}

Vertex::Vertex(char* pData, VertexLayout const& layout)
    : pData(pData)
    , layout(layout)
{
    assert(pData != nullptr);
}

ConstVertex::ConstVertex(Vertex const& vertex)
    : vertex(vertex)
{
}

VertexBufferData::VertexBufferData(VertexLayout layout)
    : layout(std::move(layout))
{
}

char const* VertexBufferData::GetData() const
{
    return buffer.data();
}

VertexLayout const& VertexBufferData::GetLayout() const noexcept
{
    return layout;
}

size_t VertexBufferData::Size() const
{
    return buffer.size() / layout.Size();
}

size_t VertexBufferData::SizeBytes() const
{
    return buffer.size();
}

Vertex VertexBufferData::Back()
{
    assert(buffer.size() != 0u);
    return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
}

Vertex VertexBufferData::Front()
{
    assert(buffer.size() != 0u);
    return Vertex{ buffer.data(), layout };
}

Vertex VertexBufferData::operator[](size_t index)
{
    assert(index < Size());
    return Vertex{ buffer.data() + layout.Size() * index, layout };
}

ConstVertex VertexBufferData::Back() const
{
    return const_cast<VertexBufferData*>(this)->Back();
}

ConstVertex VertexBufferData::Front() const
{
    return const_cast<VertexBufferData*>(this)->Front();
}

ConstVertex VertexBufferData::operator[](size_t index) const
{
    return const_cast<VertexBufferData&>(*this)[index];
}
