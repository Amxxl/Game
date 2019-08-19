#pragma once

#include <string>

#include "Drawable.h"
#include "Vector2.h"
#include "BindableCommon.h"

class Sprite : public Drawable
{
    public:
        Sprite(DX::DeviceResources* deviceResources, std::string const& texturePath);
        void Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX accumulatedTransform) const;
        DirectX::XMMATRIX GetTransform() const noexcept override;

    private:
        mutable DirectX::XMFLOAT4X4 transform;
};