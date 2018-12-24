#include "pch.h"
#include "MD5Loader.h"

using namespace axec;


bool MD5Loader::LoadMD5Mesh(ID3D11DeviceContext* deviceContext, std::wstring const& fileName, md5_model_t& model)
{
    ID3D11Device* device = DX::GetDevice(deviceContext);
    if (device == nullptr)
        return false;

    std::wifstream fileIn(fileName.c_str());
    if (!fileIn.is_open())
        return false;

    std::wstring data;

    while (!fileIn.eof())
    {
        fileIn >> data;

        if (data == L"MD5Version")
        {
            // Check version of .md5mesh file.
            int MD5Version = 0;
            fileIn >> MD5Version;

            // MD5 Version must be 10.
            if (MD5Version != 10)
                return false;
        }
        else if (data == L"commandline")
        {
            std::getline(fileIn, data); // Ignore the rest of this line.
        }
        else if (data == L"numJoints")
        {
            fileIn >> model.numJoints; // Store number of joints.
        }
        else if (data == L"numMeshes")
        {
            fileIn >> model.numMeshes; // Store number of meshes.
        }
        else if (data == L"joints")
        {
            Joint joint;

            fileIn >> data; // Skip the "{"

            for (int i = 0; i < model.numJoints; i++)
            {
                fileIn >> joint.name;

                // Sometimes the names might contain spaces. If that is the case, we need to continue
                // to read the name until we get to the closing " (quatation mark).
                if (joint.name[joint.name.size() - 1] != '"')
                {
                    wchar_t character;
                    bool found = false;
                    while (!found)
                    {
                        character = fileIn.get();

                        if (character == '"')
                            found = true;

                        joint.name += character;
                    }
                }

                fileIn >> joint.parentId; // Store Parent joint Id.
                fileIn >> data; // Skip the "("

                // Store position of this joint.
                fileIn >> joint.position.x >> joint.position.z >> joint.position.y;
                fileIn >> data >> data; // Skip the ")" and "("

                // Store orientation of this joint
                fileIn >> joint.orientation.x >> joint.orientation.z >> joint.orientation.y;

                // Remove quotation marks from joints name.
                joint.name.erase(0, 1);
                joint.name.erase(joint.name.size() - 1, 1);

                // Compute the w axis of the quaternion.
                QuaternionComputeW(joint.orientation);

                std::getline(fileIn, data); // Skip rest of this lane.
                model.joints.push_back(joint); // Store the joint into joints vector.
            }

            fileIn >> data; // Skip the "}"
        }
        else if (data == L"mesh")
        {
            md5_mesh_t mesh;
            int numVerts, numTris, numWeights;

            fileIn >> data; // Skip the "{"

            fileIn >> data;

            while (data != L"}") // Read until "}"
            {
                if (data == L"shader")
                {
                    std::wstring filePath;
                    fileIn >> filePath;

                    // Take spaces into account if file path has a space in it.
                    if (filePath[filePath.size() - 1] != '"')
                    {
                        wchar_t character;
                        bool found = false;

                        while (!found)
                        {
                            character = fileIn.get();

                            if (character == '"')
                                found = true;

                            filePath += character;
                        }
                    }

                    // Remove the quotation marks from texture path
                    filePath.erase(0, 1);
                    filePath.erase(filePath.size() - 1, 1);

                    mesh.shader = filePath;

                    // Load texture.
                    DirectX::CreateWICTextureFromFile(device, mesh.shader.c_str(), nullptr, mesh.texture.ReleaseAndGetAddressOf());

                    std::getline(fileIn, data); // Skip rest of this line.
                }
                else if (data == L"numverts")
                {
                    fileIn >> numVerts;
                    std::getline(fileIn, data); // Skip rest of this line.

                    for (int i = 0; i < numVerts; i++)
                    {
                        MD5Vertex vertex;

                        fileIn >> data >> data >> data; // Skip "vert # ("

                                                        // Store texture coordinates
                        fileIn >> vertex.textureCoordinate.x >> vertex.textureCoordinate.y;

                        fileIn >> data; // Skip ")"
                        fileIn >> vertex.StartWeight; // Index of first weight this vert will be weighted to
                        fileIn >> vertex.WeightCount; // Number of weights for this vertex.

                        std::getline(fileIn, data); // Skip rest of this lane.

                        mesh.vertices.push_back(vertex); // Push back this vertex into mesh vertices vector.
                    }
                }
                else if (data == L"numtris")
                {
                    fileIn >> numTris;
                    mesh.trianglesCount = numTris;

                    std::getline(fileIn, data); // Skip rest of this lane.

                    for (int i = 0; i < numTris; i++)
                    {
                        DWORD index;
                        fileIn >> data >> data; // Skip "tri #".

                        for (int k = 0; k < 3; k++)
                        {
                            fileIn >> index;
                            mesh.indices.push_back(index);
                        }

                        std::getline(fileIn, data); // Skip rest of this lane.
                    }
                }
                else if (data == L"numweights")
                {
                    fileIn >> numWeights;

                    std::getline(fileIn, data); // Skip rest of this lane.

                    for (int i = 0; i < numWeights; i++)
                    {
                        Weight weight;
                        fileIn >> data >> data; // Skip "weight #"

                        fileIn >> weight.jointId;
                        fileIn >> weight.bias;
                        fileIn >> data; // Skip "(".

                        // Store weight's pos in joint's local space
                        fileIn >> weight.position.x >> weight.position.z >> weight.position.y;

                        std::getline(fileIn, data); // Skip rest of this lane.

                        mesh.weights.push_back(weight); // Push back weight into mesh weight vector.
                    }
                }
                else
                    std::getline(fileIn, data); // Skip anything else.

                fileIn >> data; // Skip "}".
            }

            PrepareMesh(mesh, model);
            PrepareNormals(mesh, model);

            // Create vertex and index buffers for this mesh.
            mesh.vertexBuffer.Create(device, &mesh.vertices[0], static_cast<UINT>(mesh.vertices.size()));
            mesh.indexBuffer.Create(device, &mesh.indices[0], mesh.trianglesCount * 3);


            model.meshes.push_back(mesh); // Store mesh in model's meshes vector.
        }
    }
    return true;
}

bool MD5Loader::LoadMD5Anim(std::wstring const& fileName, md5_model_t& model)
{
    std::wifstream fileIn(fileName.c_str());
    if (!fileIn.is_open())
        return false;

    std::wstring data;
    md5_anim_t animation;

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
            fileIn >> animation.numFrames;
        }
        else if (data == L"numJoints")
        {
            fileIn >> animation.numJoints;
        }
        else if (data == L"frameRate")
        {
            fileIn >> animation.frameRate;
        }
        else if (data == L"numAnimatedComponents")
        {
            fileIn >> animation.numAnimatedComponents;
        }
        else if (data == L"hierarchy")
        {
            fileIn >> data; // Skip "{".

            for (int i = 0; i < animation.numJoints; i++)
            {
                JointInfo joint;

                fileIn >> joint.name; // Get joint name.

                if (joint.name[joint.name.size() - 1] != '"')
                {
                    wchar_t character;
                    bool found = false;
                    while (!found)
                    {
                        character = fileIn.get();

                        if (character == '"')
                            found = true;

                        joint.name += character;
                    }
                }

                // Remove the quotation marks from joints name.
                joint.name.erase(0, 1);
                joint.name.erase(joint.name.size() - 1, 1);

                fileIn >> joint.parentId;
                fileIn >> joint.flags;
                fileIn >> joint.startIndex;

                // Add joint to the joint vector.
                animation.jointInfo.push_back(joint);

                std::getline(fileIn, data); // Skip the rest of this line.
            }
        }
        else if (data == L"bounds")
        {
            fileIn >> data; // Skip "{".

            for (int i = 0; i < animation.numFrames; i++)
            {
                BoundingBox bb;

                fileIn >> data; // Skip "(".
                fileIn >> bb.min.x >> bb.min.z >> bb.min.y;
                fileIn >> data >> data; // Skip ") (".
                fileIn >> bb.max.x >> bb.max.z >> bb.max.y;
                fileIn >> data; // Skip ")"

                animation.frameBounds.push_back(bb);
            }
        }
        else if (data == L"baseframe") // Default position for the animation.
        { // All frames will build their skeletons of this.
            fileIn >> data; // Skip "{".

            for (int i = 0; i < animation.numJoints; i++)
            {
                Joint joint;

                fileIn >> data; // Skip "(".
                fileIn >> joint.position.x >> joint.position.z >> joint.position.y;
                fileIn >> data >> data; // Skip ") (".
                fileIn >> joint.orientation.x >> joint.orientation.z >> joint.orientation.y;
                fileIn >> data; // Skip ")".

                animation.baseFrameJoints.push_back(joint);
            }
        }
        else if (data == L"frame")
        {
            FrameData frame;

            fileIn >> frame.frameId;
            fileIn >> data; // Skip "{".

            for (int i = 0; i < animation.numAnimatedComponents; i++)
            {
                float tempData;
                fileIn >> tempData;
                frame.frameData.push_back(tempData);
            }

            animation.frameData.push_back(frame);

            BuildFrameSkeleton(animation, frame);

            fileIn >> data; // Skip "}".
        }
    }

    // Calculate and store some usefull animation data
    animation.frameTime = 1.0f / animation.frameRate;
    animation.totalAnimTime = animation.numFrames * animation.frameTime;
    animation.currAnimTime = 0.0f;

    // Make sure the joints exists in the model, and the parentId's match up.
    if (!CheckAnimation(model, animation))
    {
        if (model.animations.empty())
            model.HasAnimations = false;
        return false;
    }

    model.HasAnimations = true;

    // Add animation to the model.
    model.animations.push_back(animation);
    return true;
}

void MD5Loader::PrepareMesh(md5_mesh_t& mesh, md5_model_t& model)
{
    // Find each vertex position using the joint and weight.
    for (int i = 0; i < mesh.vertices.size(); ++i)
    {
        MD5Vertex vertex = mesh.vertices[i];
        vertex.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

        for (int j = 0; j < vertex.WeightCount; ++j)
        {
            Weight weight = mesh.weights[vertex.StartWeight + j];
            Joint joint = model.joints[weight.jointId];

            DirectX::XMVECTOR tempJointOrientation = DirectX::XMVectorSet(joint.orientation.x, joint.orientation.y, joint.orientation.z, joint.orientation.w);
            DirectX::XMVECTOR tempWeightPos = DirectX::XMVectorSet(weight.position.x, weight.position.y, weight.position.z, 0.0f);

            DirectX::XMVECTOR tempJointOrientationConjugate = DirectX::XMVectorSet(-joint.orientation.x, -joint.orientation.y, -joint.orientation.z, joint.orientation.w);

            DirectX::XMFLOAT3 rotatedPoint;
            DirectX::XMStoreFloat3(&rotatedPoint, DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

            vertex.position.x += (joint.position.x + rotatedPoint.x) * weight.bias;
            vertex.position.y += (joint.position.y + rotatedPoint.y) * weight.bias;
            vertex.position.z += (joint.position.z + rotatedPoint.z) * weight.bias;
        }

        // Put the vertex position into vertices for this mesh.
        mesh.vertices[i].position = vertex.position;
    }
}

void MD5Loader::PrepareNormals(md5_mesh_t& mesh, md5_model_t& model)
{
    using namespace DirectX;
    std::vector<XMFLOAT3> tempNormal;

    // normalized and unormalized normals.
    XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

    // Used to get vectors (sides) from the position of the verts
    float vecX, vecY, vecZ;

    // Two edges of our triangle
    XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

    // Compute face normals
    for (unsigned int i = 0; i < mesh.trianglesCount; ++i)
    {
        // Get the vector describing one edge of our triangle (edge, 0, 2).
        vecX = mesh.vertices[mesh.indices[(i * 3)]].position.x - mesh.vertices[mesh.indices[(i * 3) + 2]].position.x;
        vecY = mesh.vertices[mesh.indices[(i * 3)]].position.y - mesh.vertices[mesh.indices[(i * 3) + 2]].position.y;
        vecZ = mesh.vertices[mesh.indices[(i * 3)]].position.z - mesh.vertices[mesh.indices[(i * 3) + 2]].position.z;
        edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f); // Create our first edge.

        // Get the vector describing another edge of our triangle (edge 2, 1).
        vecX = mesh.vertices[mesh.indices[(i * 3) + 2]].position.x - mesh.vertices[mesh.indices[(i * 3) + 1]].position.x;
        vecY = mesh.vertices[mesh.indices[(i * 3) + 2]].position.y - mesh.vertices[mesh.indices[(i * 3) + 1]].position.y;
        vecZ = mesh.vertices[mesh.indices[(i * 3) + 2]].position.z - mesh.vertices[mesh.indices[(i * 3) + 1]].position.z;
        edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f); // Create our second edge.

        // Cross multiply the two edge vectors to get the un-normalized face normal.
        XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

        tempNormal.push_back(unnormalized);
    }

    // Compute vertex normals (normal Averaging).
    XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    int facesUsing = 0;
    float tX, tY, tZ; // temp axis variables.

    // Go through each vertex
    for (unsigned int i = 0; i < mesh.vertices.size(); ++i)
    {
        // Check which triangles use this vertex
        for (unsigned int j = 0; j < mesh.trianglesCount; ++j)
        {
            if (mesh.indices[j * 3] == i || mesh.indices[(j * 3) + 1] == i || mesh.indices[(j * 3) + 2] == i)
            {
                tX = XMVectorGetX(normalSum) + tempNormal[j].x;
                tY = XMVectorGetY(normalSum) + tempNormal[j].y;
                tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

                // If a face is using the vertex, add the unnormalized face normal to the normal Sum.
                normalSum = XMVectorSet(tX, tY, tZ, 0.0f);

                facesUsing++;
            }
        }

        // Get the actual normal by dividing the normalSum by the number of faces sharing the vertex.
        normalSum = normalSum / static_cast<float>(facesUsing);

        // Normalize the normalSum vector
        normalSum = XMVector3Normalize(normalSum);

        // Store the normal in our current vertex
        mesh.vertices[i].normal.x = -XMVectorGetX(normalSum);
        mesh.vertices[i].normal.y = -XMVectorGetY(normalSum);
        mesh.vertices[i].normal.z = -XMVectorGetZ(normalSum);

        MD5Vertex vertex = mesh.vertices[i]; // Get the current vertex

        // Create the joint space normal for easy normal calculations in animation.
        //mesh.jointSpaceNormals.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f)); // Push back a blank normal

        XMVECTOR normal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // Clear normal

        for (int k = 0; k < vertex.WeightCount; k++)
        {
            Joint joint = model.joints[mesh.weights[vertex.StartWeight + k].jointId];
            XMVECTOR jointOrientation = XMVectorSet(joint.orientation.x, joint.orientation.y, joint.orientation.z, joint.orientation.w);

            // Calculate normal based off joints orientation (turn into joint space).
            normal = XMQuaternionMultiply(XMQuaternionMultiply(XMQuaternionInverse(jointOrientation), normalSum), jointOrientation);

            XMStoreFloat3(&mesh.weights[vertex.StartWeight + k].normal, XMVector3Normalize(normal));
        }

        // Clear normalSum, facesUsing for next vertex
        normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        facesUsing = 0;
    }
}

void MD5Loader::BuildFrameSkeleton(md5_anim_t& animation, FrameData const& frameData)
{
    // Build the frame skeleton.
    std::vector<Joint> skeleton;

    for (int i = 0; i < animation.jointInfo.size(); i++)
    {
        unsigned int j = 0; // Keep track of position in frameData array.

        // Start the frames joint with the base frame's joint
        Joint joint = animation.baseFrameJoints[i];
        joint.parentId = animation.jointInfo[i].parentId;

        if (animation.jointInfo[i].flags & 1) // m_Pos.x (000001)
            joint.position.x = frameData.frameData[animation.jointInfo[i].startIndex + j++];

        if (animation.jointInfo[i].flags & 2) // m_Pos.y (000010)
            joint.position.z = frameData.frameData[animation.jointInfo[i].startIndex + j++];

        if (animation.jointInfo[i].flags & 4) // m_Pos.z (000100)
            joint.position.y = frameData.frameData[animation.jointInfo[i].startIndex + j++];

        if (animation.jointInfo[i].flags & 8) // m_Orient.x (001000)
            joint.orientation.x = frameData.frameData[animation.jointInfo[i].startIndex + j++];

        if (animation.jointInfo[i].flags & 16) // m_Orient.y (010000)
            joint.orientation.z = frameData.frameData[animation.jointInfo[i].startIndex + j++];

        if (animation.jointInfo[i].flags & 32) // m_Orient.z (100000)
            joint.orientation.y = frameData.frameData[animation.jointInfo[i].startIndex + j++];

        QuaternionComputeW(joint.orientation);

        if (joint.parentId >= 0)
        {
            Joint parentJoint = skeleton[joint.parentId];

            // Turn the XMFLOAT3 and 4's into vectors for easier computation.
            DirectX::XMVECTOR parentJointOrientation = DirectX::XMVectorSet(parentJoint.orientation.x, parentJoint.orientation.y, parentJoint.orientation.z, parentJoint.orientation.w);
            DirectX::XMVECTOR jointPos = DirectX::XMVectorSet(joint.position.x, joint.position.y, joint.position.z, 0.0f);
            DirectX::XMVECTOR parentOrientationConjugate = DirectX::XMVectorSet(-parentJoint.orientation.x, -parentJoint.orientation.y, -parentJoint.orientation.z, parentJoint.orientation.w);

            // Calculate current joints position relative to it's parents position.
            DirectX::XMFLOAT3 rotatedPos;
            DirectX::XMStoreFloat3(&rotatedPos, DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(parentJointOrientation, jointPos), parentOrientationConjugate));

            // Translate the joint to model space by adding the parent joint's pos to it.
            joint.position.x = rotatedPos.x + parentJoint.position.x;
            joint.position.y = rotatedPos.y + parentJoint.position.y;
            joint.position.z = rotatedPos.z + parentJoint.position.z;

            // Currently the joint is oriented in it's parent joints space, we now need to orient it in
            // model space by multiplying the two orientations together (parentOrientation & childOrientation) <- In that order.
            DirectX::XMVECTOR jointOrient = DirectX::XMVectorSet(joint.orientation.x, joint.orientation.y, joint.orientation.z, joint.orientation.w);
            jointOrient = DirectX::XMQuaternionMultiply(parentJointOrientation, jointOrient);

            // Normalize the orientation quaternion.
            jointOrient = DirectX::XMQuaternionNormalize(jointOrient);

            DirectX::XMStoreFloat4(&joint.orientation, jointOrient);
        }

        // Store the joint into our temporary frame skeleton.
        skeleton.push_back(joint);
    }

    // Push back our newly created frame skeleton into the animation's frameSkeleton array.
    animation.frameSkeleton.push_back(skeleton);
}

bool MD5Loader::CheckAnimation(md5_model_t const& model, md5_anim_t const& animation)
{
    if (model.numJoints != animation.numJoints)
        return false;

    // Check to make sure the joints match up
    for (unsigned int i = 0; i < model.joints.size(); ++i)
    {
        Joint const& meshJoint = model.joints[i];
        JointInfo const& animJoint = animation.jointInfo[i];

        if (meshJoint.name != animJoint.name || meshJoint.parentId != animJoint.parentId)
            return false;
    }
    return true;
}

void MD5Loader::QuaternionComputeW(DirectX::XMFLOAT4& q)
{
    float t = 1.0f - (q.x * q.x) - (q.y * q.y) - (q.z * q.z);
    t < 0.0f ? q.w = 0.0f : q.w = -sqrtf(t);
}