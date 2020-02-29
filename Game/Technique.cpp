#include "pch.h"
#include "Technique.h"

void Technique::Submit(FrameCommander& frame, const Drawable& drawable) const noexcept
{
    if (active)
    {
        for (auto const& step : steps)
        {
            step.Submit(frame, drawable);
        }
    }
}

void Technique::InitializeParentReferences(const Drawable& parent) noexcept
{
    for (auto& s : steps)
    {
        s.InitializeParentReferences(parent);
    }
}