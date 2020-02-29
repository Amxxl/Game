#pragma once
#include "Bindable.h"
#include "DynamicConstant.h"
#include "TechniqueProbe.h"

namespace Bind
{
    class ConstantBufferEx : public Bindable
    {
        public:
            void Update(DX::DeviceResources* deviceResources, Dcb::Buffer& buf)
            {
                assert(&buf.GetRootLayoutElement() == &GetRootLayoutElement());

                D3D11_MAPPED_SUBRESOURCE msr = {};
                DX::ThrowIfFailed(
                    GetContext(deviceResources)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr)
                );

                memcpy(msr.pData, buf.GetData(), buf.GetSizeInBytes());
                GetContext(deviceResources)->Unmap(pConstantBuffer.Get(), 0u);
            }

            // this exists for validation of the update buffer layout
            // reason why it's not getbuffer is because nocache doesn't store buffer.
            virtual Dcb::LayoutElement const& GetRootLayoutElement() const noexcept = 0;

        protected:
            ConstantBufferEx(DX::DeviceResources* deviceResources, Dcb::LayoutElement const& layoutRoot, UINT slot, Dcb::Buffer const* pBuf)
                : slot(slot)
            {
                D3D11_BUFFER_DESC cbd;
                cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                cbd.Usage = D3D11_USAGE_DYNAMIC;
                cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                cbd.MiscFlags = 0u;
                cbd.ByteWidth = static_cast<UINT>(layoutRoot.GetSizeInBytes());
                cbd.StructureByteStride = 0;

                if (pBuf != nullptr)
                {
                    D3D11_SUBRESOURCE_DATA csd = {};
                    csd.pSysMem = pBuf->GetData();
                    DX::ThrowIfFailed(GetDevice(deviceResources)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
                }
                else
                {
                    DX::ThrowIfFailed(GetDevice(deviceResources)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
                }
            }

        protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
            UINT slot;
    };

    class PixelConstantBufferEx : public ConstantBufferEx
    {
        public:
            using ConstantBufferEx::ConstantBufferEx;
            void Bind(DX::DeviceResources* deviceResources) noexcept override
            {
                GetContext(deviceResources)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
            }
    };

    class VertexConstantBufferEx : public ConstantBufferEx
    {
        public:
            using ConstantBufferEx::ConstantBufferEx;
            void Bind(DX::DeviceResources* deviceResources) noexcept override
            {
                GetContext(deviceResources)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
            }
    };

    template<class T>
    class CachingConstantBufferEx : public T
    {
        public:
            CachingConstantBufferEx(DX::DeviceResources* deviceResources, Dcb::CookedLayout const& layout, UINT slot)
                : T(deviceResources, *layout.ShareRoot(), slot, nullptr)
                , buf(Dcb::Buffer(layout))
            {
            }

            CachingConstantBufferEx(DX::DeviceResources* deviceResources, Dcb::Buffer const& buf, UINT slot)
                : T(deviceResources, buf.GetRootLayoutElement(), slot, &buf)
                , buf(buf)
            {
            }

            Dcb::LayoutElement const& GetRootLayoutElement() const noexcept override
            {
                return buf.GetRootLayoutElement();
            }

            Dcb::Buffer const& GetBuffer() const noexcept
            {
                return buf;
            }

            void SetBuffer(Dcb::Buffer const& buf_in)
            {
                buf.CopyFrom(buf_in);
                dirty = true;
            }

            void Bind(DX::DeviceResources* deviceResources) noexcept override
            {
                if (dirty)
                {
                    T::Update(deviceResources, buf);
                    dirty = false;
                }
                T::Bind(deviceResources);
            }

            void Accept(TechniqueProbe& probe) override
            {
                if (probe.VisitBuffer(buf))
                {
                    dirty = true;
                }
            }

        private:
            bool dirty = false;
            Dcb::Buffer buf;
    };

    using CachingPixelConstantBufferEx = CachingConstantBufferEx<PixelConstantBufferEx>;
    using CachingVertexConstantBufferEx = CachingConstantBufferEx<VertexConstantBufferEx>;

    class NocachePixelConstantBufferEx : public PixelConstantBufferEx
    {
        public:
            NocachePixelConstantBufferEx(DX::DeviceResources* deviceResources, Dcb::CookedLayout const& layout, UINT slot)
                : PixelConstantBufferEx(deviceResources, *layout.ShareRoot(), slot, nullptr)
                , pLayoutRoot(layout.ShareRoot())
            {
            }

            NocachePixelConstantBufferEx(DX::DeviceResources* deviceResources, Dcb::Buffer const& buf, UINT slot)
                : PixelConstantBufferEx(deviceResources, buf.GetRootLayoutElement(), slot, &buf)
                , pLayoutRoot(buf.ShareLayoutRoot())
            {
            }

            Dcb::LayoutElement const& GetRootLayoutElement() const noexcept override
            {
                return *pLayoutRoot;
            }

        private:
            std::shared_ptr<Dcb::LayoutElement> pLayoutRoot;
    };
}