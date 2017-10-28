#pragma once

#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "Logger.h"
#include "Constants.h"


/// Unfortunately, DirectDraw works with multiple threads poorly even with DDSCL_MULTITHREADED.
/// In this class we multiplex calls originating from client application and wrapper code.
/// We need separate thread because we want to make calls to DirectDraw even when client application
/// doesn't do that (otherwise we could insert our calls before or after application calls).
class Scheduler
{
    using Task = std::function<void ()>;

    Logger log = Logger("Scheduler");

    std::list<Task> tasks;
    std::condition_variable work_pending;
    std::mutex mutex_pending;

    bool initialized = false;
    volatile bool shutting_down = false;

    std::thread worker;

public:
    Scheduler()
    {
    }

    inline void initialize()
    {
        if (initialized)
        {
            return;
        }
        initialized = true;
        if (Constants::EnablePrimarySurfaceBackgroundBuffering)
        {
            worker = std::thread([this]()
            {
                while (!shutting_down)
                {
                    std::unique_lock<std::mutex> lock(mutex_pending);
                    work_pending.wait(lock, [&]() { return !tasks.empty() || shutting_down; });
                    std::list<Task> my_tasks = std::move(tasks);
                    lock.unlock();

                    for (auto & task : my_tasks)
                    {
                        task();
                    }
                }
            });
        }
    }

    ~Scheduler()
    {
        if (Constants::EnablePrimarySurfaceBackgroundBuffering)
        {
            {
                std::lock_guard<std::mutex> lock(mutex_pending);
                shutting_down = true;
            }
            /// FIXME. If we notify work_pending, loader will enter deadlock.

            if (worker.joinable())
            {
                /// FIXME. join() would cause a deadlock because we are in DllMain().
                worker.detach();
            }
        }
    }

    /// TODO. Use std::invoke_result_t<SpecificTask> in 2018.
    template <typename ResultType, typename SpecificTask>
    inline ResultType makeTask(SpecificTask && task)
    {
        if (Constants::EnablePrimarySurfaceBackgroundBuffering)
        {
            volatile ResultType result;
            std::condition_variable work_done;

            /// TODO. thread_local.
            std::mutex mutex_done;
            std::unique_lock<std::mutex> lock_done(mutex_done);
            {
                std::lock_guard<std::mutex> lock(mutex_pending);
                initialize();
                tasks.emplace_back([&]()
                {
                    result = task();
                    std::lock_guard<std::mutex> lock_done(mutex_done);
                    work_done.notify_one();
                });
                work_pending.notify_one();
            }
            work_done.wait(lock_done);
            return result;
        }
        else
        {
            return task();
        }
    }
};
