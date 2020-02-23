#pragma once

#include "Bindable.h"
#include "BindableCache.h"
#include "DeviceResources.h"

namespace Bind
{
    class Stencil : public Bindable
    {
        public:
            enum class Mode
            {
                Off,
                Write,
                Mask
            };

            Stencil(DX::DeviceResources* deviceResources, Mode mode)
                : mode(mode)
            {
                D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

                if (mode == Mode::Write)
                {
                    dsDesc.DepthEnable = FALSE;
                    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
                    dsDesc.StencilEnable = TRUE;
                    dsDesc.StencilWriteMask = 0xFF;
                    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
                    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
                }
                else if (mode == Mode::Mask)
                {
                    dsDesc.DepthEnable = FALSE;
                    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
                    dsDesc.StencilEnable = TRUE;
                    dsDesc.StencilReadMask = 0xFF;
                    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
                    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                }
                GetDevice(deviceResources)->CreateDepthStencilState(&dsDesc, &pStencil);
            }

            void Bind(DX::DeviceResources* deviceResources) noexcept override
            {
                GetContext(deviceResources)->OMSetDepthStencilState(pStencil.Get(), 0xFF);
            }

            static std::shared_ptr<Stencil> Resolve(DX::DeviceResources* deviceResources, Mode mode)
            {
                return BindableCache::Resolve<Stencil>(deviceResources, mode);
            }

            static std::string GenerateUID(Mode mode)
            {
                using namespace std::string_literals;
                auto const modeName = [mode]() {
                    switch (mode)
                    {
                        case Mode::Off:
                            return "off"s;
                        case Mode::Write:
                            return "write"s;
                        case Mode::Mask:
                            return "mask"s;
                    }
                    return "ERROR"s;
                };
                return typeid(Stencil).name() + "#"s + modeName();
            }

            std::string const& GetUID() const noexcept override
            {
                return GenerateUID(mode);
            }

        private:
            Mode mode;
            Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pStencil;
    };
}