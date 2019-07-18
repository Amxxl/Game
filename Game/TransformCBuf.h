#pragma once

#include "ConstantBuffers.h"
#include "Drawable.h"

namespace Bind
{
    class TransformCBuf : public Bindable
    {
        public:
            TransformCBuf(DX::DeviceResources* deviceResources, Drawable const& parent, UINT slot = 0u);
            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;
        private:
            struct Transforms
            {
                DirectX::XMMATRIX world;
                DirectX::XMMATRIX view;
                DirectX::XMMATRIX proj;
                //DirectX::XMMATRIX modelViewProj;
                //DirectX::XMMATRIX model;
            };

        private:
            static std::unique_ptr<VertexConstantBuffer<Transforms>> pVCbuf;
            Drawable const& parent;
    };
}