#pragma once

#include "pch.h"
#include "Bindable.h"
#include "BindableCache.h"

namespace Bind
{
    template<typename T>
    class ConstantBuffer : public Bindable
    {
        public:
            ConstantBuffer(DX::DeviceResources* deviceResources, T const& data, UINT slot = 0u)
                : slot(slot)
            {
                D3D11_BUFFER_DESC desc = {};

                desc.ByteWidth = static_cast<UINT>(sizeof(T));
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

                D3D11_SUBRESOURCE_DATA initialData = {};
                initialData.pSysMem = &data;

                DX::ThrowIfFailed(
                    GetDevice(deviceResources)->CreateBuffer(&desc, &initialData, &pConstantBuffer)
                );
            }

            ConstantBuffer(DX::DeviceResources* deviceResources, UINT slot = 0u)
                : slot(slot)
            {
                D3D11_BUFFER_DESC desc = {};

                desc.ByteWidth = static_cast<UINT>(sizeof(T));
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

                DX::ThrowIfFailed(
                    GetDevice(deviceResources)->CreateBuffer(&desc, nullptr, &pConstantBuffer)
                );
            }

            void Update(DX::DeviceResources* deviceResources, T const& data)
            {
                D3D11_MAPPED_SUBRESOURCE mappedResource = {};

                DX::ThrowIfFailed(
                    GetContext(deviceResources)->Map(pConstantBuffer.Get(), 0u,
                        D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource)
                );

                memcpy(mappedResource.pData, &data, sizeof(T));

                GetContext(deviceResources)->Unmap(pConstantBuffer.Get(), 0u);
            }

        protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
            UINT slot;
    };

    template<typename T>
    class VertexConstantBuffer : public ConstantBuffer<T>
    {
        private:
            using ConstantBuffer<T>::pConstantBuffer;
            using ConstantBuffer<T>::slot;
            using Bindable::GetContext;

        public:
            using ConstantBuffer<T>::ConstantBuffer;

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override
            {
                GetContext(deviceResources)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
            }

            static std::shared_ptr<VertexConstantBuffer> Resolve(DX::DeviceResources* deviceResources, T& data, UINT slot = 0)
            {
                return BindableCache::Resolve<VertexConstantBuffer>(deviceResources, data, slot);
            }

            static std::shared_ptr<VertexConstantBuffer> Resolve(DX::DeviceResources* deviceResources, UINT slot = 0)
            {
                return BindableCache::Resolve<VertexConstantBuffer>(deviceResources, slot);
            }

            static std::string GenerateUID(T const&, UINT slot)
            {
                return GenerateUID(slot);
            }
            
            static std::string GenerateUID(UINT slot = 0)
            {
                using namespace std::string_literals;
                return typeid(VertexConstantBuffer).name() + "#"s + std::to_string(slot);
            }

            virtual std::string const& GetUID() const noexcept override
            {
                return GenerateUID(slot);
            }
    };

    template<typename T>
    class PixelConstantBuffer : public ConstantBuffer<T>
    {
        private:
            using ConstantBuffer<T>::pConstantBuffer;
            using ConstantBuffer<T>::slot;
            using Bindable::GetContext;

        public:
            using ConstantBuffer<T>::ConstantBuffer;

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override
            {
                GetContext(deviceResources)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
            }

            static std::shared_ptr<PixelConstantBuffer> Resolve(DX::DeviceResources* deviceResources, T& data, UINT slot = 0)
            {
                return BindableCache::Resolve<PixelConstantBuffer>(deviceResources, data, slot);
            }

            static std::shared_ptr<PixelConstantBuffer> Resolve(DX::DeviceResources* deviceResources, UINT slot = 0)
            {
                return BindableCache::Resolve<PixelConstantBuffer>(deviceResources, slot);
            }

            static std::string GenerateUID(T const&, UINT slot)
            {
                return GenerateUID(slot);
            }

            static std::string GenerateUID(UINT slot = 0)
            {
                using namespace std::string_literals;
                return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(slot);
            }

            virtual std::string const& GetUID() const noexcept override
            {
                return GenerateUID(slot);
            }
    };
}