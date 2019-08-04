//
// Frustum.h
//

#pragma once

class Frustum
{
    public:
        enum Plane
        {
            NEAR_PLANE,
            FAR_PLANE,
            LEFT_PLANE,
            RIGHT_PLANE,
            TOP_PLANE,
            BOTTOM_PLANE,
            TOTAL_PLANES
        };

        Frustum() = default;
        Frustum(Frustum const&) = delete;
        Frustum& operator=(Frustum const&) = delete;
        ~Frustum() = default;

        void Construct(float screenDepth, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

        bool CheckPoint(DirectX::XMFLOAT3 const& point);
        bool CheckSphere(DirectX::XMFLOAT3 const& center, float radius);
        bool CheckCube(DirectX::XMFLOAT3 const& center, float radius);

        bool CheckRectangle(float minWidth, float minHeight, float minDepth, float maxWidth, float maxHeight, float maxDepth);


        DirectX::XMFLOAT4 const& GetPlane(Plane id)     { return m_planeNormals[id];           }

        DirectX::XMFLOAT4 const& GetNearPlane()   const { return m_planeNormals[NEAR_PLANE];   }
        DirectX::XMFLOAT4 const& GetFarPlane()    const { return m_planeNormals[FAR_PLANE];    }
        DirectX::XMFLOAT4 const& GetLeftPlane()   const { return m_planeNormals[LEFT_PLANE];   }
        DirectX::XMFLOAT4 const& GetRightPlane()  const { return m_planeNormals[RIGHT_PLANE];  }
        DirectX::XMFLOAT4 const& GetTopPlane()    const { return m_planeNormals[TOP_PLANE];    }
        DirectX::XMFLOAT4 const& GetBottomPlane() const { return m_planeNormals[BOTTOM_PLANE]; }

    private:
        float PlaneDotCoord(DirectX::XMFLOAT4 const& plane, DirectX::XMFLOAT3 const& point);

    private:
        DirectX::XMFLOAT4 m_planeNormals[TOTAL_PLANES];
        DirectX::XMFLOAT4 m_plane[TOTAL_PLANES];
};