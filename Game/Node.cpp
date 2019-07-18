#include "pch.h"
#include "Node.h"

namespace expr
{
    Node::Node(std::vector<Mesh*> meshPtrs, DirectX::XMMATRIX const& transform)
        : meshPtrs(std::move(meshPtrs))
    {
        DirectX::XMStoreFloat4x4(&this->transform, transform);
    }

    void Node::Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX accumulatedTransform) const
    {
        auto const built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;
        
        for (auto const pm : meshPtrs)
            pm->Draw(deviceResources, built);

        for (auto const& pc : childPtrs)
            pc->Draw(deviceResources, built);
    }

    void Node::AddChild(std::unique_ptr<Node> pChild)
    {
        assert(pChild);
        childPtrs.push_back(std::move(pChild));
    }
}