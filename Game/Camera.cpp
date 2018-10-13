//
// Camera.cpp
//

#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
    this->pos = XMFLOAT3(1.5f, 1.5f, 1.5f);
    this->posVector = XMLoadFloat3(&this->pos);
    this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
    this->projectionMatrix = XMMatrixPerspectiveFovRH(fovRadians, aspectRatio, nearZ, farZ);
}

XMMATRIX const & Camera::GetViewMatrix() const
{
    return this->viewMatrix;
}

XMMATRIX const & Camera::GetProjectionMatrix() const
{
    return this->projectionMatrix;
}

XMVECTOR const & Camera::GetPositionVector() const
{
    return this->posVector;
}

XMFLOAT3 const & Camera::GetPositionFloat3() const
{
    return this->pos;
}

XMVECTOR const & Camera::GetRotationVector() const
{
    return this->rotVector;
}

XMFLOAT3 const & Camera::GetRotationFloat3() const
{
    return this->rot;
}

void Camera::SetPosition(XMVECTOR const& pos)
{
    XMStoreFloat3(&this->pos, pos);
    this->posVector = pos;
    this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
    this->pos = XMFLOAT3(x, y, z);
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateViewMatrix();
}

void Camera::AdjustPosition(XMVECTOR const& pos)
{
    this->posVector += pos;
    XMStoreFloat3(&this->pos, this->posVector);
    this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateViewMatrix();
}

void Camera::SetRotation(XMVECTOR const& rot)
{
    this->rotVector = rot;
    XMStoreFloat3(&this->rot, rot);
    this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
    this->rot = XMFLOAT3(x, y, z);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateViewMatrix();
}

void Camera::AdjustRotation(XMVECTOR const& rot)
{
    this->rotVector += rot;
    XMStoreFloat3(&this->rot, this->rotVector);
    this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
{
    // Verify that look at pos is not the same as cam pos. They connot be the same as that wouldn't make sense and would result in undefined behavior.
    if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
        return;

    lookAtPos.x = this->pos.x - lookAtPos.x;
    lookAtPos.y = this->pos.y - lookAtPos.y;
    lookAtPos.z = this->pos.z - lookAtPos.z;

    float pitch = 0.0f;
    if (lookAtPos.y != 0.0f)
    {
        const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
        pitch = atan(lookAtPos.y / distance);
    }

    float yaw = 0.0f;
    if (lookAtPos.x != 0.0f)
    {
        yaw = atan(lookAtPos.x / lookAtPos.z);
    }

    if (lookAtPos.z > 0)
        yaw += XM_PI;


    this->SetRotation(pitch, yaw, 0.0f);
}

XMVECTOR const& Camera::GetForwardVector()
{
    return this->vec_forward;
}

XMVECTOR const& Camera::GetRightVector()
{
    return this->vec_right;
}

XMVECTOR const& Camera::GetBackwardVector()
{
    return this->vec_backward;
}

XMVECTOR const& Camera::GetLeftVector()
{
    return this->vec_left;
}

void Camera::UpdateViewMatrix()
{
    // Calculate camera rotation matrix.
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
    // Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix.
    XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    // Adjust cam target to be offset by the camera's current position
    camTarget += this->posVector;
    // Calculate up direction based on current rotation.
    XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    // Rebuild view matrix.
    this->viewMatrix = XMMatrixLookAtRH(this->posVector, camTarget, upDir);

    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

}
