#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

#include "Logger.h"
#include "Constants.h"


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
        std::thread::id id;

        ThreadData(std::thread::id id):
            id(id)
        {
        }

        std::mutex processing;

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

    std::vector<std::unique_ptr<ThreadData>> thread_data;
    std::mutex add_new_thread;

    volatile bool shutting_down = false;

    std::thread worker;

public:
    Scheduler()
    {
        thread_data.reserve(Constants::MaxThreads);
        if (Constants::EnablePrimarySurfaceBackgroundBuffering)
        {
            worker = std::thread([this]()
            {
                while (!shutting_down)
                {
                    bool no_load = false;
                    for (auto & data : thread_data)
                    {
                        if (data->id == std::thread::id())
                        {
                            break;
                        }
                        no_load |= data->work();
                    }
                    if (no_load)
                    {
                        std::this_thread::yield();
                    }
                }
            });
        }
    }

    ~Scheduler()
    {
        if (Constants::EnablePrimarySurfaceBackgroundBuffering)
        {
            shutting_down = true;
            worker.join();
        }
    }

    ThreadData & getData()
    {
        for (auto & data : thread_data)
        {
            if (data->id == std::this_thread::get_id())
            {
                return *data.get();
            }
            if (data->id == std::thread::id())
            {
                break;
            }
        }
        if (thread_data.capacity() > thread_data.size())
        {
            /// We use this lock only for initialization.
            /// If threads disappear client application may waste all effort.
            std::lock_guard<std::mutex> lock(add_new_thread);
            thread_data.push_back(std::make_unique<ThreadData>(std::this_thread::get_id()));
            return *thread_data.back().get();
        }
        /// TODO. Make something predictable.
        throw std::runtime_error("Too many threads.");
    }

    /// TODO. Use std::invoke_result_t<SpecificTask> in 2018.
    template <typename ResultType, typename SpecificTask>
    inline ResultType makeTask(SpecificTask && task)
    {
        ResultType result;
        if (Constants::EnablePrimarySurfaceBackgroundBuffering)
        {
            ThreadData & data = getData();
            std::unique_lock<std::mutex> lock(data.processing);
            data.post([&]() { std::unique_lock<std::mutex> my_lock = std::move(lock); result = task(); });
            std::lock_guard<std::mutex> wait(data.processing);
        }
        else
        {
            result = task();
        }
        return result;
    }
};
