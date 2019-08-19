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

    protected:
        virtual void UpdateMatrix();
        
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 rot;
        DirectX::XMVECTOR posVector;
        DirectX::XMVECTOR rotVector;
};