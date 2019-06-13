#pragma once

#include "spdlog/spdlog.h"

class Logger
{
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger>& Get();

    private:
        static std::shared_ptr<spdlog::logger> s_pLogger;
};