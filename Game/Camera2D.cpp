#include "pch.h"
#include "Camera2D.h"

using namespace DirectX;

Camera2D::Camera2D()
{
    pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    posVector = XMLoadFloat3(&pos);
    rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    rotVector = XMLoadFloat3(&rot);
    UpdateMatrix();
}

void Camera2D::SetProjectionValues(float width, float height, float nearZ, float farZ)
{
    orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, width, height, 0.0f, nearZ, farZ);
}

DirectX::XMMATRIX const& Camera2D::GetOrthoMatrix() const
{
    return orthoMatrix;
}

DirectX::XMMATRIX const& Camera2D::GetWorldMatrix() const
{
    return worldMatrix;
}

void Camera2D::UpdateMatrix()
{
    XMMATRIX translationOffsetMatrix = XMMatrixTranslation(-pos.x, -pos.y, 0.0f);
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
    worldMatrix = camRotationMatrix * translationOffsetMatrix;
}