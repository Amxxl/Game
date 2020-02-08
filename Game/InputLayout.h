#pragma once

#include "Bindable.h"
#include "Vertex.h"

namespace Bind
{
    class InputLayout : public Bindable
    {
        public:
            InputLayout(DX::DeviceResources* deviceResources,
                dvt::VertexLayout layout, ID3DBlob* pVertexShaderByteCode);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;

            static std::shared_ptr<InputLayout> Resolve(DX::DeviceResources* deviceResources,
                dvt::VertexLayout const& layout, ID3DBlob* pVertexShaderBytecode);

            static std::string GenerateUID(dvt::VertexLayout const& layout, ID3DBlob* pVertexShaderBytecode = nullptr);
            std::string const& GetUID() const noexcept override;

        protected:
            dvt::VertexLayout layout;
            Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    };
}