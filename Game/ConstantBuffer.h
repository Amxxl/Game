//
// ConstantBuffer.h - Helper to create constant buffer.
//

#pragma once

namespace DX
{
    template<typename T>
    class ConstantBuffer
    {
        public:
            ConstantBuffer() = default;
            explicit ConstantBuffer(_In_ ID3D11Device* device)
            {
                Create(device);
            }

            ConstantBuffer(ConstantBuffer const&) = default;
            ConstantBuffer& operator=(ConstantBuffer const&) = default;

            void Create(_In_ ID3D11Device* device)
            {
                D3D11_BUFFER_DESC desc = {};

                desc.ByteWidth = static_cast<uint32>(sizeof(T));
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

                DX::ThrowIfFailed(
                    device->CreateBuffer(&desc, nullptr, buffer.ReleaseAndGetAddressOf())
                );
            }

            // Writes new data into the constant buffer.
            void SetData(_In_ ID3D11DeviceContext* deviceContext, T const& value)
            {
                assert(buffer);

                D3D11_MAPPED_SUBRESOURCE mappedResource = { };

                DX::ThrowIfFailed(
                    deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
                );

                *static_cast<T*>(mappedResource.pData) = value;

                deviceContext->Unmap(buffer.Get(), 0);
            }

            ID3D11Buffer* Get() const { return buffer.Get(); }
            ID3D11Buffer* const* GetAddressOf() const { return buffer.GetAddressOf(); }

        private:
            Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    };
}