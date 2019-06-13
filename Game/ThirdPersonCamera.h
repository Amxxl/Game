#pragma once

#include "GameObject.h"

class ThirdPersonCamera : public GameObject
{
    public:
        ThirdPersonCamera();

        void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

        DirectX::XMMATRIX const& GetViewMatrix() const;
        DirectX::XMMATRIX const& GetProjectionMatrix() const;

        void SetOrigin(DirectX::XMFLOAT3 origin) { this->origin = origin; }

    public:
        virtual void UpdateMatrix() override;
        DirectX::XMFLOAT3 cameraPos;

    private:
        DirectX::XMMATRIX viewMatrix;
        DirectX::XMMATRIX projectionMatrix;

    public:
        DirectX::XMFLOAT3 origin;

        float r = 20.0f;
        float theta = 0.0f;
        float phi = 0.0f;
        float pitch = 0.0f;
        float yaw = 0.0f;
        float roll = 0.0f;
};