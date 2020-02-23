#pragma once
#include "DeviceResources.h"
#include "Job.h"
#include <vector>

class Pass
{
    public:
        void Accept(Job job) noexcept
        {
            jobs.push_back(job);
        }

        void Execute(DX::DeviceResources* deviceResources) const noxnd
        {
            for (auto const& j : jobs)
            {
                j.Execute(deviceResources);
            }
        }

        void Reset() noexcept
        {
            jobs.clear();
        }

    private:
        std::vector<Job> jobs;
};