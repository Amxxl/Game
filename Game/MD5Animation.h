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
            return m_iNumJoints;
        }

        JointInfo const& GetJointInfo(unsigned int index) const
        {
            assert(index < jointInfo.size());
            return jointInfo[index];
        }

        void QuaternionComputeW(DirectX::XMFLOAT4& q);

        std::vector<JointInfo> jointInfo;
        std::vector<BoundingBox> frameBounds;
        std::vector<Joint> baseFrameJoints;
        std::vector<FrameData> frameData;
        std::vector<std::vector<Joint>> frameSkeleton;

        int m_iMD5Version;
        int m_iNumFrames;
        int m_iNumJoints;
        int m_iFrameRate;
        int m_iNumAnimatedComponents;

        float frameTime;
        float totalAnimTime;
        float currAnimTime;
};