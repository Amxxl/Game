//
// DynamicVertexBuffer.h
//

#pragma once

template<typename T>
class DynamicVertexBuffer
{
    public:
        DynamicVertexBuffer() = default;
        explicit DynamicVertexBuffer(_In_ ID3D11Device* device, T* data, UINT numVertices)
        {
            Create(device, data, numVertices);
        }

        DynamicVertexBuffer(DynamicVertexBuffer const&) = delete;
        DynamicVertexBuffer& operator=(DynamicVertexBuffer const&) = delete;

        void Create(_In_ ID3D11Device* device, T* data, UINT numVertices)
        {
            this->bufferSize = numVertices;
            this->stride = std::make_unique<UINT>(static_cast<UINT>(sizeof(T)));

            D3D11_BUFFER_DESC dynamicVertexBufferDesc = {};
            dynamicVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            dynamicVertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
            dynamicVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            dynamicVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            dynamicVertexBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA vertexBufferData = {};
            vertexBufferData.pSysMem = data;

            DX::ThrowIfFailed(device->CreateBuffer(&dynamicVertexBufferDesc, vertexBufferData, buffer.GetAddressOf()));
        }

        void SetData(_In_ ID3D11DeviceContext* deviceContext, T* data, UINT numVertices)
        {
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            DX::ThrowIfFailed(deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
            memcpy(mappedResource.pData, data, sizeof(T) * numVertices);
            deviceContext->Unmap();
        }

        ID3D11Buffer* Get() const { return buffer.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { buffer.GetAddressOf(); }
        
        UINT BufferSize() const { return this->bufferSize; }
        UINT const Stride() const { return *this->stride.get(); }
        UINT const* StridePtr() const { return this->stride.get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        std::unique_ptr<UINT> stride;
        UINT bufferSize;
};