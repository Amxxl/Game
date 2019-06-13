//
// GameObject.cpp
//

#include "pch.h"
#include "GameObject.h"

using namespace DirectX;

XMVECTOR const& GameObject::GetPositionVector() const
{
    return posVector;
}

XMFLOAT3 const& GameObject::GetPositionFloat3() const
{
    return pos;
}

XMVECTOR const& GameObject::GetRotationVector() const
{
    return rotVector;
}

XMFLOAT3 const& GameObject::GetRotationFloat3() const
{
    return rot;
}

void GameObject::SetPosition(XMVECTOR const& pos)
{
    XMStoreFloat3(&this->pos, pos);
    posVector = pos;
    UpdateMatrix();
}

void GameObject::SetPosition(XMFLOAT3 const& pos)
{
    this->pos = pos;
    posVector = XMLoadFloat3(&this->pos);
    UpdateMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
    pos = XMFLOAT3(x, y, z);
    posVector = XMLoadFloat3(&pos);
    UpdateMatrix();
}

void GameObject::AdjustPosition(XMVECTOR const& pos)
{
    posVector += pos;
    XMStoreFloat3(&this->pos, this->posVector);
    UpdateMatrix();
}

void GameObject::AdjustPosition(XMFLOAT3 const& pos)
{
    this->pos.x += pos.x;
    this->pos.y += pos.y;
    this->pos.z += pos.z;
    posVector = XMLoadFloat3(&this->pos);
    UpdateMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
    pos.x += x;
    pos.y += y;
    pos.z += z;
    posVector = XMLoadFloat3(&pos);
    UpdateMatrix();
}

void GameObject::SetRotation(XMVECTOR const& rot)
{
    rotVector = rot;
    XMStoreFloat3(&this->rot, rot);
    UpdateMatrix();
}

void GameObject::SetRotation(XMFLOAT3 const& rot)
{
    this->rot = rot;
    rotVector = XMLoadFloat3(&this->rot);
    UpdateMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
    this->rot = XMFLOAT3(x, y, z);
    rotVector = XMLoadFloat3(&this->rot);
    UpdateMatrix();
}

void GameObject::AdjustRotation(XMVECTOR const& rot)
{
    rotVector += rot;
    XMStoreFloat3(&this->rot, rotVector);
    UpdateMatrix();
}

void GameObject::AdjustRotation(XMFLOAT3 const& rot)
{
    this->rot.x += rot.x;
    this->rot.y += rot.y;
    this->rot.z += rot.z;
    rotVector = XMLoadFloat3(&this->rot);
    UpdateMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
    rot.x += x;
    rot.y += y;
    rot.z += z;
    rotVector = XMLoadFloat3(&this->rot);
    UpdateMatrix();
}

void GameObject::SetLookAtPos(XMFLOAT3 lookAtPos)
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

XMVECTOR const& GameObject::GetForwardVector()
{
    return vec_forward;
}

XMVECTOR const& GameObject::GetRightVector()
{
    return vec_right;
}

XMVECTOR const& GameObject::GetBackwardVector()
{
    return vec_backward;
}

XMVECTOR const& GameObject::GetLeftVector()
{
    return vec_left;
}

void GameObject::UpdateMatrix()
{
    assert("UpdateMatrix() function must be overriden." && 0);
}