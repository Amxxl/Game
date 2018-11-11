//
// VertexBuffer.h
// 

#pragma once

template<typename T>
class VertexBuffer
{
    public:
        VertexBuffer() = default;
        explicit VertexBuffer(_In_ ID3D11Device* device, _In_ T* data, UINT numVertices)
        {
            Create(device, data, numVertices);
        }

        VertexBuffer(VertexBuffer const&) = delete;
        VertexBuffer& operator=(VertexBuffer const&) = delete;

        void Create(_In_ ID3D11Device* device, _In_ T* data, UINT numVertices)
        {
            this->bufferSize = numVertices;
            this->stride = std::make_unique<UINT>(sizeof(T));

            D3D11_BUFFER_DESC vertexBufferDesc = {};
            vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = 0;
            vertexBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA vertexBufferData = {};
            vertexBufferData.pSysMem = data;

            DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, buffer.GetAddressOf()));
        }

        ID3D11Buffer* Get() const { return buffer.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { buffer.GetAddressOf(); }
        UINT BufferSize() const { return this->bufferSize; }

        UINT const Stride() const { return *this->stride.get(); }
        UINT const* StridePtr() const { return this->stride.get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        std::unique_ptr<UINT> stride;
        UINT bufferSize = 0;

};

