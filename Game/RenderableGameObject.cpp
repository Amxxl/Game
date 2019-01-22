#include "pch.h"
#include "RenderableGameObject.h"


RenderableGameObject::RenderableGameObject()
{
}


RenderableGameObject::~RenderableGameObject()
{
}

void RenderableGameObject::Initialize(ID3D11DeviceContext* deviceContext)
{
    model.LoadMesh(deviceContext, L"Data/wraith.md5mesh");
    model.LoadAnim(L"Data/idle.md5anim");
    model.LoadAnim(L"Data/walk.md5anim");
    model.LoadAnim(L"Data/run.md5anim");
    model.LoadAnim(L"Data/attack1.md5anim");
    model.LoadAnim(L"Data/attack2.md5anim");
    model.LoadAnim(L"Data/attack3.md5anim");

    SetPosition(5.0f, 0.0f, 5.0f);
    SetRotation(0.0f, 0.0f, 0.0f);
}

void RenderableGameObject::Update(ID3D11DeviceContext* deviceContext, float deltaTime, int index)
{
    model.Update(deviceContext, deltaTime, index);
}

void RenderableGameObject::Draw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX const& viewMatrix, DirectX::XMMATRIX const& projMatrix)
{
    model.Draw(deviceContext, worldMatrix, viewMatrix, projMatrix);
}

void RenderableGameObject::UpdateMatrix()
{
    this->worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * DirectX::XMMatrixScaling(0.15f, 0.15f, 0.15f) * DirectX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
    DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}
