#include "pch.h"

#define DVT_SOURCE_FILE
#include "Vertex.h"


namespace dvt
{
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

    VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
    {
        return type;
    }

    template<VertexLayout::ElementType type>
    struct CodeLookup
    {
        static constexpr auto Exec() noexcept
        {
            return VertexLayout::Map<type>::code;
        }
    };

    char const* VertexLayout::Element::GetCode() const noexcept
    {
        return Bridge<CodeLookup>(type);
    }

    template<VertexLayout::ElementType type> struct DescGenerate
    {
        static constexpr D3D11_INPUT_ELEMENT_DESC Exec(size_t offset, UINT semanticIndex) noexcept
        {
            return {
                VertexLayout::Map<type>::semantic, semanticIndex,
                VertexLayout::Map<type>::dxgiFormat, 0, static_cast<UINT>(offset),
                D3D11_INPUT_PER_VERTEX_DATA, 0
            };
        }
    };

    D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const
    {
        return Bridge<DescGenerate>(type, GetOffset(), GetSemanticIndex());
    }

    template<VertexLayout::ElementType type>
    struct SysSizeLookup
    {
        static constexpr auto Exec() noexcept
        {
            return sizeof(VertexLayout::Map<type>::SysType);
        }
    };
    constexpr size_t VertexLayout::Element::SizeOf(ElementType type) noxnd
    {
        return Bridge<SysSizeLookup>(type);
    }

    VertexLayout::Element const& VertexLayout::ResolveByIndex(size_t index) const
    {
        return elements[index];
    }

    VertexLayout& VertexLayout::Append(ElementType type) noxnd
    {
        if (!Has(type))
        {
            UINT semanticIndex = 0;
            for (auto& i : elements)
            {
                if (i.GetType() == type)
                    semanticIndex++;
            }
            elements.emplace_back(type, Size(), semanticIndex);
        }
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

    std::string VertexLayout::GetCode() const
    {
        std::string code = "";
        for (auto const& element : elements)
            code += element.GetCode();

        return code;
    }

    bool VertexLayout::Has(ElementType type) const noexcept
    {
        for (auto& e : elements)
        {
            if (e.GetType() == type)
            {
                return true;
            }
        }
        return false;
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

    VertexBuffer::VertexBuffer(VertexLayout layout, size_t size) noxnd
        : layout(std::move(layout))
    {
        Resize(size);
    }

    template<VertexLayout::ElementType type>
    struct AttributeAiMeshFill
    {
        static constexpr void Exec(VertexBuffer* pBuf, aiMesh const& mesh) noxnd
        {
            for (auto end = mesh.mNumVertices, i = 0u; i < end; ++i)
            {
                (*pBuf)[i].Attr<type>() = VertexLayout::Map<type>::Extract(mesh, i);
            }
        }
    };

    VertexBuffer::VertexBuffer(VertexLayout layout, aiMesh const& mesh)
        : layout(std::move(layout))
    {
        Resize(mesh.mNumVertices);
        for (size_t i = 0, end = layout.GetElementCount(); i < end; ++i)
        {
            VertexLayout::Bridge<AttributeAiMeshFill>(layout.ResolveByIndex(i).GetType(), this, mesh);
        }
    }

    char const* VertexBuffer::GetData() const
    {
        return buffer.data();
    }

    VertexLayout const& VertexBuffer::GetLayout() const noexcept
    {
        return layout;
    }

    void VertexBuffer::Resize(size_t newSize)
    {
        auto const size = Size();

        if (size < newSize)
            buffer.resize(buffer.size() + layout.Size() * (newSize - size));
    }

    size_t VertexBuffer::Size() const
    {
        return buffer.size() / layout.Size();
    }

    size_t VertexBuffer::SizeBytes() const
    {
        return buffer.size();
    }

    Vertex VertexBuffer::Back()
    {
        assert(buffer.size() != 0u);
        return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
    }

    Vertex VertexBuffer::Front()
    {
        assert(buffer.size() != 0u);
        return Vertex{ buffer.data(), layout };
    }

    Vertex VertexBuffer::operator[](size_t index)
    {
        assert(index < Size());
        return Vertex{ buffer.data() + layout.Size() * index, layout };
    }

    ConstVertex VertexBuffer::Back() const
    {
        return const_cast<VertexBuffer*>(this)->Back();
    }

    ConstVertex VertexBuffer::Front() const
    {
        return const_cast<VertexBuffer*>(this)->Front();
    }

    ConstVertex VertexBuffer::operator[](size_t index) const
    {
        return const_cast<VertexBuffer&>(*this)[index];
    }
}