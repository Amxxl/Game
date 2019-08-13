#pragma once

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "BindableCommon.h"

class Mesh : public Drawable
{
    public:
        Mesh(DX::DeviceResources* deviceResources, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs);
        void Draw(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX accumulatedTransform) const;
        DirectX::XMMATRIX GetTransform() const noexcept override;
    
    private:
        mutable DirectX::XMFLOAT4X4 transform;
};