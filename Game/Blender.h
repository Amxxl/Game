#pragma once

#include "Bindable.h"
#include <array>
#include <optional>


namespace Bind
{
	class Blender : public Bindable
	{
		public:
			Blender(DX::DeviceResources* deviceResources, bool blending, std::optional<float> factor = {});
			virtual void Bind(DX::DeviceResources* deviceResources) noexcept override;
			void SetFactor(float factor);
			float GetFactor() const;

			static std::shared_ptr<Blender> Resolve(DX::DeviceResources* deviceResources, bool blending, std::optional<float> factor = {});
			static std::string GenerateUID(bool blending, std::optional<float> factor);
			virtual std::string const& GetUID() const noexcept override;

		protected:
			Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
			bool blending;
			std::optional<std::array<float, 4>> factors;
	};
}