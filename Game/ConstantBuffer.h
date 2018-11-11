//
// ConstantBuffer.h
//

#pragma once


template<typename T>
class ConstantBuffer
{
    public:
        ConstantBuffer() = default;
        explicit ConstantBuffer(_In_ ID3D11Device* device)
        {
            Create(device);
        }

        ConstantBuffer(ConstantBuffer const&) = delete;
        ConstantBuffer& operator=(ConstantBuffer const&) = delete;

        void Create(_In_ ID3D11Device* device)
        {
            D3D11_BUFFER_DESC constantBufferDesc = {};
            constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            constantBufferDesc.ByteWidth = sizeof(T);
            constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            constantBufferDesc.MiscFlags = 0;

            DX::ThrowIfFailed(device->CreateBuffer(&constantBufferDesc, nullptr, buffer.ReleaseAndGetAddressOf()));
        }

        // Writes new data into the constant buffer.
        void SetData(_In_ ID3D11DeviceContext* deviceContext, T const& value)
        {
            assert(buffer);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            DX::ThrowIfFailed(deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

            *static_cast<T*>(mappedResource.pData) = value;
            deviceContext->Unmap(buffer.Get(), 0);
        }

        ID3D11Buffer* Get() const { return buffer.Get(); }
        ID3D11Buffer* const* GetAddressOf() const { return buffer.GetAddressOf(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

};