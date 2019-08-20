#pragma once

#include "ConstantBuffers.h"
#include "Drawable.h"

namespace Bind
{
    class Transform2D : public Bindable
    {
        public:
            Transform2D(DX::DeviceResources* deviceResources, Drawable const& owner, UINT slot = 0u);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

        private:
            struct Transforms
            {
                DirectX::XMMATRIX wvpMatrix;
            };

        private:
            Drawable const& owner;
            static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertexConstantBuffer;
    };
}
