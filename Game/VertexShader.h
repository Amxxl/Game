#pragma once

#include "Bindable.h"

namespace Bind
{
    class VertexShader : public Bindable
    {
        public:
            VertexShader(DX::DeviceResources* deviceResources, std::string const& path);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;
            ID3DBlob* GetBytecode() const noexcept { return pBytecodeBlob.Get(); }

            static std::shared_ptr<VertexShader> Resolve(DX::DeviceResources* deviceResources, std::string const& path);
            static std::string GenerateUID(std::string const& path);
            virtual std::string const& GetUID() const noexcept override;

        protected:
            std::string path;

            Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
            Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    };
}