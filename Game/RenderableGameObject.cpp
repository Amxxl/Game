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
    SetPosition(5.0f, 0.0f, 5.0f);
    SetRotation(0.0f, 0.0f, 0.0f);
}

void RenderableGameObject::LoadMesh(ID3D11DeviceContext* deviceContext, std::wstring const& fileName)
{
    model.LoadMesh(deviceContext, fileName);
}

void RenderableGameObject::LoadAnim(std::wstring const& fileName)
{
    model.LoadAnim(fileName);
}

void RenderableGameObject::Update(ID3D11DeviceContext* deviceContext, float deltaTime, int index)
{
    model.Update(deviceContext, deltaTime, index);
}

void RenderableGameObject::Draw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX const& viewMatrix, DirectX::XMMATRIX const& projMatrix)
{
    model.Draw(deviceContext, m_worldMatrix, viewMatrix, projMatrix);
}

void RenderableGameObject::UpdateMatrix()
{
    m_worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y - DirectX::XMConvertToRadians(90.0f), rot.z) * DirectX::XMMatrixScaling(0.10f, 0.10f, 0.10f) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    UpdateDirectionVectors();
}