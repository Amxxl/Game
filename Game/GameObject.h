//
// GameObject.h
//

#pragma once

class GameObject
{
    public:
        GameObject() = default;
        virtual ~GameObject() {}

        DirectX::XMVECTOR const& GetPositionVector() const;
        DirectX::XMFLOAT3 const& GetPositionFloat3() const;
        DirectX::XMVECTOR const& GetRotationVector() const;
        DirectX::XMFLOAT3 const& GetRotationFloat3() const;

        void SetPosition(DirectX::XMVECTOR const& pos);
        void SetPosition(DirectX::XMFLOAT3 const& pos);
        void SetPosition(float x, float y, float z);
        void AdjustPosition(DirectX::XMVECTOR const& pos);
        void AdjustPosition(DirectX::XMFLOAT3 const& pos);
        void AdjustPosition(float x, float y, float z);
        void SetRotation(DirectX::XMVECTOR const& rot);
        void SetRotation(DirectX::XMFLOAT3 const& rot);
        void SetRotation(float x, float y, float z);
        void AdjustRotation(DirectX::XMVECTOR const& rot);
        void AdjustRotation(DirectX::XMFLOAT3 const& rot);
        void AdjustRotation(float x, float y, float z);
        void SetLookAtPos(DirectX::XMFLOAT3 lookAtPos);

        DirectX::XMVECTOR const& GetForwardVector();
        DirectX::XMVECTOR const& GetRightVector();
        DirectX::XMVECTOR const& GetBackwardVector();
        DirectX::XMVECTOR const& GetLeftVector();

    protected:
        virtual void UpdateMatrix();
        
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 rot;
        DirectX::XMVECTOR posVector;
        DirectX::XMVECTOR rotVector;

        DirectX::XMVECTOR vec_forward;
        DirectX::XMVECTOR vec_left;
        DirectX::XMVECTOR vec_right;
        DirectX::XMVECTOR vec_backward;

        DirectX::XMVECTOR const DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        DirectX::XMVECTOR const DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        DirectX::XMVECTOR const DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
        DirectX::XMVECTOR const DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
        DirectX::XMVECTOR const DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
};