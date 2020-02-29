#pragma once

#include <vector>
#include <memory>
#include "Bindable.h"
#include "DeviceResources.h"
#include "TechniqueProbe.h"

class Step
{
    public:
        Step(size_t targetPass_in)
            : targetPass{ targetPass_in }
        {}

        Step(Step&&) = default;
        Step(Step const& src) noexcept
            : targetPass(src.targetPass)
        {
            bindables.reserve(src.bindables.size());

            for (auto& pb : src.bindables)
            {
                if (auto* pCloning = dynamic_cast<Bind::CloningBindable const*>(pb.get()))
                {
                    bindables.push_back(pCloning->Clone());
                }
                else
                {
                    bindables.push_back(pb);
                }
            }
        }

        Step& operator=(Step const&) = delete;
        Step& operator=(Step&&) = delete;

        void AddBindable(std::shared_ptr<Bind::Bindable> bind_in) noexcept
        {
            bindables.push_back(std::move(bind_in));
        }

        void Submit(class FrameCommander& frame, const class Drawable& drawable) const;
        void Bind(DX::DeviceResources* deviceResources) const
        {
            for (auto const& b : bindables)
            {
                b->Bind(deviceResources);
            }
        }

        void InitializeParentReferences(const class Drawable& parent) noexcept;
        void Accept(TechniqueProbe& probe)
        {
            probe.SetStep(this);
            for (auto& pb : bindables)
            {
                pb->Accept(probe);
            }
        }

    private:
        size_t targetPass;
        std::vector<std::shared_ptr<Bind::Bindable>> bindables;
};