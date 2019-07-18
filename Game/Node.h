#pragma once

#include "Mesh.h"

namespace expr
{
    class Node
    {
        friend class Model;

        public:
            Node(std::vector<Mesh*> meshPtrs, DirectX::XMMATRIX const& transform);
            void Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX accumulatedTransform) const;

        private:
            void AddChild(std::unique_ptr<Node> pChild);

        private:
            std::vector<std::unique_ptr<Node>> childPtrs;
            std::vector<Mesh*> meshPtrs;
            DirectX::XMFLOAT4X4 transform;
    };
}
