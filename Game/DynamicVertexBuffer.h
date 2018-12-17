//
// DynamicVertexBuffer.h
//

#pragma once

template<typename T>
class DynamicVertexBuffer
{
    public:
        DynamicVertexBuffer() = default;
        explicit DynamicVertexBuffer(_In_ ID3D11Device* device, T* data, UINT vertexCount)
        {
            Create(device, data, vertexCount);
        }

        DynamicVertexBuffer(DynamicVertexBuffer const&) = delete;
        DynamicVertexBuffer& operator=(DynamicVertexBuffer const&) = delete;

        void Create(_In_ ID3D11Device* device, T* data, UINT vertexCount)
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
        void SetData(_In_ ID3D11DeviceContext* deviceContext, T* data, UINT vertexCount)
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
        
        UINT VertexCount() const { return vertexCount; }
        UINT const Stride() const { return stride; }
        UINT const* StridePtr() const { return &stride; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        UINT stride = static_cast<UINT>(sizeof(T));
        UINT vertexCount = 0;
};