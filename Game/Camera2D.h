#pragma once

#include "GameObject2D.h"

class Camera2D : public GameObject2D
{
    public:
        Camera2D();
        void SetProjectionValues(float width, float height, float nearZ, float farZ);

        DirectX::XMMATRIX const& GetOrthoMatrix() const;
        DirectX::XMMATRIX const& GetWorldMatrix() const;

    private:
        virtual void UpdateMatrix() override;

        DirectX::XMMATRIX orthoMatrix;
        DirectX::XMMATRIX worldMatrix;
};