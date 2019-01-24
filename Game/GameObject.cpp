//
// GameObject.cpp
//

#include "pch.h"
#include "GameObject.h"

using namespace DirectX;

XMVECTOR const& GameObject::GetPositionVector() const
{
    return this->posVector;
}

XMFLOAT3 const& GameObject::GetPositionFloat3() const
{
    return this->pos;
}

XMVECTOR const& GameObject::GetRotationVector() const
{
    return this->rotVector;
}

XMFLOAT3 const& GameObject::GetRotationFloat3() const
{
    return this->rot;
}

void GameObject::SetPosition(XMVECTOR const& pos)
{
    XMStoreFloat3(&this->pos, pos);
    this->posVector = pos;
    this->UpdateMatrix();
}

void GameObject::SetPosition(XMFLOAT3 const& pos)
{
    this->pos = pos;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
    this->pos = XMFLOAT3(x, y, z);
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateMatrix();
}

void GameObject::AdjustPosition(XMVECTOR const& pos)
{
    this->posVector += pos;
    XMStoreFloat3(&this->pos, this->posVector);
    this->UpdateMatrix();
}

void GameObject::AdjustPosition(XMFLOAT3 const& pos)
{
    this->pos.x += pos.x;
    this->pos.y += pos.y;
    this->pos.z += pos.z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateMatrix();
}

void GameObject::SetRotation(XMVECTOR const& rot)
{
    this->rotVector = rot;
    XMStoreFloat3(&this->rot, rot);
    this->UpdateMatrix();
}

void GameObject::SetRotation(XMFLOAT3 const& rot)
{
    this->rot = rot;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
    this->rot = XMFLOAT3(x, y, z);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateMatrix();
}

void GameObject::AdjustRotation(XMVECTOR const& rot)
{
    this->rotVector += rot;
    XMStoreFloat3(&this->rot, this->rotVector);
    this->UpdateMatrix();
}

void GameObject::AdjustRotation(XMFLOAT3 const& rot)
{
    this->rot.x += rot.x;
    this->rot.y += rot.y;
    this->rot.z += rot.z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateMatrix();
}

void GameObject::SetLookAtPos(XMFLOAT3 lookAtPos)
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

XMVECTOR const& GameObject::GetForwardVector()
{
    return this->vec_forward;
}

XMVECTOR const& GameObject::GetRightVector()
{
    return this->vec_right;
}

XMVECTOR const& GameObject::GetBackwardVector()
{
    return this->vec_backward;
}

XMVECTOR const& GameObject::GetLeftVector()
{
    return this->vec_left;
}

void GameObject::UpdateMatrix()
{
    assert("UpdateMatrix() function must be overriden." && 0);
}