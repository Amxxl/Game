//
// Camera.cpp
//

#include "pch.h"
#include "Camera3D.h"

using namespace DirectX;

Camera3D::Camera3D()
{
    this->pos = XMFLOAT3(1.5f, 1.5f, 1.5f);
    this->posVector = XMLoadFloat3(&this->pos);
    this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateMatrix();
}

void Camera3D::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
    this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

XMMATRIX const& Camera3D::GetViewMatrix() const
{
    return this->viewMatrix;
}

XMMATRIX const& Camera3D::GetProjectionMatrix() const
{
    return this->projectionMatrix;
}

void Camera3D::UpdateMatrix()
{
    // Calculate camera rotation matrix.
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
    // Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix.
    XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    // Adjust cam target to be offset by the camera's current position
    camTarget += posVector;
    // Calculate up direction based on current rotation.
    XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
    // Rebuild view matrix.
    viewMatrix = XMMatrixLookAtLH(posVector, camTarget, upDir);

    UpdateDirectionVectors();
}