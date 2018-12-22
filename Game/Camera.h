//
// Camera.h
//

#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
    public:
        Camera();

        void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

        DirectX::XMMATRIX const& GetViewMatrix() const;
        DirectX::XMMATRIX const& GetProjectionMatrix() const;

    private:
        void UpdateMatrix() override;

    private:
        DirectX::XMMATRIX viewMatrix;
        DirectX::XMMATRIX projectionMatrix;
};

