//
// MD5Model.h
//

#pragma once

#include "MD5ModelShader.h"
#include "MD5Loader.h"

class MD5Model
{
    public:
        MD5Model();
        ~MD5Model();
        
        bool LoadMesh(ID3D11DeviceContext* deviceContext, std::wstring const& fileName);
        bool LoadAnim(std::wstring const& fileName);

        void Update(ID3D11DeviceContext* deviceContext, float deltaTime, int index);
        void Draw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);

    private:
        axec::md5_model_t model;
        MD5ModelShader shader;
        int anim_index;
};