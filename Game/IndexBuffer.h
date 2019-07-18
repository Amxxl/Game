//
// IndexBuffer.h - Helper to create index buffer.
//

#pragma once

#include "Bindable.h"

namespace Bind
{
        template<typename T>
        class IndexBuffer : public Bindable
        {
        public:
            IndexBuffer() = default;
            explicit IndexBuffer(_In_ ID3D11Device* device, _In_ T* data, uint32 indexCount)
            {
                Create(device, data, indexCount);
            }

            explicit IndexBuffer(_In_ DX::DeviceResources* deviceResources, std::vector<T>& indices)
            {
                Create(GetDevice(deviceResources), indices.data(), static_cast<uint32>(indices.size()));
            }

            IndexBuffer(IndexBuffer const&) = default;
            IndexBuffer& operator=(IndexBuffer const&) = default;

            void Create(_In_ ID3D11Device* device, _In_ T* data, uint32 indexCount)
            {
                this->indexCount = indexCount;

                D3D11_BUFFER_DESC desc = { };

                desc.ByteWidth = static_cast<UINT>(sizeof(T)) * indexCount;
                desc.Usage = D3D11_USAGE_DEFAULT;
                desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

                D3D11_SUBRESOURCE_DATA indexBufferData = { };
                indexBufferData.pSysMem = data;

                DX::ThrowIfFailed(
                    device->CreateBuffer(&desc, &indexBufferData, buffer.ReleaseAndGetAddressOf())
                );
            }

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override
            {
                GetContext(deviceResources)->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
            }

            ID3D11Buffer* Get() const { return buffer.Get(); }
            ID3D11Buffer* const* GetAddressOf() const { return buffer.GetAddressOf(); }
            uint32 IndexCount() const { return indexCount; }

        private:
            Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
            uint32 indexCount = 0;
    };
}