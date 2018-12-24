//
// Frustum.h
//

#pragma once

class Frustum
{
    public:
        Frustum();
        ~Frustum();

        void ConstructFrustum(float screenDepth, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX const& viewMatrix, DirectX::XMMATRIX& world);

        bool CheckPoint(DirectX::XMFLOAT3 const& point) const;
        bool CheckSphere(DirectX::XMFLOAT3 const& center, float radius) const;
        bool CheckCube(DirectX::XMFLOAT3 const& center, float radius) const;

        DirectX::XMFLOAT4 const& GetNearPlane() const { return m_planesNorm[0]; }
        DirectX::XMFLOAT4 const& GetFarPlane() const { return m_planesNorm[1]; }
        DirectX::XMFLOAT4 const& GetLeftPlane() const { return m_planesNorm[2]; }
        DirectX::XMFLOAT4 const& GetRightPlane() const { return m_planesNorm[3]; }
        DirectX::XMFLOAT4 const& GetTopPlane() const { return m_planesNorm[4]; }
        DirectX::XMFLOAT4 const& GetBottomPlane() const { return m_planesNorm[5]; }

        DirectX::XMFLOAT3 const& GetCameraPosition() const { return DirectX::XMFLOAT3(-view._41, -view._42, -view._43); }

    private:
        DirectX::XMFLOAT4 m_planesNorm[6];
        DirectX::XMFLOAT4 m_planes[6];
        DirectX::XMFLOAT4X4 view;

};

