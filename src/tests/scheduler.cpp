#include <iostream>
#include <thread>

#include "Logger.h"
#include "Scheduler.h"


int main()
{
    volatile int a = 0;
    Scheduler scheduler;
    Logger log("main");
    std::thread increment([&]()
    {
        for (size_t i = 0; i < 1500; ++i)
        {
            log() << "Incrementing...";
            scheduler.makeTask<int>([&]() { ++a; return 0; });
        }
    });
    std::thread decrement([&]()
    {
        for (size_t i = 0; i < 1000; ++i)
        {
            log() << "Decrementing...";
            scheduler.makeTask<int>([&]() { --a; return 0; });
        }
    });
    increment.join();
    decrement.join();
    std::cout << "Result is " << a << ".\n";
    return a == 500 ? 0 : 1;
}
