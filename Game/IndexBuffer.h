//
// IndexBuffer.h
//

#pragma once

class IndexBuffer
{
    public:
        IndexBuffer() = default;
        IndexBuffer(IndexBuffer const&) = delete;

        HRESULT Create(ID3D11Device* device, DWORD* data, UINT numIndices)
        {
            this->bufferSize = numIndices;
            
            D3D11_BUFFER_DESC indexBufferDesc = {};
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.ByteWidth = sizeof(DWORD) * numIndices;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA indexBufferData = {};
            indexBufferData.pSysMem = data;

            return device->CreateBuffer(&indexBufferDesc, &indexBufferData, buffer.GetAddressOf());
        }

        ID3D11Buffer* Get() const { return buffer.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { return buffer.GetAddressOf(); }
        UINT BufferSize() const { return bufferSize; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        UINT bufferSize = 0;
};

