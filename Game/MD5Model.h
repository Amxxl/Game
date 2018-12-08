//
// MD5Model.h
//

#pragma once

#include "MD5Vertex.h"
#include "MD5ModelShader.h"
#include "DynamicVertexBuffer.h"
#include "IndexBuffer.h"


class MD5Model
{
    public:
        MD5Model();
        ~MD5Model();
        
        bool LoadModel(ID3D11DeviceContext* deviceContext, std::wstring const& fileName);
        void SetMatrices(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
        void Render(ID3D11DeviceContext* deviceContext);

    protected:
        struct Weight
        {
            int m_JointId;
            float m_Bias;
            DirectX::XMFLOAT3 m_Pos;
            DirectX::XMFLOAT3 m_Normal;
        };

        struct Joint
        {
            std::wstring m_Name;
            int m_ParentId;
            DirectX::XMFLOAT3 m_Pos;
            DirectX::XMFLOAT4 m_Orient;
        };

        struct Mesh
        {
            std::wstring m_Shader;

            std::vector<MD5Vertex> vertices;

            unsigned int trianglesCount;
            std::vector<DWORD> indices;
            
            std::vector<Weight> m_Weights;

            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
            DynamicVertexBuffer<MD5Vertex>* vertexBuffer;
            IndexBuffer<DWORD>* indexBuffer;
        };

        void PrepareMesh(Mesh& mesh);
        void PrepareNormals(Mesh& mesh);

    private:
        void QuaternionComputeW(DirectX::XMFLOAT4& q);

        int m_iMD5Version;
        int m_iNumJoints;
        int m_iNumMeshes;

        std::vector<Joint> m_Joints;
        std::vector<Mesh> m_Meshes;

        MD5ModelShader shader;
};

