#pragma once

#include "BindableCommon.h"

class PointLight
{
    public:
        PointLight(DX::DeviceResources* deviceResources, float radius = 0.5f);
        void SpawnControlWindow() noexcept;
        void Reset() noexcept;
        void Draw(DX::DeviceResources* deviceResources);
        void Bind(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX view) const noexcept;

    private:
        struct CBPointLight
        {
            alignas(16) DirectX::XMFLOAT3 position;
            alignas(16) DirectX::XMFLOAT3 ambient;
            alignas(16) DirectX::XMFLOAT3 diffuseColor;
            float diffuseIntensity;
            float attConst;
            float attLin;
            float attQuad;
        };

    private:
        CBPointLight cbData;
        mutable Bind::PixelConstantBuffer<CBPointLight> cbuf;
};

