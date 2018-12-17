//
// MD5Animation.h
//

#pragma once

class MD5Animation
{
    public:
        MD5Animation();
        ~MD5Animation();

        bool LoadAnimation(std::wstring const& fileName);

        struct JointInfo
        {
            std::wstring m_Name;
            int m_ParentId;
            int m_Flags;
            int m_StartIndex;
        };

        struct Joint
        {
            int m_iParentId;
            DirectX::XMFLOAT3 m_Pos;
            DirectX::XMFLOAT4 m_Orient;
        };

        struct FrameData
        {
            int m_iFrameId;
            std::vector<float> m_FrameData;
        };

        struct BoundingBox
        {
            DirectX::XMFLOAT3 min;
            DirectX::XMFLOAT3 max;
        };

        int GetNumJoints() const
        {
            return numJoints;
        }

        JointInfo const& GetJointInfo(unsigned int index) const
        {
            assert(index < jointInfo.size());
            return jointInfo[index];
        }

        void BuildFrameSkeleton(FrameData const& frameData);
        void QuaternionComputeW(DirectX::XMFLOAT4& q);

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