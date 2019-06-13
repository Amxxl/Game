#include "pch.h"
#include "ThirdPersonCamera.h"


ThirdPersonCamera::ThirdPersonCamera()
{
}

void ThirdPersonCamera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
    projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

DirectX::XMMATRIX const& ThirdPersonCamera::GetViewMatrix() const
{
    return viewMatrix;
}

DirectX::XMMATRIX const& ThirdPersonCamera::GetProjectionMatrix() const
{
    return projectionMatrix;
}

void ThirdPersonCamera::UpdateMatrix()
{
    auto const pos = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
        DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f) * DirectX::XMMatrixTranslation(origin.x, origin.y + 2.0f, origin.z));
    
    DirectX::XMStoreFloat3(&cameraPos, pos);

    viewMatrix = DirectX::XMMatrixLookAtLH(pos, DirectX::XMVectorSet(origin.x, origin.y + 2.0f, origin.z, 0.0f),
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * DirectX::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);

    DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f);

    vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}