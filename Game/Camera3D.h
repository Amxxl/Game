//
// Camera.h
//

#pragma once

#include "GameObject3D.h"

class Camera3D : public GameObject3D
{
    public:
        Camera3D();

        void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

        DirectX::XMMATRIX const& GetViewMatrix() const;
        DirectX::XMMATRIX const& GetProjectionMatrix() const;

    private:
        void UpdateMatrix() override;

    private:
        DirectX::XMMATRIX viewMatrix;
        DirectX::XMMATRIX projectionMatrix;
};