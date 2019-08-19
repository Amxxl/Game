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

void GameObject::UpdateMatrix()
{
    assert("UpdateMatrix() function must be overriden." && 0);
}
