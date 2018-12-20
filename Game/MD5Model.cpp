//
// MD5Model.cpp
//

#include "pch.h"
#include "MD5Model.h"


MD5Model::MD5Model()
{
}

MD5Model::~MD5Model()
{
    model.meshes.clear();
}

bool MD5Model::LoadMesh(ID3D11DeviceContext* deviceContext, std::wstring const& fileName)
{
    if (!axec::MD5Loader::LoadMD5Mesh(deviceContext, fileName, model))
        return false;

    shader.InitializeShaders(deviceContext);
    return true;
}

bool MD5Model::LoadAnim(std::wstring const& fileName)
{
    if (!axec::MD5Loader::LoadMD5Anim(fileName, model))
        return false;
    return true;
}

void MD5Model::Update(ID3D11DeviceContext* deviceContext, float deltaTime, int index)
{
    model.animations[index].currAnimTime += deltaTime;

    if (model.animations[index].currAnimTime > model.animations[index].totalAnimTime)
        model.animations[index].currAnimTime = 0.0f;

    // Which frame are we on
    float currentFrame = model.animations[index].currAnimTime * model.animations[index].frameRate;
    int frame0 = static_cast<int>(floorf(currentFrame));
    int frame1 = frame0 + 1;

    // Make sure we don't go over the number of frames.
    if (frame0 == model.animations[index].numFrames - 1)
        frame1 = 0;

    float interpolation = currentFrame - frame0; // Get the remainder (in time) between frame0 and frame1 to use as interpolation factor.

    std::vector<axec::Joint> interpolatedSkeleton; // Create a frame skeleton to store the interpolated skeletons in

    // Compute the interpolated skeleton.
    for (int i = 0; i < model.animations[index].numJoints; i++)
    {
        axec::Joint tempJoint;
        axec::Joint joint0 = model.animations[index].frameSkeleton[frame0][i];
        axec::Joint joint1 = model.animations[index].frameSkeleton[frame1][i];

        tempJoint.parentId = joint0.parentId;

        // Turn the two quaternions into XMVECTORs for easy computations.
        DirectX::XMVECTOR joint0Orient = DirectX::XMVectorSet(joint0.orientation.x, joint0.orientation.y, joint0.orientation.z, joint0.orientation.w);
        DirectX::XMVECTOR joint1Orient = DirectX::XMVectorSet(joint1.orientation.x, joint1.orientation.y, joint1.orientation.z, joint1.orientation.w);

        // Interpolate positions
        tempJoint.position.x = joint0.position.x + (interpolation * (joint1.position.x - joint0.position.x));
        tempJoint.position.y = joint0.position.y + (interpolation * (joint1.position.y - joint0.position.y));
        tempJoint.position.z = joint0.position.z + (interpolation * (joint1.position.z - joint0.position.z));

        // Interpolate orientations using spherical interpolation (Slerp).
        DirectX::XMStoreFloat4(&tempJoint.orientation, DirectX::XMQuaternionSlerp(joint0Orient, joint1Orient, interpolation));

        interpolatedSkeleton.push_back(tempJoint); // Push the joint back into our interpolated skeleton.
    }

    for (int k = 0; k < model.numMeshes; k++)
    {
        for (int i = 0; i < model.meshes[k].vertices.size(); ++i)
        {
            MD5Vertex tempVert = model.meshes[k].vertices[i];
            tempVert.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Make sure the vertex's pos is cleared first.
            tempVert.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Clear vertices normal.

            // Sum up the joints and weights information to get vertex's position and normal.
            for (int j = 0; j < tempVert.WeightCount; ++j)
            {
                axec::Weight tempWeight = model.meshes[k].weights[tempVert.StartWeight + j];
                axec::Joint tempJoint = interpolatedSkeleton[tempWeight.jointId];

                // Convert joint orientation and weight pos to vectors for easier computation.
                DirectX::XMVECTOR tempJointOrientation = DirectX::XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
                DirectX::XMVECTOR tempWeightPos = DirectX::XMVectorSet(tempWeight.position.x, tempWeight.position.y, tempWeight.position.z, 0.0f);

                // We will need to use the conjugate of the joint orientation quaternion
                DirectX::XMVECTOR tempJointOrientationConjugate = DirectX::XMQuaternionInverse(tempJointOrientation);

                // Calculate vertex position (in joint space, eg. rotate the point around (0,0,0)) for this weight using the joint orientation quaternion and its conjugate
                // We can rotate a point using a quaternion with the equation "rotatedPoint = quaternion * point * quaternionConjugate"
                DirectX::XMFLOAT3 rotatedPoint;
                DirectX::XMStoreFloat3(&rotatedPoint, DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

                // Now move the vertices position from joint space (0, 0, 0) to the joints position in world space, taking the weights bias into account
                tempVert.position.x += (tempJoint.position.x + rotatedPoint.x) * tempWeight.bias;
                tempVert.position.y += (tempJoint.position.y + rotatedPoint.y) * tempWeight.bias;
                tempVert.position.z += (tempJoint.position.z + rotatedPoint.z) * tempWeight.bias;

                // Compute the normals for this frames skeleton using the weight normals from before
                // We can compute the normals the same way we compute the vertices position,
                // only we don't have to translate them (just rotate).
                DirectX::XMVECTOR tempWeightNormal = DirectX::XMVectorSet(tempWeight.normal.x, tempWeight.normal.y, tempWeight.normal.z, 0.0f);

                // Rotate the normal
                DirectX::XMStoreFloat3(&rotatedPoint, DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(tempJointOrientation, tempWeightNormal), tempJointOrientationConjugate));

                // Add to vertices normal and take weight bias into account
                tempVert.normal.x -= rotatedPoint.x * tempWeight.bias;
                tempVert.normal.y -= rotatedPoint.y * tempWeight.bias;
                tempVert.normal.z -= rotatedPoint.z * tempWeight.bias;
            }

            model.meshes[k].vertices[i].position = tempVert.position;
            model.meshes[k].vertices[i].normal = tempVert.normal;
            DirectX::XMStoreFloat3(&model.meshes[k].vertices[i].normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&model.meshes[k].vertices[i].normal)));
        }

        model.meshes[k].vertexBuffer.SetData(deviceContext, &model.meshes[k].vertices[0], static_cast<UINT>(model.meshes[k].vertices.size()));
    }
}

void MD5Model::Draw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
    shader.SetShaderParameters(deviceContext, world, view, proj);
    
    for (int i = 0; i < model.numMeshes; i++)
    {
        deviceContext->IASetIndexBuffer(model.meshes[i].indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        UINT stride = model.meshes[i].vertexBuffer.Stride();
        UINT offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, model.meshes[i].vertexBuffer.GetAddressOf(), &stride, &offset);

        shader.SetTexture(deviceContext, model.meshes[i].texture.Get());
        shader.RenderShader(deviceContext, static_cast<UINT>(model.meshes[i].indices.size()));
    }
}