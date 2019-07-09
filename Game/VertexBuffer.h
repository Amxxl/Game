//
// VertexBuffer.h - Helper to create vertex buffer.
// 

#pragma once

namespace DX
{
    template<typename T>
    class VertexBuffer
    {
        public:
            VertexBuffer() = default;
            explicit VertexBuffer(_In_ ID3D11Device* device, _In_ T* data, uint32 vertexCount)
            {
                Create(device, data, vertexCount);
            }

            VertexBuffer(VertexBuffer const&) = default;
            VertexBuffer& operator=(VertexBuffer const&) = default;

            void Create(_In_ ID3D11Device* device, _In_ T* data, uint32 vertexCount)
            {
                this->vertexCount = vertexCount;

                D3D11_BUFFER_DESC desc = { };

                desc.ByteWidth = stride * vertexCount;
                desc.Usage = D3D11_USAGE_DEFAULT;
                desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

                D3D11_SUBRESOURCE_DATA vertexBufferData = { };
                vertexBufferData.pSysMem = data;

                DX::ThrowIfFailed(
                    device->CreateBuffer(&desc, &vertexBufferData, buffer.ReleaseAndGetAddressOf())
                );
            }

            ID3D11Buffer* Get() const { return buffer.Get(); }
            ID3D11Buffer* const* GetAddressOf() const { return buffer.GetAddressOf(); }
            uint32 VertexCount() const { return vertexCount; }

            uint32 const Stride() const { return stride; }
            uint32 const* StridePtr() const { return &stride; }

        private:
            Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
            uint32 stride = static_cast<uint32>(sizeof(T));
            uint32 vertexCount = 0;
    };
}