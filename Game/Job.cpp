#include "pch.h"
#include "Job.h"
#include "Step.h"
#include "Drawable.h"

Job::Job(const Step* pStep, const Drawable* pDrawable)
    : pDrawable{ pDrawable }
    , pStep{ pStep }
{
}

void Job::Execute(DX::DeviceResources* deviceResources) const noxnd
{
    //pDrawable->Bind(deviceResources);
    pStep->Bind(deviceResources);
    //deviceResources->DrawIndexed(pDrawable->GetIndexCount());
}