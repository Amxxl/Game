#pragma once

#include "GameObject.h"

class GameObject3D : public GameObject
{
    public:
        void SetLookAtPos(DirectX::XMFLOAT3 lookAtPos);

        DirectX::XMVECTOR const& GetForwardVector(bool omitY = false);
        DirectX::XMVECTOR const& GetRightVector(bool omitY = false);
        DirectX::XMVECTOR const& GetBackwardVector(bool omitY = false);
        DirectX::XMVECTOR const& GetLeftVector(bool omitY = false);

    protected:
        virtual void UpdateMatrix();

        void UpdateDirectionVectors();

        DirectX::XMVECTOR vec_forward;
        DirectX::XMVECTOR vec_left;
        DirectX::XMVECTOR vec_right;
        DirectX::XMVECTOR vec_backward;

        DirectX::XMVECTOR vec_forward_omitY;
        DirectX::XMVECTOR vec_left_omitY;
        DirectX::XMVECTOR vec_right_omitY;
        DirectX::XMVECTOR vec_backward_omitY;

        DirectX::XMVECTOR const DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        DirectX::XMVECTOR const DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        DirectX::XMVECTOR const DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
        DirectX::XMVECTOR const DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
        DirectX::XMVECTOR const DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
};

