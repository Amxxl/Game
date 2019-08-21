#pragma once

#include "ConstantBuffers.h"
#include "Drawable.h"

namespace Bind
{
    class Transform3D : public Bindable
    {
        public:
            Transform3D(DX::DeviceResources* deviceResources, Drawable const& parent, UINT slot = 0u);
            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

        private:
            struct Transforms
            {
                DirectX::XMMATRIX world;
                DirectX::XMMATRIX view;
                DirectX::XMMATRIX proj;
            };

        private:
            Drawable const& parent;
            static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertexConstantBuffer;
    };
}
