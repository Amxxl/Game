//
// Frustum.h
//

#pragma once

class Frustum
{
    public:
        Frustum() = default;
        Frustum(Frustum const&) = delete;
        Frustum& operator=(Frustum const&) = delete;
        ~Frustum() = default;

        void Construct(float screenDepth, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

        bool CheckPoint(DirectX::XMFLOAT3 const& point) const;
        bool CheckSphere(DirectX::XMFLOAT3 const& center, float radius) const;
        bool CheckCube(DirectX::XMFLOAT3 const& center, float radius) const;

        DirectX::XMFLOAT4 const& GetNearPlane() const { return m_planesNorm[0]; }
        DirectX::XMFLOAT4 const& GetFarPlane() const { return m_planesNorm[1]; }
        DirectX::XMFLOAT4 const& GetLeftPlane() const { return m_planesNorm[2]; }
        DirectX::XMFLOAT4 const& GetRightPlane() const { return m_planesNorm[3]; }
        DirectX::XMFLOAT4 const& GetTopPlane() const { return m_planesNorm[4]; }
        DirectX::XMFLOAT4 const& GetBottomPlane() const { return m_planesNorm[5]; }

    private:
        DirectX::XMFLOAT4 m_planesNorm[6];
        DirectX::XMFLOAT4 m_planes[6];
};