#pragma once

#include "ConditionalNoexcept.h"
#include <assimp/scene.h>
#include <utility>

#define DVT_ELEMENT_AI_EXTRACTOR(member) static SysType Extract(aiMesh const& mesh, size_t i) noexcept \
{ \
    return *reinterpret_cast<SysType const*>(&mesh.member[i]); \
}

#define LAYOUT_ELEMENT_TYPES \
    X(Position2D) \
    X(Position3D) \
    X(Texture2D) \
    X(Normal) \
    X(Tangent) \
    X(Bitangent) \
    X(Float3Color) \
    X(Float4Color) \
    X(RGBAColor) \
    X(Count)

namespace dvt
{
    class VertexLayout
    {
        public:
            enum ElementType
            {
                #define X(el) el,
                LAYOUT_ELEMENT_TYPES
                #undef X
            };

            template<ElementType> struct Map;
            template<> struct Map<Position2D>
            {
                using SysType = DirectX::XMFLOAT2;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
                static constexpr char const* semantic = "Position";
                static constexpr char const* code = "P2";
                DVT_ELEMENT_AI_EXTRACTOR(mVertices)
            };
            template<> struct Map<Position3D>
            {
                using SysType = DirectX::XMFLOAT3;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
                static constexpr char const* semantic = "Position";
                static constexpr char const* code = "P3";
                DVT_ELEMENT_AI_EXTRACTOR(mVertices)
            };
            template<> struct Map<Texture2D>
            {
                using SysType = DirectX::XMFLOAT2;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
                static constexpr char const* semantic = "Texcoord";
                static constexpr char const* code = "T2";
                DVT_ELEMENT_AI_EXTRACTOR(mTextureCoords[0])
            };
            template<> struct Map<Normal>
            {
                using SysType = DirectX::XMFLOAT3;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
                static constexpr char const* semantic = "Normal";
                static constexpr char const* code = "N";
                DVT_ELEMENT_AI_EXTRACTOR(mNormals)
            };
            template<> struct Map<Tangent>
            {
                using SysType = DirectX::XMFLOAT3;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
                static constexpr char const* semantic = "Tangent";
                static constexpr char const* code = "Nt";
                DVT_ELEMENT_AI_EXTRACTOR(mTangents)
            };
            template<> struct Map<Bitangent>
            {
                using SysType = DirectX::XMFLOAT3;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
                static constexpr char const* semantic = "Bitangent";
                static constexpr char const* code = "Nb";
                DVT_ELEMENT_AI_EXTRACTOR(mBitangents)
            };
            template<> struct Map<Float3Color>
            {
                using SysType = DirectX::XMFLOAT3;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
                static constexpr char const* semantic = "Color";
                static constexpr char const* code = "C3";
                DVT_ELEMENT_AI_EXTRACTOR(mColors[0])
            };
            template<> struct Map<Float4Color>
            {
                using SysType = DirectX::XMFLOAT4;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
                static constexpr char const* semantic = "Color";
                static constexpr char const* code = "C4";
                DVT_ELEMENT_AI_EXTRACTOR(mColors[0])
            };
            template<> struct Map<RGBAColor>
            {
                using SysType = uint32;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
                static constexpr char const* semantic = "Color";
                static constexpr char const* code = "C8";
                DVT_ELEMENT_AI_EXTRACTOR(mColors[0])
            };
            template<> struct Map<Count>
            {
                using SysType = long double;
                static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN;
                static constexpr char const* semantic = "!INVALID!";
                static constexpr char const* code = "!INV!";
                DVT_ELEMENT_AI_EXTRACTOR(mFaces)
            };

            template<template<VertexLayout::ElementType> class F, typename... Args>
            static constexpr auto Bridge(VertexLayout::ElementType type, Args&&... args) noxnd
            {
                switch (type)
                {
                    #define X(el) \
                    case VertexLayout::el: \
                        return F<VertexLayout::el>::Exec(std::forward<Args>(args)...);
                    LAYOUT_ELEMENT_TYPES
                    #undef X
                }
                assert("Invalid element type" && false);
                return F<VertexLayout::Count>::Exec(std::forward<Args>(args)...);
            }

            class Element
            {
                public:
                    Element(ElementType type, size_t offset, UINT semanticIndex = 0);
                    size_t GetOffsetAfter() const;
                    size_t GetOffset() const;
                    size_t Size() const;
                    UINT GetSemanticIndex() const;
                    static constexpr size_t SizeOf(ElementType type) noxnd;
                    ElementType GetType() const noexcept;
                    D3D11_INPUT_ELEMENT_DESC GetDesc() const;
                    char const* GetCode() const noexcept;
            
            private:
                    ElementType type;
                    size_t offset;
                    UINT semanticIndex;
            };

        public:
            template<ElementType Type>
            Element const& Resolve() const
            {
                for (auto& elem : elements)
                {
                    if (elem.GetType() == Type)
                    {
                        return elem;
                    }
                }
                assert("Could not resolve element type" && false);
                return elements.front();
            }

            Element const& ResolveByIndex(size_t index) const;
            VertexLayout& Append(ElementType type) noxnd;
            VertexLayout& operator<<(ElementType type);
            size_t Size() const;
            size_t GetElementCount() const noexcept;
            std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const;
            std::string GetCode() const;
            bool Has(ElementType type) const noexcept;

        private:
            std::vector<Element> elements;
    };

    class Vertex
    {
        friend class VertexBuffer;

        private:
            // necessary for Bridge to SetAttribute
            template<VertexLayout::ElementType type>
            struct AttributeSetting
            {
                template<typename T>
                static constexpr auto Exec(Vertex* pVertex, char* pAttribute, T&& val) noxnd
                {
                    return pVertex->SetAttribute<type>(pAttribute, std::forward<T>(val));
                }
            };

        public:
            template<VertexLayout::ElementType Type>
            auto& Attr()
            {
                auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
                return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
            }

            template<typename T>
            void SetAttributeByIndex(size_t index, T&& value)
            {
                auto const& element = layout.ResolveByIndex(index);
                auto pAttribute = pData + element.GetOffset();

                VertexLayout::Bridge<AttributeSetting>(element.GetType(), this, pAttribute, std::forward<T>(value));
            }

        protected:
            Vertex(char* pData, VertexLayout const& layout);

        private:
            // enables parameter pack setting of multiple parameters by element index.
            template<typename First, typename... Rest>
            void SetAttributeByIndex(size_t index, First&& first, Rest&& ... rest)
            {
                SetAttributeByIndex(index, std::forward<First>(first));
                SetAttributeByIndex(index + 1, std::forward<Rest>(rest)...);
            }

            // helper to reduce code duplication in SetAttributeByIndex
            template<VertexLayout::ElementType DestLayoutType, typename SrcType>
            void SetAttribute(char* pAttribute, SrcType&& value)
            {
                using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;

                if constexpr (std::is_assignable<Dest, SrcType>::value)
                    *reinterpret_cast<Dest*>(pAttribute) = value;
                else
                    assert("Parameter attribute type mismatch." && false);
            }

        private:
            char* pData = nullptr;
            VertexLayout const& layout;
    };

    class ConstVertex
    {
        public:
            ConstVertex(Vertex const& vertex);
            template<VertexLayout::ElementType Type>
            auto const& Attr() const
            {
                return const_cast<Vertex&>(vertex).Attr<Type>();
            }
        private:
            Vertex vertex;
    };

    class VertexBuffer
    {
        public:
            VertexBuffer(VertexLayout layout, size_t size = 0u) noxnd;
            VertexBuffer(VertexLayout layout, aiMesh const& mesh);

            char const* GetData() const;

            VertexLayout const& GetLayout() const noexcept;
            void Resize(size_t newSize);
            size_t Size() const;
            size_t SizeBytes() const;

            template<typename... Params>
            void EmplaceBack(Params&&... params)
            {
                assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements.");
                buffer.resize(buffer.size() + layout.Size());
                Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
            }

            Vertex Back();
            Vertex Front();
            Vertex operator[](size_t index);
            ConstVertex Back() const;
            ConstVertex Front() const;
            ConstVertex operator[](size_t index) const;

        private:
            std::vector<char> buffer;
            VertexLayout layout;
    };
}

#undef DVT_ELEMENT_AI_EXTRACTOR
#ifndef DVT_SOURCE_FILE
#undef LAYOUT_ELEMENT_TYPES
#endif