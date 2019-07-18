#pragma once

#include "DynamicVertexBuffer.h"
#include "IndexBuffer.h"
#include "MD5Vertex.h"

namespace axec//alibur
{
    struct Weight
    {
        int jointId;
        float bias;
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
    };

    struct JointInfo
    {
        std::wstring name;
        int parentId;
        int flags;
        int startIndex;
    };

    struct Joint
    {
        std::wstring name;
        int parentId;
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 orientation;
    };

    struct FrameData
    {
        int frameId;
        std::vector<float> frameData;
    };

    struct BoundingBox
    {
        DirectX::XMFLOAT3 min;
        DirectX::XMFLOAT3 max;
    };

    struct md5_mesh_t
    {
        std::wstring shader;

        std::vector<MD5Vertex> vertices;

        unsigned int trianglesCount;
        std::vector<DWORD> indices;

        std::vector<Weight> weights;

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
        DX::DynamicVertexBuffer<MD5Vertex> vertexBuffer;
        Bind::IndexBuffer<DWORD> indexBuffer;
    };

    struct md5_anim_t
    {
        int numFrames;
        int numJoints;
        int frameRate;
        int numAnimatedComponents;

        float frameTime;
        float totalAnimTime;
        float currAnimTime;

        std::vector<JointInfo> jointInfo;
        std::vector<BoundingBox> frameBounds;
        std::vector<Joint> baseFrameJoints;
        std::vector<FrameData> frameData;
        std::vector<std::vector<Joint>> frameSkeleton;
    };

    struct md5_model_t
    {
        int numJoints;
        int numMeshes;

        std::vector<Joint> joints;
        std::vector<md5_mesh_t> meshes;

        bool HasAnimations;
        std::vector<md5_anim_t> animations;
    };

    class MD5Loader
    {
        public:
            static bool LoadMD5Mesh(ID3D11DeviceContext* deviceContext, std::wstring const& fileName, md5_model_t& model);
            static bool LoadMD5Anim(std::wstring const& fileName, md5_model_t& model);

        private:
            static void PrepareMesh(md5_mesh_t& mesh, md5_model_t& model);
            static void PrepareNormals(md5_mesh_t& mesh, md5_model_t& model);
            static void BuildFrameSkeleton(md5_anim_t& animation, FrameData const& frameData);
            static bool CheckAnimation(md5_model_t const& model, md5_anim_t const& animation);
            static void QuaternionComputeW(DirectX::XMFLOAT4& q);
    };
}