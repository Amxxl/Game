#pragma once

#include "DeviceResources.h"

namespace Bind
{
    class Bindable
    {
        public:
            virtual void Bind(DX::DeviceResources* deviceResources) noexcept = 0;
            virtual ~Bindable() = default;

        protected:
            static ID3D11DeviceContext1* GetContext(DX::DeviceResources* deviceResources) noexcept;
            static ID3D11Device1* GetDevice(DX::DeviceResources* deviceResources) noexcept;
    };
}