//
// Camera.h
//

#pragma once

using namespace DirectX;

class Camera
{
    public:
        Camera();

        void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

        XMMATRIX const& GetViewMatrix() const;
        XMMATRIX const& GetProjectionMatrix() const;

        XMVECTOR const& GetPositionVector() const;
        XMFLOAT3 const& GetPositionFloat3() const;
        XMVECTOR const& GetRotationVector() const;
        XMFLOAT3 const& GetRotationFloat3() const;

        void SetPosition(XMVECTOR const& pos);
        void SetPosition(float x, float y, float z);
        void AdjustPosition(XMVECTOR const& pos);
        void AdjustPosition(float x, float y, float z);
        void SetRotation(XMVECTOR const& rot);
        void SetRotation(float x, float y, float z);
        void AdjustRotation(XMVECTOR const& rot);
        void AdjustRotation(float x, float y, float z);
        void SetLookAtPos(XMFLOAT3 lookAtPos);

        XMVECTOR const& GetForwardVector();
        XMVECTOR const& GetRightVector();
        XMVECTOR const& GetBackwardVector();
        XMVECTOR const& GetLeftVector();

    private:
        void UpdateViewMatrix();

    private:
        XMVECTOR posVector;
        XMVECTOR rotVector;
        XMFLOAT3 pos;
        XMFLOAT3 rot;
        XMMATRIX viewMatrix;
        XMMATRIX projectionMatrix;

        XMVECTOR const DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        XMVECTOR const DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        XMVECTOR const DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
        XMVECTOR const DEFAULT_LEFT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR const DEFAULT_RIGHT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);

        XMVECTOR vec_forward;
        XMVECTOR vec_left;
        XMVECTOR vec_right;
        XMVECTOR vec_backward;
        XMVECTOR vec_up;
};

