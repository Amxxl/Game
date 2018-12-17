//
// MD5Animation.cpp
//

#include "pch.h"
#include "MD5Animation.h"


MD5Animation::MD5Animation()
{
}


MD5Animation::~MD5Animation()
{
}

bool MD5Animation::LoadAnimation(std::wstring const& fileName)
{
    std::wifstream fileIn(fileName.c_str());
    if (!fileIn.is_open())
        return false;

    std::wstring data;

    while (!fileIn.eof())
    {
        fileIn >> data;

        if (data == L"MD5Version")
        {
            // Check version of .md5anim file.
            int MD5Version = 0;
            fileIn >> MD5Version;

            // MD5Version must be 10.
            if (MD5Version != 10)
                return false;
        }
        else if (data == L"commandline")
        {
            std::getline(fileIn, data); // Ignore rest of this line.
        }
        else if (data == L"numFrames")
        {
            fileIn >> numFrames;
        }
        else if (data == L"numJoints")
        {
            fileIn >> numJoints;
        }
        else if (data == L"frameRate")
        {
            fileIn >> frameRate;
        }
        else if (data == L"numAnimatedComponents")
        {
            fileIn >> numAnimatedComponents;
        }
        else if (data == L"hierarchy")
        {
            fileIn >> data; // Skip "{".

            for (int i = 0; i < numJoints; i++)
            {
                JointInfo joint;

                fileIn >> joint.m_Name; // Get joint name.

                if (joint.m_Name[joint.m_Name.size() - 1] != '"')
                {
                    wchar_t character;
                    bool found = false;
                    while (!found)
                    {
                        character = fileIn.get();

                        if (character == '"')
                            found = true;

                        joint.m_Name += character;
                    }
                }

                // Remove the quotation marks from joints name.
                joint.m_Name.erase(0, 1);
                joint.m_Name.erase(joint.m_Name.size() - 1, 1);

                fileIn >> joint.m_ParentId;
                fileIn >> joint.m_Flags;
                fileIn >> joint.m_StartIndex;

                // @todo: add check if joints exists in the model and the parentId match up.
                jointInfo.push_back(joint);

                std::getline(fileIn, data); // Skip the rest of this line.
            }
        }
        else if (data == L"bounds")
        {
            fileIn >> data; // Skip "{".

            for (int i = 0; i < numFrames; i++)
            {
                BoundingBox bb;

                fileIn >> data; // Skip "(".
                fileIn >> bb.min.x >> bb.min.z >> bb.min.y;
                fileIn >> data >> data; // Skip ") (".
                fileIn >> bb.max.x >> bb.max.z >> bb.max.y;
                fileIn >> data; // Skip ")"

                frameBounds.push_back(bb);
            }
        }
        else if (data == L"baseframe") // Default position for the animation.
        { // All frames will build their skeletons of this.
            fileIn >> data; // Skip "{".

            for (int i = 0; i < numJoints; i++)
            {
                Joint joint;

                fileIn >> data; // Skip "(".
                fileIn >> joint.m_Pos.x >> joint.m_Pos.z >> joint.m_Pos.y;
                fileIn >> data >> data; // Skip ") (".
                fileIn >> joint.m_Orient.x >> joint.m_Orient.z >> joint.m_Orient.y;
                fileIn >> data; // Skip ")".

                baseFrameJoints.push_back(joint);
            }
        }
        else if (data == L"frame")
        {
            FrameData frame;

            fileIn >> frame.m_iFrameId;
            fileIn >> data; // Skip "{".

            for (int i = 0; i < numAnimatedComponents; i++)
            {
                float tempData;
                fileIn >> tempData;
                frame.m_FrameData.push_back(tempData);
            }

            frameData.push_back(frame);
            
            this->BuildFrameSkeleton(frame);

            fileIn >> data; // Skip "}".
        }
    }

    // Calculate and store some usefull animation data
    frameTime = 1.0f / frameRate;
    totalAnimTime = numFrames * frameTime;
    currAnimTime = 0.0f;

    // @todo: add to animations list.
    return false;
}

void MD5Animation::BuildFrameSkeleton(FrameData const& frameData)
{
    // Build the frame skeleton.
    std::vector<Joint> skeleton;

    for (int i = 0; i < jointInfo.size(); i++)
    {
        unsigned int j = 0; // Keep track of position in frameData array.

                            // Start the frames joint with the base frame's joint
        Joint joint = baseFrameJoints[i];
        joint.m_iParentId = jointInfo[i].m_ParentId;

        if (jointInfo[i].m_Flags & 1) // m_Pos.x (000001)
            joint.m_Pos.x = frameData.m_FrameData[jointInfo[i].m_StartIndex + j++];

        if (jointInfo[i].m_Flags & 2) // m_Pos.y (000010)
            joint.m_Pos.z = frameData.m_FrameData[jointInfo[i].m_StartIndex + j++];

        if (jointInfo[i].m_Flags & 4) // m_Pos.z (000100)
            joint.m_Pos.y = frameData.m_FrameData[jointInfo[i].m_StartIndex + j++];

        if (jointInfo[i].m_Flags & 8) // m_Orient.x (001000)
            joint.m_Orient.x = frameData.m_FrameData[jointInfo[i].m_StartIndex + j++];

        if (jointInfo[i].m_Flags & 16) // m_Orient.y (010000)
            joint.m_Orient.z = frameData.m_FrameData[jointInfo[i].m_StartIndex + j++];

        if (jointInfo[i].m_Flags & 32) // m_Orient.z (100000)
            joint.m_Orient.y = frameData.m_FrameData[jointInfo[i].m_StartIndex + j++];

        QuaternionComputeW(joint.m_Orient);

        if (joint.m_iParentId >= 0)
        {
            Joint parentJoint = skeleton[joint.m_iParentId];

            // Turn the XMFLOAT3 and 4's into vectors for easier computation.
            DirectX::XMVECTOR parentJointOrientation = DirectX::XMVectorSet(parentJoint.m_Orient.x, parentJoint.m_Orient.y, parentJoint.m_Orient.z, parentJoint.m_Orient.w);
            DirectX::XMVECTOR jointPos = DirectX::XMVectorSet(joint.m_Pos.x, joint.m_Pos.y, joint.m_Pos.z, 0.0f);
            DirectX::XMVECTOR parentOrientationConjugate = DirectX::XMVectorSet(-parentJoint.m_Orient.x, -parentJoint.m_Orient.y, -parentJoint.m_Orient.z, parentJoint.m_Orient.w);

            // Calculate current joints position relative to it's parents position.
            DirectX::XMFLOAT3 rotatedPos;
            DirectX::XMStoreFloat3(&rotatedPos, DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(parentJointOrientation, jointPos), parentOrientationConjugate));

            // Translate the joint to model space by adding the parent joint's pos to it.
            joint.m_Pos.x = rotatedPos.x + parentJoint.m_Pos.x;
            joint.m_Pos.y = rotatedPos.y + parentJoint.m_Pos.y;
            joint.m_Pos.z = rotatedPos.z + parentJoint.m_Pos.z;

            // Currently the joint is oriented in it's parent joints space, we now need to orient it in
            // model space by multiplying the two orientations together (parentOrientation & childOrientation) <- In that order.
            DirectX::XMVECTOR jointOrient = DirectX::XMVectorSet(joint.m_Orient.x, joint.m_Orient.y, joint.m_Orient.z, joint.m_Orient.w);
            jointOrient = DirectX::XMQuaternionMultiply(parentJointOrientation, jointOrient);

            // Normalize the orientation quaternion.
            jointOrient = DirectX::XMQuaternionNormalize(jointOrient);

            DirectX::XMStoreFloat4(&joint.m_Orient, jointOrient);
        }

        // Store the joint into our temporary frame skeleton.
        skeleton.push_back(joint);
    }

    // Push back our newly created frame skeleton into the animation's frameSkeleton array.
    frameSkeleton.push_back(skeleton);
}

void MD5Animation::QuaternionComputeW(DirectX::XMFLOAT4& q)
{
    float t = 1.0f - (q.x * q.x) - (q.y * q.y) - (q.z * q.z);
    t < 0.0f ? q.w = 0.0f : q.w = -sqrtf(t);
}
