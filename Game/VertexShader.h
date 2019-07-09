#pragma once

#include "Bindable.h"

namespace Bind
{
    class VertexShader : public Bindable
    {
        public:
            VertexShader(DX::DeviceResources* deviceResources, std::wstring const& path);

            virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;
            ID3DBlob* GetBytecode() const noexcept { pBytecodeBlob.Get(); }

        protected:
            Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
            Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    };
}