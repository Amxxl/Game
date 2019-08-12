#pragma once

#include "DeviceResources.h"

namespace Bind
{
    class Bindable
    {
        public:
            virtual void Bind(DX::DeviceResources* deviceResources) noexcept = 0;
            virtual ~Bindable() = default;

            virtual std::string const& GetUID() const noexcept
            {
                assert("Must be implemented in a derrived class!" && false);
                return "";
            }

        protected:
            static ID3D11DeviceContext1* GetContext(DX::DeviceResources* deviceResources) noexcept;
            static ID3D11Device1* GetDevice(DX::DeviceResources* deviceResources) noexcept;
    };
}