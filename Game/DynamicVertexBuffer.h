//
// DynamicVertexBuffer.h - Helper to create dynamic vertex buffer.
//

#pragma once

template<typename T>
class DynamicVertexBuffer
{
    public:
        DynamicVertexBuffer() = default;
        explicit DynamicVertexBuffer(_In_ ID3D11Device* device, T* data, uint32 vertexCount)
        {
            Create(device, data, vertexCount);
        }

        DynamicVertexBuffer(DynamicVertexBuffer const&) = default;
        DynamicVertexBuffer& operator=(DynamicVertexBuffer const&) = delete;

        void Create(_In_ ID3D11Device* device, T* data, uint32 vertexCount)
        {
            this->vertexCount = vertexCount;

            D3D11_BUFFER_DESC desc = { };

            desc.ByteWidth = stride * vertexCount;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            D3D11_SUBRESOURCE_DATA vertexBufferData = { };
            vertexBufferData.pSysMem = data;

            DX::ThrowIfFailed(
                device->CreateBuffer(&desc, &vertexBufferData, buffer.ReleaseAndGetAddressOf())
            );
        }

        // Writes new data into vertex buffer.
        void SetData(_In_ ID3D11DeviceContext* deviceContext, T* data, uint32 vertexCount)
        {
            assert(buffer);
            this->vertexCount = vertexCount;

            D3D11_MAPPED_SUBRESOURCE mappedResource = { };

            DX::ThrowIfFailed(
                deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
            );

            memcpy(mappedResource.pData, data, stride * vertexCount);

            deviceContext->Unmap(buffer.Get(), 0);
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