#pragma once

#include <array>
#include <atomic>
#include <functional>
#include <stdexcept>
#include <thread>

#include "Logger.h"


/// Unfortunately, DirectDraw works with multiple threads poorly even with DDSCL_MULTITHREADED.
/// In this class we multiplex calls originating from client application and wrapper code.
/// We need separate thread because we want to make calls to DirectDraw even when client application
/// doesn't do that.
/// TODO. Do not poll constantly.
class Scheduler
{
    using Task = std::function<void ()>;

    class ThreadData
    {
        std::atomic<bool> pending_task = { false };
        Task task;

    public:
        void post(Task && task)
        {
            while (pending_task)
            {
                std::this_thread::yield();
            }
            this->task = std::move(task);
            pending_task = true;
        }

        bool work()
        {
            if (pending_task)
            {
                task();
                pending_task = false;
                return true;
            }
            return false;
        }
    };

    Logger log = Logger(Logger::Level::Trace, "Scheduler");

    std::array<ThreadData, 2> thread_data;
    std::thread::id first_thread;
    std::thread::id second_thread;

    volatile bool shutting_down = false;

    std::thread worker;

    unsigned getIndex()
    {
        auto this_id = std::this_thread::get_id();
        if (this_id == first_thread)
        {
            return 0;
        }
        else if (this_id == second_thread)
        {
            return 1;
        }

        /// Exactly two threads will appear here, once and in order.
        if (first_thread == std::thread::id())
        {
            first_thread = this_id;
            return 0;
        }
        else if (second_thread == std::thread::id())
        {
            second_thread = this_id;
            return 1;
        }
        else
        {
            throw std::runtime_error("We only support two threads.");
        }
    }

public:
    Scheduler()
    {
        worker = std::thread([this]()
        {
            while (!shutting_down)
            {
                if (!thread_data[0].work() && !thread_data[1].work())
                {
                    std::this_thread::yield();
                }
            }
        });
    }

    ~Scheduler()
    {
        shutting_down = true;
    }

    /// TODO. Use std::invoke_result_t<SpecificTask> in 2018.
    template <typename ResultType, typename SpecificTask>
    ResultType makeTask(SpecificTask && task)
    {
        ThreadData & data = thread_data[getIndex()];
        ResultType result;
        std::atomic<bool> done = { false };
        data.post([&]() { result = task(); done = true; });
        while (!done)
        {
            std::this_thread::yield();
        }
        return result;
    }
};
