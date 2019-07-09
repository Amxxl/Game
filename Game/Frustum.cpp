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
    m_plane[NEAR_PLANE].x = matrix._14 + matrix._13;
    m_plane[NEAR_PLANE].y = matrix._24 + matrix._23;
    m_plane[NEAR_PLANE].z = matrix._34 + matrix._33;
    m_plane[NEAR_PLANE].w = matrix._44 + matrix._43;

    // Normalize near plane of frustum.
    XMStoreFloat4(&m_planeNormals[NEAR_PLANE], XMPlaneNormalize(XMLoadFloat4(&m_plane[NEAR_PLANE])));

    // Calculate far plane of frustum.
    m_plane[FAR_PLANE].x = matrix._14 - matrix._13;
    m_plane[FAR_PLANE].y = matrix._24 - matrix._23;
    m_plane[FAR_PLANE].z = matrix._34 - matrix._33;
    m_plane[FAR_PLANE].w = matrix._44 - matrix._43;

    // Normalize far plane of frustum.
    XMStoreFloat4(&m_planeNormals[FAR_PLANE], XMPlaneNormalize(XMLoadFloat4(&m_plane[FAR_PLANE])));

    // Calculate left plane of frustum.
    m_plane[LEFT_PLANE].x = matrix._14 + matrix._11;
    m_plane[LEFT_PLANE].y = matrix._24 + matrix._21;
    m_plane[LEFT_PLANE].z = matrix._34 + matrix._31;
    m_plane[LEFT_PLANE].w = matrix._44 + matrix._41;

    // Normalize left plane of frustum.
    XMStoreFloat4(&m_planeNormals[LEFT_PLANE], XMPlaneNormalize(XMLoadFloat4(&m_plane[LEFT_PLANE])));

    // Calculate right plane of frustum.
    m_plane[RIGHT_PLANE].x = matrix._14 - matrix._11;
    m_plane[RIGHT_PLANE].y = matrix._24 - matrix._21;
    m_plane[RIGHT_PLANE].z = matrix._34 - matrix._31;
    m_plane[RIGHT_PLANE].w = matrix._44 - matrix._41;

    // Normalize right plane of frustum.
    XMStoreFloat4(&m_planeNormals[RIGHT_PLANE], XMPlaneNormalize(XMLoadFloat4(&m_plane[RIGHT_PLANE])));

    // Calculate top plane of frustum.
    m_plane[TOP_PLANE].x = matrix._14 - matrix._12;
    m_plane[TOP_PLANE].y = matrix._24 - matrix._22;
    m_plane[TOP_PLANE].z = matrix._34 - matrix._32;
    m_plane[TOP_PLANE].w = matrix._44 - matrix._42;

    // Normalize top plane of frustum.
    XMStoreFloat4(&m_planeNormals[TOP_PLANE], XMPlaneNormalize(XMLoadFloat4(&m_plane[TOP_PLANE])));

    // Calculate bottom plane of frustum.
    m_plane[BOTTOM_PLANE].x = matrix._14 + matrix._12;
    m_plane[BOTTOM_PLANE].y = matrix._24 + matrix._22;
    m_plane[BOTTOM_PLANE].z = matrix._34 + matrix._32;
    m_plane[BOTTOM_PLANE].w = matrix._44 + matrix._42;

    // Normalize bottom plane of frustum.
    XMStoreFloat4(&m_planeNormals[BOTTOM_PLANE], XMPlaneNormalize(XMLoadFloat4(&m_plane[BOTTOM_PLANE])));
}

bool Frustum::CheckPoint(XMFLOAT3 const& point)
{
    for (unsigned char i = 0; i < 6; i++)
    {
        if (PlaneDotCoord(m_planeNormals[i], point) <= 0.0f)
            return false;
    }

    return true;
}

bool Frustum::CheckSphere(XMFLOAT3 const& center, float radius)
{
    for (unsigned char i = 0; i < 6; i++)
    {
        if (PlaneDotCoord(m_planeNormals[i], center) <= -radius)
            return false;
    }

    return true;
}

bool Frustum::CheckCube(DirectX::XMFLOAT3 const& center, float radius)
{
    for (unsigned int i = 0; i < 6; ++i)
    {
        if (PlaneDotCoord(m_planeNormals[i], { center.x - radius, center.y - radius, center.z - radius }) >= 0.0f)
            continue;

        if (PlaneDotCoord(m_planeNormals[i], { center.x + radius, center.y - radius, center.z - radius }) >= 0.0f)
            continue;

        if (PlaneDotCoord(m_planeNormals[i], { center.x - radius, center.y + radius, center.z - radius }) >= 0.0f)
            continue;

        if (PlaneDotCoord(m_planeNormals[i], { center.x + radius, center.y + radius, center.z - radius }) >= 0.0f)
            continue;

        if (PlaneDotCoord(m_planeNormals[i], { center.x - radius, center.y - radius, center.z + radius }) >= 0.0f)
            continue;

        if (PlaneDotCoord(m_planeNormals[i], { center.x + radius, center.y - radius, center.z + radius }) >= 0.0f)
            continue;

        if (PlaneDotCoord(m_planeNormals[i], { center.x - radius, center.y + radius, center.z + radius }) >= 0.0f)
            continue;

        if (PlaneDotCoord(m_planeNormals[i], { center.x + radius, center.y + radius, center.z + radius }) >= 0.0f)
            continue;

        return false;
    }

    return true;
}

float Frustum::PlaneDotCoord(DirectX::XMFLOAT4 const& plane, DirectX::XMFLOAT3 const& point)
{
    DirectX::XMVECTOR p = DirectX::XMLoadFloat4(&plane);
    DirectX::XMVECTOR v = DirectX::XMLoadFloat3(&point);

    float d;
    DirectX::XMStoreFloat(&d, DirectX::XMPlaneDotCoord(p, v));
    return d;
}
