#pragma once

#include "GameObject.h"
#include "MD5Model.h"
#include "StepTimer.h"

class RenderableGameObject : public GameObject
{
    public:
        RenderableGameObject();
        ~RenderableGameObject();

        void Initialize(ID3D11DeviceContext* deviceContext);

        void LoadMesh(ID3D11DeviceContext* deviceContext, std::wstring const& fileName);
        void LoadAnim(std::wstring const& fileName); 

        void Update(ID3D11DeviceContext* deviceContext, float deltaTime, int index);
        void Draw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX const& viewMatrix, DirectX::XMMATRIX const& projMatrix);

    private:
        void UpdateMatrix() override;
    
    private:
        DirectX::XMMATRIX m_worldMatrix = DirectX::XMMatrixIdentity();
        MD5Model model;
};