#pragma once

#include "DeviceResources.h"

class Drawable;
class TechniqueProbe;

namespace Bind
{
    class Bindable
    {
        public:
            virtual void Bind(DX::DeviceResources* deviceResources) noexcept = 0;
            virtual void InitializeParentReference(Drawable const&) noexcept {}
            virtual void Accept(TechniqueProbe&) {}

            virtual std::string const& GetUID() const noexcept
            {
                assert("Must be implemented in a derrived class!" && false);
                return "";
            }

            virtual ~Bindable() = default;

        protected:
            static ID3D11DeviceContext1* GetContext(DX::DeviceResources* deviceResources) noexcept;
            static ID3D11Device1* GetDevice(DX::DeviceResources* deviceResources) noexcept;
    };

    class CloningBindable : public Bindable
    {
        public:
            virtual std::unique_ptr<CloningBindable> Clone() const noexcept = 0;
    };
}