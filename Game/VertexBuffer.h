//
// VertexBuffer.h
// 

#pragma once

template<typename T>
class VertexBuffer
{
    public:
        VertexBuffer() = default;
        explicit VertexBuffer(_In_ ID3D11Device* device, _In_ T* data, UINT vertexCount)
        {
            Create(device, data, vertexCount);
        }

        VertexBuffer(VertexBuffer const&) = default;
        VertexBuffer& operator=(VertexBuffer const&) = delete;

        void Create(_In_ ID3D11Device* device, _In_ T* data, UINT vertexCount)
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
        UINT VertexCount() const { return vertexCount; }

        UINT const Stride() const { return stride; }
        UINT const* StridePtr() const { return &stride; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        UINT stride = static_cast<UINT>(sizeof(T));
        UINT vertexCount = 0;
};