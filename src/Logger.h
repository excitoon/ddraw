#pragma once

#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <thread>

#include "Constants.h"
#include "LogLevel.h"


class Logger
{
    std::ofstream log;
    std::string tag;

private:
    LogLevel filter = LogLevel::Warning;

public:
    class Log
    {
        std::ofstream * log = nullptr;

    public:
        Log()
        {
        }

        Log(std::ofstream & log) :
            log(&log)
        {
        }

        ~Log()
        {
            if (log != nullptr)
            {
                *log << "\n" << std::flush;
            }
        }

        template <typename T>
        Log & operator <<(T o)
        {
            if (log != nullptr)
            {
                *log << o;
            }
            return *this;
        }
    };

    Logger(const std::string & tag):
        filter(Constants::LogFilterLevel),
        log("ddraw.log", std::ios_base::app | std::ios_base::out),
        tag(tag)
    {
    }

    Log operator ()(LogLevel level = LogLevel::Information)
    {
        if (static_cast<unsigned>(level) > static_cast<unsigned>(filter))
        {
            return {};
        }
        auto clock = std::chrono::system_clock::now();
        auto duration = clock.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000000;
        std::time_t now = std::chrono::system_clock::to_time_t(clock);
        std::tm tm_now = *std::localtime(&now);
        log << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << "." << std::setw(6) << std::setfill('0') << millis << " ";
        log << "[ " << std::this_thread::get_id() << " ] <";
        switch (level)
        {
            case LogLevel::Fatal: log << "Fatal"; break;
            case LogLevel::Error: log << "Error"; break;
            case LogLevel::Warning: log << "Warning"; break;
            case LogLevel::Information: log << "Information"; break;
            case LogLevel::Debug: log << "Debug"; break;
            case LogLevel::Trace: log << "Trace"; break;
            default: log << "Unknown";
        }
        log << "> " << tag << ": ";
        return log;
    }
};
