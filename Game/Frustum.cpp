#include "pch.h"
#include "Frustum.h"

using namespace DirectX;


void Frustum::Construct(float screenDepth, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
    // Convert the projection matrix into a 4x4 float type.
    XMFLOAT4X4 projectionMatrix;
    XMStoreFloat4x4(&projectionMatrix, projection);

    // Calculate the minimum Z distance in the frustum.
    float zMinimum = -projectionMatrix._43 / projectionMatrix._33;
    float r = screenDepth / (screenDepth - zMinimum);

    // Load the updated values back into the projection matrix.
    projectionMatrix._33 = r;
    projectionMatrix._43 = -r * zMinimum;
    projection = XMLoadFloat4x4(&projectionMatrix);

    // Create the frustum matrix from the view matrix and updated projection matrix,
    // and convert it into a 4x4 float type.
    XMFLOAT4X4 matrix;
    XMStoreFloat4x4(&matrix, XMMatrixMultiply(view, projection));

    // Calculate near plane of frustum.
    m_planes[0].x = matrix._14 + matrix._13;
    m_planes[0].y = matrix._24 + matrix._23;
    m_planes[0].z = matrix._34 + matrix._33;
    m_planes[0].w = matrix._44 + matrix._43;

    // Normalize near plane of frustum.
    XMStoreFloat4(&m_planesNorm[0], XMPlaneNormalize(XMLoadFloat4(&m_planes[0])));

    // Calculate far plane of frustum.
    m_planes[1].x = matrix._14 - matrix._13;
    m_planes[1].y = matrix._24 - matrix._23;
    m_planes[1].z = matrix._34 - matrix._33;
    m_planes[1].w = matrix._44 - matrix._43;

    // Normalize far plane of frustum.
    XMStoreFloat4(&m_planesNorm[1], XMPlaneNormalize(XMLoadFloat4(&m_planes[1])));

    // Calculate left plane of frustum.
    m_planes[2].x = matrix._14 + matrix._11;
    m_planes[2].y = matrix._24 + matrix._21;
    m_planes[2].z = matrix._34 + matrix._31;
    m_planes[2].w = matrix._44 + matrix._41;

    // Normalize left plane of frustum.
    XMStoreFloat4(&m_planesNorm[2], XMPlaneNormalize(XMLoadFloat4(&m_planes[2])));

    // Calculate right plane of frustum.
    m_planes[3].x = matrix._14 - matrix._11;
    m_planes[3].y = matrix._24 - matrix._21;
    m_planes[3].z = matrix._34 - matrix._31;
    m_planes[3].w = matrix._44 - matrix._41;

    // Normalize right plane of frustum.
    XMStoreFloat4(&m_planesNorm[3], XMPlaneNormalize(XMLoadFloat4(&m_planes[3])));

    // Calculate top plane of frustum.
    m_planes[4].x = matrix._14 - matrix._12;
    m_planes[4].y = matrix._24 - matrix._22;
    m_planes[4].z = matrix._34 - matrix._32;
    m_planes[4].w = matrix._44 - matrix._42;

    // Normalize top plane of frustum.
    XMStoreFloat4(&m_planesNorm[4], XMPlaneNormalize(XMLoadFloat4(&m_planes[4])));

    // Calculate bottom plane of frustum.
    m_planes[5].x = matrix._14 + matrix._12;
    m_planes[5].y = matrix._24 + matrix._22;
    m_planes[5].z = matrix._34 + matrix._32;
    m_planes[5].w = matrix._44 + matrix._42;

    // Normalize bottom plane of frustum.
    XMStoreFloat4(&m_planesNorm[5], XMPlaneNormalize(XMLoadFloat4(&m_planes[5])));
}

bool Frustum::CheckPoint(XMFLOAT3 const& point) const
{
    for (unsigned char i = 0; i < 6; i++)
    {
        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&point))) <= 0.0f)
            return false;
    }

    return true;
}

bool Frustum::CheckSphere(XMFLOAT3 const& center, float radius) const
{
    for (unsigned char i = 0; i < 6; i++)
    {
        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&center))) <= -radius)
            return false;
    }

    return true;
}

bool Frustum::CheckCube(DirectX::XMFLOAT3 const& center, float radius) const
{
    // Check if any one point of the cube is in the view frustum.
    for (unsigned int i = 0; i < 6; ++i)
    {
        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&XMFLOAT3(center.x - radius, center.y - radius, center.z - radius)))) > 0.0f)
            continue;

        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&XMFLOAT3(center.x + radius, center.y - radius, center.z - radius)))) > 0.0f)
            continue;

        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&XMFLOAT3(center.x - radius, center.y + radius, center.z - radius)))) > 0.0f)
            continue;

        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&XMFLOAT3(center.x + radius, center.y + radius, center.z - radius)))) > 0.0f)
            continue;

        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&XMFLOAT3(center.x - radius, center.y - radius, center.z + radius)))) > 0.0f)
            continue;

        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&XMFLOAT3(center.x + radius, center.y - radius, center.z + radius)))) > 0.0f)
            continue;

        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&XMFLOAT3(center.x - radius, center.y + radius, center.z + radius)))) > 0.0f)
            continue;

        if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_planesNorm[i]), XMLoadFloat3(&XMFLOAT3(center.x + radius, center.y + radius, center.z + radius)))) > 0.0f)
            continue;

        return false;
    }

    return true;
}