//
// Camera.cpp
//

#include "pch.h"
#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
    this->pos = XMFLOAT3(1.5f, 1.5f, 1.5f);
    this->posVector = XMLoadFloat3(&this->pos);
    this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
    this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

XMMATRIX const& Camera::GetViewMatrix() const
{
    return this->viewMatrix;
}

XMMATRIX const& Camera::GetProjectionMatrix() const
{
    return this->projectionMatrix;
}

void Camera::UpdateMatrix()
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

    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
    vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}