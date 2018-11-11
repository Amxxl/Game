//
// IndexBuffer.h
//

#pragma once

template<typename T>
class IndexBuffer
{
    public:
        IndexBuffer() = default;
        explicit IndexBuffer(_In_ ID3D11Device* device, _In_ T* data, UINT numIndices)
        {
            Create(device, data, numIndices);
        }

        IndexBuffer(IndexBuffer const&) = delete;
        IndexBuffer& operator=(IndexBuffer const&) = delete;

        void Create(_In_ ID3D11Device* device, _In_ T* data, UINT numIndices)
        {
            this->bufferSize = numIndices;
            
            D3D11_BUFFER_DESC indexBufferDesc = {};
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.ByteWidth = sizeof(T) * numIndices;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA indexBufferData = {};
            indexBufferData.pSysMem = data;

            DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexBufferData, buffer.GetAddressOf()));
        }

        ID3D11Buffer* Get() const { return buffer.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { return buffer.GetAddressOf(); }
        UINT BufferSize() const { return bufferSize; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        UINT bufferSize = 0;
};

