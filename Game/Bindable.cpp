#include "pch.h"
#include "Bindable.h"

namespace Bind
{
    ID3D11DeviceContext1* Bindable::GetContext(DX::DeviceResources* deviceResources) noexcept
    {
        return deviceResources->GetDeviceContext();
    }

    ID3D11Device1* Bindable::GetDevice(DX::DeviceResources* deviceResources) noexcept
    {
        return deviceResources->GetDevice();
    }
}
