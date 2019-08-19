#include "pch.h"
#include "GameObject3D.h"

using namespace DirectX;

void GameObject3D::SetLookAtPos(XMFLOAT3 lookAtPos)
{
    // Verify that look at pos is not the same as cam pos. They connot be the same as that wouldn't make sense and would result in undefined behavior.
    if (lookAtPos.x == pos.x && lookAtPos.y == pos.y && lookAtPos.z == pos.z)
        return;

    lookAtPos.x = pos.x - lookAtPos.x;
    lookAtPos.y = pos.y - lookAtPos.y;
    lookAtPos.z = pos.z - lookAtPos.z;

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


    SetRotation(pitch, yaw, 0.0f);
}

XMVECTOR const& GameObject3D::GetForwardVector(bool omitY)
{
    return omitY ? vec_forward_omitY : vec_forward;
}

XMVECTOR const& GameObject3D::GetRightVector(bool omitY)
{
    return omitY ? vec_right_omitY : vec_right;
}

XMVECTOR const& GameObject3D::GetBackwardVector(bool omitY)
{
    return omitY ? vec_backward_omitY : vec_backward;
}

XMVECTOR const& GameObject3D::GetLeftVector(bool omitY)
{
    return omitY ? vec_left_omitY : vec_left;
}

void GameObject3D::UpdateMatrix()
{
    assert("UpdateMatrix() function must be overridden!", && false);
}

void GameObject3D::UpdateDirectionVectors()
{
    DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
    vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

    DirectX::XMMATRIX vecRotationMatrixOmitY = DirectX::XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
    vec_forward_omitY = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrixOmitY);
    vec_backward_omitY = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrixOmitY);
    vec_left_omitY = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrixOmitY);
    vec_right_omitY = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrixOmitY);
}