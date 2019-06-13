#include "pch.h"
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::s_pLogger;

void Logger::Init()
{
    AllocConsole();
    SetConsoleTitleW(L"Engine console");

    s_pLogger = spdlog::stdout_color_mt("console");

    spdlog::set_pattern("[%I:%M:%S] - [%^%l%$] %v");
}

std::shared_ptr<spdlog::logger>& Logger::Get()
{
    return s_pLogger;
}