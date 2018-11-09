//
// VertexBuffer.h
// 

#pragma once

template<class T>
class VertexBuffer
{
    public:
        VertexBuffer() = default;
        VertexBuffer(VertexBuffer<T> const&) = delete;

        HRESULT Create(ID3D11Device* device, T* data, UINT numVertices)
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

            return device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
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

